/**
 * @file ContentScreen.cpp
 * @brief Implementation of the ContentScreen structured layout template
 *
 * This file provides the abstract implementation of the ContentScreen class,
 * which manages the standard screen layout with header, content, footer,
 * and progress bar regions.
 */

#include "ContentScreen.h"
#include "M1Shield.h"
#include <Adafruit_GFX.h>

// Text sizing constants for layout calculations
constexpr uint8_t TEXT_SIZE_1_WIDTH = 6;       // Width of size-1 text characters
constexpr uint8_t TEXT_SIZE_1_HALF_HEIGHT = 3; // Half-height of size-1 text for centering

constexpr uint8_t TEXT_SIZE_2_WIDTH = 12;      // Width of size-2 text characters
constexpr uint8_t TEXT_SIZE_2_HALF_HEIGHT = 6; // Half-height of size-2 text for centering

constexpr uint8_t TEXT_SIZE_3_WIDTH = 18;      // Width of size-3 text characters
constexpr uint8_t TEXT_SIZE_3_HALF_HEIGHT = 8; // Half-height of size-3 text for centering

// Color scheme constants for consistent theming
constexpr uint16_t SCREEN_COLOR_BG = 0x0000; // Background color for content area
constexpr uint16_t SCREEN_COLOR_FG = 0xFFFF; // Foreground color for borders and text

// Header region styling
constexpr uint16_t HEADER_HEIGHT = 40;       // Height of header region in pixels
constexpr uint16_t HEADER_COLOR_BG = 0x07E0; // Header background (bright green)
constexpr uint16_t HEADER_COLOR_FG = 0x0000; // Header text color (black on green)

// Footer region styling
constexpr uint16_t FOOTER_HEIGHT = 23;       // Height of footer region in pixels
constexpr uint16_t FOOTER_COLOR_BG = 0x0000; // Footer background color
constexpr uint16_t FOOTER_COLOR_FG = 0xFFFF; // Footer text color

// Progress bar styling
constexpr uint16_t PROGRESSBAR_HEIGHT = 10;       // Height of progress bar in pixels
constexpr uint16_t PROGRESSBAR_COLOR_BG = 0x0000; // Progress bar background
constexpr uint16_t PROGRESSBAR_COLOR_FG = 0x001F; // Progress bar foreground (blue)

/**
 * @brief Constructor initializes ContentScreen with default values
 *
 * Sets up the screen with no title, no button items, and zero progress.
 * The screen is created in inactive state and must be opened to display.
 */
ContentScreen::ContentScreen() : Screen()
{
    _title = nullptr;       // No title allocated initially
    _buttonItems = nullptr; // No button storage allocated initially
    _buttonItemCount = 0;
    _progressValue = 0;
}

/**
 * @brief Destructor frees all dynamically allocated memory
 *
 * Cleans up title buffer and button label buffers to prevent memory leaks.
 * This ensures proper resource cleanup when screens are destroyed.
 */
ContentScreen::~ContentScreen()
{
    _clearTitle();
    _clearButtonItems();
}

/**
 * @brief Implement Screen's _drawScreen() to orchestrate layout rendering
 *
 * Manages the complete screen redraw by coordinating all layout regions:
 * 1. Clear screen to background color
 * 2. Draw header with title
 * 3. Draw content area (delegated to derived class)
 * 4. Draw footer with button labels
 * 5. Draw progress bar
 * 6. Add decorative borders
 *
 * This method implements the template pattern - the layout structure is
 * fixed, but content rendering is customizable via _drawContent().
 */
void ContentScreen::_drawScreen()
{
    uint16_t screenWidth = M1Shield.getScreenWidth();
    uint16_t screenHeight = M1Shield.getScreenHeight();

    Adafruit_GFX &gfx = M1Shield.getGFX();
    gfx.fillScreen(SCREEN_COLOR_BG);

    // Render all layout regions in order
    _drawHeader();
    _drawContent(); // Implemented by derived class
    _drawFooter();
    _drawProgressBar();

    // Add decorative borders for visual separation
    uint16_t contentTop = _getContentTop();
    uint16_t contentHeight = _getContentHeight();
    gfx.drawRect(0, contentTop - 1, screenWidth, contentHeight + 2, SCREEN_COLOR_FG);

    // Draw separator line above progress bar
    uint16_t progressTop = _getProgressBarY();
    gfx.drawFastHLine(0, progressTop - 1, screenWidth, SCREEN_COLOR_FG);
}

