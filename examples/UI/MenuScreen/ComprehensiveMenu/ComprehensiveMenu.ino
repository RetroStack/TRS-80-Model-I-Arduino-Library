/*
 * M1Shield Comprehensive MenuScreen Example
 *
 * Complete demonstration of ALL MenuScreen features.
 * See README.md for complete documentation and learning guide.
 *
 * Authors: Marcel Erz (RetroStack)
 * License: MIT
 */

#include <M1Shield.h>
#include "ComprehensiveMenuScreen.h"

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

void setup()
{
    // Start serial communication for debugging
    Serial.begin(115200);
    Serial.println("=== Comprehensive MenuScreen Example Starting ===");

    // Initialize the M1Shield hardware
    M1Shield.begin(displayProvider);

    // Create and set up the main menu screen
    M1Shield.setScreen(new ComprehensiveMenuScreen());

    Serial.println("Setup complete - menu ready!");
}
void loop()
{
    // Handle all M1Shield operations
    M1Shield.loop();
}
