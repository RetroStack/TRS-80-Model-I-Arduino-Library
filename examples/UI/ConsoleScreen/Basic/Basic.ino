/*
 * ConsoleScreen Example - Demonstrates console functionality with auto-clear
 *
 * This example shows how ConsoleScreen provides terminal-like output with
 * automatic screen clearing when the bottom is reached (wrap-around behavior).
 * Features: scrolling, colors, tabs, various print methods, and memory-efficient auto-clear.
 */

// First, tell the system which display you have
#include <Display_ST7789.h>
Display_ST7789 displayProvider;

// #include <Display_ST7789_240x240.h>
// Display_ST7789_240x240 displayProvider;

// #include <Display_ST7735.h>
// Display_ST7735 displayProvider;

// #include <Display_ST7796.h>
// Display_ST7796 displayProvider;

// #include <Display_ILI9325.h>
// Display_ILI9325 displayProvider;

// #include <Display_ILI9341.h>
// Display_ILI9341 displayProvider;

// #include <Display_HX8357.h>
// Display_HX8357 displayProvider;

#include <M1Shield.h>
#include "DebugConsole.h"

// Usage in main Arduino sketch:
void setup()
{
    M1Shield.begin(displayProvider);

    DebugConsole *console = new DebugConsole();
    M1Shield.setScreen(console);
}

void loop()
{
    M1Shield.loop();
}
