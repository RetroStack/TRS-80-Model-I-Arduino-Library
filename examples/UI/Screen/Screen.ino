/*
 * ===================================================================
 * M1Shield Screen Example - Simple Screen Tutorial
 * ===================================================================
 *
 * This is the easiest possible example to get you started with screens.
 * Perfect if you're new to Arduino or the M1Shield!
 * When using a screen directly, you can control everything yourself.
 * Use ContentScreen for automatic headers/footers and MenuScreen for menus.
 *
 * WHAT YOU'LL LEARN:
 * - How to create a basic interactive screen
 * - How button presses work
 * - How to change the LED color
 * - How to display text on the screen
 *
 * WHAT IT DOES:
 * - Shows a simple screen with instructions
 * - Counts how many times you press buttons
 * - Up/Down buttons change LED color (8 different colors!)
 * - Menu button resets the counter to zero
 *
 * CONTROLS:
 * - Up Button (or Joystick Up): Next LED color
 * - Down Button (or Joystick Down): Previous LED color
 * - Menu Button: Reset button counter to 0
 * - Any button press: Increases the counter
 *
 * WHAT YOU NEED:
 * - M1Shield with working display
 *
 * TROUBLESHOOTING:
 * - Blank screen? Check if USE_ST7789 or USE_SSD1306 matches your display
 *
 * TRY THIS:
 * 1. Upload this code to your Arduino
 * 2. Watch the screen - you should see "Simple Screen" and instructions
 * 3. Press the Up button - LED should change color!
 * 4. Press any button - the counter should increase
 * 5. Press Menu - counter resets to 0
 *
 * Authors: Marcel Erz (RetroStack)
 * License: MIT
 */

// First, tell the system which display you have
// For ST7789 240x320 displays (most common, landscape becomes 320x240)
#include <Display_ST7789_320x240.h>
Display_ST7789_320x240 displayProvider;

// For ST7789 240x240 square displays
// #include <Display_ST7789_240x240.h>
// Display_ST7789_240x240 displayProvider;

// For ST7789 320x170 wide displays (landscape)
// #include <Display_ST7789_320x170.h>
// Display_ST7789_320x170 displayProvider;

// For ST7789 320x240 alternative displays (landscape)
// #include <Display_ST7789_320x240.h>
// Display_ST7789_320x240 displayProvider;

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

// Include the libraries we need
#include <M1Shield.h>      // Main M1Shield functions
#include "ExampleScreen.h" // Our simple screen

void setup()
{
    // Start serial communication (for debug messages)
    Serial.begin(115200);
    Serial.println("=== Simple Screen Example Starting ===");

    // Initialize the M1Shield hardware
    // This sets up display, buttons, joystick, LED, etc.
    M1Shield.begin(displayProvider);

    // Optional: Enable joystick input for screen control
    // Uncomment the next line to allow joystick interaction
    // M1Shield.activateJoystick();

    Serial.println("✓ M1Shield initialized!");

    // Create our simple screen and tell M1Shield to use it
    M1Shield.setScreen(new ExampleScreen());
    Serial.println("✓ Simple screen loaded!");
    Serial.println("Ready! Try pressing buttons on your M1Shield.");
}

void loop()
{
    // This is all you need! M1Shield.loop() handles everything:
    // - Reading button presses
    // - Updating the display
    // - Managing the LED
    // - Calling your screen's methods
    M1Shield.loop();

    // Seriously, that's it! The magic happens in ExampleScreen.cpp
}
