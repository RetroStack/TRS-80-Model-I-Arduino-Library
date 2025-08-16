/**
 * ===================================================================
 * Simple Content Screen Implementation
 * ===================================================================
 *
 * This file shows you exactly how to implement a ContentScreen.
 * Every line is explained so you can understand what's happening!
 *
 * STEP-BY-STEP LEARNING:
 * 1. Constructor: Set up the screen with title and menu items
 * 2. initialize(): Prepare the screen when it first opens
 * 3. _drawContent(): Draw your custom content in the middle area
 * 4. loop(): Update the screen continuously
 * 5. actionTaken(): Handle button presses
 * 6. cleanup(): Clean up when the screen closes
 */

#include "SimpleContentScreen.h"
#include <M1Shield.h>

// STEP 1: CONSTRUCTOR - Setting up the screen
// This runs ONCE when you create the screen with "new SimpleContentScreen(...)"
SimpleContentScreen::SimpleContentScreen(const char *title, const char **menuItems, int itemCount)
{
    // Save the menu information so we can use it later
    _menuItems = menuItems;  // Pointer to the menu text array
    _totalItems = itemCount; // How many menu items we have
    _selectedItem = 0;       // Start with first item selected

    // Tell ContentScreen what title to show in the header
    // The ContentScreen will automatically draw this for us!
    setTitle(title);

    // Set up footer buttons (ContentScreen draws these automatically too!)
    const char *buttons[] = {"Back", "Select"}; // Simple button labels
    setButtonItems(buttons, 2);                 // 2 buttons in the footer

    // Set progress bar to 0% initially
    setProgressValue(0);

    // Turn LED blue to show screen is ready
    M1Shield.setLEDColor(LEDColor::COLOR_BLUE);
}

// STEP 3: DRAW CONTENT - Draw your custom content in the middle area
// This runs whenever the content area needs to be redrawn
void SimpleContentScreen::_drawContent()
{
    // Get access to the display so we can draw on it
    Adafruit_GFX &display = M1Shield.getGFX();

    // Find out where the content area is located
    // ContentScreen automatically calculates this for us!
    int contentLeft = _getContentLeft();     // X position of content area
    int contentTop = _getContentTop();       // Y position of content area
    int contentWidth = _getContentWidth();   // Width of content area
    int contentHeight = _getContentHeight(); // Height of content area

    // Clear the content area (make it black)
    display.fillRect(contentLeft, contentTop, contentWidth, contentHeight, M1Shield.convertColor(0x0000));

    // If we have no menu items, show a message
    if (!_menuItems || _totalItems == 0)
    {
        display.setTextColor(M1Shield.convertColor(0xFFFF));
        display.setTextSize(1);
        display.setCursor(contentLeft + 10, contentTop + 20);
        display.print("No menu items!");
        return;
    }

    // Draw each menu item
    display.setTextSize(2); // Make text bigger and easier to read

    for (int i = 0; i < _totalItems; i++)
    {
        // Calculate where to draw this menu item
        int itemY = contentTop + 20 + (i * 25); // 25 pixels between each item
        int itemX = contentLeft + 10;           // 10 pixels from left edge

        // Is this the currently selected item?
        if (i == _selectedItem)
        {
            // YES - Draw a white background to highlight it
            display.fillRect(itemX - 5, itemY - 3, contentWidth - 10, 22, M1Shield.convertColor(0xFFFF));
            display.setTextColor(M1Shield.convertColor(0x0000), M1Shield.convertColor(0xFFFF)); // Black text on white background
        }
        else
        {
            // NO - Draw normal white text on black background
            display.setTextColor(M1Shield.convertColor(0xFFFF), M1Shield.convertColor(0x0000));
        }

        // Draw the menu item text
        display.setCursor(itemX, itemY);
        display.print(_menuItems[i]);
    }
}

// STEP 4: LOOP - Update the screen continuously
// This runs MANY TIMES PER SECOND while the screen is active
void SimpleContentScreen::loop()
{
    // Only do something if our screen is currently active
    if (!isActive())
    {
        return; // Screen is not active, do nothing
    }

    // Update the progress bar to show which item is selected
    // If we have 4 items and item 2 is selected, show 50% progress
    int progress = 0;
    if (_totalItems > 1)
    {
        progress = (_selectedItem * 100) / (_totalItems - 1);
    }
    setProgressValue(progress);
}

// STEP 5: ACTION TAKEN - Handle button presses
// This runs whenever the user presses a button or moves the joystick
Screen *SimpleContentScreen::actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY)
{
    // Only respond to input if our screen is active
    if (!isActive())
    {
        return nullptr; // Not active, ignore input
    }

    // UP BUTTON OR JOYSTICK UP: Move to previous menu item
    if (action & (JOYSTICK_UP | BUTTON_UP))
    {
        if (_selectedItem > 0)
        {
            _selectedItem--; // Go to previous item

            // Change LED color to show we moved
            M1Shield.setLEDColor(LEDColor::COLOR_GREEN);

            // Tell ContentScreen to redraw the content area
            // You can also call `refresh()`, but this will redraw everything.
            _drawContent();
        }
    }

    // DOWN BUTTON OR JOYSTICK DOWN: Move to next menu item
    else if (action & (JOYSTICK_DOWN | BUTTON_DOWN))
    {
        if (_selectedItem < _totalItems - 1)
        {
            _selectedItem++; // Go to next item

            // Change LED color to show we moved
            M1Shield.setLEDColor(LEDColor::COLOR_GREEN);

            // Tell ContentScreen to redraw the content area
            _drawContent();
        }
    }

    // MENU BUTTON OR JOYSTICK PRESS: Select the current item
    else if (action & (BUTTON_MENU | BUTTON_JOYSTICK))
    {
        // Set LED white to show selection
        M1Shield.setLEDColor(LEDColor::COLOR_WHITE);

        // For this simple example, we'll just stay on the same screen
        // In a real app, you might return a new screen here:
        // return new SettingsScreen();  // Go to settings screen

        return nullptr; // Stay on this screen
    }

    return nullptr; // Stay on this screen (don't change screens)
}
