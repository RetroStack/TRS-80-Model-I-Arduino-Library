/*
 * ContentScreen.h - Structured layout template for screens with header, content, footer, and progress bar
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef CONTENT_SCREEN_H
#define CONTENT_SCREEN_H

#include <Arduino.h>
#include "Screen.h"

/**
 * @brief Result values for confirmation dialogs
 */
enum ConfirmResult
{
    CONFIRM_LEFT, // Left button was pressed (typically "Cancel")
    CONFIRM_RIGHT // Right button was pressed (typically "OK")
};

/**
 * @brief Template base class providing structured screen layouts
 *
 * ContentScreen extends the basic Screen interface with a standardized
 * layout system including header, content area, footer, and progress bar.
 * This provides consistent visual structure across all screens.
 *
 * ## Layout Structure
 *
 * ```
 * +-----------------------------+
 * | Header (Title)              |
 * +-----------------------------+
 * |                             |
 * |                             |
 * |     Content Area            |
 * |   (implemented by           |
 * |    derived classes)         |
 * |                             |
 * |                             |
 * +-----------------------------+
 * | Footer (Buttons/Status)     |
 * +-----------------------------+
 * | Progress Bar                |
 * +-----------------------------+
 * ```
 *
 * ## Layout Regions
 *
 * - **Header**: Title display and status indicators
 * - **Content**: Main application area (implemented by subclasses)
 * - **Footer**: Button labels and help text
 * - **Progress**: Progress bar for long operations
 *
 * ## Design Benefits
 *
 * - **Consistency**: All screens share common layout patterns
 * - **Maintainability**: Layout logic centralized in one class
 * - **Usability**: Users learn one interface pattern
 * - **Flexibility**: Content area can be customized per screen
 * - **Adaptive**: Automatically adjusts layout for small displays (OLED)
 *
 * ## Implementation Pattern
 *
 * Derived classes only need to implement `_drawContent()` to render
 * their specific content within the designated area. The base class
 * handles all layout management and decoration. Use `isSmallDisplay()`
 * (inherited from Screen) to adapt content for different display sizes.
 *
 * @example Basic ContentScreen Implementation
 * ```cpp
 * class MyContentScreen : public ContentScreen {
 * protected:
 *     void _drawContent() override {
 *         // Draw within content area bounds
 *         uint16_t x = _getContentLeft();
 *         uint16_t y = _getContentTop();
 *         uint16_t w = _getContentWidth();
 *         uint16_t h = _getContentHeight();
 *
 *         // Adapt layout for display size (inherited from Screen)
 *         if (isSmallDisplay()) {
 *             _drawText(x, y, "Compact View", 0xFFFF, 1);
 *         } else {
 *             _drawText(x, y, "Full Detail View", 0xFFFF, 2);
 *         }
 *     }
 *
 * public:
 *     MyContentScreen() : ContentScreen() {
 *         _setTitle("My Screen");
 *         _setProgressValue(75);  // Optional progress (0 = default)
 *     }
 *
 *     void loop() override {
 *         // Update content as needed
 *     }
 *
 *     Screen* actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY) override {
 *         // Handle input and may be return a new screen
 *         return nullptr;
 *     }
 * };
 * ```
 *
 * @see Screen For base screen interface
 */
class ContentScreen : public Screen
{
private:
    char *_title;             // Dynamic title buffer (allocated as needed)
    char **_buttonItems;      // Dynamic array of button label buffers (allocated as needed)
    uint8_t _buttonItemCount; // Number of button labels currently set
    uint8_t _progressValue;   // Progress bar value (0-100)

    // Notification system
    char *_notificationText;              // Dynamic notification text buffer
    unsigned long _notificationStartTime; // When notification was shown
    unsigned long _notificationDuration;  // How long to show notification (ms)
    bool _notificationActive;             // Whether notification is currently active

    /**
     * @brief Draw the header region with title
     */
    void _drawHeader();

