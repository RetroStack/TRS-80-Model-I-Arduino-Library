#ifndef ROMDETECTIONDEMO_H
#define ROMDETECTIONDEMO_H

#include <ConsoleScreen.h>
#include <ROM.h>

class ROMDetectionDemo : public ConsoleScreen
{
private:
    ROM _rom;

public:
    ROMDetectionDemo();
    Screen *actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY) override;

private:
    void _executeOnce();
};

#endif