/**
 * @brief Get Y coordinate of header region start
 * @return Always 0 (header starts at top of screen)
 */
uint16_t ContentScreen::_getHeaderY() const
{
    return 0;
}

/**
 * @brief Draw the header region with title and styling
 *
 * Renders the header area with:
 * - Green background for visibility
 * - Centered title text in large font (size 3)
 * - Black text on green background for contrast
 *
 * @note Only draws if a title has been set via _setTitle()
 */
void ContentScreen::_drawHeader()
{
    uint16_t screenWidth = M1Shield.getScreenWidth();
    uint16_t top = _getHeaderY();

    Adafruit_GFX &gfx = M1Shield.getGFX();

    // Draw header background and centered title
    gfx.fillRect(0, top, screenWidth, HEADER_HEIGHT, HEADER_COLOR_BG);

    // Check whether title is set
    if (_title != nullptr && _title[0] != '\0')
    {
        // Calculate centered position for title text
        uint16_t textWidth = TEXT_SIZE_3_WIDTH * strlen(_title);

        gfx.setTextColor(HEADER_COLOR_FG);
        gfx.setTextSize(3);
        gfx.setCursor((screenWidth - textWidth) / 2, top + TEXT_SIZE_3_HALF_HEIGHT);
        gfx.print(_title);
    }
}

/**
 * @brief Get Y coordinate of content area start
 * @return Pixel position
 */
uint16_t ContentScreen::_getContentTop() const
{
    return HEADER_HEIGHT + 2;
}

/**
 * @brief Get X coordinate of content area start
 * @return Pixel position
 */
uint16_t ContentScreen::_getContentLeft() const
{
    return 1;
}

/**
 * @brief Calculate available height for content area
 * @return Height in pixels
 */
uint16_t ContentScreen::_getContentHeight() const
{
    return M1Shield.getScreenHeight() - HEADER_HEIGHT - FOOTER_HEIGHT - PROGRESSBAR_HEIGHT - 6; // Always 2px padding
}

/**
 * @brief Get available width for content area
 * @return Width in pixels
 */
uint16_t ContentScreen::_getContentWidth() const
{
    return M1Shield.getScreenWidth() - 2;
}

/**
 * @brief Calculate Y coordinate of footer region start
 * @return Pixel position of footer, accounting for progress bar below
 */
uint16_t ContentScreen::_getFooterY() const
{
    uint16_t screenHeight = M1Shield.getScreenHeight();
    uint16_t bottom = screenHeight - PROGRESSBAR_HEIGHT - 2; // 2px padding above progress bar
    return bottom - FOOTER_HEIGHT;
}

/**
 * @brief Draw the footer region with button labels from internal buffers
 *
 * Renders button labels stored in internal buffers, distributing them
 * evenly across the footer width. Only draws labels that are non-empty.
 *
 * @note Only draws if button count > 0 and buffers contain valid strings
 */
void ContentScreen::_drawFooter()
{
    uint16_t screenWidth = M1Shield.getScreenWidth();
    uint16_t top = _getFooterY();

    Adafruit_GFX &gfx = M1Shield.getGFX();

    // Draw footer background
    gfx.fillRect(0, top, screenWidth, FOOTER_HEIGHT, FOOTER_COLOR_BG);

    // Render button labels if configured
    if (_buttonItems != nullptr && _buttonItemCount > 0)
    {
        gfx.setTextColor(FOOTER_COLOR_FG);
        gfx.setTextSize(1);

        // Distribute buttons evenly across footer width
        uint16_t xDistance = screenWidth / (_buttonItemCount + 1);
        for (uint8_t i = 0; i < _buttonItemCount; i++)
        {
            // Check if button is allocated and not empty
            if (_buttonItems[i] != nullptr && _buttonItems[i][0] != '\0')
            {
                // Center each button label in its allocated space
                uint16_t textWidth = TEXT_SIZE_1_WIDTH * strlen(_buttonItems[i]);
                uint16_t xPos = xDistance * (i + 1) - (textWidth / 2);

                gfx.setCursor(xPos, top + TEXT_SIZE_1_HALF_HEIGHT);
                gfx.print(_buttonItems[i]);
            }
        }
    }
}

