/**
 * ConfigMenuExample Implementation
 *
 * Demonstrates the configuration values feature in MenuScreen.
 * Each menu item shows its current setting value on the right side.
 */

#include "ConfigMenuExample.h"
#include "M1Shield.h"
#include <Arduino.h>

// Constructor: Initialize settings and set up the menu
ConfigMenuExample::ConfigMenuExample() : MenuScreen()
{
    // Initialize default settings to demonstrate disabled items
    _soundEnabled = true;
    _difficulty = 1;  // Normal difficulty
    _timeout = 30;    // 30 seconds
    _brightness = 80; // 80% brightness
    _autoSave = false;
    _language = 0; // English

    // Set up the menu
    _setTitle("Configuration Menu");
    _setMenuItems(CONFIG_MENU_ITEMS, CONFIG_MENU_COUNT);

    // Set custom footer buttons
    const char *buttonItems[2] = {
        "[M/<] Back", "[>] Change"};
    _setButtonItems(buttonItems, 2);
}

// Return configuration values for display
const char *ConfigMenuExample::_getMenuItemConfigValue(int index)
{
    switch (index)
    {
    case 0: // Sound Effects
        return _soundEnabled ? "On" : "Off";

    case 1: // Game Difficulty
        switch (_difficulty)
        {
        case 0:
            return "Easy";
        case 1:
            return "Normal";
        case 2:
            return "Hard";
        default:
            return "Unknown";
        }

    case 2: // Screen Timeout
        snprintf(_numericBuffer, sizeof(_numericBuffer), "%ds", _timeout);
        return _numericBuffer;

    case 3: // Brightness
        snprintf(_numericBuffer, sizeof(_numericBuffer), "%d%%", _brightness);
        return _numericBuffer;

    case 4: // Auto Save
        return _autoSave ? "On" : "Off";

    case 5: // Language
        switch (_language)
        {
        case 0:
            return "English";
        case 1:
            return "Spanish";
        case 2:
            return "French";
        default:
            return "Unknown";
        }

    case 6: // Reset to Defaults
    case 7: // Back to Main Menu
    default:
        return nullptr; // No configuration value for these items
    }
}

// Determine which menu items should be enabled/disabled
bool ConfigMenuExample::_isMenuItemEnabled(int index)
{
    switch (index)
    {
    case 0: // Sound Effects - always enabled
        return true;

    case 1: // Game Difficulty - disabled when sound is off (example: sound-based difficulty features)
        return _soundEnabled;

    case 2: // Screen Timeout - always enabled
        return true;

    case 3: // Brightness - disabled when timeout is very short (example: power saving mode)
        return _timeout >= 10;

    case 4: // Auto Save - disabled when difficulty is Easy (example: auto-save not needed for easy mode)
        return _difficulty > 0;

    case 5: // Language - disabled when auto-save is on (example: language changes affect save format)
        return !_autoSave;

    case 6: // Reset to Defaults - always enabled
        return true;

    case 7: // Back to Main Menu - always enabled
        return true;

    default:
        return true;
    }
}

