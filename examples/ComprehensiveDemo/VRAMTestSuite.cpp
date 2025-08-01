#include "VRAMTestSuite.h"
#include "MainDemoMenu.h"
#include <Arduino.h>

VRAMTestSuite::VRAMTestSuite() : RAMTestSuite()
{
    _setTitle("VRAM Tests");
    setConsoleBackground(0x0000);
    setTextColor(0xFFFF, 0x0000);

    // Set button labels
    const char *buttons[] = {"M:Menu", "", "", ""};
    _setButtonItems(buttons, 4);

    Serial.println(F("VRAM Test Suite initialized"));
}

void VRAMTestSuite::_executeOnce()
{
    cls();
    setTextColor(0x07E0, 0x0000); // Green
    println(F("=== VRAM TEST SUITE ==="));
    println();
    setTextColor(0xFFFF, 0x0000);
    println(F("Testing Video RAM (1KB)"));
    println(F("Memory Range: 0x3C00-0x3FFF"));
    println(F("IC References: Z48,Z47,Z46,Z45,Z61,Z62,Z?,Z63"));
    println();

    setTextColor(0xFFE0, 0x0000); // Yellow
    println(F("This test will comprehensively verify"));
    println(F("the 1KB video RAM that stores the"));
    println(F("character data displayed on screen."));
    println();

    setTextColor(0x07FF, 0x0000); // Cyan
    println(F("Tests include:"));
    println(F("- Character pattern tests"));
    println(F("- Walking ones/zeros bit tests"));
    println(F("- March algorithms (C-, SS, LA)"));
    println(F("- Moving inversion tests"));
    println(F("- Address uniqueness verification"));
    println(F("- Display memory retention testing"));
    println(F("- Video refresh interference tests"));
    println();

    setTextColor(0xF800, 0x0000); // Red
    println(F("WARNING: Screen may flicker during"));
    println(F("video memory testing!"));
    println();

    setTextColor(0xF81F, 0x0000); // Magenta
    println(F("Starting VRAM comprehensive test..."));
    println();

    setTextColor(0xFFFF, 0x0000); // White

    // Local VRAM constants - only exist when test is running
    const uint16_t start = 0x3C00; // VRAM start address
    const uint16_t length = 1024;  // 1KB VRAM
    const char *const icRefs[] PROGMEM = {
        "Z48", "Z47", "Z46", "Z45", "Z61", "Z62", "Z?", "Z63"};

    // Run the comprehensive test suite on VRAM
    runSpecializedTest(start, length, icRefs);

    println();
    setTextColor(0x07E0, 0x0000); // Green
    println(F("VRAM testing completed!"));
    println();
    setTextColor(0xF81F, 0x0000); // Magenta
    println(F("Tests finished!"));
}

Screen *VRAMTestSuite::actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY)
{
    if (action & BUTTON_MENU)
    {
        Serial.println(F("Returning to main menu from VRAM Tests"));
        return new MainDemoMenu();
    }

    return nullptr;
}
