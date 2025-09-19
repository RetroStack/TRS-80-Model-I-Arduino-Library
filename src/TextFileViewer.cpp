/*
 * TextFileViewer.cpp - ContentScreen for viewing text files from SD card
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#include "TextFileViewer.h"
#include <SD.h>
#include "M1Shield.h"

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
    _textSize = size;
    _lineHeight = 8 * size; // Adjust line height based on text size
    _charWidth = 6 * size;  // Adjust character width based on text size
    _calculateLayout();     // Recalculate layout with new text size
    _loadCurrentPage();     // Reload current page with new layout
}

// Navigation methods
bool TextFileViewer::nextPage()
{
    if (_currentPage >= _totalPages - 1)
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

    if (!_countFileLines())
    {
        notifyF(F("Failed to read file"));
        return false;
    }

    _totalPages = _calculateTotalPages();
    _currentPage = 0;

    if (!_loadCurrentPage())
    {
        notifyF(F("Failed to load file content"));
        return false;
    }

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
    if (!isFileLoaded())
    {
        drawTextF(_getContentLeft() + 10, _getContentTop() + 20, F("No file loaded"), _textColor, _textSize);
        return;
    }

    if (_linesOnCurrentPage == 0)
    {
        drawTextF(_getContentLeft() + 10, _getContentTop() + 20, F("Page is empty"), _textColor, _textSize);
        return;
    }

    // Draw lines for current page
    uint16_t yPos = _getContentTop();
    for (uint16_t i = 0; i < _linesOnCurrentPage; i++)
    {
        if (yPos + _lineHeight <= _getContentTop() + _getContentHeight())
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

            drawText(_getContentLeft() + 5, yPos, displayLine, _textColor, _textSize);
            yPos += _lineHeight;
        }
    }

    // Show status information in footer area if available
    if (_getFooterHeight() > 0)
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

        // Clear footer area first
        M1Shield.getGFX().fillRect(0, _getFooterTop(), M1Shield.getScreenWidth(), _getFooterHeight(), _backgroundColor);

        // Draw status text
        M1Shield.getGFX().setCursor(5, _getFooterTop() + 5);
        M1Shield.getGFX().setTextColor(M1Shield.convertColor(0x7BEF));
        M1Shield.getGFX().setTextSize(1);
        M1Shield.getGFX().print(statusInfo);
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
    _linesOnCurrentPage = 0;
    _maxLineLength = 0;

    // Skip to start line
    uint32_t currentLine = 0;
    while (file.available() && currentLine < startLine)
    {
        file.readStringUntil('\n');
        currentLine++;
    }

    // Read lines for current page
    while (file.available() && _linesOnCurrentPage < linesToRead)
    {
        String line = file.readStringUntil('\n');
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

    // Count lines without loading content
    _totalFileLines = 0;
    _lastFileSize = file.size();

    while (file.available())
    {
        file.readStringUntil('\n');
        _totalFileLines++;
    }

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
    uint16_t contentHeight = _getContentHeight();
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
