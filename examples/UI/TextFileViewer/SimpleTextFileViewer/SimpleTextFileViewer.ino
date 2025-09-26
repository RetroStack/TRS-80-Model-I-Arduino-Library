/*
 * Simple Text File Viewer Example
 *
 * This example demonstrates the TextFileViewer class for displaying text files
 * from SD card with paging, scrolling, and auto-update capabilities.
 *
 * Hardware Required:
 * - Arduino Mega 2560
 * - M1Shield with display
 * - SD card with text files
 */

#include <M1Shield.h>
#include <TextFileViewer.h>

// Uncomment one of these display provider headers based on your hardware
// #include <Display_ST7789_240x240.h>
// #include <Display_ST7789_320x170.h>
// #include <Display_ST7789_320x240.h>
// #include <Display_ST7735.h>
// #include <Display_ILI9341.h>
// #include <Display_HX8357.h>
// #include <Display_ILI9325.h>
// #include <Display_ST7796.h>
// #include <Display_SSD1306.h>
#include <Display_SH1106.h>

// Uncomment one of these display provider instances based on your hardware
// Display_ST7789_240x240 displayProvider;
// Display_ST7789_320x170 displayProvider;
// Display_ST7789_320x240 displayProvider;
// Display_ST7735 displayProvider;
// Display_ILI9341 displayProvider;
// Display_HX8357 displayProvider;
// Display_ILI9325 displayProvider;
// Display_ST7796 displayProvider;
// Display_SSD1306 displayProvider;
Display_SH1106 displayProvider;

void setup()
{
    Serial.begin(115200);

    // Initialize M1Shield
    if (!M1Shield.begin(displayProvider))
    {
        Serial.println(F("Failed to initialize M1Shield"));
        while (1)
            delay(100);
    }

    Serial.println(F("M1Shield initialized"));

    // Create and configure text file viewer
    // Change "readme.txt" to your desired file on SD card
    // 
    // TIP: For browsing files instead of viewing a specific file, 
    // consider using FileBrowser which provides directory navigation:
    // M1Shield.setScreen(new FileBrowser("/path/to/your/file.txt"));
    TextFileViewer *viewer = new TextFileViewer("readme.txt");

    // Configure appearance
    viewer->setTextColor(M1Shield.convertColor(0xFFFF));       // White text
    viewer->setBackgroundColor(M1Shield.convertColor(0x0000)); // Black background
    viewer->setTextSize(1);                                    // Size 1 text

    // Enable auto-paging for log files (checks for updates every second)
    viewer->setAutoPaging(false); // Set to true for log files that are being appended

    // Set title
    viewer->setTitleF(F("Text File Viewer"));

    // Activate the screen
    M1Shield.setScreen(viewer);

    Serial.println(F("Text File Viewer ready!"));
    Serial.println(F("Controls:"));
    Serial.println(F("- UP/DOWN buttons or joystick: Navigate pages"));
    Serial.println(F("- LEFT/RIGHT buttons or joystick: Scroll horizontally"));
    Serial.println(F("- SELECT button: Toggle auto-paging"));
}

void loop()
{
    // M1Shield handles the screen updates automatically
    delay(50);
}
