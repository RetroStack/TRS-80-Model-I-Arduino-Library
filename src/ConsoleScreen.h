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
 * @brief Auto-paging behavior when console reaches bottom of screen
 */
enum ConsolePagingMode
{
    PAGING_AUTO_CLEAR,   // Clear immediately and continue (original behavior)
    PAGING_WAIT_TIMEOUT, // Wait for timeout before clearing (default)
    PAGING_WAIT_BUTTON,  // Wait for right button press before clearing
    PAGING_WAIT_BOTH     // Wait for timeout OR button press (whichever comes first)
};

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
 * - Configurable auto-paging behavior (immediate, timeout, button, or both)
 * - Color support for text foreground and background
 * - Tab support with predefined tab stops
 * - Newline character (\n) support
 * - Clear screen (cls) functionality
 * - Refresh functionality to restart from top
 * - No coordinate positioning - sequential text output only
 * - Memory efficient - doesn't store written content, auto-clears when full
 *
 * @note Content is not stored in memory - once rendered, it's forgotten
 * @note When screen fills up, console behavior depends on paging mode:
 *       - Auto-clear: immediately clears and continues (original behavior)
 *       - Wait timeout: waits 5 seconds (configurable) then clears
 *       - Wait button: waits for right button press then clears
 *       - Wait both: waits for timeout OR button press (whichever first)
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
 *         // Configure paging behavior
 *         setPagingMode(PAGING_WAIT_TIMEOUT);  // Wait 5 seconds before clearing
 *         setPagingTimeout(3000);              // Change to 3 second timeout
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

    // Auto-forward functionality
    bool _autoForwardEnabled;          // Whether auto-forward is enabled
    unsigned long _autoForwardDelayMs; // Delay in milliseconds before auto-forward triggers
    unsigned long _executeOnceCompleteTime; // Timestamp when _executeOnce() completed
    bool _autoForwardTriggered;        // Flag indicating auto-forward should activate

    // Auto-paging management
    ConsolePagingMode _pagingMode;      // Current paging behavior mode
    uint16_t _pagingTimeoutMs;          // Timeout in milliseconds for auto-paging
    bool _isWaitingForPaging;           // True when console is full and waiting
    unsigned long _pagingWaitStartTime; // When the paging wait period started
    bool _showPagingPrompt;             // Whether to show paging prompt message

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

    /**
     * @brief Check if console has reached the bottom and handle paging
     *
     * Called when text would exceed the console area. Handles the paging
     * behavior based on current paging mode settings.
     *
     * @return true if console was cleared and can continue, false if waiting
     */
    bool _handlePaging();

    /**
     * @brief Check if paging timeout has expired
     *
     * Evaluates timeout conditions for timeout-based paging modes.
     *
     * @return true if timeout has expired and console should clear
     */
    bool _shouldEndPagingWait();

    /**
     * @brief Display paging prompt message
     *
     * Shows appropriate message based on paging mode to inform
     * user how to continue.
     */
    void _showPagingMessage();

    /**
     * @brief Clear paging prompt and prepare for new content
     *
     * Removes paging prompt and resets console for continued output.
     */
    void _clearPagingMessage();

    /**
     * @brief Block execution until paging wait is resolved
     *
     * If the console is currently waiting for paging action, this method
     * will block until the wait is resolved (by timeout or user action).
     * This ensures print operations are truly blocked during paging.
     */
    void _waitForPagingIfNeeded();

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

    // Console Paging Configuration Methods

    /**
     * @brief Set console paging behavior mode
     *
     * Controls what happens when console reaches the bottom of the screen.
     *
     * @param mode Paging behavior mode:
     *             - PAGING_AUTO_CLEAR: Clear immediately (original behavior)
     *             - PAGING_WAIT_TIMEOUT: Wait for timeout before clearing (default)
     *             - PAGING_WAIT_BUTTON: Wait for right button press
     *             - PAGING_WAIT_BOTH: Wait for timeout OR button (whichever first)
     *
     * @example
     * @code
     * setPagingMode(PAGING_WAIT_BUTTON);  // Manual paging control
     * setPagingMode(PAGING_WAIT_TIMEOUT); // Auto-paging with delay
     * @endcode
     */
    void setPagingMode(ConsolePagingMode mode);

    /**
     * @brief Set paging timeout duration
     *
     * Sets how long to wait before auto-clearing when using timeout-based paging modes.
     *
     * @param timeoutMs Timeout in milliseconds (default: 5000ms = 5 seconds)
     *
     * @note Only affects PAGING_WAIT_TIMEOUT and PAGING_WAIT_BOTH modes
     * @note Minimum recommended timeout is 1000ms (1 second)
     *
     * @example
     * @code
     * setPagingTimeout(3000);  // 3 second timeout
     * setPagingMode(PAGING_WAIT_TIMEOUT);
     * @endcode
     */
    void setPagingTimeout(uint16_t timeoutMs);

    /**
     * @brief Get current paging mode
     * @return ConsolePagingMode Current paging behavior mode
     */
    ConsolePagingMode getPagingMode() const;

    /**
     * @brief Get current paging timeout
     * @return uint16_t Current timeout in milliseconds
     */
    uint16_t getPagingTimeout() const;

    /**
     * @brief Check if console is currently waiting for paging action
     * @return true if console is full and waiting for user action or timeout
     */
    bool isWaitingForPaging() const;

    /**
     * @brief Enable or disable auto-forward functionality
     *
     * When enabled, the console will automatically trigger actionTaken(BUTTON_MENU)
     * after _executeOnce() completes and the specified delay has passed.
     * This allows for automatic progression to the next screen without user intervention.
     *
     * @param enabled true to enable auto-forward, false to disable
     * @param delayMs Delay in milliseconds after _executeOnce() completion (default: 5000ms = 5 seconds)
     *
     * @note Auto-forward is disabled by default and must be explicitly enabled
     * @note The delay starts counting after _executeOnce() completes, not when the screen opens
     * @note Minimum recommended delay is 1000ms (1 second)
     * @note Auto-forward will not trigger if the user interacts with the screen before the delay expires
     *
     * @example
     * @code
     * setAutoForward(true, 3000);  // Auto-forward after 3 seconds
     * setAutoForward(false);       // Disable auto-forward
     * @endcode
     */
    void setAutoForward(bool enabled, unsigned long delayMs = 5000);

    /**
     * @brief Check if auto-forward is currently enabled
     * @return true if auto-forward is enabled
     */
    bool isAutoForwardEnabled() const;

    /**
     * @brief Get current auto-forward delay
     * @return unsigned long Current delay in milliseconds
     */
    unsigned long getAutoForwardDelay() const;

    /**
     * @brief Manually trigger page continuation
     *
     * Forces the console to clear and continue output, regardless of
     * current paging mode. Useful for programmatic control.
     *
     * @note Only has effect when console is waiting for paging
     */
    void continuePaging();

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
