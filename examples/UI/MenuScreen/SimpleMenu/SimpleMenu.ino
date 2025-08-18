/*
 * M1Shield Simple MenuScreen Example
 *
 * Basic menu with LED color selection.
 * See README.md for complete documentation.
 *
 * Authors: Marcel Erz (RetroStack)
 * License: MIT
 */

// STEP 1: Tell the system what display you have
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

// STEP 2: Include the libraries we need
#include <M1Shield.h>          // Main M1Shield functions
#include <MenuScreen.h>        // MenuScreen class
#include "SimpleMenu.h"        // Our simple menu

// STEP 3: Create our menu as a global object
SimpleMenu mainMenu;

void setup()
{
    // Start serial communication for debug messages
    // (Open "Serial Monitor" in Arduino IDE to see these)
    Serial.begin(115200);
    Serial.println("=== Simple MenuScreen Tutorial Starting ===");

    // Initialize the M1Shield hardware
    // This sets up display, buttons, joystick, LED, etc.
    M1Shield.begin(displayProvider);

    Serial.println("Hardware initialized!");

    // Set our menu as the active screen
    Serial.println("Setting main menu...");
    M1Shield.setScreen(&mainMenu);

    Serial.println("Menu loaded!");
    Serial.println("");
    Serial.println("=== SIMPLE MENU TUTORIAL ===");
    Serial.println("This example shows basic MenuScreen usage:");
    Serial.println("- Simple menu with 5 items");
    Serial.println("- Up/Down navigation");
    Serial.println("- Menu selection handling");
    Serial.println("- LED color feedback");
    Serial.println("");
    Serial.println("=== CONTROLS ===");
    Serial.println("- UP/DOWN: Navigate menu items");
    Serial.println("- MENU/RIGHT: Select current item");
    Serial.println("- Watch the LED change colors!");
    Serial.println("");
    Serial.println("Ready! Start navigating your first menu!");
}

void loop()
{
    // This is all you need! M1Shield.loop() handles everything:
    // - Reading button presses and joystick movement
    // - Calling your menu's methods automatically
    // - Managing the display updates
    // - Everything else!
    M1Shield.loop();

    // The magic happens in SimpleMenu.cpp!
}
