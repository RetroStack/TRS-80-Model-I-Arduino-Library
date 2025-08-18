/*
 * SimpleMenu.cpp
 *
 * Implementation of a simple MenuScreen demonstration.
 */

#include "SimpleMenu.h"

// Color names for debugging output
const char *colorNames[] = {
    "Red", "Green", "Blue", "Yellow", "Magenta", "Cyan", "White", "Off"};

SimpleMenu::SimpleMenu()
    : currentLedColor(0) // Start with red LED
{
    Serial.println("=== SimpleMenu::constructor() ===");
    Serial.println("Setting up simple menu...");

    // Set up the menu items array
    const char *menuItems[] = {
        "Red LED",
        "Green LED",
        "Blue LED",
        "Cycle Colors",
        "LED Off"};

    // Tell MenuScreen about our menu items
    setMenuItems(menuItems, 5);

    // Set menu title
    setTitle("Simple Menu");

    // Set up footer buttons
    const char *buttons[] = {"Back", "Select"};
    setButtonItems(buttons, 2);

    // Set initial LED color
    setLedColor(currentLedColor);
}

Screen *SimpleMenu::_getSelectedMenuItemScreen(int index)
{
    Serial.println("=== Menu Item Selected ===");
    Serial.print("Selected item: ");
    Serial.print(index);
    Serial.print(" - ");

    // Handle each menu item
    switch (index)
    {
    case 0: // Red LED
        currentLedColor = 0;
        setLedColor(currentLedColor);
        showSelectionFeedback("Red LED");
        break;

    case 1: // Green LED
        currentLedColor = 1;
        setLedColor(currentLedColor);
        showSelectionFeedback("Green LED");
        break;

    case 2: // Blue LED
        currentLedColor = 2;
        setLedColor(currentLedColor);
        showSelectionFeedback("Blue LED");
        break;

    case 3: // Cycle Colors
        currentLedColor = (currentLedColor + 1) % 8;
        setLedColor(currentLedColor);
        showSelectionFeedback("Color Cycled");
        break;

    case 4:                  // LED Off
        currentLedColor = 7; // Off
        setLedColor(currentLedColor);
        showSelectionFeedback("LED Off");
        break;

    default:
        Serial.println("Unknown item");
        break;
    }

    return nullptr; // Stay on this menu
}

void SimpleMenu::setLedColor(int colorIndex)
{
    // Set the LED color based on index
    switch (colorIndex)
    {
    case 0:
        M1Shield.setLEDColor(255, 0, 0);
        break; // Red
    case 1:
        M1Shield.setLEDColor(0, 255, 0);
        break; // Green
    case 2:
        M1Shield.setLEDColor(0, 0, 255);
        break; // Blue
    case 3:
        M1Shield.setLEDColor(255, 255, 0);
        break; // Yellow
    case 4:
        M1Shield.setLEDColor(255, 0, 255);
        break; // Magenta
    case 5:
        M1Shield.setLEDColor(0, 255, 255);
        break; // Cyan
    case 6:
        M1Shield.setLEDColor(255, 255, 255);
        break; // White
    case 7:
        M1Shield.setLEDColor(0, 0, 0);
        break; // Off
    default:
        M1Shield.setLEDColor(0, 0, 0);
        break; // Off
    }

    Serial.print("LED color changed to: ");
    Serial.println(colorNames[colorIndex]);
}

void SimpleMenu::showSelectionFeedback(const char *itemName)
{
    Serial.print("Action: ");
    Serial.println(itemName);

    // Show a quick notification to the user
    // This uses ContentScreen's notification system
    String feedback = "Selected: ";
    feedback += itemName;
    notify(feedback.c_str(), 1500); // Show for 1.5 seconds
}
