/**
 * ===================================================================
 * Comprehensive MenuScreen Implementation
 * ===================================================================
 *
 * This file shows you exactly how to implement ALL MenuScreen features.
 * Every line is explained so you can understand what's happening!
 *
 * STEP-BY-STEP LEARNING:
 * 1. Constructor: Set up menu items, title, and initial settings
 * 2. _getSelectedMenuItemScreen(): Handle what happens when items are selected
 * 3. _getMenuItemConfigValue(): Show current setting values on the right
 * 4. _isMenuItemEnabled(): Control which items are enabled/disabled
 * 5. Helper methods: Implement the actual setting changes
 * 6. User feedback: Demonstrate alerts, confirmations, and notifications
 * 7. Screen navigation: Show how to navigate to submenus and back
 */

#include "ComprehensiveMenuScreen.h"
#include "SimpleSubmenu.h"
#include <M1Shield.h>

// Menu item indices for easy reference
enum MenuItems
{
    MENU_SOUND = 0,
    MENU_DIFFICULTY = 1,
    MENU_TIMEOUT = 2,
    MENU_BRIGHTNESS = 3,
    MENU_AUTOSAVE = 4,
    MENU_LANGUAGE = 5,
    MENU_SUBMENU = 6,
    MENU_RESET = 7,
    MENU_FEEDBACK_DEMO = 8,
    MENU_EXIT = 9
};

// STEP 1: CONSTRUCTOR - Setting up the menu
// This runs ONCE when you create the menu screen
ComprehensiveMenuScreen::ComprehensiveMenuScreen()
{
    // Initialize configuration settings with reasonable defaults
    _soundEnabled = true; // Sound effects enabled
    _difficulty = 1;      // Normal difficulty (0=Easy, 1=Normal, 2=Hard)
    _timeout = 15;        // 15 second timeout
    _brightness = 80;     // 80% brightness
    _autoSave = false;    // Auto save disabled
    _language = 0;        // English (0=English, 1=Spanish, 2=French)

    // Set up the menu items array
    const char *menuItems[] = {
        "Sound Effects",      // MENU_SOUND
        "Game Difficulty",    // MENU_DIFFICULTY
        "Screen Timeout",     // MENU_TIMEOUT
        "Brightness",         // MENU_BRIGHTNESS
        "Auto Save",          // MENU_AUTOSAVE
        "Language",           // MENU_LANGUAGE
        "Go to Submenu",      // MENU_SUBMENU - NEW: Navigation demo
        "Reset to Defaults",  // MENU_RESET
        "User Feedback Demo", // MENU_FEEDBACK_DEMO
        "Exit to Main Menu"   // MENU_EXIT
    };

    // Tell MenuScreen about our menu items
    setMenuItems(menuItems, 10);

    // Set the title that appears in the header
    setTitle("TRS-80 Settings Menu");

    // Set up footer buttons (MenuScreen will show these automatically)
    const char *buttons[] = {"Back", "Change"};
    setButtonItems(buttons, 2);

    // Set initial progress (will update automatically as user navigates)
    setProgressValue(0);

    // Turn LED blue to show menu is ready
    M1Shield.setLEDColor(LEDColor::COLOR_BLUE);

    Serial.println("ComprehensiveMenuScreen created with all features!");
}

