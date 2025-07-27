#ifndef KEYBOARDTESTER_H
#define KEYBOARDTESTER_H

#include <ConsoleScreen.h>
#include <Keyboard.h>

class KeyboardTester : public ConsoleScreen
{
private:
    Keyboard _keyboard;
    unsigned long _lastUpdate;
    int _keyPressCount;

public:
    KeyboardTester();
    void loop() override;
    Screen *actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY) override;

private:
    void displayKeyboardStatus();
    void showInstructions();
};

#endif
