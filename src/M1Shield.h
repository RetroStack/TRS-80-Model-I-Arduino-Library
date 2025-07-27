/*
 * M1Shield.h - Hardware abstraction layer for TRS-80 Model I Arduino Shield
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef M1SHIELD_H
#define M1SHIELD_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include "display_config.h"

// TFT Display Support - Define ONE of these before including this header
#if defined(USE_ST7789)
// ST7789 (240x320 SPI) – very common, vibrant display
#include <Adafruit_ST7789.h>
typedef Adafruit_ST7789 TFT_Display;
#elif defined(USE_ST7789_240x240)
// ST7789 (240x240 SPI) – very common, vibrant display
#include <Adafruit_ST7789.h>
typedef Adafruit_ST7789 TFT_Display;
#elif defined(USE_ST7735)
// ST7735 (128x160 SPI) – older/smaller displays
#include <Adafruit_ST7735.h>
typedef Adafruit_ST7735 TFT_Display;
#elif defined(USE_ILI9341)
// ILI9341 (240x320 SPI) – widely used 2.4–2.8" displays
#include <Adafruit_ILI9341.h>
typedef Adafruit_ILI9341 TFT_Display;
#elif defined(USE_ILI9488)
// ILI9488 (320x480 SPI, 3-byte RGB888) – newer 3.5" displays
#include <Arduino_GFX_Library.h>
typedef Arduino_ILI9488 TFT_Display;
#elif defined(USE_HX8357)
// HX8357D (320x480 SPI) – 3.5" displays, older than ILI9488
#include <Adafruit_HX8357.h>
typedef Adafruit_HX8357 TFT_Display;
#elif defined(USE_ILI9325)
// ILI9325 (240x320, usually parallel interface) – for some shield-based displays
#include <Adafruit_TFTLCD.h> // Adafruit's legacy parallel TFT library
typedef Adafruit_TFTLCD TFT_Display;
#else
// Default to ST7789 if no display type is specified
// You can change this default to another screen if needed
#include <Adafruit_ST7789.h>
typedef Adafruit_ST7789 TFT_Display;
#endif

#include "Screen.h"

/**
 * @brief RGB LED color enumeration for status indication
 *
 * Predefined color combinations for the onboard RGB status LED.
 * Colors are used to indicate system states, errors, and user feedback.
 */
enum LEDColor
{
    COLOR_OFF,     // LED turned off
    COLOR_RED,     // Red color - typically for errors or alerts
    COLOR_GREEN,   // Green color - typically for success or ready states
    COLOR_BLUE,    // Blue color - typically for information or processing
    COLOR_YELLOW,  // Yellow color - typically for warnings
    COLOR_MAGENTA, // Magenta color - typically for special states
    COLOR_CYAN,    // Cyan color - typically for active operations
    COLOR_WHITE,   // White color - maximum brightness
};

/**
 * @brief Analog joystick direction enumeration
 *
 * Possible directional states read from the analog joystick.
 * Diagonal directions are supported through combination detection.
 */
enum JoystickDirection
{
    CENTER,    // Joystick at rest position (center)
    LEFT,      // Joystick moved left
    RIGHT,     // Joystick moved right
    UP,        // Joystick moved up
    DOWN,      // Joystick moved down
    UP_LEFT,   // Joystick moved diagonally up-left
    UP_RIGHT,  // Joystick moved diagonally up-right
    DOWN_LEFT, // Joystick moved diagonally down-left
    DOWN_RIGHT // Joystick moved diagonally down-right
};