/**
 * @brief Calculate Y coordinate of progress bar region start
 * @return Pixel position
 */
uint16_t ContentScreen::_getProgressBarY() const
{
    uint16_t screenHeight = M1Shield.getScreenHeight();
    return screenHeight - PROGRESSBAR_HEIGHT;
}

/**
 * @brief Draw the progress bar with current value
 */
void ContentScreen::_drawProgressBar()
{
    uint16_t screenWidth = M1Shield.getScreenWidth();
    uint16_t top = _getProgressBarY();

    Adafruit_GFX &gfx = M1Shield.getGFX();

    // Calculate filled width based on progress percentage
    uint16_t progressWidth = (screenWidth * _progressValue) / 100;

    // Draw filled (progress) portion in blue
    if (progressWidth > 0)
    {
        gfx.fillRect(0, top, progressWidth, PROGRESSBAR_HEIGHT, PROGRESSBAR_COLOR_FG);
    }

    // Draw remaining (empty) portion in black
    if (progressWidth < screenWidth)
    {
        gfx.fillRect(progressWidth, top, screenWidth - progressWidth, PROGRESSBAR_HEIGHT, PROGRESSBAR_COLOR_BG);
    }
}

/**
 * @brief Set button labels with efficient dynamic memory allocation
 *
 * Allocates exactly the memory needed for each button label string, freeing any
 * previously allocated button memory. This minimizes memory usage while
 * ensuring labels remain valid regardless of source string lifetime.
 *
 * @param buttonItems Array of string pointers for button labels (can be temporary)
 * @param buttonItemCount Number of strings in the array (no hard limit)
 *
 * @note Memory is allocated dynamically - only uses space needed for each string
 * @note Pass nullptr or 0 count to free all button memory and clear labels
 * @note Automatically triggers footer redraw if screen is active
 */
void ContentScreen::_setButtonItems(const char **buttonItems, uint8_t buttonItemCount)
{
    _clearButtonItems();

    // Allocate and copy new button labels if provided
    if (buttonItems != nullptr && buttonItemCount > 0)
    {
        // Allocate array of string pointers
        _buttonItems = (char **)malloc(buttonItemCount * sizeof(char *));
        if (_buttonItems != nullptr)
        {
            _buttonItemCount = buttonItemCount;

            // Initialize all pointers to nullptr first
            for (uint8_t i = 0; i < buttonItemCount; i++)
            {
                _buttonItems[i] = nullptr;
            }

            // Allocate and copy each button label
            for (uint8_t i = 0; i < buttonItemCount; i++)
            {
                if (buttonItems[i] != nullptr && buttonItems[i][0] != '\0')
                {
                    size_t labelLen = strlen(buttonItems[i]);
                    _buttonItems[i] = (char *)malloc(labelLen + 1); // +1 for null terminator
                    if (_buttonItems[i] != nullptr)
                    {
                        strcpy(_buttonItems[i], buttonItems[i]); // Safe because we allocated exact size needed
                    }
                }
            }
        }
    }

    // Update footer immediately if screen is active
    if (isActive())
        _drawFooter();
}

/**
 * @brief Clear all button labels and free allocated memory
 *
 * This method frees any dynamically allocated button label memory
 * and resets the button item count to 0. Call this to clear all
 * button labels without needing to set new ones.
 */
void ContentScreen::_clearButtonItems()
{
    if (_buttonItems != nullptr)
    {
        for (uint8_t i = 0; i < _buttonItemCount; i++)
        {
            if (_buttonItems[i] != nullptr)
            {
                free(_buttonItems[i]);
            }
        }
        free(_buttonItems);
        _buttonItems = nullptr;
    }
    _buttonItemCount = 0;
}

/**
 * @brief Set the screen title with efficient dynamic memory allocation
 *
 * Allocates exactly the memory needed for the title string, freeing any
 * previously allocated title memory. This minimizes memory usage while
 * ensuring the title remains valid regardless of source string lifetime.
 *
 * @param title Null-terminated string for title display (can be temporary)
 *
 * @note Memory is allocated dynamically - only uses space needed for the string
 * @note Pass nullptr or empty string to free title memory and clear title
 * @note Automatically triggers header redraw if screen is active
 */
