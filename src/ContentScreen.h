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

    uint8_t _getPadding() const;                                                       // Gets the padding between areas
    void _drawNotification();                                                          // Draw notification overlay in place of footer
    void _clearNotification();                                                         // Clear notification text and free memory
    void _drawAlert(const char *text);                                                 // Draw alert dialog overlay in place of footer
    void _drawConfirm(const char *text, const char *leftText, const char *rightText);  // Draw confirm dialog overlay in place of footer
    char *_truncateText(const char *text, uint16_t availableWidth, uint8_t charWidth); // Create truncated copy of text with "..." if needed

protected:
    void _drawHeader();         // Draw the header region with title
    virtual void _drawFooter(); // Draw the footer region with button labels (virtual for customization)
    void _drawProgressBar();    // Draw the progress bar region

    void _drawScreen() override;     // Implement Screen's _drawScreen() to manage layout regions
    virtual void _drawContent() = 0; // Pure virtual method for content area rendering

    // Layout dimension getters for content positioning
    uint16_t _getContentTop() const;           // Get Y coordinate of content area top edge
    uint16_t _getContentLeft() const;          // Get X coordinate of content area left edge
    uint16_t _getContentHeight() const;        // Get available height of content area
    uint16_t _getContentWidth() const;         // Get available width of content area
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
    virtual Screen *actionTaken(ActionTaken action, int8_t offsetX, int8_t offsetY) = 0; // Handle input events

    // Progress control
    void setProgressValue(int value); // Set progress bar value (0-100)
    uint8_t getProgressValue() const; // Get current progress bar value

    // Button management
    void setButtonItems(const char **buttonItems, uint8_t buttonItemCount);                 // Set button labels for footer display
    void setButtonItemsF(const __FlashStringHelper **buttonItems, uint8_t buttonItemCount); // Set button labels from FlashString array
    void clearButtonItems();                                                                // Clear all button labels and free allocated memory

    // Content area utilities
    void clearContentArea();                                                                                   // Clear content area to background color
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