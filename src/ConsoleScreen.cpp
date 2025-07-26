#include "ConsoleScreen.h"
#include "M1Shield.h"

// Text size constants for different sizes
constexpr uint8_t CHAR_WIDTH_SIZE_1 = 6;   // Width of size-1 characters
constexpr uint8_t CHAR_HEIGHT_SIZE_1 = 8;  // Height of size-1 characters
constexpr uint8_t CHAR_WIDTH_SIZE_2 = 12;  // Width of size-2 characters
constexpr uint8_t CHAR_HEIGHT_SIZE_2 = 16; // Height of size-2 characters
constexpr uint8_t CHAR_WIDTH_SIZE_3 = 18;  // Width of size-3 characters
constexpr uint8_t CHAR_HEIGHT_SIZE_3 = 24; // Height of size-3 characters

// Default console settings
constexpr uint8_t DEFAULT_TEXT_SIZE = 1;
constexpr uint8_t DEFAULT_TAB_SIZE = 4;
constexpr uint16_t DEFAULT_TEXT_COLOR = ST77XX_WHITE;
constexpr uint16_t DEFAULT_TEXT_BG_COLOR = ST77XX_BLACK;
constexpr uint16_t DEFAULT_CONSOLE_BG_COLOR = ST77XX_BLACK;

/**
 * @brief Constructor - initialize console with default settings
 *
 * Sets up the console with default text size, colors, and cursor position.
 * The console starts at the top-left of the content area.
 */
ConsoleScreen::ConsoleScreen() : ContentScreen()
{
    // Initialize text settings
    _textSize = DEFAULT_TEXT_SIZE;
    _textColor = DEFAULT_TEXT_COLOR;
    _textBgColor = DEFAULT_TEXT_BG_COLOR;
    _consoleBgColor = DEFAULT_CONSOLE_BG_COLOR;
    _tabSize = DEFAULT_TAB_SIZE;

    // Calculate character dimensions based on text size
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

    // Initialize cursor position
    _currentX = 0;
    _currentY = 0;

    // Cache will be updated when screen becomes active
    _contentLeft = 0;
    _contentTop = 0;
    _contentWidth = 0;
    _contentHeight = 0;

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
    _contentLeft = getContentX();
    _contentTop = getContentY();
    _contentWidth = getContentWidth();
    _contentHeight = getContentHeight();
}

/**
 * @brief Main loop processing for console screen updates
 *
 * Delegates to ContentScreen for standard screen processing.
 * Override this method in derived classes for custom behavior.
 */
void ConsoleScreen::loop()
{
    ContentScreen::loop();
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
    gfx.fillRect(_contentLeft, _contentTop, _contentWidth, _contentHeight, _consoleBgColor);
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

// Text Output Methods

/**
 * @brief Print text to console without newline
 *
 * @param text String to print to console
 */
void ConsoleScreen::print(const String &text)
{
    for (unsigned int i = 0; i < text.length(); i++)
    {
        _processChar(text.charAt(i));
    }
}

/**
 * @brief Print text to console without newline (C-string version)
 *
 * @param text Null-terminated string to print to console
 */
void ConsoleScreen::print(const char *text)
{
    if (text == nullptr)
        return;

    while (*text)
    {
        _processChar(*text);
        text++;
    }
}

/**
 * @brief Print integer to console without newline
 *
 * @param value Integer value to print
 */
void ConsoleScreen::print(int value)
{
    print(String(value));
}

/**
 * @brief Print float to console without newline
 *
 * @param value Float value to print
 * @param decimals Number of decimal places (default: 2)
 */
void ConsoleScreen::print(float value, int decimals)
{
    print(String(value, decimals));
}

/**
 * @brief Print text to console with newline
 *
 * @param text String to print to console
 */
void ConsoleScreen::println(const String &text)
{
    print(text);
    _processChar('\n');
}

/**
 * @brief Print text to console with newline (C-string version)
 *
 * @param text Null-terminated string to print to console
 */
void ConsoleScreen::println(const char *text)
{
    print(text);
    _processChar('\n');
}

/**
 * @brief Print integer to console with newline
 *
 * @param value Integer value to print
 */
void ConsoleScreen::println(int value)
{
    print(value);
    _processChar('\n');
}

/**
 * @brief Print float to console with newline
 *
 * @param value Float value to print
 * @param decimals Number of decimal places (default: 2)
 */
void ConsoleScreen::println(float value, int decimals)
{
    print(value, decimals);
    _processChar('\n');
}

/**
 * @brief Print newline only
 *
 * Moves cursor to the beginning of the next line.
 */
void ConsoleScreen::println()
{
    _processChar('\n');
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

    Adafruit_GFX &gfx = M1Shield.getGFX();

    // Clear console area
    gfx.fillRect(_contentLeft, _contentTop, _contentWidth, _contentHeight, _consoleBgColor);

    // Reset cursor to top-left
    _currentX = 0;
    _currentY = 0;
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
    _textColor = foreground;
    _textBgColor = background;
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
    _consoleBgColor = color;
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