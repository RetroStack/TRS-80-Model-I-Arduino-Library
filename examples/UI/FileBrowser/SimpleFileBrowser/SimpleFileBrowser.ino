/*
 * SimpleFileBrowser.ino - Basic SD Card File Browser Example
 *
 * This example demonstrates how to use the FileBrowser class to browse
 * directories and files on the SD card, automatically opening appropriate
 * viewers for different file types.
 *
 * Features demonstrated:
 * - Directory navigation with ".." support
 * - File type detection (text vs binary)
 * - Automatic viewer selection
 * - File size display
 * - Pre-selection of specific files
 *
 * Hardware: Arduino Mega 2560 + M1Shield + SD Card
 *
 * Authors: Marcel Erz (RetroStack)
 * License: MIT
 */

#include <M1Shield.h>
#include <FileBrowser.h>

// Uncomment ONE of these display provider lines based on your hardware:
// See M1Shield.h for all supported displays and their setup requirements

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
    Serial.println("=== Simple FileBrowser Example ===");

    // Initialize M1Shield
    M1Shield.begin(displayProvider);

    // Create and set the file browser screen
    //
    // Simplified constructor examples (all use the same constructor with defaults):
    // M1Shield.setScreen(new FileBrowser());                          // Browse from root
    // M1Shield.setScreen(new FileBrowser("/", "config.txt"));         // Pre-select file in root
    // M1Shield.setScreen(new FileBrowser("/logs"));                   // Browse specific directory
    // M1Shield.setScreen(new FileBrowser("/logs", "error.log"));      // Browse directory with file selection
    // M1Shield.setScreen(new FileBrowser("/logs/system.log"));        // Auto-parse file path (navigate to /logs, select system.log)
    // M1Shield.setScreen(new FileBrowser("/data", "config.txt", true)); // Restricted browsing

    M1Shield.setScreen(new FileBrowser());

    Serial.println("FileBrowser loaded and ready!");
    Serial.println();
    Serial.println("Controls:");
    Serial.println("- UP/DOWN: Navigate through files and directories");
    Serial.println("- RIGHT/MENU: Select file or enter directory");
    Serial.println("- LEFT: Go back to parent directory");
    Serial.println();
    Serial.println("File Types:");
    Serial.println("- .txt, .log files: Open with TextFileViewer");
    Serial.println("- Other files: Open with BinaryFileViewer (hex dump)");
    Serial.println("- Directories: Navigate into them");
    Serial.println("- '..': Go to parent directory");
}

void loop()
{
    // Handle all M1Shield operations
    M1Shield.loop();
}
