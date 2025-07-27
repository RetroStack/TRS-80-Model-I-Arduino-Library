#ifndef ROMFEATURESMENU_H
#define ROMFEATURESMENU_H

#include <MenuScreen.h>

class ROMFeaturesMenu : public MenuScreen
{
public:
    ROMFeaturesMenu();
    Screen *actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY) override;
};

#endif
