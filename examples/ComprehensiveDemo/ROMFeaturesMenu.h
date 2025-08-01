#ifndef ROMFEATURESMENU_H
#define ROMFEATURESMENU_H

#include <MenuScreen.h>

class ROMFeaturesMenu : public MenuScreen
{
protected:
    Screen *_getSelectedMenuItemScreen(int index) override;

public:
    ROMFeaturesMenu();
};

#endif
