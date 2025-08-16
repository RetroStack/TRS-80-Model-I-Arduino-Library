/**
 * SimpleMenuScreen - Basic MenuScreen Example for Beginners
 *
 * Shows how to create a simple menu using the MenuScreen framework.
 * MenuScreen automatically handles navigation, you just handle selections.
 */

#ifndef SIMPLE_MENU_SCREEN_H
#define SIMPLE_MENU_SCREEN_H

#include "MenuScreen.h"

class SimpleMenuScreen : public MenuScreen
{
protected:
    // Called when user selects a menu item
    // Return nullptr to stay on menu, or new Screen() to navigate
    Screen *_getSelectedMenuItemScreen(int index);

public:
    SimpleMenuScreen();
    virtual ~SimpleMenuScreen() = default;
};

#endif /* SIMPLE_MENU_SCREEN_H */
