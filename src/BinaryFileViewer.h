/*
 * BinaryFileViewer.h - ContentScreen for viewing binary files in hex dump format
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef BINARYFILEVIEWER_H
#define BINARYFILEVIEWER_H

#include <Arduino.h>
#include <SD.h>
#include "ContentScreen.h"

class BinaryFileViewer : public ContentScreen
{
private:
    String _filename;        // File to display
    File _file;              // SD card file handle
    uint32_t _currentOffset; // Current file offset
    uint32_t _fileSize;      // Total file size
    bool _fileOpen;          // File open status

    // Display buffer for current page
    uint8_t *_pageBuffer;    // Buffer for current page data
    uint16_t _bufferSize;    // Size of page buffer
    uint16_t _bytesInBuffer; // Actual bytes loaded in buffer

    // File operations
    bool _openFile();        // Open the file for reading
    void _closeFile();       // Close the file
    bool _loadCurrentPage(); // Load current page into buffer
    void _freePageBuffer();  // Free page buffer memory

    // Display calculations
    uint16_t _getLinesPerPage() const; // Calculate lines that fit on screen
    uint16_t _getBytesPerLine() const; // Calculate bytes per line for optimal display
    uint32_t _getPageSize() const;     // Calculate total bytes per page

    // Display methods
    void _displayBinaryContent(); // Display the hex dump

public:
    BinaryFileViewer(const char *filename); // Constructor with filename, sets default button items
    ~BinaryFileViewer();                    // Destructor

    // File information
    uint32_t getCurrentOffset() const; // Get current file offset
    uint32_t getFileSize() const;      // Get total file size
    bool isFileOpen() const;           // Check if file is open
    uint32_t getCurrentPage() const;   // Get current page number (1-based)
    uint32_t getTotalPages() const;    // Get total number of pages

    // Navigation
    bool nextPage();                  // Go to next page
    bool previousPage();              // Go to previous page
    bool goToPage(uint32_t page);     // Go to specific page (0-based)
    bool goToOffset(uint32_t offset); // Go to specific file offset
    bool goToLastPage();              // Go to last page

protected:
    bool open() override;         // Initialize file and screen
    void _drawContent() override; // Draw the hex dump content
    Screen *actionTaken(ActionTaken action, int8_t offsetX, int8_t offsetY) override;
};

#endif /* BINARYFILEVIEWER_H */
