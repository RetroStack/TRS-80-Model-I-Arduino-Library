/*
 * TextFileViewer.h - ContentScreen for viewing text files from SD card
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef TEXTFILEVIEWER_H
#define TEXTFILEVIEWER_H

#include <Arduino.h>
#include "ContentScreen.h"

class TextFileViewer : public ContentScreen
{
private:
    String _filename;            // File to display
    bool _autoPaging;            // Auto-paging for log files
    unsigned long _lastCheck;    // Last auto-paging check time
    unsigned long _lastFileSize; // Last known file size for change detection

    // Current page content only (memory efficient)
    String *_currentPageLines;    // Array of text lines for current page only
    uint16_t _linesOnCurrentPage; // Number of lines actually loaded on current page
    uint16_t _maxLinesPerPage;    // Maximum lines that can fit on one page

    // File and navigation state
    uint32_t _totalFileLines;   // Total number of lines in file (from last count)
    uint32_t _currentPage;      // Current page number (0-based)
    uint32_t _totalPages;       // Total number of pages
    uint16_t _horizontalOffset; // Horizontal scroll offset
    uint16_t _maxLineLength;    // Length of longest line on current page

    // Display settings
    uint16_t _textColor;       // Text color
    uint16_t _backgroundColor; // Background color
    uint8_t _textSize;         // Text size
    uint8_t _lineHeight;       // Height per line in pixels
    uint8_t _charWidth;        // Width per character in pixels

    // File operations - load only what's needed
    bool _loadCurrentPage();         // Load only the current page from file
    void _freeCurrentPage();         // Free current page memory
    bool _countFileLines();          // Count total lines in file without loading content
    uint32_t _calculateTotalPages(); // Calculate total pages based on lines per page
    void _calculateLayout();         // Calculate lines per page and layout
    bool _checkFileUpdate();         // Check if file has been updated (for auto-paging)
    void _updateMaxLineLength();     // Update maximum line length for current page

public:
    TextFileViewer(const char *filename); // Constructor with filename, sets default button items
    ~TextFileViewer();                    // Destructor

    // Configuration
    void setAutoPaging(bool enabled);        // Enable/disable auto-paging
    void setTextColor(uint16_t color);       // Set text color
    void setBackgroundColor(uint16_t color); // Set background color
    void setTextSize(uint8_t size);          // Set text size

    // Navigation
    bool nextPage();              // Go to next page
    bool previousPage();          // Go to previous page
    bool goToPage(uint32_t page); // Go to specific page
    bool goToLastPage();          // Go to last page
    void scrollLeft();            // Scroll left horizontally
    void scrollRight();           // Scroll right horizontally
    void resetHorizontalScroll(); // Reset horizontal scroll to beginning

    // Status
    uint32_t getCurrentPage() const;  // Get current page number (1-based for display)
    uint32_t getTotalPages() const;   // Get total number of pages
    bool isAutoPagingEnabled() const; // Check if auto-paging is enabled
    bool isFileLoaded() const;        // Check if file data is available

    // File operations
    bool refreshFile(); // Refresh file information and current page

protected:
    // ContentScreen overrides
    bool open() override;                                                             // Initialize and load file
    void close() override;                                                            // Cleanup resources
    void loop() override;                                                             // Handle auto-paging and updates
    void _drawContent() override;                                                     // Draw file content
    Screen *actionTaken(ActionTaken action, int8_t offsetX, int8_t offsetY) override; // Handle navigation input
};

#endif /* TEXTFILEVIEWER_H */
