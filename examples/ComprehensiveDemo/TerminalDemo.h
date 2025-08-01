#ifndef TERMINALDEMO_H
#define TERMINALDEMO_H

#include <M1TerminalScreen.h>

class TerminalDemo : public M1TerminalScreen
{
public:
    TerminalDemo();
    Screen *actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY) override;
};

#endif
