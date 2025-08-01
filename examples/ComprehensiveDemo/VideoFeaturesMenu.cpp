#include "VideoFeaturesMenu.h"
#include "MainDemoMenu.h"
#include "VideoFullScreenDemo.h"
#include "VideoViewPortDemo.h"
#include <Arduino.h>

VideoFeaturesMenu::VideoFeaturesMenu() : MenuScreen()
{
    _setTitle("Video Features");

    // Create menu items dynamically - they'll be copied by _setMenuItems and these will be freed automatically
    const char *menuItems[] = {
        "Full Screen Demo",
        "ViewPort Demo"};

    _setMenuItems(menuItems, 2);

    Serial.println(F("Video Features Menu initialized"));
}

Screen *VideoFeaturesMenu::_getSelectedMenuItemScreen(int index)
{
    // Handle exit request (back navigation)
    if (index == -1)
    {
        Serial.println(F("Returning to main menu from Video Features"));
        return new MainDemoMenu();
    }

    switch (index)
    {
    case 0: // Full Screen Demo
        Serial.println(F("Opening Video Full Screen Demo"));
        return new VideoFullScreenDemo();

    case 1: // ViewPort Demo
        Serial.println(F("Opening Video ViewPort Demo"));
        return new VideoViewPortDemo();

    default:
        return nullptr;
    }
}