void ContentScreen::_setTitle(const char *title)
{
    _clearTitle();

    // Allocate and copy new title if provided
    if (title != nullptr && title[0] != '\0')
    {
        size_t titleLen = strlen(title);
        _title = (char *)malloc(titleLen + 1); // +1 for null terminator
        if (_title != nullptr)
        {
            strcpy(_title, title); // Safe because we allocated exact size needed
        }
    }

    // Update header immediately if screen is active
    if (isActive())
        _drawHeader();
}

/**
 * @brief Clear the current title
 *
 * Frees any dynamically allocated title memory and resets the title
 * to nullptr. Call this to clear the title without needing to set a new one.
 */
void ContentScreen::_clearTitle()
{
    if (_title != nullptr)
    {
        free(_title);
        _title = nullptr;
    }
}

/**
 * @brief Get the current screen title
 * @return Current title string, or nullptr if no title is set
 */
const char *ContentScreen::_getTitle() const
{
    return _title; // May be nullptr if no title set
}

/**
 * @brief Set progress bar value with immediate update and bounds checking
 *
 * Updates the progress bar value and immediately redraws the progress
 * bar if the screen is currently active. The value is automatically
 * clamped to the valid range of 0-100.
 *
 * @param value Progress percentage (0-100, automatically clamped)
 *
 * @note Setting value to 0 effectively hides the progress bar
 * @note Values outside 0-100 range are automatically corrected
 */
void ContentScreen::_setProgressValue(int value)
{
    // Clamp value to valid range (0-100)
    if (value > 100)
        value = 100;
    if (value < 0)
        value = 0;

    _progressValue = value;

    // Update progress bar immediately if screen is active
    if (isActive())
        _drawProgressBar();
}

/**
 * @brief Get the current progress bar value
 * @return Progress percentage (0-100)
 */
uint8_t ContentScreen::_getProgressValue() const
{
    return _progressValue;
}

/**
 * @brief Clear content area to background color
 *
 * Convenience method to clear just the content region while
 * preserving header, footer, and progress bar areas.
 */
void ContentScreen::_clearContentArea()
{
    uint16_t x = _getContentLeft();
    uint16_t y = _getContentTop();
    uint16_t width = _getContentWidth();
    uint16_t height = _getContentHeight();

    Adafruit_GFX &gfx = M1Shield.getGFX();
    gfx.fillRect(x, y, width, height, 0x0000);
}

/**
 * @brief Draw text within content area with clipping
 *
 * Renders text at the specified position within the content area.
 * Coordinates are relative to the content area, not the full screen.
 * Text that would extend beyond content bounds is clipped.
 *
 * @param x X position relative to content area start
 * @param y Y position relative to content area start
 * @param text Null-terminated string to display
 * @param color Text color (RGB565 format)
 * @param size Text size multiplier (1=normal, 2=double, etc.)
 *
 * @note Coordinates are relative to content area, not screen
 * @note Text extending beyond content area will be clipped
 * @note Uses current GFX cursor and color settings from M1Shield
 */
void ContentScreen::_drawText(uint16_t x, uint16_t y, const char *text, uint16_t color, uint8_t size)
{
    if (!isActive())
        return;

    if (text == nullptr || text[0] == '\0')
    {
        return; // Nothing to draw
    }

    // Convert relative coordinates to absolute screen coordinates
    uint16_t absoluteX = _getContentLeft() + x;
    uint16_t absoluteY = _getContentTop() + y;

    // Get content area bounds for clipping
    uint16_t contentRight = _getContentLeft() + _getContentWidth();
    uint16_t contentBottom = _getContentTop() + _getContentHeight();

    // Skip drawing if text position is outside content area
    if (absoluteX >= contentRight || absoluteY >= contentBottom)
    {
        return;
    }

    Adafruit_GFX &gfx = M1Shield.getGFX();

    // Set text properties and draw
    gfx.setTextColor(color);
    gfx.setTextSize(size);
    gfx.setCursor(absoluteX, absoluteY);
    gfx.print(text);
}