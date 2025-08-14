/*
 * Screen.h - Abstract base class for all screen implementations in the TRS-80 Model I library
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef SCREEN_H
#define SCREEN_H

#include <Arduino.h>

/**
 * @brief Input action enumeration using bit flags for combining multiple simultaneous actions
 *
 * ActionTaken uses bit flags to allow detection of multiple simultaneous inputs.
 * Values are organized in ranges:
 * - 1-128: Joystick directions
 * - 256-8192: Button presses
 * - Convenience masks for common input combinations
 *
 * @note Use bitwise operations to check for multiple actions: `if (action & BUTTON_LEFT)`
 */
enum ActionTaken : uint16_t
{
    NONE = 0,

    // Joystick directions (bit range 1-128)
    JOYSTICK_LEFT = 1,         // Joystick moved left
    JOYSTICK_RIGHT = 2,        // Joystick moved right
    JOYSTICK_UP = 4,           // Joystick moved up
    JOYSTICK_DOWN = 8,         // Joystick moved down
    JOYSTICK_UP_LEFT = 16,     // Joystick moved diagonally up-left
    JOYSTICK_UP_RIGHT = 32,    // Joystick moved diagonally up-right
    JOYSTICK_DOWN_LEFT = 64,   // Joystick moved diagonally down-left
    JOYSTICK_DOWN_RIGHT = 128, // Joystick moved diagonally down-right

    // Button presses (bit range 256-8192)
    BUTTON_MENU = 256,      // Menu button pressed
    BUTTON_LEFT = 512,      // Left directional button pressed
    BUTTON_RIGHT = 1024,    // Right directional button pressed
    BUTTON_UP = 2048,       // Up directional button pressed
    BUTTON_DOWN = 4096,     // Down directional button pressed
    BUTTON_JOYSTICK = 8192, // Joystick button (center press) pressed

    // Convenience masks for input detection
    JOYSTICK_ANY = JOYSTICK_LEFT | JOYSTICK_RIGHT | JOYSTICK_UP | JOYSTICK_DOWN |
                   JOYSTICK_UP_LEFT | JOYSTICK_UP_RIGHT | JOYSTICK_DOWN_LEFT | JOYSTICK_DOWN_RIGHT,    // Any joystick movement
    BUTTON_ANY = BUTTON_MENU | BUTTON_LEFT | BUTTON_RIGHT | BUTTON_UP | BUTTON_DOWN | BUTTON_JOYSTICK, // Any button press
    BUTTON_DIRECTIONAL = BUTTON_LEFT | BUTTON_RIGHT | BUTTON_UP | BUTTON_DOWN,                         // Any directional button (excludes menu/joystick buttons)

    // Directional input masks (joystick + button combinations)
    LEFT_ANY = JOYSTICK_LEFT | JOYSTICK_UP_LEFT | JOYSTICK_DOWN_LEFT | BUTTON_LEFT,      // Any leftward input
    RIGHT_ANY = JOYSTICK_RIGHT | JOYSTICK_UP_RIGHT | JOYSTICK_DOWN_RIGHT | BUTTON_RIGHT, // Any rightward input
    UP_ANY = JOYSTICK_UP | JOYSTICK_UP_LEFT | JOYSTICK_UP_RIGHT | BUTTON_UP,             // Any upward input
    DOWN_ANY = JOYSTICK_DOWN | JOYSTICK_DOWN_LEFT | JOYSTICK_DOWN_RIGHT | BUTTON_DOWN    // Any downward input
};

/**
 * @brief Abstract base class for all screen implementations
 *
 * The Screen class provides the foundational interface for all user interface screens.
 * It implements a standard lifecycle pattern with activation states, input handling,
 * and rendering management.
 *
 * ## Lifecycle
 *
 * 1. **Construction**: Screen created but inactive
 * 2. **open()**: Screen activated, display initialized
 * 3. **loop()**: Called repeatedly while active for updates
 * 4. **actionTaken()**: Called when user input occurs
 * 5. **close()**: Screen deactivated, cleanup performed
 *
 * ## Thread Safety
 *
 * Screen instances are designed for single-threaded use within the Arduino main loop.
 * Multiple screens should not be active simultaneously on the same display.
 *
 * ## Memory Management
 *
 * - Screens are managed by M1Shield which frees them as needed.
 * - New screens are provided through the `actionTaken` method of the current screen.
 * - Only one screen is active at a time to conserve memory.
 * - Copy operations are disabled to prevent accidental duplication.
 * - Virtual destructor ensures proper cleanup of derived classes.
 *
 * @example Basic Screen Implementation
 * ```cpp
 * class MyScreen : public Screen {
 * protected:
 *     void _drawScreen() override {
 *         // Initialize display content
 *     }
 *
 * public:
 *     void loop() override {
 *         // Update screen content each frame
 *     }
 *
 *     Screen* actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY) override {
 *         if (action & BUTTON_MENU) {
 *             // Handle menu button
 *             return previousScreen;  // Return to navigate
 *         }
 *         return nullptr;  // Stay on current screen
 *     }
 * };
 * ```
 *
 * @see ActionTaken For input handling constants
 */
