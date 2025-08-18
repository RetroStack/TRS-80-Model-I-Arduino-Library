/*
 * M1Shield Comprehensive ContentScreen Example
 *
 * Complete demonstration of ALL ContentScreen features.
 * See README.md for complete documentation and learning guide.
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
#include "ComprehensiveContent.h"

// Create the comprehensive content screen instance
ComprehensiveContent contentScreen;

void setup()
{
    // Initialize serial for debugging
    Serial.begin(115200);
    Serial.println("=== Comprehensive ContentScreen Example ===");

    // Initialize M1Shield
    M1Shield.begin(displayProvider);

    // Set our content screen as active
    M1Shield.setScreen(&contentScreen);

    Serial.println("Comprehensive ContentScreen loaded!");
    Serial.println("Explore all ContentScreen features:");
    Serial.println("- Multiple content modes");
    Serial.println("- Advanced user feedback");
    Serial.println("- Real-time data display");
    Serial.println("- Custom graphics and charts");
    Serial.println("- Dynamic layout management");
}

void loop()
{
    // Handle all M1Shield operations
    M1Shield.loop();
}
