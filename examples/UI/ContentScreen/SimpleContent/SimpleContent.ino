/*
 * M1Shield Simple ContentScreen Example
 *
 * Basic content screen with header, footer, and user feedback.
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
#include "SimpleContent.h"

// Create the content screen instance
SimpleContent contentScreen;

void setup()
{
    // Initialize serial for debugging
    Serial.begin(115200);
    Serial.println("=== Simple ContentScreen Example ===");

    // Initialize M1Shield
    M1Shield.begin(displayProvider);

    // Set our content screen as active
    M1Shield.setScreen(&contentScreen);

    Serial.println("ContentScreen loaded and ready!");
}

void loop()
{
    // Handle all M1Shield operations
    M1Shield.loop();
}
