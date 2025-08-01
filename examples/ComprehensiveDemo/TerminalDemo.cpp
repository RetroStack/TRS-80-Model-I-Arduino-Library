#include "TerminalDemo.h"
#include "MainDemoMenu.h"
#include <Arduino.h>

TerminalDemo::TerminalDemo() : M1TerminalScreen()
{
    _setTitle("Terminal Demo");

    // Set button labels for terminal navigation
    const char *buttons[] = {"M:Menu", "LF:Left", "RT:Right", "UP/DN"};
    _setButtonItems(buttons, 4);

    Serial.println(F("Terminal Demo initialized"));
}

Screen *TerminalDemo::actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY)
{
    if (!isActive())
        return nullptr;

    Serial.println(F("Terminal actionTaken called"));
    if (action & BUTTON_MENU)
    {
        Serial.println(F("Returning to main menu from Terminal Demo"));
        return new MainDemoMenu();
    }

    // Let parent handle other actions
    return M1TerminalScreen::actionTaken(action, offsetX, offsetY);
}
