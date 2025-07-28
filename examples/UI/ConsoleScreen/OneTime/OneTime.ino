/*
 * ===================================================================
 * ConsoleScreen One-Time Execution Example
 * ===================================================================
 *
 * This example demonstrates the _executeOnce() method in ConsoleScreen!
 *
 * WHAT'S ONE-TIME EXECUTION:
 * - _executeOnce() - called exactly once, 1 second after console opens
 * - Perfect for initialization, data loading, or setup tasks
 * - Avoids the continuous execution overhead of loop() for one-time tasks
 * - Works alongside the existing auto-clear wrap-around behavior
 *
 * BEHAVIOR DEMONSTRATION:
 * 1. Console opens with "Initializing console..." message
 * 2. After 1 second: _executeOnce() runs automatically
 *    - Shows system initialization messages
 *    - Changes colors and displays status
 * 3. Then: loop() runs continuously for ongoing updates
 *    - Shows current time and status
 *    - Demonstrates auto-clear when console fills up
 *
 * HOW TO TEST:
 * - Upload and watch Serial Monitor for detailed logging
 * - Watch continuous time updates and auto-clear behavior
 * - Press joystick to restart and see _executeOnce() run again
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

#include <M1Shield.h>
#include "OneTimeExecutionDemo.h"

void setup()
{
    Serial.begin(115200);
    M1Shield.begin(displayProvider);
    
    // Optional: Enable joystick input for console navigation
    // Uncomment the next line to allow joystick control
    // M1Shield.activateJoystick();

    Serial.println("=== ConsoleScreen One-Time Execution Demo ===");
    Serial.println("Watch the console for initialization sequence!");

    M1Shield.setScreen(new OneTimeExecutionDemo());
}

void loop()
{
    M1Shield.loop();
}
