#include "ROMFeaturesMenu.h"
#include "MainDemoMenu.h"
#include "ROMDetectionDemo.h"
#include "ROMContentViewer.h"
#include <Arduino.h>

ROMFeaturesMenu::ROMFeaturesMenu() : MenuScreen()
{
    _setTitle("ROM Features");

    addMenuItem("ROM Detection");
    addMenuItem("ROM Content View");
    addMenuItem("Back to Main");

    Serial.println("ROM Features Menu initialized");
}

Screen *ROMFeaturesMenu::actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY)
{
    if (action & BUTTON_JOYSTICK)
    {
        int selectedIndex = getSelectedIndex();

        switch (selectedIndex)
        {
        case 0: // ROM Detection
            Serial.println("Opening ROM Detection Demo");
            return new ROMDetectionDemo();

        case 1: // ROM Content View
            Serial.println("Opening ROM Content Viewer");
            return new ROMContentViewer();

        case 2: // Back to Main
            Serial.println("Returning to main menu from ROM Features");
            return new MainDemoMenu();

        default:
            return nullptr;
        }
    }

    if (action & BUTTON_MENU)
    {
        Serial.println("Returning to main menu from ROM Features");
        return new MainDemoMenu();
    }

    return MenuScreen::actionTaken(action, offsetX, offsetY);
}
