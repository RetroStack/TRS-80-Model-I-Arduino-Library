#include "VideoFeaturesMenu.h"
#include "MainDemoMenu.h"
#include "VideoFullScreenDemo.h"
#include "VideoViewPortDemo.h"
#include <Arduino.h>

VideoFeaturesMenu::VideoFeaturesMenu() : MenuScreen()
{
    _setTitle("Video Features");

    addMenuItem("Full Screen Demo");
    addMenuItem("ViewPort Demo");
    addMenuItem("Back to Main");

    Serial.println("Video Features Menu initialized");
}

Screen *VideoFeaturesMenu::actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY)
{
    if (action & BUTTON_JOYSTICK)
    {
        int selectedIndex = getSelectedIndex();

        switch (selectedIndex)
        {
        case 0: // Full Screen Demo
            Serial.println("Opening Video Full Screen Demo");
            return new VideoFullScreenDemo();

        case 1: // ViewPort Demo
            Serial.println("Opening Video ViewPort Demo");
            return new VideoViewPortDemo();

        case 2: // Back to Main
            Serial.println("Returning to main menu from Video Features");
            return new MainDemoMenu();

        default:
            return nullptr;
        }
    }

    if (action & BUTTON_MENU)
    {
        Serial.println("Returning to main menu from Video Features");
        return new MainDemoMenu();
    }

    return MenuScreen::actionTaken(action, offsetX, offsetY);
}
