/*
 * SimpleButtonScreen.ino - Simple button screen demonstration
 *
 * Demonstrates the ButtonScreen class with horizontal navigation,
 * config values, and enabled/disabled button items.
 *
 * Hardware: Arduino Mega 2560 with M1Shield
 *
 * Usage:
 * - Use left/right arrows to navigate button items horizontally
 * - Press up arrow or joystick to select an item
 * - Press menu button to exit
 *
 * Author: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#include <M1Shield.h>
#include "SimpleButton.h"

// Uncomment the display type you're using:
// For ST7789 240x240 square displays
// #include <Display_ST7789_240x240.h>
// Display_ST7789_240x240 displayProvider;

// For ST7789 320x170 wide displays (landscape)
// #include <Display_ST7789_320x170.h>
// Display_ST7789_320x170 displayProvider;

// For ST7789 320x240 displays (landscape)
#include <Display_ST7789_320x240.h>
Display_ST7789_320x240 displayProvider;

// For smaller ST7735 128x160 displays
// #include <Display_ST7735.h>
// Display_ST7735 displayProvider;

// For ILI9341 240x320 displays (landscape becomes 320x240)
// #include <Display_ILI9341.h>
// Display_ILI9341 displayProvider;

// For large HX8357 320x480 displays
// #include <Display_HX8357.h>
// Display_HX8357 displayProvider;

// For parallel ILI9325 240x320 displays (landscape becomes 320x240)
// #include <Display_ILI9325.h>
// Display_ILI9325 displayProvider;

// For large ST7796 320x480 displays (landscape becomes 480x320)
// #include <Display_ST7796.h>
// Display_ST7796 displayProvider;

// For monochrome SSD1306 OLED displays (128x64)
// #include <Display_SSD1306.h>
// Display_SSD1306 displayProvider;

// For monochrome SH1106 OLED displays (128x64)
// #include <Display_SH1106.h>
// Display_SH1106 displayProvider;

void setup()
{
    Serial.begin(115200);
    Serial.println(F("Button Screen"));

    // Initialize the M1Shield with display
    if (!M1Shield.begin(displayProvider))
    {
        Serial.println(F("Failed to initialize M1Shield!"));
        while (1)
            delay(100);
    }

    Serial.println(F("M1Shield initialized successfully"));
    Serial.println(F("Use left/right to navigate, up to select, menu to exit"));

    // Set the button screen as the active screen
    M1Shield.setScreen(new SimpleButton());
}

void loop()
{
    M1Shield.loop();
}
