#include "CassetteMenu.h"
#include "CassetteSongPlayer.h"
#include "MainDemoMenu.h"
#include <Model1.h>
#include <Arduino.h>
#include "logger.h"

CassetteMenu::CassetteMenu() : MenuScreen()
{
    _setTitle("Cassette Interface");

    // Create menu items dynamically - they'll be copied by _setMenuItems and these will be freed automatically
    const char *menuItems[] = {
        "Song Player",
        "Remote Control",
        "Character Mode",
        "Character Gen"};

    _setMenuItems(menuItems, 4);

    _remoteActive = false; // Initialize remote as inactive
    _charGenA = true;      // Initialize to Character Generator A (default)
    _is64CharMode = true;  // Initialize to 64-character mode (default assumption)

    // Set up logger for debugging
    _cassette.setLogger(logger);

    Serial.println(F("Cassette Menu initialized"));
}

bool CassetteMenu::open()
{
    // Activate test signal to read initial hardware state
    Model1.activateTestSignal();
    Serial.println(F("Test signal activated for initial state reading"));

    // Read and store the current character mode state
    _is64CharMode = _cassette.is64CharacterMode();
    Serial.print(F("Initial character mode detected: "));
    Serial.println(_is64CharMode ? "64 characters" : "32 characters");

    // Deactivate test signal after reading
    Model1.deactivateTestSignal();
    Serial.println(F("Test signal deactivated after initial state reading"));

    // Call parent implementation
    return MenuScreen::open();
}

void CassetteMenu::close()
{
    // Deactivate remote if it's currently active when leaving the menu
    if (_remoteActive)
    {
        Serial.println(F("Deactivating remote control before leaving cassette menu..."));

        // Briefly activate test signal for the deactivation operation
        Model1.activateTestSignal();
        _cassette.deactivateRemote();
        _remoteActive = false;
        Model1.deactivateTestSignal();

        Serial.println(F("Remote control deactivated"));
    }

    Serial.println(F("Cassette Interface closed"));

    Screen::close();
}

Screen *CassetteMenu::_getSelectedMenuItemScreen(int index)
{
    switch (index)
    {
    case 0: // Song Player
        Serial.println(F("Opening Song Player"));
        return new CassetteSongPlayer();

    case 1: // Remote Control (toggle)
        toggleRemote();
        return nullptr; // Stay on this screen

    case 2: // Character Mode (toggle 64/32)
        toggleCharacterMode();
        return nullptr; // Stay on this screen

    case 3: // Character Gen (toggle A/B)
        toggleCharacterGen();
        return nullptr; // Stay on this screen

    case -1: // Back to Main
        Serial.println(F("Returning to main menu from Cassette Menu"));
        return new MainDemoMenu();

    default:
        return nullptr;
    }
}

const char *CassetteMenu::_getMenuItemConfigValue(uint8_t index)
{
    switch (index)
    {
    case 1: // Remote Control
        return _remoteActive ? "Active" : "Inactive";
    case 2: // Character Mode
        return _is64CharMode ? "64 chars" : "32 chars";
    case 3: // Character Gen
        return _charGenA ? "Gen A" : "Gen B";
    default:
        return nullptr;
    }
}

void CassetteMenu::toggleRemote()
{
    // Briefly activate test signal for cassette operations
    Model1.activateTestSignal();
    Serial.println(F("Test signal activated for remote toggle"));

    if (_remoteActive)
    {
        Serial.println(F("Deactivating cassette remote control..."));
        _cassette.deactivateRemote();
        _remoteActive = false;
        Serial.println(F("Cassette remote control deactivated"));
    }
    else
    {
        Serial.println(F("Activating cassette remote control..."));
        _cassette.activateRemote();
        _remoteActive = true;
        Serial.println(F("Cassette remote control activated"));
    }

    // Deactivate test signal after operation
    Model1.deactivateTestSignal();
    Serial.println(F("Test signal deactivated after remote toggle"));

    // Redraw the menu to show updated config value
    _drawContent();
}

void CassetteMenu::toggleCharacterMode()
{
    // Briefly activate test signal for cassette operations
    Model1.activateTestSignal();
    Serial.println(F("Test signal activated for character mode toggle"));

    if (_is64CharMode)
    {
        Serial.println(F("Switching to 32-character mode..."));
        _cassette.set32CharacterMode();
        _is64CharMode = false;
        Serial.println(F("Switched to 32-character mode"));
    }
    else
    {
        Serial.println(F("Switching to 64-character mode..."));
        _cassette.set64CharacterMode();
        _is64CharMode = true;
        Serial.println(F("Switched to 64-character mode"));
    }

    // Deactivate test signal after operation
    Model1.deactivateTestSignal();
    Serial.println(F("Test signal deactivated after character mode toggle"));

    // Redraw the menu to show updated config value
    _drawContent();
}

void CassetteMenu::toggleCharacterGen()
{
    // Briefly activate test signal for cassette operations
    Model1.activateTestSignal();
    Serial.println(F("Test signal activated for character generator toggle"));

    if (_charGenA)
    {
        Serial.println(F("Switching to Character Generator B..."));
        _cassette.setCharGenB();
        _charGenA = false;
        Serial.println(F("Character Generator B selected"));
    }
    else
    {
        Serial.println(F("Switching to Character Generator A..."));
        _cassette.setCharGenA();
        _charGenA = true;
        Serial.println(F("Character Generator A selected"));
    }

    // Deactivate test signal after operation
    Model1.deactivateTestSignal();
    Serial.println(F("Test signal deactivated after character generator toggle"));

    // Redraw the menu to show updated config value
    _drawContent();
}