    /**
     * @brief Draw the footer region with button labels
     */
    void _drawFooter();

    /**
     * @brief Draw the progress bar region
     */
    void _drawProgressBar();

    /**
     * @brief Gets the padding between areas
     * @return Padding in pixels
     */
    uint8_t _getPadding() const;

    /**
     * @brief Draw notification overlay in place of footer
     */
    void _drawNotification();

    /**
     * @brief Update notification state and check for expiration
     */
    void _updateNotification();

    /**
     * @brief Clear notification text and free memory
     */
    void _clearNotification();

    /**
     * @brief Draw alert dialog overlay in place of footer
     * @param text Alert message to display
     */
    void _drawAlert(const char *text);

    /**
     * @brief Draw confirm dialog overlay in place of footer
     * @param text Main message to display
     * @param leftText Left button text (left-aligned)
     * @param rightText Right button text (right-aligned)
     */
    void _drawConfirm(const char *text, const char *leftText, const char *rightText);

protected:
    /**
     * @brief Implement Screen's _drawScreen() to manage layout regions
     *
     * Orchestrates the complete screen redraw by calling:
     * 1. _drawHeader() - Title and status area
     * 2. _drawContent() - Content area (implemented by derived class)
     * 3. _drawFooter() - Button labels and help text
     * 4. _drawProgressBar() - Progress indicator
     */
    void _drawScreen() override;

    /**
     * @brief Pure virtual method for content area rendering
     *
     * Derived classes must implement this to draw their specific content
     * within the designated content area bounds. Use the getter methods
     * to determine the available drawing area.
     *
     * @note Called automatically by _drawScreen() layout management
     * @see _getContentLeft(), _getContentTop(), _getContentWidth(), _getContentHeight()
     */
    virtual void _drawContent() = 0;

    // Layout dimension getters for content positioning

    /**
     * @brief Get Y coordinate of content area top edge
     * @return Pixel position of content area start
     */
    uint16_t _getContentTop() const;

    /**
     * @brief Get X coordinate of content area left edge
     * @return Pixel position of content area start
     */
    uint16_t _getContentLeft() const;

    /**
     * @brief Get available height of content area
     * @return Height in pixels available for content
     */
    uint16_t _getContentHeight() const;

    /**
     * @brief Get available width of content area
     * @return Width in pixels available for content
     */
    uint16_t _getContentWidth() const;

    /**
     * @brief Get Y coordinate of header region
     * @return Pixel position of header area
     */
    uint16_t _getHeaderY() const;

    /**
     * @brief Get height of header region
     * @return Height in pixels of header area
     */
    uint16_t _getHeaderHeight() const;

    /**
     * @brief Get Y coordinate of footer region
     * @return Pixel position of footer area
     */
    uint16_t _getFooterY() const;

    /**
     * @brief Get height of footer region
     * @return Height in pixels of footer area
     */
    uint16_t _getFooterHeight() const;

    /**
     * @brief Get Y coordinate of progress bar region
     * @return Pixel position of progress bar area
     */
    uint16_t _getProgressBarY() const;

    /**
     * @brief Get height of progress bar region
     * @return Height in pixels of progress bar area
     */
    uint16_t _getProgressBarHeight() const;

    // Content management methods

public:
    /**
     * @brief Constructor
     */
    ContentScreen();

    /**
     * @brief Destructor - frees dynamically allocated title and button memory
     */
    virtual ~ContentScreen();

    // Required Screen interface methods - must be implemented by derived classes

    /**
     * @brief Main update loop for content-specific logic
     *
     * Derived classes implement this for ongoing updates, animations,
     * and time-based content changes.
     */
    virtual void loop() = 0;

