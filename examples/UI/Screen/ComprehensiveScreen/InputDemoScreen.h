/*
 * InputDemoScreen.h
 *
 * Demonstrates ALL input handling capabilities of the Screen class:
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
    Screen *actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY) override;
    bool open() override;

private:
    void drawInputStatus();
    void drawJoystickVisualization();
    void drawButtonStatus();
    void drawInputHistory();
    void resetButtonColors();
    uint16_t getActionColor(ActionTaken action);
    const char *getActionName(ActionTaken action);
};

#endif /* INPUT_DEMO_SCREEN_H */
