#include "DRAMTestSuite.h"
#include "MainDemoMenu.h"
#include <Arduino.h>

DRAMTestSuite::DRAMTestSuite() : RAMTestSuite()
{
    _setTitle("DRAM Tests");
    setConsoleBackground(0x0000);
    setTextColor(0xFFFF, 0x0000);

    // Set button labels
    const char *buttons[] = {"M:Menu", "", "", ""};
    _setButtonItems(buttons, 4);

    Serial.println(F("DRAM Test Suite initialized"));
}

void DRAMTestSuite::_executeOnce()
{
    cls();
    setTextColor(0x07E0, 0x0000); // Green
    println(F("=== DRAM TEST SUITE ==="));
    println();
    setTextColor(0xFFFF, 0x0000);
    println(F("Testing Dynamic RAM (16KB)"));
    println(F("Memory Range: 0x4000-0x7FFF"));
    println(F("IC References: Z17,Z16,Z18,Z19,Z15,Z20,Z14,Z13"));
    println();

    setTextColor(0xFFE0, 0x0000); // Yellow
    println(F("This test will comprehensively verify"));
    println(F("the 16KB dynamic RAM expansion using"));
    println(F("multiple test patterns and algorithms."));
    println();

    setTextColor(0x07FF, 0x0000); // Cyan
    println(F("Tests include:"));
    println(F("- Pattern tests (0x55, 0xAA, checkerboard)"));
    println(F("- Walking ones/zeros bit tests"));
    println(F("- March algorithms (C-, SS, LA)"));
    println(F("- Moving inversion tests"));
    println(F("- Address uniqueness verification"));
    println(F("- Data retention testing"));
    println(F("- Read destructive fault detection"));
    println();

    setTextColor(0xF81F, 0x0000); // Magenta
    println(F("Starting DRAM comprehensive test..."));
    println();

    setTextColor(0xFFFF, 0x0000); // White

    // Local DRAM constants - only exist when test is running
    const uint16_t start = 0x4000;     // DRAM start address
    const uint16_t length = 1024 * 16; // 16KB DRAM
    const char *const icRefs[] PROGMEM = {
        "Z17", "Z16", "Z18", "Z19", "Z15", "Z20", "Z14", "Z13"};

    // Run the comprehensive test suite on DRAM
    runSpecializedTest(start, length, icRefs);

    println();
    setTextColor(0x07E0, 0x0000); // Green
    println(F("DRAM testing completed!"));
    println();
    setTextColor(0xF81F, 0x0000); // Magenta
    println(F("Tests finished!"));
}

Screen *DRAMTestSuite::actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY)
{
    if (action & BUTTON_MENU)
    {
        Serial.println(F("Returning to main menu from DRAM Tests"));
        return new MainDemoMenu();
    }

    return nullptr;
}
