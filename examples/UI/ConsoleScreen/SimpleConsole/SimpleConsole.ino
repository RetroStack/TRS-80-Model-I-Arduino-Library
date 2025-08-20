/*
 * M1Shield Simple ConsoleScreen Example
 *
 * Basic console screen demonstrating text output, colors, and scrolling.
 * See README.md for complete documentation.
 *
 * Authors: Marcel Erz (RetroStack)
 * License: MIT
 */

// Display setup - choose your display type
#include <Display_ST7789_320x240.h>
Display_ST7789_320x240 displayProvider;

// Alternative display options (uncomment as needed):
// #include <Display_ST7789_240x240.h>
// Display_ST7789_240x240 displayProvider;

// #include <Display_ST7789_320x170.h>
// Display_ST7789_320x170 displayProvider;

// #include <Display_ST7735.h>
// Display_ST7735 displayProvider;

// #include <Display_ILI9341.h>
// Display_ILI9341 displayProvider;

// Include required libraries
#include <M1Shield.h>
#include "SimpleConsole.h"

void setup()
{
    // Initialize serial for debugging
    Serial.begin(115200);
    Serial.println("=== Simple ConsoleScreen Example ===");

    // Initialize M1Shield
    M1Shield.begin(displayProvider);

    // Set our console screen as active
    M1Shield.setScreen(new SimpleConsole());

    Serial.println("ConsoleScreen loaded and ready!");
}

void loop()
{
    // Main loop - let M1Shield handle everything
    M1Shield.loop();
}
