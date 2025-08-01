#include "ROMFeaturesMenu.h"
#include "MainDemoMenu.h"
#include "ROMDetectionDemo.h"
#include "ROMContentViewer.h"
#include <Arduino.h>

ROMFeaturesMenu::ROMFeaturesMenu() : MenuScreen()
{
    _setTitle("ROM Features");

    // Create menu items dynamically - they'll be copied by _setMenuItems and these will be freed automatically
    const char *menuItems[] = {
        "ROM Detection",
        "ROM Content View"};

    _setMenuItems(menuItems, 2);

    Serial.println(F("ROM Features Menu initialized"));
}

Screen *ROMFeaturesMenu::_getSelectedMenuItemScreen(int index)
{
    // Handle exit request (back navigation)
    if (index == -1)
    {
        Serial.println(F("Returning to main menu from ROM Features"));
        return new MainDemoMenu();
    }

    switch (index)
    {
    case 0: // ROM Detection
        Serial.println(F("Opening ROM Detection Demo"));
        return new ROMDetectionDemo();

    case 1: // ROM Content View
        Serial.println(F("Opening ROM Content Viewer"));
        return new ROMContentViewer();

    default:
        return nullptr;
    }
}
