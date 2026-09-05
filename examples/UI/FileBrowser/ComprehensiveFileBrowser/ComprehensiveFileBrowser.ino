/*
 * ComprehensiveFileBrowser.ino - Advanced SD Card File Browser Example
 *
 * This example demonstrates advanced FileBrowser features including:
 * - Directory restrictions (root directory limiting)
 * - Pre-selection of specific files
 * - Custom file type extensions
 * - Starting from specific directories
 * - Multiple browser configurations
 *
 * Features demonstrated:
 * - Root directory restriction
 * - Target file pre-selection and auto-scroll
 * - Custom text file extensions
 * - Directory-specific browsing
 * - Menu integration for multiple browsers
 *
 * Hardware: Arduino Mega 2560 + M1Shield + SD Card
 *
 * Authors: Marcel Erz (RetroStack)
 * License: MIT
 */

#include <M1Shield.h>
#include <FileBrowser.h>
#include <MenuScreen.h>
#include "FileBrowserMenu.h"

// First, tell the system which display you have.
// Uncomment exactly one and comment out the rest.

// For ST7789 240x320 displays (most common, landscape becomes 320x240)
// #include <Display_ST7789_320x240.h>
// Display_ST7789_320x240 displayProvider;

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
#include <Display_ILI9341.h>
Display_ILI9341 displayProvider;

// For large HX8357 320x480 displays
// #include <Display_HX8357.h>
// Display_HX8357 displayProvider;

// For monochrome SSD1306 OLED displays (128x64)
// #include <Display_SSD1306.h>
// Display_SSD1306 displayProvider;

// For monochrome SH1106 OLED displays (128x64)
// #include <Display_SH1106.h>
// Display_SH1106 displayProvider;

void setup()
{
    // Initialize serial for debugging
    Serial.begin(115200);
    Serial.println("=== Comprehensive FileBrowser Example ===");

    // Initialize M1Shield
    M1Shield.begin(displayProvider);

    // Create and set the main menu
    M1Shield.setScreen(new FileBrowserMenu());

    Serial.println("FileBrowser menu loaded and ready!");
    Serial.println();
    Serial.println("Example Configurations:");
    Serial.println("1. Basic root browsing - Full SD card access");
    Serial.println("2. Restricted browsing - Limited to /logs directory");
    Serial.println("3. Pre-selection - Opens /data and highlights config.txt");
    Serial.println("4. Custom types - Additional file extensions for text viewer");
    Serial.println("5. Document focus - Only browse /documents with document types");
    Serial.println();
    Serial.println("Setup Required:");
    Serial.println("- Create /logs directory with some .log files");
    Serial.println("- Create /data directory with config.txt file");
    Serial.println("- Create /documents directory with .txt/.md files");
    Serial.println("- Add various file types to test extension handling");
}

void loop()
{
    // Handle all M1Shield operations
    M1Shield.loop();
}