/**
 * @brief Hardware abstraction layer for the TRS-80 Model I Arduino Shield
 *
 * M1ShieldClass provides a unified interface to all hardware components on the
 * TRS-80 Model I shield, including input devices, display, and status indicators.
 * It manages the complete hardware lifecycle and provides a clean API for
 * screen-based applications.
 *
 * ## Hardware Components
 *
 * - **TFT Display**: Configurable support for ST7789, ST7735, ILI9341, ILI9488, HX8357, ILI9325
 * - **Input Controls**: 5 digital buttons + analog joystick with center press
 * - **RGB Status LED**: Full-color status indication
 * - **Screen Management**: Automatic screen lifecycle and navigation
 *
 * ## Display Support
 *
 * The shield supports multiple TFT display types through compile-time configuration:
 * ```cpp
 * #define USE_ST7789   // For ST7789 displays (default)
 * #define USE_ST7789_240x240   // For ST7789 displays (240x240 resolution)
 * #define USE_ST7735   // For ST7735 displays
 * #define USE_ILI9341  // For ILI9341 displays
 * #define USE_ILI9488  // For ILI9488 displays
 * #define USE_HX8357  // For HX8357 displays
 * #define USE_ILI9325  // For ILI9325 displays
 * ```
 *
 * ## Input System
 *
 * - **Digital Buttons**: Menu, Left, Right, Up, Down
 * - **Analog Joystick**: 8-directional movement + center press
 * - **Debouncing**: Built-in 50ms debouncing for all inputs
 * - **Multi-input**: Simultaneous input detection using bit flags
 *
 * ## Screen Management
 *
 * The M1Shield manages screen lifecycle automatically:
 * 1. Current screen receives input via `actionTaken()`
 * 2. Screen returns new screen pointer for navigation
 * 3. M1Shield handles screen transitions (close old, open new)
 * 4. Only one screen active at a time for memory efficiency
 *
 * ## Error Handling
 *
 * - Invalid screen pointers are handled gracefully
 * - Display operations include error recovery
 *
 * @example Basic Usage
 * ```cpp
 * void setup() {
 *     M1Shield.begin();
 *     M1Shield.setScreen(new MyScreen());
 *     M1Shield.setLED(COLOR_GREEN);  // Ready indicator
 * }
 *
 * void loop() {
 *     M1Shield.loop();  // Handle input and screen updates
 * }
 * ```
 *
 * @see Screen For screen implementation details
 * @see LEDColor For status indication options
 */
class M1ShieldClass
{
private:
#if defined(USE_ILI9488)
    TFT_Display *_tft;     // Pointer for Arduino_GFX displays
    Arduino_DataBus *_bus; // DataBus for ILI9488
#else
    TFT_Display _tft; // Object for Adafruit_GFX displays
#endif
    Screen *_screen; // Currently active screen

    // Button debouncing state tracking
    unsigned long _menuPressed;     // Last menu button press timestamp
    unsigned long _upPressed;       // Last up button press timestamp
    unsigned long _downPressed;     // Last down button press timestamp
    unsigned long _leftPressed;     // Last left button press timestamp
    unsigned long _rightPressed;    // Last right button press timestamp
    unsigned long _joystickPressed; // Last joystick button press timestamp

    // Display properties
    uint16_t _screenWidth;  // Display width in pixels
    uint16_t _screenHeight; // Display height in pixels

    /**
     * @brief Internal debouncing helper for button state detection
     * @param pin GPIO pin number to read
     * @param pressedState Reference to timestamp for this button
     * @return Latest timestamp (0 = released, any other is pressed)
     */
    unsigned long _getDebouncedState(int pin, unsigned long pressedState) const;

    /**
     * @brief Set shield to active state (internal)
     */
    void _active() const;

    /**
     * @brief Set shield to inactive state (internal)
     */
    void _inactive() const;

public:
    /**
     * @brief Default constructor initializes shield in uninitialized state
     */
    M1ShieldClass();

    /**
     * @brief Destructor performs cleanup of display and screen resources
     */
    ~M1ShieldClass();

    /**
     * @brief Initialize shield hardware and display
     *
     * Performs complete hardware initialization including:
     * - TFT display setup and configuration
     * - GPIO pin configuration for buttons and LED
     * - Screen dimension detection
     * - Hardware validation
     *
     * @note Must be called before any other shield operations
     * @note Safe to call multiple times - subsequent calls return cached result
     */
    void begin();

    /**
     * @brief Check if display hardware was successfully initialized
     * @return true if display is ready for use
     */
    bool isDisplayInitialized() const;

    /**
     * @brief Get reference to underlying graphics context
     *
     * Provides direct access to Adafruit_GFX for advanced drawing operations.
     * Use this for custom graphics that bypass the screen system.
     *
     * @return Reference to Adafruit_GFX instance
     * @note Direct graphics operations may interfere with active screens
     */
    Adafruit_GFX &getGFX();

    /**
     * @brief Get display width in pixels
     * @return Display width, or 0 if not initialized
     */
    uint16_t getScreenWidth() const;

    /**
     * @brief Get display height in pixels
     * @return Display height, or 0 if not initialized
     */
    uint16_t getScreenHeight() const;

    /**
     * @brief Set the active screen and perform transition
     *
     * Changes the currently active screen, handling proper lifecycle:
     * 1. Close current screen if one exists
     * 2. Set new screen as active
     * 3. Open new screen for display
     *
     * @param screen Pointer to new screen to activate, or nullptr to clear
     * @return true if screen transition successful
     *
     * @note Previous screen is closed but not deleted - caller retains ownership
     * @note New screen must remain valid while active
     */
    bool setScreen(Screen *screen);

