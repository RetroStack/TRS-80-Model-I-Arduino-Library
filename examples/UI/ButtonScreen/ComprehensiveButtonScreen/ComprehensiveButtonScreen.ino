/*
 * ComprehensiveButtonScreen.ino - Comprehensive button screen demonstration
 *
 * Demonstrates advanced features of the ButtonScreen class including:
 * - Dynamic button item updates
 * - FlashString button items
 * - Real-time configuration values
 * - Complex state management
 * - Multiple button screens
 *
 * Hardware: Arduino Mega 2560 with M1Shield
 *
 * Author: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#include <M1Shield.h>
#include "ComprehensiveButton.h"

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
    Serial.println(F("Comprehensive Button Screen Example"));

    if (!M1Shield.begin(displayProvider))
    {
        Serial.println(F("Failed to initialize M1Shield!"));
        while (1)
            delay(100);
    }

    Serial.println(F("M1Shield initialized"));
    Serial.println(F("Features demonstrated:"));
    Serial.println(F("- Dynamic config values"));
    Serial.println(F("- Real-time updates"));
    Serial.println(F("- State-dependent enabling"));
    Serial.println(F("- FlashString button items"));
    Serial.println(F("- Complex navigation"));

    // Start with main menu
    M1Shield.setScreen(new ComprehensiveButton());
}

void loop()
{
    M1Shield.loop();
}
