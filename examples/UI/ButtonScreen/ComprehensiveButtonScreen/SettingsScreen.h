#ifndef SETTINGS_SCREEN_H
#define SETTINGS_SCREEN_H

#include <ConsoleScreen.h>
#include "ComprehensiveButton.h"

class SettingsScreen : public ConsoleScreen
{
private:
    int _settingType;

public:
    SettingsScreen(int type);

protected:
    void _executeOnce() override;
    Screen *actionTaken(ActionTaken action, int8_t offsetX, int8_t offsetY) override;
};

#endif /* SETTINGS_SCREEN_H */
