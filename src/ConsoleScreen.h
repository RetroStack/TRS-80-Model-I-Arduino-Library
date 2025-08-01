/*
 * ConsoleScreen.h - Scrollable console/terminal screen for text output
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef CONSOLE_SCREEN_H
#define CONSOLE_SCREEN_H

#include <Arduino.h>
#include "ContentScreen.h"

/**
 * @brief Scrollable console screen for terminal-like text output
 *
 * ConsoleScreen provides a scrollable terminal interface similar to a serial monitor.
 * When text reaches the bottom of the screen, the entire console is automatically
 * cleared and text continues from the top (wrap-around behavior).
 * Text can be added using print() and println() methods with color support.
 *
 * Implements Arduino's Print interface for full compatibility with Arduino ecosystem.
 * All standard Print methods (print various data types, formatting) are automatically
 * available through inheritance.
 *
 * Key Features:
 * - Implements Arduino Print interface for full ecosystem compatibility
 * - Automatic screen clearing when content reaches bottom (memory efficient)
 * - Color support for text foreground and background
 * - Tab support with predefined tab stops
 * - Newline character (\n) support
 * - Clear screen (cls) functionality
 * - Refresh functionality to restart from top
 * - No coordinate positioning - sequential text output only
 * - Memory efficient - doesn't store written content, auto-clears when full
 *
 * @note Content is not stored in memory - once rendered, it's forgotten
 * @note When screen fills up, entire console clears and starts from top (wrap-around)
 * @note This auto-clear behavior is memory efficient and ideal for continuous logging
 * @note Suitable for logging, debugging, and serial monitor interfaces
 *
 * @example
 * @code
 * class DebugConsole : public ConsoleScreen {
 * protected:
 *     void _executeOnce() override {
 *         // One-time initialization after 1 second
 *         cls();
 *         setTextColor(0x07E0, 0x0000);
 *         println("=== DEBUG CONSOLE READY ===");
 *         println("System initialized successfully");
 *         println("Status: ONLINE");
 *     }
 *
 * public:
 *     DebugConsole() {
 *         _setTitle("Debug Console");
 *         // _executeOnce() will be called automatically after 1 second
 *     }
 *
 *     void loop() override {
 *         // Optional: continuous updates after _executeOnce()
 *         static unsigned long lastUpdate = 0;
 *         if (millis() - lastUpdate > 5000) { // Every 5 seconds
 *             lastUpdate = millis();
 *             print("Uptime: ");
 *             println(millis() / 1000);
 *         }
 *         ConsoleScreen::loop();
 *     }
 * };
 * @endcode
 */
class ConsoleScreen : public ContentScreen, public Print
{
private:
    // Text positioning and rendering state
    uint16_t _currentX;   // Current horizontal cursor position (pixels)
    uint16_t _currentY;   // Current vertical cursor position (pixels)
    uint16_t _lineHeight; // Height of each text line (pixels)
    uint16_t _charWidth;  // Width of each character (pixels)
    uint8_t _textSize;    // Text size (1, 2, 3, etc.)

    // Color settings
    uint16_t _textColor;      // Current text foreground color
    uint16_t _textBgColor;    // Current text background color
    uint16_t _consoleBgColor; // Console area background color

    // Tab settings
    uint8_t _tabSize; // Number of characters per tab stop

    // Bulk operation optimization
    bool _inBulkWrite; // Flag to track if we're in a bulk write operation

    // Screen dimensions (cached for efficiency)
    uint16_t _contentLeft;   // Left edge of content area
    uint16_t _contentTop;    // Top edge of content area
    uint16_t _contentWidth;  // Width of content area
    uint16_t _contentHeight; // Height of content area

    // One-time execution tracking
    unsigned long _screenOpenTime; // Timestamp when screen was opened
    bool _hasExecutedOnce;         // Whether _executeOnce() has been called

    /**
     * @brief Update cached screen dimensions from ContentScreen
     */
    void _updateDimensions();

    /**
     * @brief Process a single character for output
     *
     * Handles special characters like newline (\n) and tab (\t),
     * and renders printable characters to the screen.
     *
     * @param c Character to process and potentially render
     */
    void _processChar(char c);

    /**
     * @brief Render a single character at the current cursor position
     *
     * Draws the character with current color settings and advances
     * the cursor position. Handles line wrapping automatically.
     *
     * @param c Character to render (must be printable)
     */
    void _renderChar(char c);

    /**
     * @brief Move to the next line (newline operation)
     *
     * Advances the cursor to the beginning of the next line.
     * If at the bottom of the screen, scrolls content up.
     */
    void _newLine();

