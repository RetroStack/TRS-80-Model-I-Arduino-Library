/*
 * ContentScreen.h - Structured layout template for screens with header, content, footer, and progress bar
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef CONTENT_SCREEN_H
#define CONTENT_SCREEN_H

#include <Arduino.h>
#include "Screen.h"

// Result values for confirmation dialogs
enum ConfirmResult
{
    CONFIRM_LEFT, // Left button was pressed (typically "Cancel")
    CONFIRM_RIGHT // Right button was pressed (typically "OK")
};

// Template base class providing structured screen layouts
class ContentScreen : public Screen
{
private:
    char **_buttonItems;      // Dynamic array of button label buffers (allocated as needed)
    uint8_t _buttonItemCount; // Number of button labels currently set
    uint8_t _progressValue;   // Progress bar value (0-100)

    // Notification system
    char *_notificationText;              // Dynamic notification text buffer
    unsigned long _notificationStartTime; // When notification was shown
    unsigned long _notificationDuration;  // How long to show notification (ms)
    bool _notificationActive;             // Whether notification is currently active
    uint16_t _notificationBgColor;        // Custom notification background color

    Screen *(*_backScreenFactory)(const String &context); // Builds the screen the menu button returns to
    String _backScreenContext;                            // Passed to that factory when it runs
    const __FlashStringHelper *_errorMessage;             // Drawn in place of the content when set

    uint8_t _getPadding() const;                                                       // Gets the padding between areas
    void _drawNotification();                                                          // Draw notification overlay in place of footer
    void _clearNotification();                                                         // Clear notification text and free memory
    void _drawAlert(const char *text);                                                 // Draw alert dialog overlay in place of footer
    void _drawConfirm(const char *text, const char *leftText, const char *rightText);  // Draw confirm dialog overlay in place of footer


    // The band the notification and dialog overlays draw into. Normally the
    // footer; on a small panel the footer has no height, so they borrow the
    // bottom of the content area rather than not drawing at all.
    uint16_t _getOverlayTop() const;    // Y coordinate of the overlay band
    uint16_t _getOverlayHeight() const; // Height of the overlay band
    uint8_t _getOverlayTextSize() const; // Text size the overlay band can carry



protected:
    // Text fitting. Protected because MenuScreen, ButtonScreen and
    // ConsoleScreen each grew their own truncation when these were private.
    char *_truncateText(const char *text, uint16_t availableWidth, uint8_t charWidth); // Truncated copy with "..." if needed
    void _drawCenteredText(const char *text, uint16_t x0, uint16_t y,
                           uint16_t availableWidth, uint8_t charWidth); // Centered, truncated and clipped to the region

    Screen *_handleBackAction(ActionTaken action); // Back screen when the menu button was pressed, else nullptr

    // Wrap-around scan for the next selectable item. MenuScreen and
    // ButtonScreen had line-for-line identical copies of this, differing only
    // in where the count came from and which predicate they asked.
    virtual bool _isItemEnabled(uint8_t index) const { (void)index; return true; }
    uint8_t _findNextEnabledItem(uint8_t startIndex, bool forward, uint8_t itemCount) const;

    // A screen that cannot show its content -- no card, unreadable file --
    // stays open and says so, rather than refusing to open and leaving the
    // device with no screen at all.
    void _setErrorState(const __FlashStringHelper *message); // Show this instead of the content
    void _clearErrorState();                                 // Resume drawing content
    bool _hasErrorState() const;                             // Whether an error is being shown

    void _drawHeader();         // Draw the header region with title
    virtual void _drawFooter(); // Draw the footer region with button labels (virtual for customization)
    void _refreshFooter();      // Redraw just the footer on every enabled render target
    void _drawProgressBar();    // Draw the progress bar region

    void _drawScreen() override;         // Implement Screen's _drawScreen() to manage layout regions
    virtual void _drawContent() = 0;     // Pure virtual method for primary content area rendering
    virtual void _drawSecondaryContent() // Virtual method for secondary content area rendering (default: empty)
    {
        // Default implementation does nothing - secondary content is optional
    }
    void _drawMainContent(); // Combines primary and secondary content areas with borders

    // Layout dimension getters for content positioning
    uint16_t _getContentTop() const;    // Get Y coordinate of content area top edge
    uint16_t _getContentLeft() const;   // Get X coordinate of content area left edge
    uint16_t _getContentHeight() const; // Get available height of content area
    uint16_t _getContentWidth() const;  // Get available width of content area

    // Secondary content area dimensions (default: zero size, not visible)
    virtual uint16_t _getSecondaryContentTop() const;    // Get Y coordinate of secondary content area top edge
    virtual uint16_t _getSecondaryContentLeft() const;   // Get X coordinate of secondary content area left edge
    virtual uint16_t _getSecondaryContentHeight() const; // Get available height of secondary content area
    virtual uint16_t _getSecondaryContentWidth() const;  // Get available width of secondary content area

    uint16_t _getHeaderTop() const;            // Get Y coordinate of header region
    uint16_t _getHeaderHeight() const;         // Get height of header region
    virtual uint16_t _getFooterTop() const;    // Get Y coordinate of footer region (virtual for ButtonScreen override)
    virtual uint16_t _getFooterHeight() const; // Get height of footer region (virtual for ButtonScreen override)
    uint16_t _getProgressBarTop() const;       // Get Y coordinate of progress bar region
    uint16_t _getProgressBarHeight() const;    // Get height of progress bar region

    uint8_t _getButtonItemCount() const;             // Get number of button items (for derived classes)
    const char *_getButtonItem(uint8_t index) const; // Get button item text by index (for derived classes)

public:
    // Content management methods

public:
    ContentScreen();
    virtual ~ContentScreen(); // Destructor - frees dynamically allocated button memory

    // Required Screen interface methods - must be implemented by derived classes
    virtual void loop() override;                                                        // Base implementation handles notification timeouts, derived classes can override
    Screen *actionTaken(ActionTaken action, int8_t offsetX, int8_t offsetY) override = 0; // Handle input events

    // Where the menu button goes back to. A factory rather than a screen
    // pointer: setScreen() closes and deletes the current screen before opening
    // the next, so a stored pointer to the previous screen would already be
    // dangling by the time the user pressed back.
    typedef Screen *(*BackScreenFactory)(const String &context);
    void setBackScreen(BackScreenFactory factory, const String &context = String()); // Set where back leads
    bool hasBackScreen() const;                                                      // Whether a back destination is set

    // Progress control
    void setProgressValue(int value); // Set progress bar value (0-100)
    uint8_t getProgressValue() const; // Get current progress bar value

    // Button management
    void setButtonItems(const char **buttonItems, uint8_t buttonItemCount);                 // Set button labels for footer display
    void setButtonItems(String *buttonItems, uint8_t buttonItemCount); // Arduino String array form
    void setButtonItemsF(const __FlashStringHelper **buttonItems, uint8_t buttonItemCount); // Set button labels from FlashString array
    void clearButtonItems();                                                                // Clear all button labels and free allocated memory

    // Content area utilities
    void clearContentArea();                                                                                   // Clear content area to background color
    void clearSecondaryContentArea();                                                                          // Clear secondary content area to background color
    void drawText(uint16_t x, uint16_t y, const char *text, uint16_t color, uint8_t size = 1);                 // Draw text within content area with clipping
    void drawText(uint16_t x, uint16_t y, String text, uint16_t color, uint8_t size = 1);                      // Draw text in content area from Arduino String object
    void drawTextF(uint16_t x, uint16_t y, const __FlashStringHelper *text, uint16_t color, uint8_t size = 1); // Draw text in content area from FlashString (F() macro)

    // Notification system methods
    // Non-blocking temporary notification message with customizable background color (defaults to yellow)
    void notify(const char *text, unsigned long durationMs = 3000, uint16_t backgroundColor = 0xFFE0);                 // Show a notification that temporarily replaces the footer
    void notify(String text, unsigned long durationMs = 3000, uint16_t backgroundColor = 0xFFE0);                      // Show a notification from Arduino String object
    void notifyF(const __FlashStringHelper *text, unsigned long durationMs = 3000, uint16_t backgroundColor = 0xFFE0); // Show a notification from FlashString (F() macro)

    bool isNotificationActive() const; // Check if a notification is currently active
    void dismissNotification();        // Manually dismiss current notification

    // Alert system methods
    // Blocking alert dialog with cyan background
    void alert(const char *text);                 // Show alert with character string
    void alert(String text);                      // Show alert from Arduino String object
    void alertF(const __FlashStringHelper *text); // Show alert from FlashString (F() macro)

    // Confirmation system methods
    // Blocking confirmation dialog with magenta background
    ConfirmResult confirm(const char *text, const char *leftText = "Cancel", const char *rightText = "OK");                             // Show character string with left/right button messages
    ConfirmResult confirm(String text, String leftText, String rightText);                                                              // Show confirmation from Arduino String objects
    ConfirmResult confirmF(const __FlashStringHelper *text, const __FlashStringHelper *leftText, const __FlashStringHelper *rightText); // Show confirmation from FlashString (F() macro)
};

#endif /* CONTENT_SCREEN_H */