// STEP 2: HANDLE MENU ITEM SELECTION
// This runs when user selects a menu item (presses Menu/Right button)
Screen *ComprehensiveMenuScreen::_getSelectedMenuItemScreen(int index)
{
    // Flash LED white to show selection happened
    M1Shield.setLEDColor(LEDColor::COLOR_WHITE);
    delay(100);
    M1Shield.setLEDColor(LEDColor::COLOR_BLUE);

    Serial.print("Menu item selected: ");
    Serial.print(index);
    Serial.print(" - ");

    switch (index)
    {
    case MENU_SOUND:
        Serial.println("Sound Effects");
        _toggleSound();
        notify("Sound effects toggled!", 2000);
        break;

    case MENU_DIFFICULTY:
        if (_isMenuItemEnabled(MENU_DIFFICULTY))
        {
            Serial.println("Game Difficulty");
            _cycleDifficulty();
            notify("Difficulty changed!", 2000);
        }
        else
        {
            Serial.println("Game Difficulty (DISABLED - Sound is OFF)");
            alert("Enable Sound Effects first to change difficulty!");
        }
        break;

    case MENU_TIMEOUT:
        Serial.println("Screen Timeout");
        _adjustTimeout(5); // Increase by 5 seconds
        notify("Timeout adjusted!", 2000);
        break;

    case MENU_BRIGHTNESS:
        if (_isMenuItemEnabled(MENU_BRIGHTNESS))
        {
            Serial.println("Brightness");
            _adjustBrightness(10); // Increase by 10%
            notify("Brightness adjusted!", 2000);
        }
        else
        {
            Serial.println("Brightness (DISABLED - Timeout too low)");
            alert("Set timeout to 10s or higher to adjust brightness!");
        }
        break;

    case MENU_AUTOSAVE:
        Serial.println("Auto Save");
        _toggleAutoSave();
        notify("Auto save toggled!", 2000);
        break;

    case MENU_LANGUAGE:
        if (_isMenuItemEnabled(MENU_LANGUAGE))
        {
            Serial.println("Language");
            _cycleLanguage();
            notify("Language changed!", 2000);
        }
        else
        {
            Serial.println("Language (DISABLED - Auto Save is ON)");
            alert("Turn OFF Auto Save to change language!");
        }
        break;

    case MENU_SUBMENU:
        Serial.println("Go to Submenu - NAVIGATION DEMO!");
        // This is where MenuScreen's navigation power really shows!
        // We can return a new screen to navigate to
        alert("Navigating to submenu...");
        Serial.println("Creating new SimpleSubmenu and navigating to it");
        return new SimpleSubmenu(); // Navigate to the submenu!

    case MENU_RESET:
        Serial.println("Reset to Defaults");
        if (confirm("Reset all settings to defaults?", "Cancel", "Reset") == CONFIRM_RIGHT)
        {
            _resetToDefaults();
            alert("All settings have been reset to defaults!");
            Serial.println("Settings reset to defaults");
        }
        else
        {
            Serial.println("Reset cancelled by user");
        }
        break;

    case MENU_FEEDBACK_DEMO:
        Serial.println("User Feedback Demo");
        _showUserFeedbackDemo();
        break;

    case MENU_EXIT:
        Serial.println("Exit to Main Menu");
        if (confirm("Exit settings menu?", "Stay", "Exit") == CONFIRM_RIGHT)
        {
            alert("Thanks for using the settings menu!");
            Serial.println("User confirmed exit");
            // In a real app, you would return a new screen here
            // For this example, we'll just show a message and stay
            notify("Exit confirmed! (Would navigate in real app)", 3000);
        }
        else
        {
            Serial.println("Exit cancelled by user");
        }
        break;

    default:
        Serial.println("Unknown menu item!");
        alert("Unknown menu selection!");
        break;
    }

    // Return nullptr to stay on this menu screen
    // In a real application, you might return new screens for some items
    return nullptr;
}

// STEP 3: PROVIDE CONFIGURATION VALUES
// This runs whenever the menu is drawn to show current setting values
const char *ComprehensiveMenuScreen::_getMenuItemConfigValue(uint8_t index)
{
    switch (index)
    {
    case MENU_SOUND:
        return _soundEnabled ? "On" : "Off";

    case MENU_DIFFICULTY:
        return _difficultyNames[_difficulty];

    case MENU_TIMEOUT:
        snprintf(_numericBuffer, sizeof(_numericBuffer), "%ds", _timeout);
        return _numericBuffer;

    case MENU_BRIGHTNESS:
        snprintf(_numericBuffer, sizeof(_numericBuffer), "%d%%", _brightness);
        return _numericBuffer;

    case MENU_AUTOSAVE:
        return _autoSave ? "On" : "Off";

    case MENU_LANGUAGE:
        return _languageNames[_language];

    case MENU_SUBMENU:
        return nullptr; // No configuration value for navigation item

    case MENU_RESET:
    case MENU_FEEDBACK_DEMO:
    case MENU_EXIT:
        return nullptr; // No configuration value for these items

    default:
        return nullptr;
    }
}

// STEP 4: CONTROL ENABLED/DISABLED STATE
// This runs whenever the menu is drawn to determine which items are enabled
bool ComprehensiveMenuScreen::_isMenuItemEnabled(uint8_t index) const
{
    switch (index)
    {
    case MENU_SOUND:
    case MENU_TIMEOUT:
    case MENU_AUTOSAVE:
    case MENU_SUBMENU:
    case MENU_RESET:
    case MENU_FEEDBACK_DEMO:
    case MENU_EXIT:
        return true; // These are always enabled

    case MENU_DIFFICULTY:
        // Game difficulty only available when sound is enabled
        return _soundEnabled;

    case MENU_BRIGHTNESS:
        // Brightness only adjustable when timeout is 10s or higher
        return _timeout >= 10;

    case MENU_LANGUAGE:
        // Language can't be changed when auto save is enabled
        return !_autoSave;

    default:
        return true;
    }
}

// STEP 5: OPTIONAL LOOP PROCESSING
// This runs continuously while the menu is active
void ComprehensiveMenuScreen::loop()
{
    // In this example, we don't need continuous processing
    // But you could add things like:
    // - Periodic updates to configuration values
    // - Background animations
    // - Network status checks
    // - Sensor readings
    // etc.
}

// STEP 6: HELPER METHODS FOR SETTING CHANGES
// These implement the actual configuration changes

