/*
 * Screen.h - Abstract base class for all screen implementations in the TRS-80 Model I library
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef SCREEN_H
#define SCREEN_H

#include <Arduino.h>
#include "ILogger.h"

// Input action enumeration using bit flags for combining multiple simultaneous actions
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

// Abstract base class for all screen implementations
class Screen
{
private:
    bool _active;     // Track whether this screen is currently active
    ILogger *_logger; // Logger instance for debugging output

protected:
    virtual void _drawScreen() = 0; // Pure virtual method for initial screen rendering

public:
    Screen();                    // Default constructor initializes screen in inactive state
    virtual ~Screen() = default; // Virtual destructor ensures proper cleanup of derived classes
    bool isSmallDisplay() const; // Check if the current display is considered small (height <= 128 pixels)

    void setLogger(ILogger &logger); // Set logger for debugging output
    ILogger *getLogger() const;      // Get logger instance

    bool isActive() const; // Check if this screen is currently active

    void refresh(); // Force a complete redraw of the screen content

    virtual bool open(); // Activate the screen and perform initial setup

    virtual void close(); // Deactivate the screen and perform cleanup

    virtual void loop() = 0; // Main update loop called every frame while screen is active

    // Handle user input events and return navigation result
    virtual Screen *actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY) = 0;

    Screen(const Screen &) = delete; // Disable copy constructor to prevent screen duplication

    Screen &operator=(const Screen &) = delete; // Disable copy assignment to prevent screen duplication
};

#endif /* SCREEN_H */