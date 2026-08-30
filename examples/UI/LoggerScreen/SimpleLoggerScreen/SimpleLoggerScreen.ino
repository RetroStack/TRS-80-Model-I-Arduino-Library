/*
 * SimpleLoggerScreen.ino - The smallest useful LoggerScreen
 *
 * A LoggerScreen is an ILogger that draws to the panel instead of a serial
 * port, so anything that takes a logger can write to the screen. This shows
 * the whole of it: construct one, hand it to M1Shield, log.
 *
 * See ComprehensiveLoggerScreen for the rotational buffer, colour coding,
 * timestamps and replay-on-activate.
 *
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#include <M1Shield.h>
#include <LoggerScreen.h>

// First, tell the system which display you have.
// Uncomment exactly one and comment out the rest.

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

void setup()
{
    Serial.begin(115200);

    M1Shield.begin(displayProvider);

    // Screens are created with new, never as globals: a global constructor
    // runs before begin(), so there would be no display to measure yet.
    LoggerScreen *logger = new LoggerScreen("Simple Logger");
    M1Shield.setScreen(logger);

    // The four levels. On a colour panel each gets its own colour; on a
    // monochrome one they are told apart by the level label.
    logger->info("Screen ready");
    logger->warn("This is a warning");
    logger->err("This is an error");
    logger->debug("This is a debug line");

    // F() keeps the text in flash rather than SRAM, which matters on a part
    // with 8KB of it.
    logger->infoF(F("Free to log from anywhere"));
}

void loop()
{
    // Drives input and redraw. Nothing reaches the screen without it.
    M1Shield.loop();
}
