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
constexpr uint16_t HEADER_SMALL_HEIGHT = 16; // Height of small header region in pixels
constexpr uint16_t HEADER_COLOR_BG = 0x07E0; // Header background (bright green)
constexpr uint16_t HEADER_COLOR_FG = 0x0000; // Header text color (black on green)

// Footer region styling
constexpr uint16_t FOOTER_HEIGHT = 10;       // Height of footer region in pixels
constexpr uint16_t FOOTER_SMALL_HEIGHT = 0;  // Height of small footer region in pixels
constexpr uint16_t FOOTER_COLOR_BG = 0x0000; // Footer background color
constexpr uint16_t FOOTER_COLOR_FG = 0xFFFF; // Footer text color

// Progress bar styling
constexpr uint16_t PROGRESSBAR_HEIGHT = 10;             // Height of progress bar in pixels
constexpr uint16_t PROGRESSBAR_SMALL_HEIGHT = 2;        // Height of small progress bar in pixels
constexpr uint16_t PROGRESSBAR_COLOR_BG = 0x0000;       // Progress bar background
constexpr uint16_t PROGRESSBAR_COLOR_FG = 0xFFE0;       // Progress bar foreground (yellow)
constexpr uint16_t PROGRESSBAR_SMALL_COLOR_FG = 0xFFFF; // Progress bar foreground (white)

// Notification styling
constexpr uint16_t NOTIFICATION_COLOR_BG = 0xFFE0; // Notification background (yellow)
constexpr uint16_t NOTIFICATION_COLOR_FG = 0x0000; // Notification text color (black)

// Alert and Confirmation styling
constexpr uint16_t ALERT_COLOR_BG = 0x07FF;   // Alert background (cyan)
constexpr uint16_t ALERT_COLOR_FG = 0x0000;   // Alert text color (black)
constexpr uint16_t CONFIRM_COLOR_BG = 0xF81F; // Confirm background (magenta)
constexpr uint16_t CONFIRM_COLOR_FG = 0x0000; // Confirm text color (black)

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

    // Initialize notification system
    _notificationText = nullptr;
    _notificationStartTime = 0;
    _notificationDuration = 0;
    _notificationActive = false;
}

/**
 * @brief Destructor frees all dynamically allocated memory
 *
 * Cleans up title buffer and button label buffers to prevent memory leaks.
 * This ensures proper resource cleanup when screens are destroyed.
 */
