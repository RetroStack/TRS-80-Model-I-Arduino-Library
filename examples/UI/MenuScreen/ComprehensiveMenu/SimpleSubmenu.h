/**
 * ===================================================================
 * Simple Submenu Example
 * ===================================================================
 *
 * This demonstrates MenuScreen navigation capabilities by providing
 * a simple submenu that can be navigated to from the main menu.
 */

#ifndef SIMPLE_SUBMENU_H
#define SIMPLE_SUBMENU_H

#include <MenuScreen.h>

class SimpleSubmenu : public MenuScreen
{
protected:
    // Handle menu item selection - return screen to navigate to
    Screen *_getSelectedMenuItemScreen(int index) override;

public:
    SimpleSubmenu();
    virtual ~SimpleSubmenu() = default;
};

#endif /* SIMPLE_SUBMENU_H */