    /**
     * @brief Set RGB LED color using individual channel control
     * @param r Red channel intensity (0-255)
     * @param g Green channel intensity (0-255)
     * @param b Blue channel intensity (0-255)
     */
    void setLEDColor(uint8_t r, uint8_t g, uint8_t b) const;

    /**
     * @brief Set RGB LED color using predefined color enumeration
     * @param color Predefined color from LEDColor enumeration
     * @see LEDColor For available color options
     */
    void setLEDColor(LEDColor color, uint8_t intensity = 255) const;

    // Button state detection methods

    /**
     * @brief Check current menu button state (non-consuming)
     * @return true if menu button is currently pressed
     */
    bool isMenuPressed() const;

    /**
     * @brief Check and consume menu button press event
     * @return true if button was pressed since last call (one-shot)
     */
    bool wasMenuPressed();

    /**
     * @brief Check current left button state (non-consuming)
     * @return true if left button is currently pressed
     */
    bool isLeftPressed() const;

    /**
     * @brief Check and consume left button press event
     * @return true if button was pressed since last call (one-shot)
     */
    bool wasLeftPressed();

    /**
     * @brief Check current right button state (non-consuming)
     * @return true if right button is currently pressed
     */
    bool isRightPressed() const;

    /**
     * @brief Check and consume right button press event
     * @return true if button was pressed since last call (one-shot)
     */
    bool wasRightPressed();

    /**
     * @brief Check current up button state (non-consuming)
     * @return true if up button is currently pressed
     */
    bool isUpPressed() const;

    /**
     * @brief Check and consume up button press event
     * @return true if button was pressed since last call (one-shot)
     */
    bool wasUpPressed();

    /**
     * @brief Check current down button state (non-consuming)
     * @return true if down button is currently pressed
     */
    bool isDownPressed() const;

    /**
     * @brief Check and consume down button press event
     * @return true if button was pressed since last call (one-shot)
     */
    bool wasDownPressed();

    /**
     * @brief Check current joystick button state (non-consuming)
     * @return true if joystick center button is currently pressed
     */
    bool isJoystickPressed() const;

    /**
     * @brief Check and consume joystick button press event
     * @return true if button was pressed since last call (one-shot)
     */
    bool wasJoystickPressed();

    /**
     * @brief Get current joystick directional state
     *
     * Reads analog joystick position and returns the primary direction.
     * Supports 8-directional movement including diagonal directions.
     *
     * @return JoystickDirection Current direction or CENTER if at rest
     * @see JoystickDirection For possible return values
     */
    JoystickDirection getJoystickDirection() const;

    /**
     * @brief Check if joystick is in center (rest) position
     * @return true if joystick is centered within deadzone
     */
    bool isJoystickCentered() const;

    /**
     * @brief Get raw joystick X-axis position
     * @return X-axis analog value (0-255, 128 = center)
     */
    uint8_t getJoystickX() const;

    /**
     * @brief Get raw joystick Y-axis position
     * @return Y-axis analog value (0-255, 128 = center)
     */
    uint8_t getJoystickY() const;

    /**
     * @brief Main update loop for shield operations
     *
     * Handles all shield operations including:
     * - Input polling and debouncing
     * - Screen update calls
     * - Input event processing and screen navigation
     * - Hardware state management
     *
     * @note Must be called regularly from main Arduino loop()
     * @note Automatically manages screen lifecycle and transitions
     */
    void loop();

    /**
     * @brief Disable copy constructor - shield manages hardware resources
     *
     * Hardware resources cannot be safely shared between instances.
     */
    M1ShieldClass(const M1ShieldClass &) = delete;

    /**
     * @brief Disable copy assignment - shield manages hardware resources
     *
     * Hardware resources cannot be safely shared between instances.
     */
    M1ShieldClass &operator=(const M1ShieldClass &) = delete;
};

/**
 * @brief Global M1Shield instance for hardware access
 *
 * Singleton instance providing system-wide access to shield hardware.
 * Use this instance for all shield operations rather than creating
 * additional instances.
 *
 * @example
 * ```cpp
 * void setup() {
 *     M1Shield.begin();
 *     M1Shield.setScreen(&myScreen);
 * }
 * ```
 */
extern M1ShieldClass M1Shield;

#endif /* M1SHIELD_H */