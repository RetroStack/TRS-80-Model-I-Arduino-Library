/*
 * SimpleMenu.h
 *
 * A simple MenuScreen demonstration showing basic menu functionality.
 * Perfect for learning the basics!
 */

#ifndef SIMPLE_MENU_H
#define SIMPLE_MENU_H

#include <Arduino.h>
#include <MenuScreen.h>
#include <M1Shield.h>

/*
 * SimpleMenu - A basic menu example for LED color selection
 *
 * This class demonstrates the minimum requirements for creating
 * a functional MenuScreen:
 * 1. Inherit from MenuScreen
 * 2. Implement the constructor to set up menu items
 * 3. Implement _getSelectedMenuItemScreen() to handle selections
 *
 * Features demonstrated:
 * - Basic menu setup with items and title
 * - Simple item selection handling
 * - LED color control via M1Shield
 * - Serial output for debugging
 * - Return to same menu after selections
 */
class SimpleMenu : public MenuScreen
{
private:
    int currentLedColor; // Current LED color index (0-7)

public:
    // Constructor
    SimpleMenu();

    // MenuScreen overrides
    Screen *_getSelectedMenuItemScreen(int index);

protected:
    // Override for debugging
    void _drawContent() override;

private:
    void setLedColor(int colorIndex);
    void showSelectionFeedback(const char *itemName);
};

#endif /* SIMPLE_MENU_H */
