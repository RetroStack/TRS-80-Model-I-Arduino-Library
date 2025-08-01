#include "CassetteRemoteConfig.h"
#include "CassetteMenu.h"
#include <Arduino.h>

CassetteRemoteConfig::CassetteRemoteConfig() : MenuScreen()
{
    _setTitle("Remote Config");

    // Create menu items dynamically - they'll be copied by _setMenuItems and these will be freed automatically
    const char *menuItems[] = {
        "Activate Remote",
        "Deactivate Remote"};

    _setMenuItems(menuItems, 2);

    Serial.println(F("Cassette Remote Config initialized"));
}

void CassetteRemoteConfig::activateRemote()
{
    Serial.println(F("Activating cassette remote control..."));
    _cassette.activateRemote();
    Serial.println(F("Cassette remote control activated"));
}

void CassetteRemoteConfig::deactivateRemote()
{
    Serial.println(F("Deactivating cassette remote control..."));
    _cassette.deactivateRemote();
    Serial.println(F("Cassette remote control deactivated"));
}

Screen *CassetteRemoteConfig::_getSelectedMenuItemScreen(int index)
{
    switch (index)
    {
    case 0: // Activate Remote
        activateRemote();
        return nullptr; // Stay on this screen

    case 1: // Deactivate Remote
        deactivateRemote();
        return nullptr; // Stay on this screen

    case -1: // Back to Cassette Menu
        Serial.println(F("Returning to Cassette Menu"));
        return new CassetteMenu();

    default:
        return nullptr;
    }
}