    /**
     * @brief Handle input events within the structured layout
     *
     * Process user input and determine navigation behavior. The base
     * ContentScreen provides layout management, while derived classes
     * handle content-specific input logic.
     *
     * @param action Bit flags indicating active inputs
     * @param offsetX X position for joystick input
     * @param offsetY Y position for joystick input
     * @return Screen navigation result (nullptr = stay, other = navigate)
     */
    virtual Screen *actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY) = 0;

    /**
     * @brief Set the screen title displayed in header
     * @param title Null-terminated string for title (dynamically allocated copy made)
     * @note Pass nullptr or empty string to free title memory and clear title
     */
    void setTitle(const char *title);

    /**
     * @brief Set the screen title from Arduino String object
     * @param title String object for title (automatically converted and copied)
     */
    void setTitle(String title);

    /**
     * @brief Set the screen title from FlashString (F() macro)
     * @param title FlashString for title (automatically converted and copied)
     */
    void setTitleF(const __FlashStringHelper *title);

    /**
     * @brief Clear the current title
     *
     * Frees any dynamically allocated title memory and resets the title
     * to nullptr. Call this to clear the title without needing to set a new one.
     */
    void clearTitle();

    /**
     * @brief Get current screen title
     * @return Current title string, or nullptr if no title is set
     */
    const char *getTitle() const;

    /**
     * @brief Set progress bar value
     * @param value Progress percentage (0-100)
     */
    void setProgressValue(int value);

    /**
     * @brief Get current progress bar value
     * @return Progress percentage (0-100)
     */
    uint8_t getProgressValue() const;

    /**
     * @brief Set button labels for footer display with dynamic memory allocation
     * @param buttonItems Array of string pointers for button labels (dynamically allocated copies made)
     * @param buttonItemCount Number of strings in the array (no hard limit)
     * @note Pass nullptr or 0 count to free all button memory and clear labels
     */
    void setButtonItems(const char **buttonItems, uint8_t buttonItemCount);

    /**
     * @brief Set button labels from FlashString array (F() macro)
     * @param buttonItems Array of FlashString pointers for button labels (automatically converted and copied)
     * @param buttonItemCount Number of FlashStrings in the array
     */
    void setButtonItemsF(const __FlashStringHelper **buttonItems, uint8_t buttonItemCount);

    /**
     * @brief Clear all button labels and free allocated memory
     *
     * This method frees any dynamically allocated button label memory
     * and resets the button item count to 0. Call this to clear all
     * button labels without needing to set new ones.
     */
    void clearButtonItems();

    /**
     * @brief Clear content area to background color
     *
     * Convenience method to clear just the content region while
     * preserving header, footer, and progress bar areas.
     */
    void clearContentArea();

    /**
     * @brief Draw text within content area with clipping
     * @param x X position relative to content area
     * @param y Y position relative to content area
     * @param text Text to display
     * @param color Text color
     * @param size Text size multiplier
     */
    void drawText(uint16_t x, uint16_t y, const char *text, uint16_t color, uint8_t size = 1);

    /**
     * @brief Draw text in content area from Arduino String object
     *
     * Convenient version of drawText() that accepts String objects for dynamic text.
     * Coordinates are relative to the content area, not the full screen.
     *
     * @param x X position relative to content area
     * @param y Y position relative to content area
     * @param text String object text to display
     * @param color Text color
     * @param size Text size multiplier
     *
     * @note Convenient for dynamic text content built at runtime
     * @note Coordinates are relative to content area, not screen
     * @note Text extending beyond content area will be clipped
     */
    void drawText(uint16_t x, uint16_t y, String text, uint16_t color, uint8_t size = 1);

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
     *
     * @example
     * @code
     * drawTextF(10, 20, F("Status: Ready"), M1Shield.GREEN, 1);
     * drawTextF(10, 40, F("Memory: 75% free"), M1Shield.YELLOW, 1);
     * @endcode
     */
    void drawTextF(uint16_t x, uint16_t y, const __FlashStringHelper *text, uint16_t color, uint8_t size = 1);

    // Notification system methods

    /**
     * @brief Show a notification that temporarily replaces the footer
     * @param text Notification text to display (dynamically allocated copy made)
     * @param durationMs How long to show notification in milliseconds
     * @note Notification is displayed with yellow background and black text
     * @note Previous footer will be restored when notification expires
     */
    void notify(const char *text, unsigned long durationMs = 3000);

    /**
     * @brief Show a notification from Arduino String object
     * @param text String object notification text (automatically converted and copied)
     * @param durationMs How long to show notification in milliseconds
     * @note Convenient for dynamic notification content built at runtime
     */
    void notify(String text, unsigned long durationMs = 3000);

    /**
     * @brief Show a notification from FlashString (F() macro)
     * @param text FlashString notification text (automatically converted and copied)
     * @param durationMs How long to show notification in milliseconds
     * @note More memory efficient than notify() for static text
     */
    void notifyF(const __FlashStringHelper *text, unsigned long durationMs = 3000);

    /**
     * @brief Check if a notification is currently active
     * @return true if notification is being displayed, false otherwise
     */
    bool isNotificationActive() const;

    /**
     * @brief Manually dismiss current notification
     * @note Notification will be cleared and footer will be restored immediately
     */
    void dismissNotification();

    // Alert and Confirmation system methods

    /**
     * @brief Show a blocking alert dialog with cyan background
     * @param text Alert message to display (center-aligned)
     * @note Blocks execution until LEFT or RIGHT button is pressed
     * @note Alert is displayed with cyan background and black text
     * @note Function returns when either LEFT or RIGHT button is pressed
     */
    void alert(const char *text);

    /**
     * @brief Show a blocking alert dialog from Arduino String object
     * @param text String object alert message (automatically converted)
     * @note Convenient for dynamic alert content built at runtime
     * @note Blocks execution until LEFT or RIGHT button is pressed
     */
    void alert(String text);

    /**
     * @brief Show a blocking alert dialog from FlashString (F() macro)
     * @param text FlashString alert message (automatically converted)
     * @note More memory efficient than alert() for static text
     * @note Blocks execution until LEFT or RIGHT button is pressed
     */
    void alertF(const __FlashStringHelper *text);

    /**
     * @brief Show a blocking confirmation dialog with magenta background
     * @param text Main confirmation message (center-aligned)
     * @param leftText Left button text (default: "Cancel", left-aligned)
     * @param rightText Right button text (default: "OK", right-aligned)
     * @return CONFIRM_LEFT if left button pressed, CONFIRM_RIGHT if right button pressed
     * @note Blocks execution until LEFT or RIGHT button is pressed
     * @note Dialog is displayed with magenta background and black text
     */
    ConfirmResult confirm(const char *text, const char *leftText = "Cancel", const char *rightText = "OK");

    /**
     * @brief Show a blocking confirmation dialog with Arduino String objects
     * @param text Main confirmation message (center-aligned)
     * @param leftText Left button text (left-aligned)
     * @param rightText Right button text (right-aligned)
     * @return CONFIRM_LEFT if left button pressed, CONFIRM_RIGHT if right button pressed
     * @note Convenient for dynamic string content built at runtime
     * @note Dialog is displayed with magenta background and black text
     */
    ConfirmResult confirm(String text, String leftText, String rightText);

    /**
     * @brief Show a blocking confirmation dialog with all F-strings
     * @param text FlashString main message (automatically converted)
     * @param leftText FlashString left button text (automatically converted)
     * @param rightText FlashString right button text (automatically converted)
     * @return CONFIRM_LEFT if left button pressed, CONFIRM_RIGHT if right button pressed
     * @note Fully memory efficient version with all parameters as F-strings
     */
    ConfirmResult confirmF(const __FlashStringHelper *text, const __FlashStringHelper *leftText, const __FlashStringHelper *rightText);
};

#endif /* CONTENT_SCREEN_H */