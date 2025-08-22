#ifndef INFO_SCREEN_H
#define INFO_SCREEN_H

#include <ConsoleScreen.h>

class InfoScreen : public ConsoleScreen
{
public:
    InfoScreen();

protected:
    void _executeOnce() override;
    Screen *actionTaken(ActionTaken action, int8_t offsetX, int8_t offsetY) override;
};

#endif /* INFO_SCREEN_H */
