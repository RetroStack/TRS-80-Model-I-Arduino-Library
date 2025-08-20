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
#include <SerialLogger.h>
#include <Model1.h>
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

// Serial logger for debugging M1Shield and Model1 issues
SerialLogger logger;

void setup()
{
    // Start serial communication for debug messages
    Serial.begin(115200);
    delay(1000); // Give serial time to initialize

    Serial.println("=== Comprehensive Screen Example ===");
    Serial.println("Demonstrating ALL Screen class capabilities");

    // Set up logging for M1Shield and Model1 to capture any errors
    M1Shield.setLogger(logger); // <-- Sets logger onto all attached screen automatically
    Model1.setLogger(logger);

    // Set up logging for our screen
    // Initialize the M1Shield hardware
    M1Shield.begin(displayProvider);

    // Enable joystick input for comprehensive input testing
    M1Shield.activateJoystick();

    // Set our comprehensive screen as the active screen
    if (M1Shield.setScreen(new ComprehensiveScreen()))
    {
        Serial.println("Screen demo loaded successfully!");
    }
    else
    {
        Serial.println("ERROR: Failed to load screen demo!");
    }

    Serial.println("");
    Serial.println("=== SCREEN FEATURES TUTORIAL ===");
    Serial.println("This example demonstrates ALL Screen base class capabilities:");
    Serial.println("1. Complete input handling (all buttons + joystick)");
    Serial.println("2. Screen lifecycle management (open/close/active)");
    Serial.println("3. Display size adaptation (small/large displays)");
    Serial.println("4. Screen navigation and transitions");
    Serial.println("5. Advanced input processing and combinations");
    Serial.println("6. Performance optimization and real-time updates");
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
    // Let M1Shield handle input and call our screen's methods
    M1Shield.loop();
}
