/*
 * TextFileViewer.cpp - ContentScreen for viewing text files from SD card
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#include "TextFileViewer.h"
#include <SD.h>
#include "M1Shield.h"

// Longest line this viewer will keep. A file with no newline in it -- any
// binary renamed .txt -- otherwise grows a single String one realloc per byte
// until only avr-libc's malloc margin separates the heap from the stack.
constexpr uint16_t MAX_LINE_LENGTH = 512;

// Read one newline-terminated line, discarding anything past the cap.
static String readLineCapped(File &file)
{
    String line;
    line.reserve(64);

    while (file.available())
    {
        char c = (char)file.read();
        if (c == '\n')
        {
            break;
        }
        if (line.length() < MAX_LINE_LENGTH)
        {
            line += c;
        }
    }

    return line;
}

// Constructor
TextFileViewer::TextFileViewer(const char *filename) : ContentScreen()
{
    _filename = String(filename);
    _autoPaging = false;
    _lastCheck = 0;
    _lastFileSize = 0;

    // Current page data only
    _currentPageLines = nullptr;
    _linesOnCurrentPage = 0;
    _maxLinesPerPage = 0;

    // File state
    _totalFileLines = 0;
    _currentPage = 0;
    _totalPages = 0;
    _horizontalOffset = 0;
    _maxLineLength = 0;

    // Default display settings
    _textColor = 0xFFFF;       // White
    _backgroundColor = 0x0000; // Black
    _textSize = 1;
    _lineHeight = 8; // 8 pixels per line for text size 1
    _charWidth = 6;  // 6 pixels per character for text size 1

    // Set default title
    setTitleF(F("Text File Viewer"));

    // Set default button items
    const char *buttonLabels[] = {"Up/Dn:Page", "Lf/Rt:Scroll", "Sel:AutoPage"};
    setButtonItems(buttonLabels, 3);
}

// Destructor
TextFileViewer::~TextFileViewer()
{
    _freeCurrentPage();
}

// Configuration methods
void TextFileViewer::setAutoPaging(bool enabled)
{
    _autoPaging = enabled;
    if (_autoPaging)
    {
        _lastCheck = millis();
    }
}

void TextFileViewer::setTextColor(uint16_t color)
{
    _textColor = color;
}

void TextFileViewer::setBackgroundColor(uint16_t color)
{
    _backgroundColor = color;
}

void TextFileViewer::setTextSize(uint8_t size)
{
    if (size == 0)
    {
        return; // A zero size divides by zero in _calculateLayout()
    }

    _textSize = size;
    _lineHeight = 8 * size; // Adjust line height based on text size
    _charWidth = 6 * size;  // Adjust character width based on text size
    _calculateLayout();     // Recalculate layout with new text size

    // The page count depends on _maxLinesPerPage, which just changed. Leaving
    // it stale made the tail of the file unreachable when the text grew, and
    // underflowed the line count on the last page when it shrank.
    _totalPages = _calculateTotalPages();
    if (_totalPages > 0 && _currentPage >= _totalPages)
    {
        _currentPage = _totalPages - 1;
    }

    _loadCurrentPage(); // Reload current page with new layout
}

// Navigation methods
bool TextFileViewer::nextPage()
{
    // _totalPages is unsigned - "_totalPages - 1" wraps when it is zero,
    // which let _currentPage advance past the end of an empty file.
    if (_totalPages == 0 || _currentPage + 1 >= _totalPages)
        return false;

    _currentPage++;
    _horizontalOffset = 0; // Reset horizontal scroll
    return _loadCurrentPage();
}

bool TextFileViewer::previousPage()
{
    if (_currentPage == 0)
        return false;

    _currentPage--;
    _horizontalOffset = 0; // Reset horizontal scroll
    return _loadCurrentPage();
}

bool TextFileViewer::goToPage(uint32_t page)
{
    if (page >= _totalPages)
        return false;

    _currentPage = page;
    _horizontalOffset = 0; // Reset horizontal scroll
    return _loadCurrentPage();
}

bool TextFileViewer::goToLastPage()
{
    if (_totalPages == 0)
        return false;

    _currentPage = _totalPages - 1;
    _horizontalOffset = 0; // Reset horizontal scroll
    return _loadCurrentPage();
}

void TextFileViewer::scrollLeft()
{
    if (_horizontalOffset > 0)
    {
        _horizontalOffset -= 5; // Scroll 5 characters at a time
    }
}

void TextFileViewer::scrollRight()
{
    // Calculate maximum scroll based on screen width and longest line
    uint16_t maxCharsOnScreen = _getContentWidth() / _charWidth;

    if (_maxLineLength > maxCharsOnScreen &&
        _horizontalOffset < (_maxLineLength - maxCharsOnScreen))
    {
        _horizontalOffset += 5; // Scroll 5 characters at a time
    }
}

void TextFileViewer::resetHorizontalScroll()
{
    _horizontalOffset = 0;
}

// Status methods
uint32_t TextFileViewer::getCurrentPage() const
{
    return _currentPage + 1; // Return 1-based page number for display
}

uint32_t TextFileViewer::getTotalPages() const
{
    return _totalPages;
}

bool TextFileViewer::isAutoPagingEnabled() const
{
    return _autoPaging;
}

bool TextFileViewer::isFileLoaded() const
{
    return _totalFileLines > 0 && _totalPages > 0;
}

// File operations
bool TextFileViewer::refreshFile()
{
    // Count lines first, then reload current page
    if (_countFileLines())
    {
        _totalPages = _calculateTotalPages();
        // Ensure current page is still valid
        if (_currentPage >= _totalPages && _totalPages > 0)
        {
            _currentPage = _totalPages - 1;
        }
        return _loadCurrentPage();
    }
    return false;
}

// ContentScreen overrides
bool TextFileViewer::open()
{
    if (!ContentScreen::open())
        return false;

    _calculateLayout();

    // Returning false here deletes the screen and leaves the device with none,
    // so a file the browser could not read froze it. Stay open and say so.
    if (!_countFileLines())
    {
        _setErrorState(F("Cannot read file"));
        return true;
    }

    _totalPages = _calculateTotalPages();
    _currentPage = 0;

    if (!_loadCurrentPage())
    {
        _setErrorState(F("Cannot load file"));
        return true;
    }

    _clearErrorState();

    // ContentScreen::open() drew the screen before the file was counted and
    // loaded, so the first frame showed an empty page. Redraw now that there
    // is content. (BinaryFileViewer::open() already does the same.)
    refresh();

    return true;
}

void TextFileViewer::close()
{
    _freeCurrentPage();
    _totalFileLines = 0;
    _totalPages = 0;

    ContentScreen::close();
}

void TextFileViewer::loop()
{
    ContentScreen::loop();

    // Handle auto-paging check every second
    if (_autoPaging && millis() - _lastCheck >= 1000)
    {
        _lastCheck = millis();
        if (_checkFileUpdate())
        {
            // File was updated, go to last page
            goToLastPage();
        }
    }
}

void TextFileViewer::_drawContent()
{
    // drawText()/drawTextF() take content-relative coordinates and add the
    // content origin themselves; passing absolute ones shifted everything down
    // by a second header height.
    if (!isFileLoaded())
    {
        drawTextF(10, 20, F("No file loaded"), _textColor, _textSize);
        return;
    }

    if (_linesOnCurrentPage == 0)
    {
        drawTextF(10, 20, F("Page is empty"), _textColor, _textSize);
        return;
    }

    // Last row belongs to the status line
    uint16_t textHeight = (_getContentHeight() > _lineHeight) ? (_getContentHeight() - _lineHeight) : 0;

    // Draw lines for current page
    uint16_t yPos = 0;
    for (uint16_t i = 0; i < _linesOnCurrentPage; i++)
    {
        if (yPos + _lineHeight <= textHeight)
        {
            String displayLine = _currentPageLines[i];

            // Handle horizontal scrolling
            if (_horizontalOffset > 0 && displayLine.length() > _horizontalOffset)
            {
                displayLine = displayLine.substring(_horizontalOffset);
            }
            else if (_horizontalOffset > 0)
            {
                displayLine = ""; // Line is shorter than scroll offset
            }

            drawText(5, yPos, displayLine, _textColor, _textSize);
            yPos += _lineHeight;
        }
    }

    // Status line, in the row reserved for it at the bottom of the content
    if (textHeight > 0)
    {
        String statusInfo = "Page " + String(getCurrentPage()) + "/" + String(getTotalPages());
        if (_horizontalOffset > 0)
        {
            statusInfo += " | Scroll: " + String(_horizontalOffset);
        }
        if (_autoPaging)
        {
            statusInfo += " | Auto";
        }

        drawText(5, textHeight, statusInfo, 0x7BEF, 1);
    }
}

Screen *TextFileViewer::actionTaken(ActionTaken action, int8_t offsetX, int8_t offsetY)
{
    (void)offsetX; // Parameter not used
    (void)offsetY; // Parameter not used

    if (!isActive())
    {
        return nullptr;
    }

    // The menu button is advertised as back on every one of these screens; it
    // used to fall through and return nullptr, leaving the screen with no exit.
    Screen *backScreen = _handleBackAction(action);
    if (backScreen != nullptr)
    {
        return backScreen;
    }

    // Handle navigation actions
    if (action & UP_ANY)
    {
        previousPage();
    }
    else if (action & DOWN_ANY)
    {
        nextPage();
    }
    else if (action & LEFT_ANY)
    {
        scrollLeft();
    }
    else if (action & RIGHT_ANY)
    {
        scrollRight();
    }
    else if (action & BUTTON_SELECT)
    {
        // Toggle auto-paging
        setAutoPaging(!_autoPaging);
        if (_autoPaging)
        {
            notifyF(F("Auto-paging enabled"));
        }
        else
        {
            notifyF(F("Auto-paging disabled"));
        }
    }

    // Stay on this screen
    return nullptr;
}

// Private helper methods
bool TextFileViewer::_loadCurrentPage()
{
    // Free existing page data
    _freeCurrentPage();

    // Initialize SD card
    if (!SD.begin(M1Shield.getSDCardSelectPin()))
    {
        return false;
    }

    // Open file for reading
    File file = SD.open(_filename.c_str(), FILE_READ);
    if (!file)
    {
        return false;
    }

    // Calculate which lines to read for current page
    uint32_t startLine = _currentPage * _maxLinesPerPage;
    uint32_t endLine = min(startLine + _maxLinesPerPage, _totalFileLines);
    uint32_t linesToRead = endLine - startLine;

    if (linesToRead == 0)
    {
        file.close();
        return true; // Empty page is valid
    }

    // Allocate memory for current page only
    _currentPageLines = new String[linesToRead];
    if (!_currentPageLines)
    {
        file.close();
        return false;
    }
    _linesOnCurrentPage = 0;
    _maxLineLength = 0;

    // Skip to start line
    uint32_t currentLine = 0;
    while (file.available() && currentLine < startLine)
    {
        readLineCapped(file);
        currentLine++;
    }

    // Read lines for current page
    while (file.available() && _linesOnCurrentPage < linesToRead)
    {
        String line = readLineCapped(file);
        // Remove carriage return if present
        if (line.endsWith("\r"))
        {
            line.remove(line.length() - 1);
        }

        _currentPageLines[_linesOnCurrentPage] = line;

        // Track maximum line length for horizontal scrolling
        if (line.length() > _maxLineLength)
        {
            _maxLineLength = line.length();
        }

        _linesOnCurrentPage++;
    }

    file.close();

    // Update title to show filename and page info
    String title = "File: " + _filename + " (" + String(getCurrentPage()) + "/" + String(getTotalPages()) + ")";
    setTitle(title);

    return true;
}

void TextFileViewer::_freeCurrentPage()
{
    if (_currentPageLines)
    {
        delete[] _currentPageLines;
        _currentPageLines = nullptr;
    }
    _linesOnCurrentPage = 0;
    _maxLineLength = 0;
}

bool TextFileViewer::_countFileLines()
{
    // Initialize SD card
    if (!SD.begin(M1Shield.getSDCardSelectPin()))
    {
        return false;
    }

    // Open file for reading
    File file = SD.open(_filename.c_str(), FILE_READ);
    if (!file)
    {
        return false;
    }

    // Count lines without loading content. This reads the whole file, which is
    // seconds on a large one, and used to show nothing at all -- the progress
    // bar existed but no library operation ever drove it.
    _totalFileLines = 0;
    _lastFileSize = file.size();
    const uint32_t totalBytes = _lastFileSize;
    uint8_t lastShown = 0;

    setProgressValue(0);

    while (file.available())
    {
        readLineCapped(file);
        _totalFileLines++;

        if (totalBytes > 0)
        {
            uint8_t percent = (uint8_t)(((uint32_t)file.position() * 100) / totalBytes);
            if (percent != lastShown)
            {
                lastShown = percent;
                setProgressValue(percent);
            }
        }
    }

    setProgressValue(100);

    file.close();
    return true;
}

uint32_t TextFileViewer::_calculateTotalPages()
{
    if (_totalFileLines == 0 || _maxLinesPerPage == 0)
        return 0;

    return (_totalFileLines + _maxLinesPerPage - 1) / _maxLinesPerPage; // Ceiling division
}

void TextFileViewer::_calculateLayout()
{
    // The status line is drawn as the last row of the content area. It used to
    // go into the footer rect, which _drawScreen() paints over immediately
    // afterwards with the button labels, so it was never visible.
    uint16_t contentHeight = _getContentHeight();
    contentHeight = (contentHeight > _lineHeight) ? (contentHeight - _lineHeight) : 0;
    _maxLinesPerPage = contentHeight / _lineHeight;

    // Ensure at least one line per page
    if (_maxLinesPerPage == 0)
    {
        _maxLinesPerPage = 1;
    }
}

bool TextFileViewer::_checkFileUpdate()
{
    // Check if file size has changed (simple but effective)
    if (!SD.begin(M1Shield.getSDCardSelectPin()))
    {
        return false;
    }

    File file = SD.open(_filename.c_str(), FILE_READ);
    if (!file)
    {
        return false;
    }

    unsigned long currentSize = file.size();
    file.close();

    bool fileChanged = (currentSize != _lastFileSize);
    if (fileChanged)
    {
        // Refresh file data
        refreshFile();
    }

    return fileChanged;
}

void TextFileViewer::_updateMaxLineLength()
{
    // This is handled in _loadCurrentPage() for efficiency
    // No need for separate implementation since we only load current page
}
