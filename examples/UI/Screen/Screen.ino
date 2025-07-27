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
#include <Display_ST7789.h>
Display_ST7789 displayProvider;

// #include <Display_ST7789_240x240.h>
// Display_ST7789_240x240 displayProvider;

// #include <Display_ST7735.h>
// Display_ST7735 displayProvider;

// #include <Display_ST7796.h>
// Display_ST7796 displayProvider;

// #include <Display_ILI9325.h>
// Display_ILI9325 displayProvider;

// #include <Display_ILI9341.h>
// Display_ILI9341 displayProvider;

// #include <Display_HX8357.h>
// Display_HX8357 displayProvider;

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