void ComprehensiveMenuScreen::_toggleSound()
{
    _soundEnabled = !_soundEnabled;
    Serial.print("Sound effects: ");
    Serial.println(_soundEnabled ? "ON" : "OFF");

    // If sound was turned off and difficulty was Easy, change to Normal
    // because Easy difficulty will become disabled
    if (!_soundEnabled && _difficulty == 0)
    {
        _difficulty = 1; // Change from Easy to Normal
        Serial.println("Auto-adjusted difficulty from Easy to Normal (Easy becomes disabled when sound is off)");
    }
}

void ComprehensiveMenuScreen::_cycleDifficulty()
{
    _difficulty = (_difficulty + 1) % 3; // Cycle: 0->1->2->0
    Serial.print("Difficulty: ");
    Serial.println(_difficultyNames[_difficulty]);

    // If difficulty was set to Easy, disable auto save
    if (_difficulty == 0 && _autoSave)
    {
        _autoSave = false;
        Serial.println("Auto-disabled Auto Save (Auto Save not allowed on Easy difficulty)");
    }
}

void ComprehensiveMenuScreen::_adjustTimeout(int delta)
{
    _timeout += delta;

    // Keep timeout in reasonable range (5-60 seconds)
    if (_timeout < 5)
        _timeout = 5;
    if (_timeout > 60)
        _timeout = 60;

    Serial.print("Timeout: ");
    Serial.print(_timeout);
    Serial.println("s");

    // If timeout is too low, adjust brightness to a safe level
    if (_timeout < 10 && _brightness > 50)
    {
        _brightness = 50;
        Serial.println("Auto-adjusted brightness to 50% (brightness limited when timeout < 10s)");
    }
}

void ComprehensiveMenuScreen::_adjustBrightness(int delta)
{
    _brightness += delta;

    // Keep brightness in reasonable range (10-100%)
    if (_brightness < 10)
        _brightness = 10;
    if (_brightness > 100)
        _brightness = 100;

    Serial.print("Brightness: ");
    Serial.print(_brightness);
    Serial.println("%");
}

void ComprehensiveMenuScreen::_toggleAutoSave()
{
    _autoSave = !_autoSave;
    Serial.print("Auto Save: ");
    Serial.println(_autoSave ? "ON" : "OFF");

    // If auto save was turned on, reset language to English
    if (_autoSave && _language != 0)
    {
        _language = 0; // Reset to English
        Serial.println("Auto-reset language to English (language locked when Auto Save is on)");
    }
}

void ComprehensiveMenuScreen::_cycleLanguage()
{
    _language = (_language + 1) % 3; // Cycle: 0->1->2->0
    Serial.print("Language: ");
    Serial.println(_languageNames[_language]);
}

void ComprehensiveMenuScreen::_resetToDefaults()
{
    // Reset all settings to their default values
    _soundEnabled = false; // Default to OFF to show disabled items
    _difficulty = 1;       // Normal
    _timeout = 5;          // 5s (low enough to disable brightness)
    _brightness = 50;      // 50%
    _autoSave = true;      // ON (to disable language)
    _language = 0;         // English

    Serial.println("All settings reset to defaults (with some items disabled for demo)");
}

void ComprehensiveMenuScreen::_showUserFeedbackDemo()
{
    Serial.println("=== USER FEEDBACK DEMO STARTING ===");

    // Demo 1: Simple alert
    alert("This is a simple alert dialog!");
    Serial.println("Alert demo completed");

    // Demo 2: Confirmation with choice handling
    ConfirmResult choice = confirm("Do you like this menu system?", "No", "Yes");
    if (choice == CONFIRM_LEFT)
    {
        alert("No problem! We'll keep improving it.");
        Serial.println("User said No to liking the menu");
    }
    else
    {
        alert("Great! Thanks for the positive feedback!");
        Serial.println("User said Yes to liking the menu");
    }

    // Demo 3: Notification (non-blocking)
    notify("This notification will auto-dismiss in 4 seconds!", 4000);
    Serial.println("Notification demo started (will auto-dismiss)");

    // Demo 4: Chained confirmations for complex workflows
    if (confirm("Want to see a more complex example?", "Skip", "Show") == CONFIRM_RIGHT)
    {
        alert("Complex workflows can chain multiple dialogs...");

        if (confirm("Should we simulate a file operation?", "Cancel", "Continue") == CONFIRM_RIGHT)
        {
            alert("Simulating file save...");
            notify("File saved successfully!", 3000);
            Serial.println("Complex demo: simulated file operation");
        }
        else
        {
            notify("File operation cancelled", 2000);
            Serial.println("Complex demo: file operation cancelled");
        }
    }
    else
    {
        Serial.println("User skipped complex demo");
    }

    Serial.println("=== USER FEEDBACK DEMO COMPLETED ===");
}