ContentScreen::~ContentScreen()
{
    clearTitle();
    clearButtonItems();
    _clearNotification();
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
    if (!isActive())
        return;

    uint16_t screenWidth = M1Shield.getScreenWidth();

    Adafruit_GFX &gfx = M1Shield.getGFX();
    gfx.fillScreen(M1Shield.convertColor(SCREEN_COLOR_BG));

    // Update notification state before drawing
    _updateNotification();

    // Render all layout regions in order
    _drawHeader();
    _drawContent(); // Implemented by derived class

    // Draw either notification or footer
    if (_notificationActive)
    {
        _drawNotification();
    }
    else
    {
        _drawFooter();
    }

    _drawProgressBar();

    // Add decorative borders if not a small display
    if (!isSmallDisplay())
    {
        // Add decorative borders for visual separation
        uint16_t contentTop = _getContentTop();
        uint16_t contentHeight = _getContentHeight();
        gfx.drawRect(0, contentTop - 1, screenWidth, contentHeight + 2, M1Shield.convertColor(SCREEN_COLOR_FG));

        // Draw separator line above progress bar
        uint16_t progressTop = _getProgressBarY();
        gfx.drawFastHLine(0, progressTop - 1, screenWidth, M1Shield.convertColor(SCREEN_COLOR_FG));
    }
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
 * @brief Get height of header region
 * @return Height in pixels of header area
 *
 * Returns the standard header height, or a smaller height for small displays.
 */
uint16_t ContentScreen::_getHeaderHeight() const
{
    // Use smaller header height for small displays
    return isSmallDisplay() ? HEADER_SMALL_HEIGHT : HEADER_HEIGHT;
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
    if (!isActive())
        return;

    uint16_t screenWidth = M1Shield.getScreenWidth();
    uint16_t top = _getHeaderY();

    Adafruit_GFX &gfx = M1Shield.getGFX();

    // Draw header background and centered title
    gfx.fillRect(0, top, screenWidth, _getHeaderHeight(), M1Shield.convertColor(HEADER_COLOR_BG));

    // Check whether title is set
    if (_title != nullptr && _title[0] != '\0')
    {
        gfx.setTextColor(M1Shield.convertColor(HEADER_COLOR_FG));
        if (isSmallDisplay())
        {
            // Calculate centered position for title text
            uint16_t textWidth = TEXT_SIZE_1_WIDTH * strlen(_title);
            gfx.setTextSize(1);

            // Adding 2 pixels as header needs to be 16 pixel altogether
            gfx.setCursor((screenWidth - textWidth) / 2, top + TEXT_SIZE_1_HALF_HEIGHT + 2);
        }
        else
        {
            // Calculate centered position for title text
            uint16_t textWidth = TEXT_SIZE_3_WIDTH * strlen(_title);
            gfx.setTextSize(3);
            gfx.setCursor((screenWidth - textWidth) / 2, top + TEXT_SIZE_3_HALF_HEIGHT);
        }
        gfx.print(_title);
    }
}

/**
 * @brief Get Y coordinate of content area start
 * @return Pixel position
 */
uint16_t ContentScreen::_getContentTop() const
{
    uint8_t padding = _getPadding();
    return _getHeaderHeight() + padding;
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
    uint8_t padding = _getPadding();
    return M1Shield.getScreenHeight() - _getHeaderHeight() - padding - _getFooterHeight() - padding - _getProgressBarHeight() - padding;
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
    uint8_t padding = _getPadding();
    uint16_t bottom = screenHeight - _getProgressBarHeight() - padding;
    return bottom - _getFooterHeight();
}

/**
 * @brief Get height of footer region
 * @return Height in pixels of footer area
 *
 * Returns the standard footer height, or a smaller height for small displays.
 */
uint16_t ContentScreen::_getFooterHeight() const
{
    // Use smaller footer height for small displays
    return isSmallDisplay() ? FOOTER_SMALL_HEIGHT : FOOTER_HEIGHT;
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
    if (!isActive() || isSmallDisplay())
        return;

    uint16_t screenWidth = M1Shield.getScreenWidth();
    uint16_t top = _getFooterY();

    Adafruit_GFX &gfx = M1Shield.getGFX();

    // Draw footer background
    gfx.fillRect(0, top, screenWidth, _getFooterHeight(), M1Shield.convertColor(FOOTER_COLOR_BG));

    // Render button labels if configured
    if (_buttonItems != nullptr && _buttonItemCount > 0)
    {
        gfx.setTextColor(M1Shield.convertColor(FOOTER_COLOR_FG));
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

                gfx.setCursor(xPos, top + TEXT_SIZE_1_HALF_HEIGHT - 1);
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
    return screenHeight - _getProgressBarHeight();
}

/**
 * @brief Get height of progress bar region
 * @return Height in pixels of progress bar area
 */
uint16_t ContentScreen::_getProgressBarHeight() const
{
    // Use smaller progress bar height for small displays
    return isSmallDisplay() ? PROGRESSBAR_SMALL_HEIGHT : PROGRESSBAR_HEIGHT;
}

/**
 * @brief Gets the padding between areas
 * @return Padding in pixels
 */
uint8_t ContentScreen::_getPadding() const
{
    // Use smaller padding for small displays to maximize content area
    if (isSmallDisplay())
    {
        return 0; // 0 pixel padding for small displays
    }

    return 2; // 2 pixels padding between regions
}

/**
 * @brief Draw the progress bar with current value
 */
void ContentScreen::_drawProgressBar()
{
    if (!isActive())
        return;

    uint16_t screenWidth = M1Shield.getScreenWidth();
    uint16_t top = _getProgressBarY();

    Adafruit_GFX &gfx = M1Shield.getGFX();

    // Calculate filled width based on progress percentage
    uint16_t progressWidth = (screenWidth * _progressValue) / 100;

    // Draw filled (progress) portion in blue
    uint16_t height = _getProgressBarHeight();

    bool isSmall = isSmallDisplay();
    if (isSmall)
    {
        // Make it even smaller, but add a gap to content
        height--;
        top++;
    }

    if (progressWidth > 0)
    {
        gfx.fillRect(0, top, progressWidth, height, M1Shield.convertColor(isSmall ? PROGRESSBAR_SMALL_COLOR_FG : PROGRESSBAR_COLOR_FG));
    }

    // Draw remaining (empty) portion in black
    if (progressWidth < screenWidth)
    {
        gfx.fillRect(progressWidth, top, screenWidth - progressWidth, height, M1Shield.convertColor(PROGRESSBAR_COLOR_BG));
    }
}

/**
 * @brief Set footer button items from FlashString array
 *
 * Convenience method for setting button items from FlashString array.
 * Converts FlashString array to regular string array and delegates to setButtonItems().
 *
 * @param buttonItems Array of button labels stored in flash memory
 * @param buttonItemCount Number of button items in the array
 *
 * @note More memory efficient than storing button strings in RAM
 * @note Follows same allocation and update behavior as setButtonItems()
 */
void ContentScreen::setButtonItemsF(const __FlashStringHelper **buttonItems, uint8_t buttonItemCount)
{
    if (buttonItems == nullptr || buttonItemCount <= 0)
    {
        clearButtonItems();
        return;
    }

    // Allocate array of string pointers
    char **stringArray = (char **)malloc(buttonItemCount * sizeof(char *));
    if (stringArray == nullptr)
    {
        clearButtonItems();
        return;
    }

    // Initialize all pointers to nullptr for safe cleanup
    for (uint8_t i = 0; i < buttonItemCount; i++)
    {
        stringArray[i] = nullptr;
    }

    // Convert each FlashString to regular string
    for (uint8_t i = 0; i < buttonItemCount; i++)
    {
        if (buttonItems[i] != nullptr)
        {
            size_t len = strlen_P((const char *)buttonItems[i]);
            stringArray[i] = (char *)malloc(len + 1);
            if (stringArray[i] != nullptr)
            {
                strcpy_P(stringArray[i], (const char *)buttonItems[i]);
            }
        }
    }

    // Delegate to regular setButtonItems method
    setButtonItems((const char **)stringArray, buttonItemCount);

    // Free temporary string array
    for (uint8_t i = 0; i < buttonItemCount; i++)
    {
        free(stringArray[i]);
    }
    free(stringArray);
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
void ContentScreen::setButtonItems(const char **buttonItems, uint8_t buttonItemCount)
{
    clearButtonItems();

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
    {
        Adafruit_GFX &gfx = M1Shield.getGFX();
        gfx.startWrite();
        _drawFooter();
        gfx.endWrite();
        M1Shield.display(); // Push changes to display
    }
}

/**
 * @brief Clear all button labels and free allocated memory
 *
 * This method frees any dynamically allocated button label memory
 * and resets the button item count to 0. Call this to clear all
 * button labels without needing to set new ones.
 */
void ContentScreen::clearButtonItems()
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

    // Update footer immediately if screen is active
    if (isActive())
    {
        Adafruit_GFX &gfx = M1Shield.getGFX();
        gfx.startWrite();
        _drawFooter();
        gfx.endWrite();
        M1Shield.display(); // Push changes to display
    }
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
void ContentScreen::setTitle(const char *title)
{
    clearTitle();

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
    {
        Adafruit_GFX &gfx = M1Shield.getGFX();
        gfx.startWrite();
        _drawHeader();
        gfx.endWrite();
        M1Shield.display(); // Push changes to display
    }
}

/**
 * @brief Set screen title from FlashString
 *
 * Convenience method for setting title from FlashString (F() macro).
 * Converts the FlashString to a regular string and delegates to setTitle().
 *
 * @param title Title text stored in flash memory (from F() macro)
 *
 * @note More memory efficient than storing title strings in RAM
 * @note Follows same allocation and update behavior as setTitle()
 */
void ContentScreen::setTitleF(const __FlashStringHelper *title)
{
    if (title == nullptr)
    {
        clearTitle();
        return;
    }

    // Get length and allocate buffer
    size_t len = strlen_P((const char *)title);
    char *buffer = (char *)malloc(len + 1);
    if (buffer == nullptr)
    {
        clearTitle();
        return;
    }

    // Copy from flash to RAM buffer
    strcpy_P(buffer, (const char *)title);

    // Delegate to regular setTitle method
    setTitle(buffer);

    // Free temporary buffer
    free(buffer);
}

/**
 * @brief Clear the current title
 *
 * Frees any dynamically allocated title memory and resets the title
 * to nullptr. Call this to clear the title without needing to set a new one.
 */
void ContentScreen::clearTitle()
{
    if (_title != nullptr)
    {
        free(_title);
        _title = nullptr;
    }

    // Update footer immediately if screen is active
    if (isActive())
    {
        Adafruit_GFX &gfx = M1Shield.getGFX();
        gfx.startWrite();
        _drawFooter();
        gfx.endWrite();
        M1Shield.display(); // Push changes to display
    }
}

/**
 * @brief Get the current screen title
 * @return Current title string, or nullptr if no title is set
 */
const char *ContentScreen::getTitle() const
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
void ContentScreen::setProgressValue(int value)
{
    // Clamp value to valid range (0-100)
    if (value > 100)
        value = 100;
    if (value < 0)
        value = 0;

    _progressValue = value;

    // Update progress bar immediately if screen is active
    if (isActive())
    {
        Adafruit_GFX &gfx = M1Shield.getGFX();
        gfx.startWrite();
        _drawProgressBar();
        gfx.endWrite();
        M1Shield.display(); // Push changes to display
    }
}

/**
 * @brief Get the current progress bar value
 * @return Progress percentage (0-100)
 */
uint8_t ContentScreen::getProgressValue() const
{
    return _progressValue;
}

/**
 * @brief Clear content area to background color
 *
 * Convenience method to clear just the content region while
 * preserving header, footer, and progress bar areas.
 */
void ContentScreen::clearContentArea()
{
    if (!isActive())
        return;

    uint16_t x = _getContentLeft();
    uint16_t y = _getContentTop();
    uint16_t width = _getContentWidth();
    uint16_t height = _getContentHeight();

    Adafruit_GFX &gfx = M1Shield.getGFX();
    gfx.startWrite();
    gfx.fillRect(x, y, width, height, M1Shield.convertColor(SCREEN_COLOR_BG));
    gfx.endWrite();

    M1Shield.display(); // Push changes to display
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
void ContentScreen::drawText(uint16_t x, uint16_t y, const char *text, uint16_t color, uint8_t size)
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
    gfx.setTextColor(M1Shield.convertColor(color));
    gfx.setTextSize(size);
    gfx.setCursor(absoluteX, absoluteY);
    gfx.print(text);
}

/**
 * @brief Draw text in content area from FlashString (F() macro)
 *
 * Memory-efficient version of drawText() that accepts FlashString for static text.
 * Coordinates are relative to the content area, not the full screen.
 *
 * @param x X position relative to content area
 * @param y Y position relative to content area
 * @param text FlashString text to display (from F() macro)
 * @param color Text color
 * @param size Text size multiplier
 *
 * @note More memory efficient than regular drawText() for static text
 * @note FlashString is automatically converted for display
 * @note Coordinates are relative to content area, not screen
 * @note Text extending beyond content area will be clipped
 */
void ContentScreen::drawTextF(uint16_t x, uint16_t y, const __FlashStringHelper *text, uint16_t color, uint8_t size)
{
    if (!isActive())
        return;

    if (text == nullptr)
    {
        return; // Nothing to draw
    }

    // Convert FlashString to regular string and delegate to existing method
    size_t len = strlen_P((const char *)text);
    char *buffer = (char *)malloc(len + 1);
    if (buffer == nullptr)
    {
        return; // Failed allocation
    }

    strcpy_P(buffer, (const char *)text);

    // Delegate to regular drawText method
    drawText(x, y, buffer, color, size);

    // Free temporary buffer
    free(buffer);
}

// =====================================================================================
// Notification System Implementation
// =====================================================================================

/**
 * @brief Show a notification that temporarily replaces the footer
 * @param text Notification text to display (dynamically allocated copy made)
 * @param durationMs How long to show notification in milliseconds
 */
void ContentScreen::notify(const char *text, unsigned long durationMs)
{
    if (text == nullptr)
        return;

    // Clear any existing notification
    _clearNotification();

    // Allocate and copy notification text
    size_t len = strlen(text);
    _notificationText = (char *)malloc(len + 1);
    if (_notificationText == nullptr)
        return; // Failed allocation

    strcpy(_notificationText, text);

    // Set notification timing
    _notificationStartTime = millis();
    _notificationDuration = durationMs;
    _notificationActive = true;

    // Trigger immediate redraw to show notification
    if (isActive())
    {
        refresh();
    }
}

/**
 * @brief Show a notification from FlashString (F() macro)
 * @param text FlashString notification text (automatically converted and copied)
 * @param durationMs How long to show notification in milliseconds
 */
void ContentScreen::notifyF(const __FlashStringHelper *text, unsigned long durationMs)
{
    if (text == nullptr)
        return;

    // Convert FlashString to regular string
    size_t len = strlen_P((const char *)text);
    char *buffer = (char *)malloc(len + 1);
    if (buffer == nullptr)
        return; // Failed allocation

    strcpy_P(buffer, (const char *)text);

    // Delegate to regular notify method
    notify(buffer, durationMs);

    // Free temporary buffer
    free(buffer);
}

/**
 * @brief Check if a notification is currently active
 * @return true if notification is being displayed, false otherwise
 */
bool ContentScreen::isNotificationActive() const
{
    return _notificationActive;
}

/**
 * @brief Manually dismiss current notification
 */
void ContentScreen::dismissNotification()
{
    if (_notificationActive)
    {
        _clearNotification();

        // Restore footer efficiently (no need for full screen refresh)
        if (isActive())
        {
            Adafruit_GFX &gfx = M1Shield.getGFX();
            gfx.startWrite();
            _drawFooter();
            gfx.endWrite();
            M1Shield.display();
        }
    }
}

/**
 * @brief Draw notification overlay in place of footer
 */
void ContentScreen::_drawNotification()
{
    if (!isActive() || !_notificationActive || _notificationText == nullptr || isSmallDisplay())
        return;

    uint16_t screenWidth = M1Shield.getScreenWidth();
    uint16_t top = _getFooterY();
    uint16_t height = _getFooterHeight();

    Adafruit_GFX &gfx = M1Shield.getGFX();

    // Draw notification background (yellow)
    gfx.fillRect(0, top, screenWidth, height, M1Shield.convertColor(NOTIFICATION_COLOR_BG));

    // Draw notification text (black on yellow)
    gfx.setTextColor(M1Shield.convertColor(NOTIFICATION_COLOR_FG));
    gfx.setTextSize(1);

    // Center text horizontally
    uint16_t textWidth = TEXT_SIZE_1_WIDTH * strlen(_notificationText);
    uint16_t xPos = (screenWidth - textWidth) / 2;

    // Center text vertically in footer area
    uint16_t yPos = top + (height - 8) / 2; // 8 is approximate height of size-1 text

    gfx.setCursor(xPos, yPos);
    gfx.print(_notificationText);
}

/**
 * @brief Update notification state and check for expiration
 */
void ContentScreen::_updateNotification()
{
    if (!_notificationActive)
        return;

    // Check if notification has expired
    unsigned long currentTime = millis();
    if (currentTime - _notificationStartTime >= _notificationDuration)
    {
        _clearNotification();
    }
}

/**
 * @brief Clear notification text and free memory
 */
void ContentScreen::_clearNotification()
{
    if (_notificationText != nullptr)
    {
        free(_notificationText);
        _notificationText = nullptr;
    }

    _notificationActive = false;
    _notificationStartTime = 0;
    _notificationDuration = 0;
}

// =====================================================================================
// Alert and Confirmation System Implementation
// =====================================================================================

/**
 * @brief Show a blocking alert dialog with magenta background
 * @param text Alert message to display (center-aligned)
 */
void ContentScreen::alert(const char *text)
{
    if (!isActive() || text == nullptr || isSmallDisplay())
        return;

    // Clear any existing notification to prevent conflicts
    _clearNotification();

    // Draw the alert dialog
    _drawAlert(text);
    M1Shield.display();

    // Block execution until LEFT or RIGHT button is pressed
    while (true)
    {
        M1Shield.loop(); // Keep the shield processing

        if (M1Shield.wasLeftPressed() || M1Shield.wasRightPressed())
        {
            break; // Exit on either button press
        }

        delay(10); // Small delay to prevent excessive CPU usage
    }

    // Restore the footer efficiently (no need for full screen refresh)
    Adafruit_GFX &gfx = M1Shield.getGFX();
    gfx.startWrite();
    _drawFooter();
    gfx.endWrite();
    M1Shield.display();
}

/**
 * @brief Show a blocking alert dialog from FlashString (F() macro)
 * @param text FlashString alert message (automatically converted)
 */
void ContentScreen::alertF(const __FlashStringHelper *text)
{
    if (text == nullptr)
        return;

    // Convert FlashString to regular string
    size_t len = strlen_P((const char *)text);
    char *buffer = (char *)malloc(len + 1);
    if (buffer == nullptr)
        return; // Failed allocation

    strcpy_P(buffer, (const char *)text);

    // Delegate to regular alert method
    alert(buffer);

    // Free temporary buffer
    free(buffer);
}

/**
 * @brief Show a blocking confirmation dialog with magenta background
 * @param text Main confirmation message (center-aligned)
 * @param leftText Left button text (left-aligned)
 * @param rightText Right button text (right-aligned)
 * @return CONFIRM_LEFT if left button pressed, CONFIRM_RIGHT if right button pressed
 */
ConfirmResult ContentScreen::confirm(const char *text, const char *leftText, const char *rightText)
{
    if (!isActive() || text == nullptr || isSmallDisplay())
        return CONFIRM_LEFT; // Default to left/cancel for safety

    // Clear any existing notification to prevent conflicts
    _clearNotification();

    // Draw the confirmation dialog
    _drawConfirm(text, leftText, rightText);
    M1Shield.display();

    // Block execution until LEFT or RIGHT button is pressed
    while (true)
    {
        M1Shield.loop(); // Keep the shield processing

        if (M1Shield.wasLeftPressed())
        {
            // Restore the footer efficiently and return left choice
            Adafruit_GFX &gfx = M1Shield.getGFX();
            gfx.startWrite();
            _drawFooter();
            gfx.endWrite();
            M1Shield.display();
            return CONFIRM_LEFT;
        }
        else if (M1Shield.wasRightPressed())
        {
            // Restore the footer efficiently and return right choice
            Adafruit_GFX &gfx = M1Shield.getGFX();
            gfx.startWrite();
            _drawFooter();
            gfx.endWrite();
            M1Shield.display();
            return CONFIRM_RIGHT;
        }

        delay(10); // Small delay to prevent excessive CPU usage
    }
}

/**
 * @brief Show a blocking confirmation dialog from FlashString (F() macro)
 * @param text FlashString main message (automatically converted)
 * @param leftText Left button text (left-aligned)
 * @param rightText Right button text (right-aligned)
 * @return CONFIRM_LEFT if left button pressed, CONFIRM_RIGHT if right button pressed
 */
ConfirmResult ContentScreen::confirmF(const __FlashStringHelper *text, const char *leftText, const char *rightText)
{
    if (text == nullptr)
        return CONFIRM_LEFT; // Default to left/cancel for safety

    // Convert FlashString to regular string
    size_t len = strlen_P((const char *)text);
    char *buffer = (char *)malloc(len + 1);
    if (buffer == nullptr)
        return CONFIRM_LEFT; // Failed allocation, default to left/cancel

    strcpy_P(buffer, (const char *)text);

    // Delegate to regular confirm method
    ConfirmResult result = confirm(buffer, leftText, rightText);

    // Free temporary buffer
    free(buffer);

    return result;
}

/**
 * @brief Draw alert dialog overlay in place of footer
 * @param text Alert message to display
 */
void ContentScreen::_drawAlert(const char *text)
{
    if (!isActive() || text == nullptr || isSmallDisplay())
        return;

    uint16_t screenWidth = M1Shield.getScreenWidth();
    uint16_t top = _getFooterY();
    uint16_t height = _getFooterHeight();

    Adafruit_GFX &gfx = M1Shield.getGFX();

    // Draw alert background (cyan)
    gfx.fillRect(0, top, screenWidth, height, M1Shield.convertColor(ALERT_COLOR_BG));

    // Draw alert text (black on cyan)
    gfx.setTextColor(M1Shield.convertColor(ALERT_COLOR_FG));
    gfx.setTextSize(1);

    // Center text horizontally
    uint16_t textWidth = TEXT_SIZE_1_WIDTH * strlen(text);
    uint16_t xPos = (screenWidth - textWidth) / 2;

    // Center text vertically in footer area
    uint16_t yPos = top + (height - 8) / 2; // 8 is approximate height of size-1 text

    gfx.setCursor(xPos, yPos);
    gfx.print(text);
}

/**
 * @brief Draw confirm dialog overlay in place of footer
 * @param text Main message to display
 * @param leftText Left button text (left-aligned)
 * @param rightText Right button text (right-aligned)
 */
void ContentScreen::_drawConfirm(const char *text, const char *leftText, const char *rightText)
{
    if (!isActive() || text == nullptr || isSmallDisplay())
        return;

    uint16_t screenWidth = M1Shield.getScreenWidth();
    uint16_t top = _getFooterY();
    uint16_t height = _getFooterHeight();

    Adafruit_GFX &gfx = M1Shield.getGFX();

    // Draw confirm background (magenta)
    gfx.fillRect(0, top, screenWidth, height, M1Shield.convertColor(CONFIRM_COLOR_BG));

    // Draw confirm text (black on magenta)
    gfx.setTextColor(M1Shield.convertColor(CONFIRM_COLOR_FG));
    gfx.setTextSize(1);

    // Calculate positions for three text elements
    uint16_t textY = top + (height - 8) / 2; // Vertically center all text

    // Left button text (left-aligned)
    if (leftText != nullptr && leftText[0] != '\0')
    {
        gfx.setCursor(2, textY); // Small margin from left edge
        gfx.print(leftText);
    }

    // Main message (center-aligned)
    uint16_t mainTextWidth = TEXT_SIZE_1_WIDTH * strlen(text);
    uint16_t mainTextX = (screenWidth - mainTextWidth) / 2;
    gfx.setCursor(mainTextX, textY);
    gfx.print(text);

    // Right button text (right-aligned)
    if (rightText != nullptr && rightText[0] != '\0')
    {
        uint16_t rightTextWidth = TEXT_SIZE_1_WIDTH * strlen(rightText);
        uint16_t rightTextX = screenWidth - rightTextWidth - 2; // Small margin from right edge
        gfx.setCursor(rightTextX, textY);
        gfx.print(rightText);
    }
}