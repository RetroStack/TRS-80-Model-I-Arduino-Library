#ifndef TERMINALDEMO_H
#define TERMINALDEMO_H

#include <M1TerminalScreen.h>

class TerminalDemo : public M1TerminalScreen
{
private:
    unsigned long _lastDemoUpdate;
    int _demoStep;
    bool _demoRunning;

public:
    TerminalDemo();
    void loop() override;
    Screen *actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY) override;

private:
    void runTerminalDemo();
    void showDemoInstructions();
};

#endif
