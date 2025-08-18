/*
 * M1Shield Comprehensive Screen Example
 *
 * Complete demonstration of ALL Screen class features.
 * See README.md for complete documentation and learning guide.
 *
 * Authors: Marcel Erz (RetroStack)
 * License: MIT
 */

#include <M1Shield.h>
#include <Screen.h>
#include "ComprehensiveScreen.h" // Our comprehensive screen examples

// For ST7789 240x320 displays (most common, landscape becomes 320x240)
#include <Display_ST7789_320x240.h>
Display_ST7789_320x240 displayProvider;

// For ST7789 240x240 square displays
// #include <Display_ST7789_240x240.h>
// Display_ST7789_240x240 displayProvider;

// For ST7789 320x170 wide displays (landscape)
// #include <Display_ST7789_320x170.h>
// Display_ST7789_320x170 displayProvider;

// For smaller ST7735 128x160 displays
// #include <Display_ST7735.h>
// Display_ST7735 displayProvider;

// For large ST7796 320x480 displays (landscape becomes 480x320)
// #include <Display_ST7796.h>
// Display_ST7796 displayProvider;

// For parallel ILI9325 240x320 displays (landscape becomes 320x240)
// #include <Display_ILI9325.h>
// Display_ILI9325 displayProvider;

// For ILI9341 240x320 displays (landscape becomes 320x240)
// #include <Display_ILI9341.h>
// Display_ILI9341 displayProvider;

// For large HX8357 320x480 displays
// #include <Display_HX8357.h>
// Display_HX8357 displayProvider;

// For monochrome SSD1306 OLED displays (128x64)
// #include <Display_SSD1306.h>
// Display_SSD1306 displayProvider;

// For monochrome SH1106 OLED displays (128x64)
// #include <Display_SH1106.h>
// Display_SH1106 displayProvider;

// ----------------------------------

// This demonstrates the three core screen capabilities
ComprehensiveScreen screenDemo;

void setup()
{
    // Start serial communication for debug messages
    // (Open "Serial Monitor" in Arduino IDE to see these)
    Serial.begin(115200);
    Serial.println("=== Screen Complete Tutorial Starting ===");

    // Initialize the M1Shield hardware
    // This sets up display, buttons, joystick, LED, etc.
    M1Shield.begin(displayProvider);

    // Enable joystick input for comprehensive input testing
    // This is important for testing all Screen input capabilities
    M1Shield.activateJoystick();

    Serial.println("Hardware initialized!");

    // Set our comprehensive screen as the active screen
    Serial.println("Setting comprehensive screen demo...");
    M1Shield.setScreen(&screenDemo);

    Serial.println("Screen demo loaded!");
    Serial.println("");
    Serial.println("=== SCREEN FEATURES TUTORIAL ===");
    Serial.println("This example demonstrates ALL Screen base class capabilities:");
    Serial.println("1. Complete input handling (all buttons + joystick)");
    Serial.println("2. Screen lifecycle management (open/close/active)");
    Serial.println("3. Display size adaptation (small/large displays)");
    Serial.println("4. Screen navigation and transitions");
    Serial.println("5. Advanced input processing and combinations");
    Serial.println("6. Performance optimization and best practices");
    Serial.println("");
    Serial.println("=== CONTROLS ===");
    Serial.println("- ALL BUTTONS: Test complete input handling");
    Serial.println("- JOYSTICK: Test analog input and all directions");
    Serial.println("- MENU: Navigate between different demo screens");
    Serial.println("- UP/DOWN: Adjust demo parameters");
    Serial.println("- LEFT/RIGHT: Change settings");
    Serial.println("- JOYSTICK PRESS: Special actions");
    Serial.println("");
    Serial.println("=== DEMO PROGRESSION ===");
    Serial.println("1. INPUT DEMO: Tests all input capabilities");
    Serial.println("2. LIFECYCLE DEMO: Shows screen state management");
    Serial.println("3. ANIMATION DEMO: Demonstrates real-time updates");
    Serial.println("Press MENU to cycle through all three demos!");
    Serial.println("");
    Serial.println("Ready! Start exploring all Screen capabilities!");
}

void loop()
{
    // This is all you need! M1Shield.loop() handles everything:
    // - Reading button presses and joystick movement
    // - Calling your screen's loop() method
    // - Calling your screen's actionTaken() when inputs occur
    // - Managing the display updates
    // - Everything else!
    M1Shield.loop();

    // The magic happens in ComprehensiveScreen.cpp!
}
