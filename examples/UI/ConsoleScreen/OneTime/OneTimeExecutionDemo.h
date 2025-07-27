#ifndef ONETIMEEXECUTIONDEMO_H
#define ONETIMEEXECUTIONDEMO_H

#include <ConsoleScreen.h>

class OneTimeExecutionDemo : public ConsoleScreen
{
private:
    bool _systemInitialized;
    unsigned long _lastTimeUpdate;
    int _messageCounter;

protected:
    void _executeOnce() override;

public:
    OneTimeExecutionDemo();
    void loop() override;
    Screen *actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY) override;
};

#endif
