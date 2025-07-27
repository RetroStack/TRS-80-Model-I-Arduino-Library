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
 * @brief Template base class providing structured screen layouts
 *
 * ContentScreen extends the basic Screen interface with a standardized
 * layout system including header, content area, footer, and progress bar.
 * This provides consistent visual structure across all screens.
 *
 * ## Layout Structure
 *
 * ```
 * ┌─────────────────────────────┐
 * │ Header (Title)              │
 * ├─────────────────────────────┤
 * │                             │
 * │                             │
 * │     Content Area            │
 * │   (implemented by           │
 * │    derived classes)         │
 * │                             │
 * │                             │
 * ├─────────────────────────────┤
 * │ Footer (Buttons/Status)     │
 * ├─────────────────────────────┤
 * │ Progress Bar                │
 * └─────────────────────────────┘
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
 *
 * ## Implementation Pattern
 *
 * Derived classes only need to implement `_drawContent()` to render
 * their specific content within the designated area. The base class
 * handles all layout management and decoration.
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
 *         // Your content rendering here
 *         _drawText(x, y, "Hello World!");
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
     * @brief Get Y coordinate of footer region
     * @return Pixel position of footer area
     */
    uint16_t _getFooterY() const;

    /**
     * @brief Get Y coordinate of progress bar region
     * @return Pixel position of progress bar area
     */
    uint16_t _getProgressBarY() const;

    // Content management methods

    /**
     * @brief Set the screen title displayed in header
     * @param title Null-terminated string for title (dynamically allocated copy made)
     * @note Pass nullptr or empty string to free title memory and clear title
     */
    void _setTitle(const char *title);

    /**
     * @brief Clear the current title
     *
     * Frees any dynamically allocated title memory and resets the title
     * to nullptr. Call this to clear the title without needing to set a new one.
     */
    void _clearTitle();

    /**
     * @brief Get current screen title
     * @return Current title string, or nullptr if no title is set
     */
    const char *_getTitle() const;

    /**
     * @brief Set progress bar value
     * @param value Progress percentage (0-100)
     */
    void _setProgressValue(int value);

    /**
     * @brief Get current progress bar value
     * @return Progress percentage (0-100)
     */
    uint8_t _getProgressValue() const;

    /**
     * @brief Set button labels for footer display with dynamic memory allocation
     * @param buttonItems Array of string pointers for button labels (dynamically allocated copies made)
     * @param buttonItemCount Number of strings in the array (no hard limit)
     * @note Pass nullptr or 0 count to free all button memory and clear labels
     */
    void _setButtonItems(const char **buttonItems, uint8_t buttonItemCount);

    /**
     * @brief Clear all button labels and free allocated memory
     *
     * This method frees any dynamically allocated button label memory
     * and resets the button item count to 0. Call this to clear all
     * button labels without needing to set new ones.
     */
    void _clearButtonItems();

    /**
     * @brief Clear content area to background color
     *
     * Convenience method to clear just the content region while
     * preserving header, footer, and progress bar areas.
     */
    void _clearContentArea();

    /**
     * @brief Draw text within content area with clipping
     * @param x X position relative to content area
     * @param y Y position relative to content area
     * @param text Text to display
     * @param color Text color
     * @param size Text size multiplier
     */
    void _drawText(uint16_t x, uint16_t y, const char *text, uint16_t color, uint8_t size = 1);

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
};

#endif /* CONTENT_SCREEN_H */