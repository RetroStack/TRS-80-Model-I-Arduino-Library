/**
 * SimpleMenuScreen Implementation
 *
 * Shows how to implement a basic MenuScreen. The MenuScreen base class
 * handles all the navigation and display - you just handle selections.
 */

#include "SimpleMenuScreen.h"
#include <M1Shield.h>
#include <Arduino.h>

const char *MAIN_MENU_ITEMS[11] = {
    "Run BASIC",
    "Load Program",
    "Save Program",
    "Font: A/B",
    "View: Left",
    "Settings",
    "Games",
    "Utilities",
    "About",
    "Help",
    "Exit"};

// Constructor: Set up the menu title, items, and initial progress
SimpleMenuScreen::SimpleMenuScreen() : MenuScreen()
{
    setTitle("TRS-80 Main Menu");

    setMenuItems(MAIN_MENU_ITEMS, 11);
}

// Handle menu item selection
// Return nullptr to stay on menu, or new Screen() to navigate to different screen
Screen *SimpleMenuScreen::_getSelectedMenuItemScreen(int index)
{
    // Handle exit request
    if (index == -1)
    {
        Serial.println("Exiting menu...");
        M1Shield.setLEDColor(LEDColor::COLOR_RED);
        return nullptr;
    }

    // Handle valid menu selections
    if (index >= 0 && index < 11)
    {
        Serial.print("Selected: ");
        Serial.println(MAIN_MENU_ITEMS[index]);

        // Each menu item gets different LED color for visual feedback
        switch (index)
        {
        case 0: // Run BASIC
            Serial.println("  Starting BASIC interpreter...");
            M1Shield.setLEDColor(LEDColor::COLOR_GREEN);
            // TODO: return new BasicInterpreterScreen();
            break;

        case 1: // Load Program
            Serial.println("  Opening file browser...");
            M1Shield.setLEDColor(LEDColor::COLOR_BLUE);
            // TODO: return new FileBrowserScreen();
            break;

        case 2: // Save Program
            Serial.println("  Opening save dialog...");
            M1Shield.setLEDColor(LEDColor::COLOR_CYAN);
            // TODO: return new SaveDialogScreen();
            break;

        case 3: // Font: A/B
            Serial.println("  Toggling font...");
            M1Shield.setLEDColor(LEDColor::COLOR_YELLOW);
            // TODO: return new FontSelectionScreen();
            break;

        case 4: // View: Left
            Serial.println("  Changing view layout...");
            M1Shield.setLEDColor(LEDColor::COLOR_MAGENTA);
            // TODO: return new ViewConfigScreen();
            break;

        case 5: // Settings
            Serial.println("  Opening settings...");
            M1Shield.setLEDColor(LEDColor::COLOR_WHITE);
            // TODO: return new SettingsMenuScreen();
            break;

        case 6: // Games
            Serial.println("  Opening games menu...");
            M1Shield.setLEDColor(LEDColor::COLOR_GREEN);
            // TODO: return new GamesMenuScreen();
            break;

        case 7: // Utilities
            Serial.println("  Opening utilities...");
            M1Shield.setLEDColor(LEDColor::COLOR_BLUE);
            // TODO: return new UtilitiesMenuScreen();
            break;

        case 8: // About
            Serial.println("  Showing about information...");
            M1Shield.setLEDColor(LEDColor::COLOR_CYAN);
            // TODO: return new AboutScreen();
            break;

        case 9: // Help
            Serial.println("  Opening help...");
            M1Shield.setLEDColor(LEDColor::COLOR_YELLOW);
            // TODO: return new HelpScreen();
            break;

        case 10: // Exit
            Serial.println("  Exiting application...");
            M1Shield.setLEDColor(LEDColor::COLOR_MAGENTA);
            break;

        default:
            Serial.println("  Unknown menu item");
            break;
        }
    }

    return nullptr; // Stay on current menu for this example
}
