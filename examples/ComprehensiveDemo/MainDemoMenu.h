#ifndef MAINDEMOMENU_H
#define MAINDEMOMENU_H

#include <MenuScreen.h>

class MainDemoMenu : public MenuScreen
{
public:
    MainDemoMenu();

protected:
    Screen *_getSelectedMenuItemScreen(int index) override;
};

#endif
