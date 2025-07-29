/*
 * ConsoleScreen Example - Demonstrates console functionality with auto-clear
 *
 * This example shows how ConsoleScreen provides terminal-like output with
 * automatic screen clearing when the bottom is reached (wrap-around behavior).
 * Features: scrolling, colors, tabs, various print methods, and memory-efficient auto-clear.
 */

// First, tell the system which display you have
// For ST7789 240x320 displays (most common, landscape becomes 320x240)
#include <Display_ST7789_320x240.h>
Display_ST7789_320x240 displayProvider;

// For ST7789 240x240 square displays
// #include <Display_ST7789_240x240.h>
// Display_ST7789_240x240 displayProvider;

// For ST7789 320x170 wide displays (landscape)
// #include <Display_ST7789_320x170.h>
// Display_ST7789_320x170 displayProvider;

// For ST7789 320x240 alternative displays (landscape)
// #include <Display_ST7789_320x240.h>
// Display_ST7789_320x240 displayProvider;

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

#include <M1Shield.h>
#include "DebugConsole.h"

// Usage in main Arduino sketch:
void setup()
{
    M1Shield.begin(displayProvider);

    // Optional: Enable joystick input for console navigation
    // Uncomment the next line to allow joystick control
    // M1Shield.activateJoystick();

    DebugConsole *console = new DebugConsole();
    M1Shield.setScreen(console);
}

void loop()
{
    M1Shield.loop();
}
