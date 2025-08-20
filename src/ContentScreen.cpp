/*
 * ContentScreen.cpp - Structured layout template for screens with header, content, footer, and progress bar
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
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
constexpr uint16_t FOOTER_HEIGHT = 20;       // Height of footer region in pixels (increased for text size 2)
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

// Constructor
ContentScreen::ContentScreen() : Screen()
{
    _buttonItems = nullptr; // No button storage allocated initially
    _buttonItemCount = 0;
    _progressValue = 0;

    // Initialize notification system
    _notificationText = nullptr;
    _notificationStartTime = 0;
    _notificationDuration = 0;
    _notificationActive = false;
}

// Destructor
ContentScreen::~ContentScreen()
{
    clearButtonItems();
    _clearNotification();
}

// Draw the complete screen layout with header, content, footer, and progress bar
void ContentScreen::_drawScreen()
{
    if (!isActive())
        return;

    uint16_t screenWidth = M1Shield.getScreenWidth();

    Adafruit_GFX &gfx = M1Shield.getGFX();
    gfx.fillScreen(M1Shield.convertColor(SCREEN_COLOR_BG));

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

void ContentScreen::loop()
{
    // Base ContentScreen loop handles notification timeout management
    // Derived classes can override this but should call ContentScreen::loop() first

    if (_notificationActive)
    {
        // Check notification timeout more frequently when active
        static unsigned long lastNotificationCheck = 0;
        unsigned long currentTime = millis();

        if (currentTime - lastNotificationCheck >= 200) // Check every 200ms
        {
            lastNotificationCheck = currentTime;

            // Check if notification should expire
            if (currentTime - _notificationStartTime >= _notificationDuration)
            {
                // Notification has expired - clear it and redraw footer immediately
                _clearNotification();

                // Immediately redraw the footer to show the change
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
    }
}

uint16_t ContentScreen::_getHeaderY() const
{
    return 0;
}

uint16_t ContentScreen::_getHeaderHeight() const
{
    // Use smaller header height for small displays
    return isSmallDisplay() ? HEADER_SMALL_HEIGHT : HEADER_HEIGHT;
}

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
    const char *title = getTitle();
    if (title != nullptr && title[0] != '\0')
    {
        gfx.setTextColor(M1Shield.convertColor(HEADER_COLOR_FG));
        char *truncatedTitle = nullptr;

        if (isSmallDisplay())
        {
            // Truncate title for small display if needed
            truncatedTitle = _truncateText(title, screenWidth - 4, TEXT_SIZE_1_WIDTH); // 4 pixel margin

            gfx.setTextSize(1);
            if (truncatedTitle != nullptr)
            {
                // Calculate centered position for truncated title text
                uint16_t textWidth = TEXT_SIZE_1_WIDTH * strlen(truncatedTitle);
                // Adding 2 pixels as header needs to be 16 pixel altogether
                gfx.setCursor((screenWidth - textWidth) / 2, top + TEXT_SIZE_1_HALF_HEIGHT + 2);
                gfx.print(truncatedTitle);
                free(truncatedTitle);
            }
            else
            {
                // Fallback: display original title if truncation failed
                uint16_t textWidth = TEXT_SIZE_1_WIDTH * strlen(title);
                gfx.setCursor((screenWidth - textWidth) / 2, top + TEXT_SIZE_1_HALF_HEIGHT + 2);
                gfx.print(title);
            }
        }
        else
        {
            // Truncate title for regular display if needed
            truncatedTitle = _truncateText(title, screenWidth - 4, TEXT_SIZE_3_WIDTH); // 4 pixel margin

            gfx.setTextSize(3);
            if (truncatedTitle != nullptr)
            {
                // Calculate centered position for truncated title text
                uint16_t textWidth = TEXT_SIZE_3_WIDTH * strlen(truncatedTitle);
                gfx.setCursor((screenWidth - textWidth) / 2, top + TEXT_SIZE_3_HALF_HEIGHT);
                gfx.print(truncatedTitle);
                free(truncatedTitle);
            }
            else
            {
                // Fallback: display original title if truncation failed
                uint16_t textWidth = TEXT_SIZE_3_WIDTH * strlen(title);
                gfx.setCursor((screenWidth - textWidth) / 2, top + TEXT_SIZE_3_HALF_HEIGHT);
                gfx.print(title);
            }
        }
    }
}

uint16_t ContentScreen::_getContentTop() const
{
    uint8_t padding = _getPadding();
    return _getHeaderHeight() + padding;
}

uint16_t ContentScreen::_getContentLeft() const
{
    return 1;
}

uint16_t ContentScreen::_getContentHeight() const
{
    uint8_t padding = _getPadding();
    return M1Shield.getScreenHeight() - _getHeaderHeight() - padding - _getFooterHeight() - padding - _getProgressBarHeight() - padding;
}

uint16_t ContentScreen::_getContentWidth() const
{
    return M1Shield.getScreenWidth() - 2;
}

uint16_t ContentScreen::_getFooterY() const
{
    uint16_t screenHeight = M1Shield.getScreenHeight();
    uint8_t padding = _getPadding();
    uint16_t bottom = screenHeight - _getProgressBarHeight() - padding;
    return bottom - _getFooterHeight();
}

uint16_t ContentScreen::_getFooterHeight() const
{
    // Use smaller footer height for small displays
    return isSmallDisplay() ? FOOTER_SMALL_HEIGHT : FOOTER_HEIGHT;
}

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

        // Calculate vertical center position for text in footer
        uint16_t textY = top + (_getFooterHeight() - 8) / 2; // 8 is approximate height of size-1 text

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

                gfx.setCursor(xPos, textY);
                gfx.print(_buttonItems[i]);
            }
        }
    }
}

uint16_t ContentScreen::_getProgressBarY() const
{
    uint16_t screenHeight = M1Shield.getScreenHeight();
    return screenHeight - _getProgressBarHeight();
}

uint16_t ContentScreen::_getProgressBarHeight() const
{
    // Use smaller progress bar height for small displays
    return isSmallDisplay() ? PROGRESSBAR_SMALL_HEIGHT : PROGRESSBAR_HEIGHT;
}

uint8_t ContentScreen::_getPadding() const
{
    // Use smaller padding for small displays to maximize content area
    if (isSmallDisplay())
    {
        return 0; // 0 pixel padding for small displays
    }

    return 2; // 2 pixels padding between regions
}

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
        if (getLogger())
        {
            getLogger()->errF(F("ContentScreen: Failed to allocate memory for button items array"));
        }
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
            else if (getLogger())
            {
                getLogger()->errF(F("ContentScreen: Failed to allocate memory for button item %d"), i);
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
                    else if (getLogger())
                    {
                        getLogger()->errF(F("ContentScreen: Failed to allocate memory for button label %d"), i);
                    }
                }
            }
        }
        else if (getLogger())
        {
            getLogger()->errF(F("ContentScreen: Failed to allocate memory for button items array"));
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

uint8_t ContentScreen::getProgressValue() const
{
    return _progressValue;
}

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

void ContentScreen::drawText(uint16_t x, uint16_t y, String text, uint16_t color, uint8_t size)
{
    drawText(x, y, text.c_str(), color, size);
}

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

void ContentScreen::notify(const char *text, unsigned long durationMs)
{
    if (text == nullptr)
        return;

    if (getLogger())
    {
        getLogger()->infoF(F("ContentScreen: Showing notification '%s' for %lu ms"), text, durationMs);
    }

    // Clear any existing notification
    _clearNotification();

    // Allocate and copy notification text
    size_t len = strlen(text);
    _notificationText = (char *)malloc(len + 1);
    if (_notificationText == nullptr)
    {
        if (getLogger())
        {
            getLogger()->errF(F("ContentScreen: Failed to allocate memory for notification"));
        }
        return; // Failed allocation
    }

    strcpy(_notificationText, text);

    // Set notification timing
    _notificationStartTime = millis();
    _notificationDuration = durationMs;
    _notificationActive = true;

    // Trigger immediate redraw to show notification
    if (isActive())
    {
        _drawNotification();
    }
}

void ContentScreen::notify(String text, unsigned long durationMs)
{
    notify(text.c_str(), durationMs);
}

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

bool ContentScreen::isNotificationActive() const
{
    return _notificationActive;
}

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

    // Draw notification text (black on yellow) - using text size 2 for better visibility
    gfx.setTextColor(M1Shield.convertColor(NOTIFICATION_COLOR_FG));
    gfx.setTextSize(2);

    // Truncate notification text if needed (leave 4 pixel margin)
    char *truncatedText = _truncateText(_notificationText, screenWidth - 4, TEXT_SIZE_2_WIDTH);
    if (truncatedText != nullptr)
    {
        // Center text horizontally
        uint16_t textWidth = TEXT_SIZE_2_WIDTH * strlen(truncatedText);
        uint16_t xPos = (screenWidth - textWidth) / 2;

        // Center text vertically in footer area
        uint16_t yPos = top + (height - 16) / 2; // 16 is approximate height of size-2 text

        gfx.setCursor(xPos, yPos);
        gfx.print(truncatedText);
        free(truncatedText);
    }
    else
    {
        // Fallback: display original notification text if truncation failed
        uint16_t textWidth = TEXT_SIZE_2_WIDTH * strlen(_notificationText);
        uint16_t xPos = (screenWidth - textWidth) / 2;
        uint16_t yPos = top + (height - 16) / 2;

        gfx.setCursor(xPos, yPos);
        gfx.print(_notificationText);
    }
}

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

void ContentScreen::alert(const char *text)
{
    if (!isActive() || text == nullptr || isSmallDisplay())
        return;

    // Clear any existing notification to prevent conflicts
    _clearNotification();

    if (getLogger())
    {
        getLogger()->infoF(F("ContentScreen: Showing alert '%s'"), text);
    }

    // Draw the alert dialog
    _drawAlert(text);
    M1Shield.display();

    // Block execution until LEFT or RIGHT button is pressed
    while (true)
    {
        if (M1Shield.wasLeftPressed() || M1Shield.wasRightPressed())
        {
            break; // Exit on either button press
        }

        delay(10); // Small delay to prevent excessive CPU usage
    }

    if (getLogger())
    {
        getLogger()->infoF(F("ContentScreen: Alert confirmed"));
    }

    // Restore the footer efficiently (no need for full screen refresh)
    Adafruit_GFX &gfx = M1Shield.getGFX();
    gfx.startWrite();
    _drawFooter();
    gfx.endWrite();
    M1Shield.display();
}

void ContentScreen::alert(String text)
{
    alert(text.c_str());
}

void ContentScreen::alertF(const __FlashStringHelper *text)
{
    if (text == nullptr)
        return;

    // Convert FlashString to regular string
    size_t len = strlen_P((const char *)text);
    char *buffer = (char *)malloc(len + 1);
    if (buffer == nullptr)
    {
        if (getLogger())
        {
            getLogger()->errF(F("ContentScreen: Failed to allocate memory for flash alert text"));
        }
        return; // Failed allocation
    }

    strcpy_P(buffer, (const char *)text);

    // Delegate to regular alert method
    alert(buffer);

    // Free temporary buffer
    free(buffer);
}

ConfirmResult ContentScreen::confirm(const char *text, const char *leftText, const char *rightText)
{
    if (!isActive() || text == nullptr || isSmallDisplay())
        return CONFIRM_LEFT; // Default to left/cancel for safety

    // Clear any existing notification to prevent conflicts
    _clearNotification();

    if (getLogger())
    {
        getLogger()->infoF(F("ContentScreen: Showing confirmation dialog '%s' with buttons '%s' and '%s'"), text, leftText, rightText);
    }

    // Draw the confirmation dialog
    _drawConfirm(text, leftText, rightText);
    M1Shield.display();

    // Block execution until LEFT or RIGHT button is pressed
    while (true)
    {
        if (M1Shield.wasLeftPressed())
        {
            if (getLogger())
            {
                getLogger()->infoF(F("ContentScreen: Confirmed with left button '%s'"), leftText);
            }

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
            if (getLogger())
            {
                getLogger()->infoF(F("ContentScreen: Confirmed with right button '%s'"), rightText);
            }

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

ConfirmResult ContentScreen::confirm(String text, String leftText, String rightText)
{
    // Delegate to regular confirm method using c_str()
    return confirm(text.c_str(), leftText.c_str(), rightText.c_str());
}

ConfirmResult ContentScreen::confirmF(const __FlashStringHelper *text, const __FlashStringHelper *leftText, const __FlashStringHelper *rightText)
{
    if (text == nullptr || leftText == nullptr || rightText == nullptr)
        return CONFIRM_LEFT; // Default to left/cancel for safety

    // Convert all FlashStrings to regular strings
    size_t textLen = strlen_P((const char *)text);
    size_t leftLen = strlen_P((const char *)leftText);
    size_t rightLen = strlen_P((const char *)rightText);

    char *textBuffer = (char *)malloc(textLen + 1);
    char *leftBuffer = (char *)malloc(leftLen + 1);
    char *rightBuffer = (char *)malloc(rightLen + 1);

    if (textBuffer == nullptr || leftBuffer == nullptr || rightBuffer == nullptr)
    {
        if (getLogger())
        {
            getLogger()->errF(F("ContentScreen: Failed to allocate memory for flash confirm dialog"));
        }

        // Free any successfully allocated buffers before returning
        if (textBuffer)
            free(textBuffer);
        if (leftBuffer)
            free(leftBuffer);
        if (rightBuffer)
            free(rightBuffer);
        return CONFIRM_LEFT; // Failed allocation, default to left/cancel
    }

    strcpy_P(textBuffer, (const char *)text);
    strcpy_P(leftBuffer, (const char *)leftText);
    strcpy_P(rightBuffer, (const char *)rightText);

    // Delegate to regular confirm method
    ConfirmResult result = confirm(textBuffer, leftBuffer, rightBuffer);

    // Free temporary buffers
    free(textBuffer);
    free(leftBuffer);
    free(rightBuffer);

    return result;
}

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

    // Draw alert text (black on cyan) - using text size 2 for better visibility
    gfx.setTextColor(M1Shield.convertColor(ALERT_COLOR_FG));
    gfx.setTextSize(2);

    // Calculate vertical center position
    uint16_t textY = top + (height - 16) / 2; // 16 is approximate height of size-2 text

    // Draw left indicator "<" (left-aligned)
    gfx.setCursor(2, textY);
    gfx.print("<");

    // Calculate available width for main text (account for indicators and margins)
    uint16_t indicatorWidth = TEXT_SIZE_2_WIDTH;                      // Width of "<" or ">"
    uint16_t availableWidth = screenWidth - (2 * indicatorWidth) - 8; // 8 pixels total margin

    // Truncate main text if needed
    char *truncatedText = _truncateText(text, availableWidth, TEXT_SIZE_2_WIDTH);
    if (truncatedText != nullptr)
    {
        // Center main text horizontally
        uint16_t textWidth = TEXT_SIZE_2_WIDTH * strlen(truncatedText);
        uint16_t xPos = (screenWidth - textWidth) / 2;
        gfx.setCursor(xPos, textY);
        gfx.print(truncatedText);
        free(truncatedText);
    }
    else
    {
        // Fallback: display original text if truncation failed
        uint16_t textWidth = TEXT_SIZE_2_WIDTH * strlen(text);
        uint16_t xPos = (screenWidth - textWidth) / 2;
        gfx.setCursor(xPos, textY);
        gfx.print(text);
    }

    // Draw right indicator ">" (right-aligned)
    uint16_t rightIndicatorX = screenWidth - TEXT_SIZE_2_WIDTH - 2; // TEXT_SIZE_2_WIDTH for ">" character
    gfx.setCursor(rightIndicatorX, textY);
    gfx.print(">");
}

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

    // Draw confirm text (black on magenta) - using text size 2 for better visibility
    gfx.setTextColor(M1Shield.convertColor(CONFIRM_COLOR_FG));
    gfx.setTextSize(2);

    // Calculate positions for text elements
    uint16_t textY = top + (height - 16) / 2; // 16 is approximate height of size-2 text, vertically center all text

    // Calculate space used by button text to determine available space for main message
    uint16_t leftButtonWidth = 0;
    uint16_t rightButtonWidth = 0;

    if (leftText != nullptr && leftText[0] != '\0')
    {
        leftButtonWidth = TEXT_SIZE_2_WIDTH * (strlen(leftText) + 1) + 4; // +1 for "<", +4 for margins
    }

    if (rightText != nullptr && rightText[0] != '\0')
    {
        rightButtonWidth = TEXT_SIZE_2_WIDTH * (strlen(rightText) + 1) + 4; // +1 for ">", +4 for margins
    }

    // Calculate available width for main message
    uint16_t availableWidth = screenWidth - leftButtonWidth - rightButtonWidth - 8; // 8 additional margin

    // Left button text (left-aligned) - when provided, show as indicator
    if (leftText != nullptr && leftText[0] != '\0')
    {
        gfx.setCursor(2, textY); // Small margin from left edge
        gfx.print("<");
        gfx.print(leftText);
    }

    // Main message (center-aligned) - truncated if needed
    char *truncatedText = _truncateText(text, availableWidth, TEXT_SIZE_2_WIDTH);
    if (truncatedText != nullptr)
    {
        uint16_t mainTextWidth = TEXT_SIZE_2_WIDTH * strlen(truncatedText);
        uint16_t mainTextX = (screenWidth - mainTextWidth) / 2;
        gfx.setCursor(mainTextX, textY);
        gfx.print(truncatedText);
        free(truncatedText);
    }
    else
    {
        // Fallback: display original text if truncation failed
        uint16_t mainTextWidth = TEXT_SIZE_2_WIDTH * strlen(text);
        uint16_t mainTextX = (screenWidth - mainTextWidth) / 2;
        gfx.setCursor(mainTextX, textY);
        gfx.print(text);
    }

    // Right button text (right-aligned) - when provided, show as indicator
    if (rightText != nullptr && rightText[0] != '\0')
    {
        uint16_t rightTextWidth = TEXT_SIZE_2_WIDTH * (strlen(rightText) + 1);
        uint16_t rightTextX = screenWidth - rightTextWidth - 2; // Small margin from right edge
        gfx.setCursor(rightTextX, textY);
        gfx.print(rightText);
        gfx.print(">");
    }
}

char *ContentScreen::_truncateText(const char *text, uint16_t availableWidth, uint8_t charWidth)
{
    if (text == nullptr || text[0] == '\0')
        return nullptr;

    uint16_t textLen = strlen(text);
    uint16_t textWidth = textLen * charWidth;

    // If text fits within available width, return a copy
    if (textWidth <= availableWidth)
    {
        char *copy = (char *)malloc(textLen + 1);
        if (copy == nullptr)
        {
            if (getLogger())
            {
                getLogger()->errF(F("ContentScreen: Failed to allocate memory for text copy"));
            }
            return nullptr;
        }
        strcpy(copy, text);
        return copy;
    }

    // Calculate maximum characters that fit including "..." (3 characters)
    uint16_t maxChars = availableWidth / charWidth;
    if (maxChars < 4) // Need at least 4 chars for "a..."
    {
        // If we can't even fit "a...", return just "..."
        if (maxChars >= 3)
        {
            char *truncated = (char *)malloc(4);
            if (truncated == nullptr)
            {
                if (getLogger())
                {
                    getLogger()->errF(F("ContentScreen: Failed to allocate memory for truncated text ..."));
                }
                return nullptr;
            }
            strcpy(truncated, "...");
            return truncated;
        }
        return nullptr; // Can't fit anything meaningful
    }

    // Create truncated text with "..." at end
    uint16_t textChars = maxChars - 3; // Reserve 3 chars for "..."
    char *truncated = (char *)malloc(maxChars + 1);
    if (truncated == nullptr)
    {
        if (getLogger())
        {
            getLogger()->errF(F("ContentScreen: Failed to allocate memory for truncated text"));
        }
        return nullptr;
    }

    strncpy(truncated, text, textChars);
    strcpy(truncated + textChars, "...");
    return truncated;
}