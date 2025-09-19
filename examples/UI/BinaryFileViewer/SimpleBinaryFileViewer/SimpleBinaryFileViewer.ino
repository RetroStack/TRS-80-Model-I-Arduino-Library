#include <M1Shield.h>
#include <BinaryFileViewer.h>

// ============================================
// Display Provider Configuration
// ============================================
// Uncomment ONE of the following display configurations

// ST7789 240x240 (Round displays)
// #include <Display_ST7789_240x240.h>
// Display_ST7789_240x240 displayProvider;

// ST7789 320x240 (Rectangular displays)
#include <Display_ST7789_320x240.h>
Display_ST7789_320x240 displayProvider;

// ST7735 128x160 (Small TFT displays)
// #include <Display_ST7735.h>
// Display_ST7735 displayProvider;

// ILI9341 240x320 (Common TFT displays)
// #include <Display_ILI9341.h>
// Display_ILI9341 displayProvider;

// HX8357 320x480 (Large TFT displays)
// #include <Display_HX8357.h>
// Display_HX8357 displayProvider;

// ============================================
// Example Application
// ============================================

void setup()
{
    Serial.begin(115200);

    // Initialize the M1Shield with display
    M1Shield.begin(displayProvider);

    // Create a binary file viewer
    // Note: The file "firmware.bin" should exist on your SD card
    //
    // TIP: For browsing files instead of viewing a specific file,
    // consider using FileBrowser which provides directory navigation:
    // M1Shield.setScreen(new FileBrowser("/path/to/your/file.bin"));
    BinaryFileViewer *viewer = new BinaryFileViewer("firmware.bin");

    // Optional: Set custom title
    viewer->setTitleF(F("Firmware Hex Dump"));

    // Set the screen
    M1Shield.setScreen(viewer);

    Serial.println(F("Binary File Viewer ready!"));
    Serial.println(F("Controls:"));
    Serial.println(F("- UP: Previous page"));
    Serial.println(F("- DOWN: Next page"));
    Serial.println(F("- LEFT: First page"));
    Serial.println(F("- RIGHT: Last page"));
}

void loop()
{
    M1Shield.loop();
}
