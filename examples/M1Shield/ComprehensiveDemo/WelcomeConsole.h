#ifndef WELCOMECONSOLE_H
#define WELCOMECONSOLE_H

#include <ConsoleScreen.h>

class WelcomeConsole : public ConsoleScreen
{
private:
    unsigned long _startTime;
    bool _introComplete;
    int _animationStep;

public:
    WelcomeConsole();
    void loop() override;
    Screen *actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY) override;

private:
    void showWelcomeAnimation();
    void showFeatureList();
    void showInstructions();
};

#endif
