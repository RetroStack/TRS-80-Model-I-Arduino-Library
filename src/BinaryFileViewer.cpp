/*
 * BinaryFileViewer.cpp - ContentScreen for viewing binary files in hex dump format
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#include "BinaryFileViewer.h"
#include "M1Shield.h"

// Constructor
BinaryFileViewer::BinaryFileViewer(const char *filename)
{
    _filename = String(filename);
    _currentOffset = 0;
    _fileSize = 0;
    _fileOpen = false;
    _pageBuffer = nullptr;
    _bufferSize = 0;
    _bytesInBuffer = 0;

    // Set default title
    setTitleF(F("Binary File Viewer"));

    // Set default button items
    const char *buttons[] = {"Up:Prev", "Dn:Next"};
    setButtonItems(buttons, 2);
}

// Destructor
BinaryFileViewer::~BinaryFileViewer()
{
    _closeFile();
    _freePageBuffer();
}

// File operations
bool BinaryFileViewer::_openFile()
{
    if (_fileOpen)
    {
        _closeFile();
    }

    _file = SD.open(_filename.c_str(), FILE_READ);
    if (!_file)
    {
        return false;
    }

    _fileSize = _file.size();
    _fileOpen = true;
    return true;
}

void BinaryFileViewer::_closeFile()
{
    if (_fileOpen && _file)
    {
        _file.close();
        _fileOpen = false;
    }
}

bool BinaryFileViewer::_loadCurrentPage()
{
    if (!_fileOpen || !_file)
    {
        return false;
    }

    // Calculate page size if not done yet
    if (_bufferSize == 0)
    {
        _bufferSize = _getPageSize();
        _pageBuffer = (uint8_t *)malloc(_bufferSize);
        if (!_pageBuffer)
        {
            return false;
        }
    }

    // Seek to current offset
    if (!_file.seek(_currentOffset))
    {
        return false;
    }

    // Read page data
    _bytesInBuffer = _file.read(_pageBuffer, _bufferSize);

    return true;
}

void BinaryFileViewer::_freePageBuffer()
{
    if (_pageBuffer)
    {
        free(_pageBuffer);
        _pageBuffer = nullptr;
        _bufferSize = 0;
        _bytesInBuffer = 0;
    }
}

// Display calculations
uint16_t BinaryFileViewer::_getLinesPerPage() const
{
    // Get available content height
    uint16_t availableHeight = _getContentHeight();
    uint16_t lineHeight = 8; // Default text size line height

    // Calculate lines, with minimum of 5
    uint16_t calculatedLines = availableHeight / lineHeight;

    if (calculatedLines < 5)
        calculatedLines = 5;

    return calculatedLines;
}

uint16_t BinaryFileViewer::_getBytesPerLine() const
{
    // Get available content width
    uint16_t contentWidth = _getContentWidth();
    uint16_t charWidth = 6; // Default character width
    uint16_t maxChars = contentWidth / charWidth;

    // Format: "XXXXXXXX: " (10 chars) + hex bytes (3 chars each) + " " (1 char) + ASCII (1 char each)
    // Total per byte: 4 characters (3 hex + 1 ASCII)
    // Fixed overhead: 11 characters (8-digit address + ": " + space between hex and ASCII)

    // Calculate: maxChars = 11 + (4 * bytesPerLine)
    // Solve for bytesPerLine: (maxChars - 11) / 4
    uint16_t calculatedBytes = (maxChars > 11) ? (maxChars - 11) / 4 : 8;

    // Ensure reasonable bounds: minimum 8, maximum 32
    if (calculatedBytes < 8)
        calculatedBytes = 8;
    else if (calculatedBytes > 32)
        calculatedBytes = 32;

    // Prefer multiples of 8 for better alignment
    calculatedBytes = (calculatedBytes / 8) * 8;
    if (calculatedBytes == 0)
        calculatedBytes = 8;

    return calculatedBytes;
}

uint32_t BinaryFileViewer::_getPageSize() const
{
    return _getLinesPerPage() * _getBytesPerLine();
}

// Display methods
void BinaryFileViewer::_drawContent()
{
    // Clear content area
    clearContentArea();

    if (!_fileOpen)
    {
        // Try to open file first
        if (!_openFile())
        {
            drawTextF(10, 10, F("Error: Could not open file"), M1Shield.convertColor(0xF800), 1);
            drawText(10, 25, _filename, M1Shield.convertColor(0xF800), 1);
            return;
        }
    }

    // Load current page if needed
    if (!_loadCurrentPage())
    {
        drawTextF(10, 10, F("Error loading file data"), M1Shield.convertColor(0xF800), 1);
        return;
    }

    _displayBinaryContent();
}

void BinaryFileViewer::_displayBinaryContent()
{
    if (!_fileOpen || !_pageBuffer)
    {
        drawTextF(10, 10, F("File not available"), M1Shield.convertColor(0xFFFF), 1);
        return;
    }

    uint16_t linesPerPage = _getLinesPerPage();
    uint16_t bytesPerLine = _getBytesPerLine();
    uint16_t lineHeight = 8; // Text size 1 line height
    uint16_t startX = _getContentLeft() + 5;
    uint16_t startY = _getContentTop() + 5;

    // Display hex dump
    for (uint16_t line = 0; line < linesPerPage; line++)
    {
        uint32_t lineOffset = _currentOffset + (line * bytesPerLine);
        uint16_t lineStartInBuffer = line * bytesPerLine;
        uint16_t currentY = startY + (line * lineHeight);

        // Stop if we've reached end of buffer or content area
        if (lineStartInBuffer >= _bytesInBuffer || currentY >= (_getContentTop() + _getContentHeight() - lineHeight))
            break;

        uint16_t currentX = startX;

        // Build and display address part (8-digit hex address)
        String addressPart = "";
        if (lineOffset < 0x10000000)
            addressPart += "0";
        if (lineOffset < 0x01000000)
            addressPart += "0";
        if (lineOffset < 0x00100000)
            addressPart += "0";
        if (lineOffset < 0x00010000)
            addressPart += "0";
        if (lineOffset < 0x00001000)
            addressPart += "0";
        if (lineOffset < 0x00000100)
            addressPart += "0";
        if (lineOffset < 0x00000010)
            addressPart += "0";
        addressPart += String(lineOffset, HEX);
        addressPart += ": ";

        // Draw address in yellow
        drawText(currentX, currentY, addressPart, M1Shield.convertColor(0xFFE0), 1);
        currentX += addressPart.length() * 6; // 6 pixels per character

        // Build and display hex bytes
        String hexPart = "";
        for (uint16_t byte = 0; byte < bytesPerLine; byte++)
        {
            uint16_t bufferIndex = lineStartInBuffer + byte;
            if (bufferIndex < _bytesInBuffer)
            {
                uint8_t value = _pageBuffer[bufferIndex];
                if (value < 0x10)
                    hexPart += "0";
                hexPart += String(value, HEX);
                hexPart += " ";
            }
            else
            {
                hexPart += "-- ";
            }
        }

        // Draw hex bytes in cyan
        drawText(currentX, currentY, hexPart, M1Shield.convertColor(0x07FF), 1);
        currentX += hexPart.length() * 6;

        // Build and display ASCII representation
        String asciiPart = " ";
        for (uint16_t byte = 0; byte < bytesPerLine; byte++)
        {
            uint16_t bufferIndex = lineStartInBuffer + byte;
            if (bufferIndex < _bytesInBuffer)
            {
                uint8_t value = _pageBuffer[bufferIndex];
                // Standard hex editor convention:
                // 0x00-0x1F: Control characters -> "."
                // 0x20-0x7F: Printable ASCII -> actual character
                // 0x80-0xFF: Extended/graphics characters -> "."
                if (value >= 0x20 && value <= 0x7F)
                {
                    asciiPart += (char)value;
                }
                else
                {
                    asciiPart += ".";
                }
            }
            else
            {
                asciiPart += " ";
            }
        }

        // Draw ASCII in white
        drawText(currentX, currentY, asciiPart, M1Shield.convertColor(0xFFFF), 1);
    }
}

// Public methods
uint32_t BinaryFileViewer::getCurrentOffset() const
{
    return _currentOffset;
}

uint32_t BinaryFileViewer::getFileSize() const
{
    return _fileSize;
}

bool BinaryFileViewer::isFileOpen() const
{
    return _fileOpen;
}

uint32_t BinaryFileViewer::getCurrentPage() const
{
    uint32_t pageSize = _getPageSize();
    return pageSize > 0 ? (_currentOffset / pageSize) + 1 : 1;
}

uint32_t BinaryFileViewer::getTotalPages() const
{
    uint32_t pageSize = _getPageSize();
    return pageSize > 0 ? ((_fileSize + pageSize - 1) / pageSize) : 1;
}

// Navigation methods
bool BinaryFileViewer::nextPage()
{
    uint32_t pageSize = _getPageSize();
    if (_currentOffset + pageSize < _fileSize)
    {
        _currentOffset += pageSize;
        refresh();
        return true;
    }
    return false;
}

bool BinaryFileViewer::previousPage()
{
    uint32_t pageSize = _getPageSize();
    if (_currentOffset >= pageSize)
    {
        _currentOffset -= pageSize;
        refresh();
        return true;
    }
    else if (_currentOffset > 0)
    {
        _currentOffset = 0;
        refresh();
        return true;
    }
    return false;
}

bool BinaryFileViewer::goToPage(uint32_t page)
{
    uint32_t pageSize = _getPageSize();
    uint32_t totalPages = getTotalPages();

    if (page >= totalPages)
        return false;

    _currentOffset = page * pageSize;
    if (_currentOffset >= _fileSize)
        _currentOffset = _fileSize - 1;

    refresh();
    return true;
}

bool BinaryFileViewer::goToOffset(uint32_t offset)
{
    if (offset >= _fileSize)
        return false;

    _currentOffset = offset;
    refresh();
    return true;
}

bool BinaryFileViewer::goToLastPage()
{
    uint32_t totalPages = getTotalPages();
    if (totalPages > 0)
    {
        return goToPage(totalPages - 1);
    }
    return false;
}

// Protected methods
bool BinaryFileViewer::open()
{
    // Call parent implementation first
    if (!ContentScreen::open())
    {
        return false;
    }

    if (_openFile())
    {
        refresh();
    }
    else
    {
        notifyF(F("Error: Could not open file"));
    }

    return true;
}

Screen *BinaryFileViewer::actionTaken(ActionTaken action, int8_t offsetX, int8_t offsetY)
{
    (void)offsetX; // Parameter not used
    (void)offsetY; // Parameter not used

    if (!isActive())
    {
        return nullptr;
    }

    if (action & BUTTON_MENU)
    {
        // Return to previous screen or close
        return nullptr;
    }

    if (action & UP_ANY)
    {
        previousPage();
        return nullptr;
    }

    if (action & DOWN_ANY)
    {
        nextPage();
        return nullptr;
    }

    if (action & LEFT_ANY)
    {
        // Go to first page
        if (_currentOffset > 0)
        {
            _currentOffset = 0;
            refresh();
        }
        return nullptr;
    }

    if (action & RIGHT_ANY)
    {
        // Go to last page
        goToLastPage();
        return nullptr;
    }

    if (action & BUTTON_SELECT)
    {
        // Future: Could implement search or goto offset functionality
        notifyF(F("SELECT: Feature not implemented"));
        return nullptr;
    }

    return nullptr;
}
