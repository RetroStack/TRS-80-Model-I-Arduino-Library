/*
 * SimpleContent.cpp - Minimal ContentScreen Example
 *
 * Bare minimum ContentScreen demonstration:
 * - Header with title
 * - Simple content display
 * - Basic button interaction
 *
 * Authors: Marcel Erz (RetroStack)
 * License: MIT
 */

#include "SimpleContent.h"
#include "SecondContent.h"
#include <M1Shield.h>

SimpleContent::SimpleContent() : ContentScreen()
{
    // Set the title that appears in the header
    setTitle("Simple Content");

    // Set footer button hints
    const char *buttons[] = {"Alert", "Switch"};
    setButtonItems(buttons, 2);
}

void SimpleContent::_drawContent()
{
    // This method draws content in the main area
    // (between header and footer)

    drawTextF(10, 10, F("Hello ContentScreen!"), M1Shield.convertColor(0xFFFF), 2);
    drawTextF(10, 40, F("This is the content area."), M1Shield.convertColor(0xFFFF), 1);
    drawTextF(10, 60, F("UP: Show alert"), M1Shield.convertColor(0x07FF), 1);
    drawTextF(10, 75, F("MENU: Switch to screen 2"), M1Shield.convertColor(0x07FF), 1);
}

void SimpleContent::loop()
{
    // Call base class loop to handle notification timeouts
    ContentScreen::loop();

    // Add any additional loop logic here if needed
    // For simple screens, the base class handling is usually sufficient
}

Screen *SimpleContent::actionTaken(ActionTaken action, int8_t offsetX, int8_t offsetY)
{
    // Handle button presses
    if (action & BUTTON_MENU)
    {
        // Switch to the second screen
        return new SecondContent();
    }

    if (action & BUTTON_UP)
    {
        // Show a simple alert
        alertF(F("Button pressed!"));
        return nullptr; // Stay on this screen
    }

    // For any other buttons, stay on this screen
    return nullptr;
}