// Handle menu item selection
Screen *ConfigMenuExample::_getSelectedMenuItemScreen(int index)
{
    // Handle exit request
    if (index == -1)
    {
        Serial.println("Exiting configuration menu...");
        M1Shield.setLEDColor(LEDColor::COLOR_BLUE);
        return nullptr; // Return to previous screen
    }

    // Handle valid menu selections
    if (index >= 0 && index < CONFIG_MENU_COUNT)
    {
        // Check if the selected item is enabled
        if (!_isMenuItemEnabled(index))
        {
            Serial.print("Menu item disabled: ");
            Serial.println(CONFIG_MENU_ITEMS[index]);

            // Provide feedback about why it's disabled
            switch (index)
            {
            case 1: // Game Difficulty
                Serial.println("  -> Enable sound effects first");
                break;
            case 3: // Brightness
                Serial.println("  -> Increase timeout above 10s first");
                break;
            case 4: // Auto Save
                Serial.println("  -> Change difficulty from Easy first");
                break;
            case 5: // Language
                Serial.println("  -> Turn off auto-save first");
                break;
            }

            M1Shield.setLEDColor(LEDColor::COLOR_RED);
            return nullptr; // Stay on menu, no action taken
        }

        Serial.print("Configuring: ");
        Serial.println(CONFIG_MENU_ITEMS[index]);

        switch (index)
        {
        case 0: // Sound Effects
            toggleSound();
            M1Shield.setLEDColor(_soundEnabled ? LEDColor::COLOR_GREEN : LEDColor::COLOR_RED);
            Serial.println("  -> This may enable/disable other options!");
            break;

        case 1: // Game Difficulty
            cycleDifficulty();
            M1Shield.setLEDColor(LEDColor::COLOR_YELLOW);
            Serial.println("  -> This may enable/disable auto-save!");
            break;

        case 2:                // Screen Timeout
            adjustTimeout(10); // Increase by 10 seconds
            M1Shield.setLEDColor(LEDColor::COLOR_CYAN);
            Serial.println("  -> This may enable/disable brightness!");
            break;

        case 3:                   // Brightness
            adjustBrightness(10); // Increase by 10%
            M1Shield.setLEDColor(LEDColor::COLOR_WHITE);
            break;

        case 4: // Auto Save
            toggleAutoSave();
            M1Shield.setLEDColor(_autoSave ? LEDColor::COLOR_GREEN : LEDColor::COLOR_RED);
            Serial.println("  -> This may enable/disable language!");
            break;

        case 5: // Language
            cycleLanguage();
            M1Shield.setLEDColor(LEDColor::COLOR_MAGENTA);
            break;

        case 6: // Reset to Defaults
            resetToDefaults();
            M1Shield.setLEDColor(LEDColor::COLOR_RED);
            Serial.println("  Settings reset to defaults");
            Serial.println("  -> Check which items are now enabled/disabled!");
            break;

        case 7: // Back to Main Menu
            Serial.println("  Returning to main menu...");
            M1Shield.setLEDColor(LEDColor::COLOR_BLUE);
            return nullptr; // Return to previous screen
        }

        // Force redraw to show updated configuration values and enabled/disabled states
        if (isActive())
        {
            refresh();
        }
    }

    return nullptr; // Stay on this menu
}

// Helper methods for toggling settings

void ConfigMenuExample::toggleSound()
{
    _soundEnabled = !_soundEnabled;
    Serial.print("  Sound: ");
    Serial.println(_soundEnabled ? "Enabled" : "Disabled");
}

void ConfigMenuExample::cycleDifficulty()
{
    _difficulty = (_difficulty + 1) % 3;
    Serial.print("  Difficulty: ");
    Serial.println(_difficulty == 0 ? "Easy" : _difficulty == 1 ? "Normal"
                                                                : "Hard");
}

void ConfigMenuExample::adjustTimeout(int delta)
{
    int newTimeout = _timeout + delta;
    if (newTimeout < 5)
        newTimeout = 5; // Minimum 5 seconds
    if (newTimeout > 300)
        newTimeout = 300; // Maximum 5 minutes
    _timeout = newTimeout;
    Serial.print("  Timeout: ");
    Serial.print(_timeout);
    Serial.println(" seconds");
}

void ConfigMenuExample::adjustBrightness(int delta)
{
    int newBrightness = _brightness + delta;
    if (newBrightness < 10)
        newBrightness = 10; // Minimum 10%
    if (newBrightness > 100)
        newBrightness = 100; // Maximum 100%
    _brightness = newBrightness;
    Serial.print("  Brightness: ");
    Serial.print(_brightness);
    Serial.println("%");
}

void ConfigMenuExample::toggleAutoSave()
{
    _autoSave = !_autoSave;
    Serial.print("  Auto Save: ");
    Serial.println(_autoSave ? "Enabled" : "Disabled");
}

void ConfigMenuExample::cycleLanguage()
{
    _language = (_language + 1) % 3;
    Serial.print("  Language: ");
    Serial.println(_language == 0 ? "English" : _language == 1 ? "Spanish"
                                                               : "French");
}

void ConfigMenuExample::resetToDefaults()
{
    Serial.println("  Resetting to defaults that demonstrate disabled items:");

    // Set specific values to show disabled item behavior
    _soundEnabled = false; // This will disable "Game Difficulty"
    _difficulty = 0;       // Easy mode - this will disable "Auto Save"
    _timeout = 5;          // Short timeout - this will disable "Brightness"
    _brightness = 80;
    _autoSave = true; // This will disable "Language"
    _language = 0;

    Serial.println("    Sound: OFF (disables Game Difficulty)");
    Serial.println("    Difficulty: Easy (disables Auto Save)");
    Serial.println("    Timeout: 5s (disables Brightness)");
    Serial.println("    Auto Save: ON (disables Language)");
    Serial.println("  Notice how some menu items are now grayed out!");
}
