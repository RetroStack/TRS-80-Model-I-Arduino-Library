/*
 * MenuScreen.h - Abstract base class for paginated menu screens
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef MENU_SCREEN_H
#define MENU_SCREEN_H

#include <Arduino.h>
#include "ContentScreen.h"

// Abstract base class for paginated menu screens with navigation support
class MenuScreen : public ContentScreen
{
private:
    uint8_t _currentPage; // Current page being displayed (0-based)

    char **_menuItems;      // Dynamically allocated array of menu item strings
    uint8_t _menuItemCount; // Total number of menu items across all pages

    uint8_t _selectedMenuItemIndex; // Currently selected menu item index (global, not page-relative)

    uint8_t _getItemsPerPage() const; // Calculate maximum items that can fit on one page

    uint8_t _findNextEnabledItem(uint8_t startIndex, bool forward) const; // Find the next enabled menu item

protected:
    void _drawContent(); // Draw the menu content area with paginated item list

    void _setSelectedMenuItemIndex(uint8_t index); // Set the currently selected menu item by index

    uint8_t _getSelectedMenuItemIndex() const; // Get the index of the currently selected menu item

    // Abstract method to get the screen for a selected menu item
    virtual Screen *_getSelectedMenuItemScreen(int index) = 0;

    // Get configuration value string for a menu item (optional override)
    virtual const char *_getMenuItemConfigValue(uint8_t index) { (void)index; return nullptr; }

    // Get configuration value FlashString for a menu item (optional override)
    virtual const __FlashStringHelper *_getMenuItemConfigValueF(uint8_t index) { (void)index; return nullptr; }

    // Check if a menu item is enabled/selectable (optional override)
    virtual bool _isMenuItemEnabled(uint8_t index) const { (void)index; return true; }

public:
    MenuScreen(); // Constructor

    virtual ~MenuScreen(); // Destructor - frees dynamically allocated menu items

    // Screen Interface Implementation

    virtual void loop() {} // Main loop processing for menu screen updates

    // Handle user input actions and navigate accordingly
    Screen *actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY) override;

    void setMenuItems(const char **menuItems, uint8_t menuItemCount); // Set the menu items to be displayed and navigated

    void setMenuItems(String *menuItems, uint8_t menuItemCount); // Set the menu items from an array of String objects

    void setMenuItemsF(const __FlashStringHelper **menuItems, uint8_t menuItemCount); // Set the menu items (Flash version)

    void clearMenuItems(); // Clear and free all dynamically allocated menu items
};

#endif /* MENU_SCREEN_H */