    /**
     * @brief Process tab character - move to next tab stop
     *
     * Advances cursor to the next tab stop position based on
     * the configured tab size. If the tab would exceed the line
     * width, moves to the next line instead.
     */
    void _processTab();

protected:
    /**
     * @brief Optional one-time execution method called 1 second after console opens
     *
     * This virtual method provides an alternative to continuous loop() processing
     * for tasks that need to run exactly once after the console is displayed.
     * Perfect for initialization, data loading, or setup tasks that don't need
     * to be repeated continuously.
     *
     * Default implementation does nothing - derived classes can override to
     * implement one-time processing logic.
     *
     * @note Called automatically 1 second after console becomes active
     * @note Only called once per console activation
     * @note Called before any loop() iterations
     * @note Does not replace loop() - both can be used together
     *
     * @example One-time console initialization
     * ```cpp
     * void _executeOnce() override {
     *     println("=== CONSOLE INITIALIZED ===");
     *     println("Loading system data...");
     *     _loadSystemStatus();
     *     println("Ready for operation!");
     * }
     * ```
     */
    virtual void _executeOnce() {};

public:
    /**
     * @brief Constructor - initialize console with default settings
     *
     * Sets up the console with default text size, colors, and cursor position.
     * The console starts at the top-left of the content area.
     */
    ConsoleScreen();

    /**
     * @brief Destructor
     */
    virtual ~ConsoleScreen();

    /**
     * @brief Override Screen::open() to initialize timing for one-time execution
     * @note Resets the one-time execution timer when console becomes active
     */
    bool open() override;

    // Print Interface Implementation (required for Arduino Print class)

    /**
     * @brief Write a single character to the console (Print interface)
     *
     * This method implements the Arduino Print interface requirement.
     * It processes and renders a single character to the console, handling
     * special characters like newline and tab appropriately.
     *
     * @param c Character to write to console
     * @return size_t Number of bytes written (1 if successful, 0 if failed)
     *
     * @note This method enables all Print class functionality automatically
     * @note Handles special characters: \n (newline), \t (tab), \r (carriage return)
     * @note All other Print methods (print/println variants) use this internally
     */
    size_t write(uint8_t c) override;

    /**
     * @brief Write buffer of characters to console (Print interface optimization)
     *
     * Optimized bulk write method for better performance when writing
     * multiple characters at once.
     *
     * @param buffer Pointer to character buffer
     * @param size Number of characters to write
     * @return size_t Number of bytes successfully written
     */
    size_t write(const uint8_t *buffer, size_t size) override;

    // Screen Interface Implementation

    /**
     * @brief Main loop processing for console screen updates
     *
     * Delegates to ContentScreen for standard screen processing.
     * Override this method in derived classes for custom behavior.
     */
    void loop() override;

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
    Screen *actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY) override;

    // Console Text Output Methods
    //
    // Note: ConsoleScreen inherits from Arduino's Print class, which automatically
    // provides all standard print() and println() methods for various data types:
    // - print(int), print(long), print(float), print(double)
    // - print(char), print(unsigned char), print(const char*)
    // - print(String), print(const String&)
    // - println() variants of all above
    // - print with base conversion: print(value, HEX), print(value, BIN), etc.
    //
    // All of these methods are automatically available and work through the
    // write() method implementation - no additional methods needed!

    // Console Control Methods

    /**
     * @brief Clear screen and reset cursor to top-left
     *
     * Clears the entire console area and positions the cursor
     * at the top-left corner. This method is also called automatically
     * when text reaches the bottom of the screen for memory efficiency.
     *
     * @note This is the same operation that happens automatically when console fills up
     * @note Equivalent to calling refresh()
     *
     * @example
     * @code
     * cls();
     * println("Screen cleared!");
     * // Console will auto-clear again when it fills up
     * @endcode
     */
    void cls();

    /**
     * @brief Refresh screen and reset cursor to top-left
     *
     * Clears the console area and resets the cursor position.
     * Same functionality as cls() - provided for API consistency.
     * Also called automatically when console reaches bottom.
     */
    void refresh();

    // Console Configuration Methods

    /**
     * @brief Set text color for subsequent output
     *
     * @param foreground Text foreground color (RGB565 format)
     * @param background Text background color (RGB565 format, optional)
     *
     * @example
     * @code
     * setTextColor(0x07E0, 0x0000);
     * println("This text is green on black");
     * @endcode
     */
    void setTextColor(uint16_t foreground, uint16_t background = 0x0000);

    /**
     * @brief Set console background color
     *
     * Sets the background color for the entire console area.
     * Takes effect on next refresh() or cls() call.
     *
     * @param color Background color (RGB565 format)
     */
    void setConsoleBackground(uint16_t color);

    /**
     * @brief Set text size for subsequent output
     *
     * @param size Text size multiplier (1=smallest, 2=medium, 3=large, etc.)
     *
     * @note Changing text size affects line height and character width
     * @note Cursor position is maintained but may need adjustment
     */
    void setTextSize(uint8_t size);

    /**
     * @brief Set tab size in characters
     *
     * @param size Number of characters per tab stop (default: 4)
     *
     * @example
     * @code
     * setTabSize(8);  // Set tab stops every 8 characters
     * print("Col1\tCol2\tCol3");
     * @endcode
     */
    void setTabSize(uint8_t size);

protected:
    /**
     * @brief Draw the console content area
     *
     * Fills the console background and handles the rendering area setup.
     * Called automatically by ContentScreen's rendering system.
     */
    void _drawContent() override;
};

#endif /* CONSOLE_SCREEN_H */
