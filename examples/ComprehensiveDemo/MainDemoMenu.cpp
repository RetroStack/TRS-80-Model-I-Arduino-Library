#include "MainDemoMenu.h"
#include "TerminalDemo.h"
#include "ROMFeaturesMenu.h"
#include "VideoFeaturesMenu.h"
#include "DRAMTestSuite.h"
#include "VRAMTestSuite.h"
#include "KeyboardTester.h"
#include "CassetteMenu.h"
#include <Model1.h>
#include <Arduino.h>

MainDemoMenu::MainDemoMenu() : MenuScreen()
{
    _setTitle("M1Shield Demo");

    // Create menu items dynamically - they'll be copied by _setMenuItems and these will be freed automatically
    const char *menuItems[] = {
        "Terminal Demo",
        "ROM Features",
        "Video Features",
        "DRAM Testing",
        "VRAM Testing",
        "Keyboard Test",
        "Cassette Interface"};

    _setMenuItems(menuItems, 7);

    Serial.println(F("Main Demo Menu initialized"));
}

Screen *MainDemoMenu::_getSelectedMenuItemScreen(int index)
{
    switch (index)
    {
    case 0: // Terminal Demo
        Serial.println(F("Opening Terminal Demo"));
        return new TerminalDemo();

    case 1: // ROM Features
        Serial.println(F("Opening ROM Features Menu"));
        return new ROMFeaturesMenu();

    case 2: // Video Features
        Serial.println(F("Opening Video Features Menu"));
        return new VideoFeaturesMenu();

    case 3: // DRAM Testing
        Serial.println(F("Opening DRAM Test Suite"));
        return new DRAMTestSuite();

    case 4: // VRAM Testing
        Serial.println(F("Opening VRAM Test Suite"));
        return new VRAMTestSuite();

    case 5: // Keyboard Test
        Serial.println(F("Opening Keyboard Tester"));
        return new KeyboardTester();

    case 6: // Cassette Player
        Serial.println(F("Opening Cassette Menu"));
        return new CassetteMenu();

    default:
        return nullptr;
    }
}
