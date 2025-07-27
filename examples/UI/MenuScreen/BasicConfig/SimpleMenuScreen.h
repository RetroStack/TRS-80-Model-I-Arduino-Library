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
private:
    // Menu items that will be displayed
    static constexpr const char *MAIN_MENU_ITEMS[12] = {
        "Run BASIC",
        "Load Program",
        "Save Program",
        "Font: A/B",
        "View: Left",
        "Settings",
        "Games",
        "Utilities",
        "About",
        "Help",
        "Debug Mode",
        "Exit"};
    static constexpr uint8_t MAIN_MENU_COUNT = 12;

protected:
    // Called when user selects a menu item
    // Return nullptr to stay on menu, or new Screen() to navigate
    Screen *_getSelectedMenuItemScreen(int index) override;

public:
    SimpleMenuScreen();
    virtual ~SimpleMenuScreen() = default;
};

#endif /* SIMPLE_MENU_SCREEN_H */
