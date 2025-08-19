/*
 * InputDemoScreen.h
 *
 * Demonstrates ALL inprivate:
    // Organized layout methods - each handles small/large display logic internally
    void drawControllerLayout(); // Button layout (compact for small, full for large)
    void drawJoystickArea();     // Joystick visualization (text for small, visual for large)
    void drawStatusArea();       // Status and information area

    // Selective update methods for performance optimization
    void updateAfterInput(ActionTaken action, int8_t offsetX, int8_t offsetY);
    void updateButtonVisuals();   // Update only button area
    void updateJoystickVisual();  // Update only joystick area
    void updateStatusVisual();    // Update only status area

    // Utility methods
    void drawButton(int x, int y, int w, int h, const char *label, bool pressed, uint16_t pressedColor);
    void resetButtonColors();
    uint16_t getActionColor(ActionTaken action);
    const char *getActionName(ActionTaken action);lities of the Screen class:
 * - All button types (MENU, LEFT, RIGHT, UP, DOWN, JOYSTICK)
 * - All joystick directions including diagonals
 * - Analog joystick offset values (offsetX, offsetY)
 * - Input combination detection
 * - Real-time input state display
 */

#ifndef INPUT_DEMO_SCREEN_H
#define INPUT_DEMO_SCREEN_H

#include <Arduino.h>
#include <Screen.h>
#include <M1Shield.h>

class InputDemoScreen : public Screen
{
private:
    // Track all possible inputs
    struct InputState
    {
        bool menuPressed;
        bool leftPressed;
        bool rightPressed;
        bool upPressed;
        bool downPressed;
        bool joystickPressed;
        int joystickX;
        int joystickY;
        unsigned long lastInputTime;
        int totalInputs;
        String lastAction;
    } inputState;

    // Visual feedback
    int buttonTestColors[6]; // Colors for each button when pressed
    unsigned long colorResetTime;

public:
    InputDemoScreen();

    void _drawScreen() override;
    void loop() override;
    Screen *actionTaken(ActionTaken action, int8_t offsetX, int8_t offsetY) override;
    bool open() override;

private:
    // New organized layout methods
    void drawCompactLayout();    // For small displays
    void drawControllerLayout(); // Button layout like a real controller
    void drawJoystickArea();     // Prominent joystick visualization
    void drawStatusArea();       // Status and information area

    // Selective update methods for performance optimization
    void updateAfterInput(ActionTaken action, int8_t offsetX, int8_t offsetY);
    void updateCompactStatus();  // Update only compact status area
    void updateButtonVisuals();  // Update only button area
    void updateJoystickVisual(); // Update only joystick area
    void updateStatusVisual();   // Update only status area

    // Utility methods
    void drawButton(int x, int y, int w, int h, const char *label, bool pressed, uint16_t pressedColor);
    void resetButtonColors();
    uint16_t getActionColor(ActionTaken action);
    const char *getActionName(ActionTaken action);
};

#endif /* INPUT_DEMO_SCREEN_H */
