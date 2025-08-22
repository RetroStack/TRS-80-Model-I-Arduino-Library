#include "InfoScreen.h"
#include "ComprehensiveButton.h"

InfoScreen::InfoScreen() : ConsoleScreen()
{
    setTitleF(F("System Information"));
}

void InfoScreen::_executeOnce()
{
    cls();
    println(F("TRS-80 Model I Library"));
    println(F("ButtonScreen Demo"));
    println();
    println(F("Current Settings:"));
    print(F("Audio: "));
    println(settings.audioEnabled ? F("ON") : F("OFF"));
    print(F("Brightness: "));
    print(settings.brightness);
    println(F("%"));
    print(F("Volume: "));
    print(settings.volume);
    println(F("%"));
    print(F("WiFi: "));
    println(settings.wifiEnabled ? F("ON") : F("OFF"));
    print(F("Contrast: "));
    print(settings.contrast);
    println(F("%"));
    print(F("Debug: "));
    println(settings.debugMode ? F("ON") : F("OFF"));
    println();
    println(F("MENU: Return"));
}

Screen *InfoScreen::actionTaken(ActionTaken action, int8_t offsetX, int8_t offsetY)
{
    if (action & BUTTON_MENU)
    {
        return new ComprehensiveButton(); // Let ButtonScreen handle navigation back to previous screen
    }
    return ConsoleScreen::actionTaken(action, offsetX, offsetY);
}
