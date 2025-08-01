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

/**
 * @brief Constructor - initialize console with default settings
 *
 * Sets up the console with default text size, colors, cursor position,
 * and timing for one-time execution. The console starts at the top-left
 * of the content area.
 */
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

    // Initialize bulk write optimization
    _inBulkWrite = false;

    // Set default button labels
    const char *buttonItems[1] = {"[M] Back"};
    _setButtonItems(buttonItems, 1);
}

/**
 * @brief Destructor
 */
ConsoleScreen::~ConsoleScreen()
{
    // Base class handles cleanup
}

/**
 * @brief Update cached screen dimensions from ContentScreen
 */
void ConsoleScreen::_updateDimensions()
{
    _contentLeft = _getContentLeft();
    _contentTop = _getContentTop();
    _contentWidth = _getContentWidth();
    _contentHeight = _getContentHeight();
}

/**
 * @brief Main loop processing for console screen updates
 *
 * Handles one-time execution timing and delegates to ContentScreen
 * for standard screen processing. Override this method in derived
 * classes for custom behavior.
 */
void ConsoleScreen::loop()
{
    // Check for one-time execution
    if (!_hasExecutedOnce && (millis() - _screenOpenTime >= 1000))
    {
        _executeOnce();
        _hasExecutedOnce = true;
    }

    // Call parent loop is not necessary since nothing is defined there
}

/**
 * @brief Handle user input actions
 *
 * Base implementation provides standard navigation.
 * Override in derived classes for custom input handling.
 *
 * @param action  The input action that occurred
 * @param offsetX Horizontal offset for analog input
 * @param offsetY Vertical offset for analog input
 * @return Screen* Pointer to screen to navigate to, or nullptr to stay current
 */
Screen *ConsoleScreen::actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY)
{
    if (!isActive())
    {
        return nullptr;
    }

    // Handle back/menu button to exit console
    if (action & (BUTTON_MENU | BUTTON_LEFT))
    {
        return nullptr; // Let base class handle navigation
    }

    // No navigation for other actions - stay on console
    return nullptr;
}

/**
 * @brief Draw the console content area
 *
 * Fills the console background and handles the rendering area setup.
 * Called automatically by ContentScreen's rendering system.
 */
void ConsoleScreen::_drawContent()
{
    _updateDimensions();

    Adafruit_GFX &gfx = M1Shield.getGFX();

    // Fill console background
    gfx.fillRect(_contentLeft, _contentTop, _contentWidth, _contentHeight, M1Shield.convertColor(_consoleBgColor));
}

/**
 * @brief Move to the next line (newline operation)
 *
 * Advances the cursor to the beginning of the next line.
 * If at the bottom of the screen, clears the entire screen and
 * starts fresh from the top-left for memory efficiency.
 */
void ConsoleScreen::_newLine()
{
    _updateDimensions();

    _currentX = 0;
    _currentY += _lineHeight;

    // Check if we've reached the bottom of the screen
    // If so, clear screen and start from top (memory efficient)
    if (_currentY + _lineHeight > _contentHeight)
    {
        cls();
    }
}

/**
 * @brief Process tab character - move to next tab stop
 *
 * Advances cursor to the next tab stop position based on
 * the configured tab size. If the tab would exceed the line
 * width, moves to the next line instead.
 */
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

/**
 * @brief Override Screen::open() to initialize timing for one-time execution
 *
 * Resets the one-time execution tracking when the console becomes active.
 * This ensures _executeOnce() will be called 1 second after opening,
 * even if the console was previously opened and closed.
 */
bool ConsoleScreen::open()
{
    // Call parent implementation first
    bool result = ContentScreen::open();

    // Reset one-time execution tracking
    _screenOpenTime = millis();
    _hasExecutedOnce = false;

    return result;
}

// Print Interface Implementation

/**
 * @brief Write a single character to the console (Print interface)
 *
 * This method implements the required Print interface method, enabling
 * all Arduino Print functionality automatically. It processes and renders
 * a single character to the console.
 *
 * @param c Character to write to console
 * @return size_t Number of bytes written (1 if successful, 0 if failed)
 */
size_t ConsoleScreen::write(uint8_t c)
{
    _processChar((char)c);
    if (isActive())
    {
        M1Shield.display(); // Push changes to display
    }
    return 1; // Always successful since _processChar handles all characters
}

/**
 * @brief Write buffer of characters to console (Print interface optimization)
 *
 * Optimized bulk write method for better performance when writing
 * multiple characters at once. Uses startWrite/endWrite for maximum
 * performance on SPI displays.
 *
 * @param buffer Pointer to character buffer
 * @param size Number of characters to write
 * @return size_t Number of bytes successfully written
 */
size_t ConsoleScreen::write(const uint8_t *buffer, size_t size)
{
    if (!isActive() || size == 0)
        return 0;

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

/**
 * @brief Clear screen and reset cursor to top-left
 *
 * Clears the entire console area and positions the cursor
 * at the top-left corner. Equivalent to calling refresh().
 */
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

/**
 * @brief Refresh screen and reset cursor to top-left
 *
 * Clears the console area and resets the cursor position.
 * Same functionality as cls() - provided for API consistency.
 */
void ConsoleScreen::refresh()
{
    cls();
}

// Console Configuration Methods

/**
 * @brief Set text color for subsequent output
 *
 * @param foreground Text foreground color (RGB565 format)
 * @param background Text background color (RGB565 format, optional)
 */
void ConsoleScreen::setTextColor(uint16_t foreground, uint16_t background)
{
    _textColor = M1Shield.convertColor(foreground);
    _textBgColor = M1Shield.convertColor(background);
}

/**
 * @brief Set console background color
 *
 * Sets the background color for the entire console area.
 * Takes effect on next refresh() or cls() call.
 *
 * @param color Background color (RGB565 format)
 */
void ConsoleScreen::setConsoleBackground(uint16_t color)
{
    _consoleBgColor = M1Shield.convertColor(color);
}

/**
 * @brief Set text size for subsequent output
 *
 * @param size Text size multiplier (1=smallest, 2=medium, 3=large, etc.)
 *
 * @note Changing text size affects line height and character width
 * @note Cursor position is maintained but may need adjustment
 */
void ConsoleScreen::setTextSize(uint8_t size)
{
    // Only allow small text on very small displays
    if (_isSmallDisplay())
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

/**
 * @brief Set tab size in characters
 *
 * @param size Number of characters per tab stop (default: 4)
 */
void ConsoleScreen::setTabSize(uint8_t size)
{
    if (size < 1)
        size = 1;
    _tabSize = size;
}

/**
 * @brief Process a single character for output
 *
 * Handles special characters like newline (\n) and tab (\t),
 * and renders printable characters to the screen.
 *
 * @param c Character to process and potentially render
 */
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

/**
 * @brief Render a single character at the current cursor position
 *
 * Draws the character with current color settings and advances
 * the cursor position. Handles line wrapping automatically.
 *
 * @param c Character to render (must be printable)
 */
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