#include "ROMDetectionDemo.h"
#include "ROMFeaturesMenu.h"
#include <Arduino.h>
#include "logger.h"

ROMDetectionDemo::ROMDetectionDemo() : ConsoleScreen()
{
    _setTitle("ROM Detection");
    setConsoleBackground(0x0000);
    setTextColor(0xFFFF, 0x0000);
    _rom.setLogger(logger);

    // Set button labels
    const char *buttons[] = {"M:Back", "", "", ""};
    _setButtonItems(buttons, 4);

    Serial.println(F("ROM Detection Demo initialized"));
}

void ROMDetectionDemo::_executeOnce()
{
    cls();
    setTextColor(0x07E0, 0x0000); // Green
    println(F("=== ROM DETECTION DEMO ==="));
    println();
    setTextColor(0xFFFF, 0x0000);
    println(F("Analyzing ROM contents..."));
    println();
    Model1.activateTestSignal();
    uint16_t a = _rom.getChecksum(0);
    uint16_t b = _rom.getChecksum(1);
    uint16_t c = _rom.getChecksum(2);
    uint16_t d = _rom.getChecksum(3);
    Model1.deactivateTestSignal();

    setTextColor(0xFFFF, 0x0000);
    print(F("Checksum ROM A: "));
    setTextColor(0x07FF, 0x0000);
    println(a, HEX);

    setTextColor(0xFFFF, 0x0000);
    print(F("Checksum ROM B: "));
    setTextColor(0x07FF, 0x0000);
    println(b, HEX);

    setTextColor(0xFFFF, 0x0000);
    print(F("Checksum ROM C: "));
    setTextColor(0x07FF, 0x0000);
    println(c, HEX);

    setTextColor(0xFFFF, 0x0000);
    print(F("Checksum ROM D: "));
    setTextColor(0x07FF, 0x0000);
    println(d, HEX);

    // Try to detect ROM type
    Model1.activateTestSignal();
    const __FlashStringHelper *romType = _rom.identifyROM();
    Model1.deactivateTestSignal();
    print(F("ROM Type: "));
    if (romType != nullptr)
    {
        setTextColor(0x07E0, 0x0000); // Green
        println(romType);
    }
    else
    {
        setTextColor(0xF800, 0x0000); // Red
        println(F("Unknown"));
    }

    println();
    setTextColor(0xF81F, 0x0000); // Magenta
    println(F("Detection complete!"));
}

Screen *ROMDetectionDemo::actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY)
{
    if (action & BUTTON_MENU)
    {
        Serial.println(F("Returning to ROM Features Menu"));
        return new ROMFeaturesMenu();
    }

    return nullptr;
}
