/*
 * M1Shield.h - Hardware abstraction layer for TRS-80 Model    unsigne    bool begin();                                       // Initialize shield hardware and displaynceButton(uint8_t pin, unsigned long &pressedState); // Internal debouncing helper for button state detectionino Shield
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef M1SHIELD_H
#define M1SHIELD_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include "Screen.h"
#include "DisplayProvider.h"
#include "ILogger.h"

// RGB LED color enumeration for status indication
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

// Analog joystick direction enumeration
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

// Hardware abstraction layer for the TRS-80 Model I Arduino Shield
class M1ShieldClass
{
private:
    Screen *_screen;                   // Currently active screen
    DisplayProvider *_displayProvider; // Display provider instance
    ILogger *_logger;                  // Logger instance for debugging output

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

    bool _activeJoystick; // True if joystick is currently active

    // Internal debouncing helper for button state detection
    unsigned long _getDebouncedState(int pin, unsigned long pressedState) const;

    void _active() const;   // Set shield to active state (internal)
    void _inactive() const; // Set shield to inactive state (internal)

public:
    M1ShieldClass(); // Default constructor initializes shield in uninitialized state

    ~M1ShieldClass(); // Destructor performs cleanup of display and screen resources

    bool begin();                          // Initialize shield hardware and display
    bool begin(DisplayProvider &provider); // Initialize shield with custom display provider

    void setLogger(ILogger &logger); // Set logger for debugging output
    ILogger *getLogger() const;      // Get logger instance

    void activateJoystick();   // Activate joystick input
    void deactivateJoystick(); // Deactivate joystick input

    bool isDisplayInitialized() const; // Check if display hardware was successfully initialized
    Adafruit_GFX &getGFX();            // Get reference to underlying graphics context

    uint16_t getScreenWidth() const;  // Get display width in pixels
    uint16_t getScreenHeight() const; // Get display height in pixels

    DisplayProvider &getDisplayProvider() const; // Get reference to the display provider

    bool display();                        // Update the display
    uint16_t convertColor(uint16_t color); // Convert a color value for the current display type

    bool setScreen(Screen *screen); // Set the active screen and perform transition

    void setLEDColor(uint8_t r, uint8_t g, uint8_t b) const;         // Set RGB LED color using individual channel control
    void setLEDColor(LEDColor color, uint8_t intensity = 255) const; // Set RGB LED color using predefined color enumeration

    // Button state detection methods

    bool isMenuPressed() const; // Check current menu button state (non-consuming)
    bool wasMenuPressed();      // Check and consume menu button press event

    bool isLeftPressed() const; // Check current left button state (non-consuming)
    bool wasLeftPressed();      // Check and consume left button press event

    bool isRightPressed() const; // Check current right button state (non-consuming)
    bool wasRightPressed();      // Check and consume right button press event

    bool isUpPressed() const; // Check current up button state (non-consuming)
    bool wasUpPressed();      // Check and consume up button press event

    bool isDownPressed() const; // Check current down button state (non-consuming)
    bool wasDownPressed();      // Check and consume down button press event

    bool isJoystickPressed() const; // Check current joystick button state (non-consuming)
    bool wasJoystickPressed();      // Check and consume joystick button press event

    JoystickDirection getJoystickDirection() const; // Get current joystick directional state
    bool isJoystickCentered() const;                // Check if joystick is in center (rest) position
    uint8_t getJoystickX() const;                   // Get raw joystick X-axis position
    uint8_t getJoystickY() const;                   // Get raw joystick Y-axis position

    // ========== Cassette Interface Methods ==========
    // WARNING: Incorrect usage can damage your Arduino!
    // CR1 and CR2 may be connected together on some systems.

    void setCR1Mode(bool isOutput) const; // Configure CR1 pin as input or output
    void setCR2Mode(bool isOutput) const; // Configure CR2 pin as input or output

    void writeCR1(bool value) const; // Write digital value to CR1 pin (when configured as output)
    void writeCR2(bool value) const; // Write digital value to CR2 pin (when configured as output)

    bool readCR1() const; // Read digital value from CR1 pin (when configured as input)
    bool readCR2() const; // Read digital value from CR2 pin (when configured as input)

    void writeCassetteIn(uint8_t value) const; // Write analog value to Model 1 cassette input (A14)
    uint16_t readCassetteOut() const;          // Read analog value from Model 1 cassette output (A15)

    void loop(); // Main update loop for shield operations

private:
    M1ShieldClass(const M1ShieldClass &) = delete;            // Disable copy constructor - shield manages hardware resources
    M1ShieldClass &operator=(const M1ShieldClass &) = delete; // Disable copy assignment - shield manages hardware resources
};

extern M1ShieldClass M1Shield; // Global M1Shield instance for hardware access

#endif /* M1SHIELD_H */