class Screen
{
private:
    bool _active; // Track whether this screen is currently active

protected:
    /**
     * @brief Pure virtual method for initial screen rendering
     *
     * Called when the screen is first activated via open(). Derived classes
     * must implement this to set up their initial display state.
     *
     * @note This should only handle initial setup of full screen refreshes. Use loop() for changes.
     */
    virtual void _drawScreen() = 0;

public:
    /**
     * @brief Default constructor initializes screen in inactive state
     */
    Screen();

    /**
     * @brief Virtual destructor ensures proper cleanup of derived classes
     */
    virtual ~Screen() = default;

    /**
     * @brief Check if the current display is considered small (height <= 128 pixels)
     *
     * Small displays typically include OLED screens (128x64) that require more compact
     * layouts compared to larger TFT displays (320x240, 480x320, etc.). This method
     * provides a unified way for all screen implementations to adapt their layouts
     * for different display sizes.
     *
     * @return true if display height is 128 pixels or smaller, false for larger displays
     *
     * @example Adaptive Layout
     * ```cpp
     * void MyScreen::_drawScreen() {
     *     if (isSmallDisplay()) {
     *         // Use compact layout for OLED displays
     *         drawCompactHeader();
     *         drawCompactContent();
     *     } else {
     *         // Use full layout for larger TFT displays
     *         drawFullHeader();
     *         drawDetailedContent();
     *     }
     * }
     * ```
     */
    bool isSmallDisplay() const;

    /**
     * @brief Check if this screen is currently active
     * @return true if screen is active and should be processing input/updates
     */
    bool isActive() const;

    /**
     * @brief Force a complete redraw of the screen content
     *
     * Calls _drawScreen() if the screen is currently active. Use this when
     * the display state may have been corrupted or needs complete refresh.
     */
    void refresh();

    /**
     * @brief Activate the screen and perform initial setup
     *
     * Transitions the screen from inactive to active state and triggers
     * initial rendering. Safe to call multiple times - subsequent calls
     * on an already active screen return true immediately.
     *
     * @return true if activation was successful, false on error
     * @note Override this in derived classes for custom initialization logic
     */
    virtual bool open();

    /**
     * @brief Deactivate the screen and perform cleanup
     *
     * Transitions the screen from active to inactive state. Override in
     * derived classes to implement custom cleanup logic.
     *
     * @note Safe to call multiple times on inactive screens
     */
    virtual void close();

    /**
     * @brief Main update loop called every frame while screen is active
     *
     * This pure virtual method must be implemented by derived classes to
     * handle ongoing screen updates, animations, and time-based logic.
     * Called repeatedly by the main application loop.
     *
     * @note Should be efficient as it's called frequently
     */
    virtual void loop() = 0;

    /**
     * @brief Handle user input events and return navigation result
     *
     * This pure virtual method processes user input actions and determines
     * navigation behavior. The return value controls screen transitions.
     *
     * @param action Bit flags indicating which inputs are active
     * @param offsetX Horizontal offset for touch/pointer input (0-based)
     * @param offsetY Vertical offset for touch/pointer input (0-based)
     *
     * @return Screen* Navigation result:
     *         - nullptr: Stay on current screen
     *         - other Screen*: Navigate to different screen
     *
     * @example Input Handling
     * ```cpp
     * Screen* MyScreen::actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY) {
     *     if (action & BUTTON_MENU) {
     *         return &mainMenu;  // Navigate to main menu
     *     }
     *     if (action != NONE) {
     *         handleMovement(action);
     *         return nullptr;  // Stay on current screen
     *     }
     *     return nullptr;
     * }
     * ```
     */
    virtual Screen *actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY) = 0;

    /**
     * @brief Disable copy constructor to prevent accidental screen duplication
     *
     * Screens maintain state and should not be copied. Use pointers or
     * references for screen management.
     */
    Screen(const Screen &) = delete;

    /**
     * @brief Disable copy assignment to prevent accidental screen duplication
     *
     * Screens maintain state and should not be copied. Use pointers or
     * references for screen management.
     */
    Screen &operator=(const Screen &) = delete;
};

#endif /* SCREEN_H */