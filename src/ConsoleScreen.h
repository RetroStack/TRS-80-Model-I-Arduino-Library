/*
 * ConsoleScreen.h - Scrollable console/terminal screen for text output
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef CONSOLE_SCREEN_H
#define CONSOLE_SCREEN_H

#include <Arduino.h>
#include "ContentScreen.h"

// Auto-paging behavior when console reaches bottom of screen
enum ConsolePagingMode
{
    PAGING_AUTO_CLEAR,   // Clear immediately and continue (original behavior)
    PAGING_WAIT_TIMEOUT, // Wait for timeout before clearing (default)
    PAGING_WAIT_BUTTON,  // Wait for right button press before clearing
    PAGING_WAIT_BOTH     // Wait for timeout OR button press (whichever comes first)
};

// Scrollable console screen for terminal-like text output
class ConsoleScreen : public ContentScreen, public Print
{
private:
    // Text positioning and rendering state
    uint16_t _currentX;   // Current horizontal cursor position (pixels)
    uint16_t _currentY;   // Current vertical cursor position (pixels)
    uint16_t _lineHeight; // Height of each text line (pixels)
    uint16_t _charWidth;  // Width of each character (pixels)
    uint8_t _textSize;    // Text size (1, 2, 3, etc.)

    // Color settings
    uint16_t _textColor;      // Current text foreground color
    uint16_t _textBgColor;    // Current text background color
    uint16_t _consoleBgColor; // Console area background color

    // Tab settings
    uint8_t _tabSize; // Number of characters per tab stop

    // Bulk operation optimization
    bool _inBulkWrite; // Flag to track if we're in a bulk write operation

    // Screen dimensions (cached for efficiency)
    uint16_t _contentLeft;   // Left edge of content area
    uint16_t _contentTop;    // Top edge of content area
    uint16_t _contentWidth;  // Width of content area
    uint16_t _contentHeight; // Height of content area

    // One-time execution tracking
    unsigned long _screenOpenTime; // Timestamp when screen was opened
    bool _hasExecutedOnce;         // Whether _executeOnce() has been called

    // Auto-forward functionality
    bool _autoForwardEnabled;               // Whether auto-forward is enabled
    unsigned long _autoForwardDelayMs;      // Delay in milliseconds before auto-forward triggers
    unsigned long _executeOnceCompleteTime; // Timestamp when _executeOnce() completed
    bool _autoForwardTriggered;             // Flag indicating auto-forward should activate

    // Auto-paging management
    ConsolePagingMode _pagingMode;      // Current paging behavior mode
    uint16_t _pagingTimeoutMs;          // Timeout in milliseconds for auto-paging
    bool _isWaitingForPaging;           // True when console is full and waiting
    unsigned long _pagingWaitStartTime; // When the paging wait period started
    bool _showPagingPrompt;             // Whether to show paging prompt message
    bool _pagingPaused;                 // True when paging timeout is paused by LEFT button

    void _updateDimensions(); // Update cached screen dimensions from ContentScreen

    void _processChar(char c); // Process a single character for output

    void _renderChar(char c); // Render a single character at the current cursor position

    void _newLine(); // Move to the next line (newline operation)

    void _processTab(); // Process tab character - move to next tab stop

    bool _handlePaging(); // Check if console has reached the bottom and handle paging

    bool _shouldEndPagingWait(); // Check if paging timeout has expired

    void _showPagingMessage(); // Display paging prompt message

    void _clearPagingMessage(); // Clear paging prompt and prepare for new content

    void _waitForPagingIfNeeded(); // Block execution until paging wait is resolved

protected:
    // Optional one-time execution method called 1 second after console opens
    virtual void _executeOnce() {};

public:
    ConsoleScreen();          // Constructor - initialize console with default settings
    virtual ~ConsoleScreen(); // Destructor

    bool open() override; // Override Screen::open() to initialize timing for one-time execution

    // Print Interface Implementation (required for Arduino Print class)
    size_t write(uint8_t c) override;                          // Write a single character to the console (Print interface)
    size_t write(const uint8_t *buffer, size_t size) override; // Write buffer of characters to console (Print interface optimization)

    // Screen Interface Implementation
    void loop() override;                                                             // Main loop processing for console screen updates
    Screen *actionTaken(ActionTaken action, int8_t offsetX, int8_t offsetY) override; // Handle user input actions

    // Console Control Methods
    void cls();     // Clear screen and reset cursor to top-left
    void refresh(); // Refresh screen and reset cursor to top-left

    // Console Configuration Methods
    void setTextColor(uint16_t foreground, uint16_t background = 0x0000); // Set text color for subsequent output
    void setConsoleBackground(uint16_t color);                            // Set console background color
    void setTextSize(uint8_t size);                                       // Set text size for subsequent output
    void setTabSize(uint8_t size);                                        // Set tab size in characters

    // Console Paging Configuration Methods
    void setPagingMode(ConsolePagingMode mode);                      // Set console paging behavior mode
    void setPagingTimeout(uint16_t timeoutMs);                       // Set paging timeout duration
    ConsolePagingMode getPagingMode() const;                         // Get current paging mode
    uint16_t getPagingTimeout() const;                               // Get current paging timeout
    bool isPagingWaiting() const;                                    // Check if console is currently waiting for paging action
    bool isWaitingForPaging() const;                                 // Check if console is currently waiting for paging action
    void setAutoForward(bool enabled, unsigned long delayMs = 5000); // Enable or disable auto-forward functionality
    bool isAutoForwardEnabled() const;                               // Check if auto-forward is currently enabled
    unsigned long getAutoForwardDelay() const;                       // Get current auto-forward delay
    void continuePaging();                                           // Manually trigger page continuation

protected:
    void _drawContent() override; // Draw the console content area
};

#endif /* CONSOLE_SCREEN_H */
