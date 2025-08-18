/**
 * Simple Example Screen - Beginner Tutorial
 *
 * This shows the absolute minimum needed to create a working screen.
 * Perfect for learning the basics!
 */

#ifndef SIMPLE_SCREEN_H
#define SIMPLE_SCREEN_H

#include <Screen.h>

class SimpleScreen : public Screen
{
private:
    int _buttonPressCount; // Count how many times buttons were pressed
    int _ledColor;         // Which LED color to show (0-7)

protected:
    void _drawScreen() override; // This draws what you see on screen

public:
    SimpleScreen();

    void loop() override;                                                               // This runs continuously
    Screen *actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY) override; // This handles button presses
};

#endif /* SIMPLE_SCREEN_H */
