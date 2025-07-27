#ifndef DEBUGCONSOLE_H
#define DEBUGCONSOLE_H

#include <ConsoleScreen.h>

class DebugConsole : public ConsoleScreen
{
private:
    unsigned long _lastUpdate;
    int _counter;

public:
    DebugConsole();
    void loop() override;
    Screen *actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY) override;
};

#endif
