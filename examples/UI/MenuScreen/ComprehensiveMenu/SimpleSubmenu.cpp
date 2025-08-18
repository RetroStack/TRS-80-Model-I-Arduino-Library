/**
 * ===================================================================
 * Simple Submenu Implementation
 * ===================================================================
 *
 * This demonstrates how MenuScreen can navigate to other screens,
 * showcasing the full navigation capabilities of the MenuScreen framework.
 */

#include "SimpleSubmenu.h"
#include <M1Shield.h>

// Menu item indices for the submenu
enum SubmenuItems
{
    SUBMENU_OPTION_A = 0,
    SUBMENU_OPTION_B = 1,
    SUBMENU_OPTION_C = 2,
    SUBMENU_BACK = 3
};

SimpleSubmenu::SimpleSubmenu()
{
    // Set up the submenu items
    const char *menuItems[] = {
        "Submenu Option A",
        "Submenu Option B",
        "Submenu Option C",
        "Back to Main Menu"};

    // Tell MenuScreen about our menu items
    setMenuItems(menuItems, 4);

    // Set the title for this submenu
    setTitle("Example Submenu");

    // Set up footer buttons
    const char *buttons[] = {"Back", "Select"};
    setButtonItems(buttons, 2);

    // Set initial progress
    setProgressValue(0);

    // Turn LED green to show we're in a submenu
    M1Shield.setLEDColor(LEDColor::COLOR_GREEN);

    Serial.println("SimpleSubmenu created - demonstrating screen navigation!");
}

Screen *SimpleSubmenu::_getSelectedMenuItemScreen(int index)
{
    // Flash LED white to show selection happened
    M1Shield.setLEDColor(LEDColor::COLOR_WHITE);
    delay(100);
    M1Shield.setLEDColor(LEDColor::COLOR_GREEN);

    Serial.print("Submenu item selected: ");
    Serial.print(index);
    Serial.print(" - ");

    switch (index)
    {
    case SUBMENU_OPTION_A:
        Serial.println("Submenu Option A");
        alert("You selected Option A in the submenu!");
        notify("Option A activated!", 2000);
        break;

    case SUBMENU_OPTION_B:
        Serial.println("Submenu Option B");
        if (confirm("Execute Option B?", "Cancel", "Execute") == CONFIRM_RIGHT)
        {
            alert("Option B executed successfully!");
            Serial.println("Option B confirmed and executed");
        }
        else
        {
            notify("Option B cancelled", 2000);
            Serial.println("Option B cancelled by user");
        }
        break;

    case SUBMENU_OPTION_C:
        Serial.println("Submenu Option C");
        alert("Option C is not implemented yet!");
        notify("Coming soon...", 2000);
        break;

    case SUBMENU_BACK:
        Serial.println("Back to Main Menu");
        // This demonstrates MenuScreen's navigation capabilities!
        // We can either:
        // 1. Return nullptr and let LEFT button handle back navigation automatically
        // 2. Return a specific screen to navigate to
        // 3. Use confirm dialog and handle the navigation explicitly
        //
        // For this demo, we'll show option 3 for educational purposes
        if (confirm("Return to main menu?", "Stay", "Back") == CONFIRM_RIGHT)
        {
            Serial.println("User confirmed return to main menu");
            alert("Use LEFT button for automatic back navigation!");
            notify("Or implement explicit navigation here", 3000);
            Serial.println("TIP: Press LEFT button to see automatic back navigation");
            // Note: Returning nullptr here means we stay on this screen
            // The user can then press LEFT button to see automatic back navigation
        }
        else
        {
            Serial.println("User cancelled return to main menu");
            notify("Staying in submenu", 2000);
        }
        break;

    default:
        Serial.println("Unknown submenu item!");
        alert("Unknown submenu selection!");
        break;
    }

    // Return nullptr to stay on this submenu screen
    // MenuScreen will automatically handle back navigation via LEFT button
    return nullptr;
}
