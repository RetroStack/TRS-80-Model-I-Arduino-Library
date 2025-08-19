/*
 * SecondContent.cpp - Second ContentScreen for demonstrating screen switching
 *
 * Simple second screen to show navigation between ContentScreens
 *
 * Authors: Marcel Erz (RetroStack)
 * License: MIT
 */

#include "SecondContent.h"
#include "SimpleContent.h"
#include <M1Shield.h>

SecondContent::SecondContent() : ContentScreen()
{
    // Set the title that appears in the header
    setTitle("Second Screen");

    // Set footer button hints
    const char *buttons[] = {"Back", "Notify"};
    setButtonItems(buttons, 2);
}

void SecondContent::_drawContent()
{
    // This method draws content in the main area
    // (between header and footer)

    drawTextF(10, 10, F("Welcome to Screen 2!"), M1Shield.convertColor(0x07E0), 2);
    drawTextF(10, 40, F("This demonstrates screen"), M1Shield.convertColor(0xFFFF), 1);
    drawTextF(10, 55, F("switching between"), M1Shield.convertColor(0xFFFF), 1);
    drawTextF(10, 70, F("ContentScreen instances."), M1Shield.convertColor(0xFFFF), 1);
    drawTextF(10, 95, F("MENU: Back to first screen"), M1Shield.convertColor(0x07FF), 1);
    drawTextF(10, 110, F("UP: Show notification"), M1Shield.convertColor(0x07FF), 1);
    drawTextF(10, 125, F("DOWN: Show confirmation"), M1Shield.convertColor(0x07FF), 1);
}

void SecondContent::loop()
{
    // Call base class loop to handle notification timeouts
    ContentScreen::loop();

    // Add any additional loop logic here if needed
    // For simple screens, the base class handling is usually sufficient
}

Screen *SecondContent::actionTaken(ActionTaken action, int8_t offsetX, int8_t offsetY)
{
    // Handle button presses
    if (action & BUTTON_MENU)
    {
        // Switch back to the first screen
        return new SimpleContent();
    }

    if (action & BUTTON_UP)
    {
        // Show a notification
        notifyF(F("Hello from Screen 2!"));
        return nullptr; // Stay on this screen
    }

    if (action & BUTTON_DOWN)
    {
        // Show a confirmation dialog
        ConfirmResult result = confirmF(F("Sure?"), F("Yes"), F("No"));
        if (result == CONFIRM_LEFT)
        {
            notifyF(F("You chose Yes!"));
        }
        else
        {
            notifyF(F("You chose No!"));
        }
        return nullptr; // Stay on this screen
    }

    // For any other buttons, stay on this screen
    return nullptr;
}
