/*
 * ConsoleScreen.cpp - Console-style screen implementation
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#include "ConsoleScreen.h"
#include "M1Shield.h"
#include <Adafruit_GFX.h>

// Text size constants for different sizes
constexpr uint8_t CHAR_WIDTH_SIZE_1 = 6;   // Width of size-1 characters
constexpr uint8_t CHAR_HEIGHT_SIZE_1 = 8;  // Height of size-1 characters
constexpr uint8_t CHAR_WIDTH_SIZE_2 = 12;  // Width of size-2 characters
constexpr uint8_t CHAR_HEIGHT_SIZE_2 = 16; // Height of size-2 characters
constexpr uint8_t CHAR_WIDTH_SIZE_3 = 18;  // Width of size-3 characters
constexpr uint8_t CHAR_HEIGHT_SIZE_3 = 24; // Height of size-3 characters

// Default console settings
constexpr uint8_t DEFAULT_TAB_SIZE = 4;
constexpr uint16_t DEFAULT_TEXT_COLOR = 0xFFFF;
constexpr uint16_t DEFAULT_TEXT_BG_COLOR = 0x0000;
constexpr uint16_t DEFAULT_CONSOLE_BG_COLOR = 0x0000;

// Constructor - initialize console with default settings
ConsoleScreen::ConsoleScreen() : ContentScreen()
{
    // Initialize text settings
    _textSize = 1;
    _charWidth = CHAR_WIDTH_SIZE_1;
    _lineHeight = CHAR_HEIGHT_SIZE_1;

    _textColor = M1Shield.convertColor(DEFAULT_TEXT_COLOR);
    _textBgColor = M1Shield.convertColor(DEFAULT_TEXT_BG_COLOR);
    _consoleBgColor = M1Shield.convertColor(DEFAULT_CONSOLE_BG_COLOR);
    _tabSize = DEFAULT_TAB_SIZE;

    // Initialize cursor position
    _currentX = 0;
    _currentY = 0;

    // Cache will be updated when screen becomes active
    _contentLeft = 0;
    _contentTop = 0;
    _contentWidth = 0;
    _contentHeight = 0;

    // Initialize one-time execution tracking
    _screenOpenTime = 0;
    _hasExecutedOnce = false;

    // Initialize auto-forward functionality (disabled by default)
    _autoForwardEnabled = false;
    _autoForwardDelayMs = 5000; // Default 5 second delay
    _executeOnceCompleteTime = 0;
    _autoForwardTriggered = false;

    // Initialize paging management
    _pagingMode = PAGING_WAIT_BOTH; // Default to button and timeout-based paging
    _pagingTimeoutMs = 5000;        // Default 5 second timeout
    _isWaitingForPaging = false;
    _pagingWaitStartTime = 0;
    _showPagingPrompt = true; // Show prompts by default
    _pagingPaused = false;    // Paging timeout not paused initially

    // Initialize bulk write optimization
    _inBulkWrite = false;

    // Set default button labels
    const char *buttonItems[1] = {"[M] Back"};
    setButtonItems(buttonItems, 1);
}

// Initialize console when screen becomes active
bool ConsoleScreen::open()
{
    // Call parent implementation first
    bool result = ContentScreen::open();

    // Reset one-time execution tracking
    _screenOpenTime = millis();
    _hasExecutedOnce = false;

    // Reset auto-forward tracking
    _executeOnceCompleteTime = 0;
    _autoForwardTriggered = false;

    // Reset paging state
    _pagingPaused = false;

    return result;
}

// Destructor
ConsoleScreen::~ConsoleScreen()
{
    // Base class handles cleanup
}

// Update cached screen dimensions from ContentScreen
void ConsoleScreen::_updateDimensions()
{
    _contentLeft = _getContentLeft();
    _contentTop = _getContentTop();
    _contentWidth = _getContentWidth();
    _contentHeight = _getContentHeight();
}

// Main loop processing for console screen updates
void ConsoleScreen::loop()
{
    // Check for one-time execution
    if (!_hasExecutedOnce && (millis() - _screenOpenTime >= 1000))
    {
        _executeOnce();
        _hasExecutedOnce = true;
        _executeOnceCompleteTime = millis(); // Capture completion timestamp for auto-forward
    }

    // Check for auto-forward trigger (after executeOnce completes and delay passes)
    if (_autoForwardEnabled && _hasExecutedOnce && _executeOnceCompleteTime > 0)
    {
        if (millis() - _executeOnceCompleteTime >= _autoForwardDelayMs)
        {
            // Set flag and trigger auto-forward by calling actionTaken with BUTTON_MENU
            _autoForwardTriggered = true;
            _executeOnceCompleteTime = 0; // Prevent repeated triggering

            // Call actionTaken and handle the result properly
            Screen *newScreen = actionTaken(BUTTON_MENU, 0, 0);
            if (newScreen != nullptr)
            {
                M1Shield.setScreen(newScreen);
            }
        }
    }

    // Call parent loop is not necessary since nothing is defined there
}

// Handle user input actions
Screen *ConsoleScreen::actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY)
{
    if (!isActive())
    {
        return nullptr;
    }

    // Check if auto-forward was triggered (no user action)
    if (_autoForwardTriggered)
    {
        _autoForwardTriggered = false; // Reset flag
        // Continue processing the BUTTON_MENU action (already set when called from loop)
    }
    else
    {
        // If this is a user-initiated action, cancel pending auto-forward
        if (_executeOnceCompleteTime > 0)
        {
            _executeOnceCompleteTime = 0; // Cancel pending auto-forward
        }
    }

    // Handle back/menu button to exit console
    if (action & (BUTTON_MENU | BUTTON_LEFT))
    {
        return nullptr; // Let base class handle navigation
    }

    // No navigation for other actions - stay on console
    return nullptr;
}

// Draw the console content area
void ConsoleScreen::_drawContent()
{
    _updateDimensions();

    Adafruit_GFX &gfx = M1Shield.getGFX();

    // Fill console background
    gfx.fillRect(_contentLeft, _contentTop, _contentWidth, _contentHeight, M1Shield.convertColor(_consoleBgColor));
}

// Move to the next line (newline operation)
void ConsoleScreen::_newLine()
{
    _updateDimensions();

    _currentX = 0;
    _currentY += _lineHeight;

    // Check if we've reached the bottom of the screen
    if (_currentY + _lineHeight + _lineHeight >= _contentHeight)
    {
        if (!_handlePaging())
        {
            // If _handlePaging() returns false, we're waiting for user action
            // Don't advance cursor further - stay at current position
            _currentY -= _lineHeight; // Revert the Y advance
        }
    }
}

// Process tab character - move to next tab stop
void ConsoleScreen::_processTab()
{
    _updateDimensions();

    // Calculate current character position
    uint16_t charPos = _currentX / _charWidth;

    // Calculate next tab stop
    uint16_t nextTabStop = ((charPos / _tabSize) + 1) * _tabSize;
    uint16_t nextTabX = nextTabStop * _charWidth;

    // Check if tab stop fits on current line
    if (nextTabX < _contentWidth)
    {
        _currentX = nextTabX;
    }
    else
    {
        _newLine();
    }
}

// Print Interface Implementation

// Write a single character to the console (Print interface)
size_t ConsoleScreen::write(uint8_t c)
{
    // Block execution if we're waiting for paging action
    _waitForPagingIfNeeded();

    _processChar((char)c);
    if (isActive())
    {
        M1Shield.display(); // Push changes to display
    }
    return 1; // Always successful since _processChar handles all characters
}

// Write buffer of characters to console (Print interface optimization)
size_t ConsoleScreen::write(const uint8_t *buffer, size_t size)
{
    if (!isActive() || size == 0)
        return 0;

    // Block execution if we're waiting for paging action
    _waitForPagingIfNeeded();

    Adafruit_GFX &gfx = M1Shield.getGFX();

    // Start bulk write transaction for better SPI performance
    gfx.startWrite();

    // Set text properties once for the entire bulk operation
    gfx.setTextColor(_textColor, _textBgColor);
    gfx.setTextSize(_textSize);

    // Flag that we're in a bulk write operation
    _inBulkWrite = true;

    size_t n = 0;
    while (size--)
    {
        // Process character without individual display updates
        _processChar((char)*buffer++);
        n++;
    }

    // Clear bulk write flag
    _inBulkWrite = false;

    // End bulk write transaction
    gfx.endWrite();

    // Single display update after processing entire buffer
    if (n > 0)
    {
        M1Shield.display();
    }

    return n;
}

// Console Control Methods

// Clear screen and reset cursor to top-left
void ConsoleScreen::cls()
{
    _updateDimensions();

    if (!isActive())
        return;

    Adafruit_GFX &gfx = M1Shield.getGFX();

    // Clear console area
    gfx.fillRect(_contentLeft, _contentTop, _contentWidth, _contentHeight, M1Shield.convertColor(_consoleBgColor));

    // Reset cursor to top-left
    _currentX = 0;
    _currentY = 0;

    M1Shield.display(); // Push changes to display
}

void ConsoleScreen::refresh()
{
    cls();
}

// Console Configuration Methods

void ConsoleScreen::setTextColor(uint16_t foreground, uint16_t background)
{
    _textColor = M1Shield.convertColor(foreground);
    _textBgColor = M1Shield.convertColor(background);
}

void ConsoleScreen::setConsoleBackground(uint16_t color)
{
    _consoleBgColor = M1Shield.convertColor(color);
}

void ConsoleScreen::setTextSize(uint8_t size)
{
    // Only allow small text on very small displays
    if (isSmallDisplay())
        return;

    if (size < 1)
        size = 1;

    _textSize = size;

    // Update character dimensions
    switch (_textSize)
    {
    case 1:
        _charWidth = CHAR_WIDTH_SIZE_1;
        _lineHeight = CHAR_HEIGHT_SIZE_1;
        break;
    case 2:
        _charWidth = CHAR_WIDTH_SIZE_2;
        _lineHeight = CHAR_HEIGHT_SIZE_2;
        break;
    case 3:
        _charWidth = CHAR_WIDTH_SIZE_3;
        _lineHeight = CHAR_HEIGHT_SIZE_3;
        break;
    default:
        _charWidth = CHAR_WIDTH_SIZE_1 * _textSize;
        _lineHeight = CHAR_HEIGHT_SIZE_1 * _textSize;
        break;
    }
}

void ConsoleScreen::setTabSize(uint8_t size)
{
    if (size < 1)
        size = 1;
    _tabSize = size;
}

void ConsoleScreen::_processChar(char c)
{
    if (c == '\n')
    {
        _newLine();
    }
    else if (c == '\t')
    {
        _processTab();
    }
    else if (c >= 32 && c <= 126) // Printable ASCII characters
    {
        _renderChar(c);
    }
    // Ignore other control characters
}

void ConsoleScreen::_renderChar(char c)
{
    _updateDimensions();

    if (!isActive())
        return;

    Adafruit_GFX &gfx = M1Shield.getGFX();

    // Check if character would exceed line width
    if (_currentX + _charWidth > _contentWidth)
    {
        _newLine();
    }

    // Calculate absolute screen position
    uint16_t x = _contentLeft + _currentX;
    uint16_t y = _contentTop + _currentY;

    // Only set text properties if not in bulk write mode (already set)
    if (!_inBulkWrite)
    {
        gfx.setTextColor(_textColor, _textBgColor);
        gfx.setTextSize(_textSize);
    }

    gfx.setCursor(x, y);
    gfx.print(c);

    // Advance cursor position
    _currentX += _charWidth;
}

void ConsoleScreen::_waitForPagingIfNeeded()
{
    if (!_isWaitingForPaging)
        return;

    // Show paging prompt once when we start waiting
    if (_showPagingPrompt)
    {
        _showPagingMessage();
        M1Shield.display();
    }

    // Block execution until paging wait is resolved
    while (_isWaitingForPaging)
    {
        // Check for pause/resume with LEFT/RIGHT buttons (for timeout-based modes)
        if ((_pagingMode == PAGING_WAIT_TIMEOUT || _pagingMode == PAGING_WAIT_BOTH))
        {
            if (M1Shield.wasLeftPressed() && !_pagingPaused)
            {
                // LEFT button pauses the timeout
                _pagingPaused = true;
                _showPagingMessage(); // Update the message to show paused state
                M1Shield.display();
            }
            else if (M1Shield.wasRightPressed())
            {
                _pagingPaused = false;
                // RIGHT button continues immediately when not paused
                _clearPagingMessage();
                cls(); // Clear and reset console
                _isWaitingForPaging = false;
                break;
            }
        }

        // Check for other button presses to continue (for button-based modes)
        if ((_pagingMode == PAGING_WAIT_BUTTON || _pagingMode == PAGING_WAIT_BOTH) &&
            (M1Shield.wasMenuPressed() || M1Shield.wasLeftPressed() || M1Shield.wasRightPressed() ||
             M1Shield.wasUpPressed() || M1Shield.wasDownPressed() || M1Shield.wasJoystickPressed()))
        {
            _clearPagingMessage();
            cls(); // Clear and reset console
            _isWaitingForPaging = false;
            break;
        }

        // Check for timeout expiration (for timeout-based modes)
        if ((_pagingMode == PAGING_WAIT_TIMEOUT || _pagingMode == PAGING_WAIT_BOTH) &&
            _shouldEndPagingWait())
        {
            _clearPagingMessage();
            cls(); // Clear and reset console
            _isWaitingForPaging = false;
            break;
        }

        // Small delay to prevent excessive CPU usage
        delay(10);
    }
}

// ========== Paging Management Methods ==========

bool ConsoleScreen::_handlePaging()
{
    switch (_pagingMode)
    {
    case PAGING_AUTO_CLEAR:
        // Original behavior - clear immediately
        cls();
        return true;

    case PAGING_WAIT_TIMEOUT:
    case PAGING_WAIT_BUTTON:
    case PAGING_WAIT_BOTH:
        // Start waiting for paging action
        _isWaitingForPaging = true;
        _pagingWaitStartTime = millis();
        if (_showPagingPrompt)
        {
            _showPagingMessage();
        }
        return false; // Don't continue yet
    }

    return true; // Default fallback
}

bool ConsoleScreen::_shouldEndPagingWait()
{
    if (!_isWaitingForPaging || _pagingPaused)
        return false;

    unsigned long elapsed = millis() - _pagingWaitStartTime;

    // Only check timeout for modes that use it
    return (_pagingMode == PAGING_WAIT_TIMEOUT || _pagingMode == PAGING_WAIT_BOTH) &&
           (elapsed >= _pagingTimeoutMs);
}

void ConsoleScreen::_showPagingMessage()
{
    Adafruit_GFX &gfx = M1Shield.getGFX();

    // Save current text settings
    uint16_t savedTextColor = _textColor;
    uint16_t savedBgColor = _textBgColor;
    uint8_t savedTextSize = _textSize;

    // Set prompt styling (smaller, different color)
    gfx.setTextColor(M1Shield.convertColor(0x7BEF), M1Shield.convertColor(0x0000)); // Light gray on black
    gfx.setTextSize(1);

    // Position at bottom of content area
    uint16_t promptY = _contentTop + _contentHeight - CHAR_HEIGHT_SIZE_1 - 2;
    gfx.setCursor(_contentLeft + 2, promptY);

    // Show appropriate message based on mode
    switch (_pagingMode)
    {
    case PAGING_WAIT_TIMEOUT:
        if (_pagingPaused)
        {
            gfx.print("PAUSED - Press RIGHT to continue");
        }
        else
        {
            gfx.print("Auto in ");
            gfx.print((_pagingTimeoutMs - (millis() - _pagingWaitStartTime)) / 1000 + 1);
            gfx.print("s | LEFT:pause RIGHT:skip");
        }
        break;

    case PAGING_WAIT_BUTTON:
        gfx.print("Press MENU/UP/DOWN/JOYSTICK to continue");
        break;

    case PAGING_WAIT_BOTH:
        if (_pagingPaused)
        {
            gfx.print("PAUSED - Press RIGHT to continue");
        }
        else
        {
            gfx.print("Auto ");
            gfx.print((_pagingTimeoutMs - (millis() - _pagingWaitStartTime)) / 1000 + 1);
            gfx.print("s | LEFT:pause RIGHT:skip");
        }
        break;

    default:
        break;
    }

    // Restore original text settings
    gfx.setTextColor(savedTextColor, savedBgColor);
    gfx.setTextSize(savedTextSize);
}

void ConsoleScreen::_clearPagingMessage()
{
    // Simply clear the prompt area by filling with background color
    Adafruit_GFX &gfx = M1Shield.getGFX();
    uint16_t promptY = _contentTop + _contentHeight - CHAR_HEIGHT_SIZE_1 - 2;
    gfx.fillRect(_contentLeft, promptY, _contentWidth, CHAR_HEIGHT_SIZE_1 + 2, _consoleBgColor);
}

// ========== Paging Configuration Methods ==========

void ConsoleScreen::setPagingMode(ConsolePagingMode mode)
{
    _pagingMode = mode;
}

void ConsoleScreen::setPagingTimeout(uint16_t timeoutMs)
{
    _pagingTimeoutMs = timeoutMs;
}

ConsolePagingMode ConsoleScreen::getPagingMode() const
{
    return _pagingMode;
}

uint16_t ConsoleScreen::getPagingTimeout() const
{
    return _pagingTimeoutMs;
}

bool ConsoleScreen::isWaitingForPaging() const
{
    return _isWaitingForPaging;
}

void ConsoleScreen::continuePaging()
{
    if (_isWaitingForPaging)
    {
        _clearPagingMessage();
        cls();
        _isWaitingForPaging = false;
    }
}

void ConsoleScreen::setAutoForward(bool enabled, unsigned long delayMs)
{
    _autoForwardEnabled = enabled;
    _autoForwardDelayMs = delayMs;

    // If disabling auto-forward, cancel any pending auto-forward
    if (!enabled)
    {
        _executeOnceCompleteTime = 0;
        _autoForwardTriggered = false;
    }
}

bool ConsoleScreen::isAutoForwardEnabled() const
{
    return _autoForwardEnabled;
}

unsigned long ConsoleScreen::getAutoForwardDelay() const
{
    return _autoForwardDelayMs;
}