/*
 * SimpleConsole.cpp - Basic ConsoleScreen Implementation
 *
 * This example demonstrates basic ConsoleScreen functionality including
 * text output, colors, scrolling, and simple user interaction.
 */

#include "SimpleConsole.h"
#include <M1Shield.h>

SimpleConsole::SimpleConsole() : ConsoleScreen()
{
    lastUpdate = 0;
    messageCount = 0;
    showingWelcome = true;

    // Configure console appearance - these are safe to set in constructor
    setTitleF(F("Simple Console"));
    // Note: Do not call display operations here - use _executeOnce() instead
}

void SimpleConsole::_executeOnce()
{
    // This method is called automatically 1 second after the screen opens
    setConsoleBackground(M1Shield.convertColor(0x0000)); // Black background
    setTextColor(M1Shield.convertColor(0xFFFF));         // White text
    setTextSize(1);
    setTabSize(4);

    // It's the proper place for initial display operations
    cls();
    showWelcomeMessage();
}

void SimpleConsole::showWelcomeMessage()
{
    // Welcome header
    setTextColor(M1Shield.convertColor(0x07E0)); // Green
    println(F("=== Simple Console Demo ==="));
    println();

    setTextColor(M1Shield.convertColor(0xFFFF)); // White
    println(F("This example demonstrates:"));
    println(F("• Text output with colors"));
    println(F("• Automatic scrolling"));
    println(F("• Real-time status updates"));
    println();

    showControls();

    setTextColor(M1Shield.convertColor(0xFFE0)); // Yellow
    println(F("Status messages will appear below:"));
    println();
}

void SimpleConsole::showControls()
{
    setTextColor(M1Shield.convertColor(0x07FF)); // Cyan
    println(F("Controls:"));
    setTextColor(M1Shield.convertColor(0xBDF7)); // Light gray
    println(F("  MENU - Clear screen"));
    println(F("  LEFT - Show welcome"));
    println(F("  RIGHT - Add test message"));
    println();
}

void SimpleConsole::loop()
{
    // Call parent ConsoleScreen::loop() to handle auto-forward and notifications
    ConsoleScreen::loop();

    // Add automatic status messages every 3 seconds
    if (millis() - lastUpdate >= 3000)
    {
        lastUpdate = millis();
        addStatusMessage();
    }
}

void SimpleConsole::addStatusMessage()
{
    messageCount++;

    // Timestamp
    setTextColor(M1Shield.convertColor(0x8410)); // Gray
    print(F("["));
    print(millis() / 1000);
    print(F("s] "));

    // Message with varying colors
    switch (messageCount % 4)
    {
    case 0:
        setTextColor(M1Shield.convertColor(0x07E0)); // Green
        print(F("INFO: "));
        setTextColor(M1Shield.convertColor(0xFFFF)); // White
        println(F("System running normally"));
        break;

    case 1:
        setTextColor(M1Shield.convertColor(0xFFE0)); // Yellow
        print(F("WARN: "));
        setTextColor(M1Shield.convertColor(0xFFFF)); // White
        println(F("Memory usage at 75%"));
        break;

    case 2:
        setTextColor(M1Shield.convertColor(0x001F)); // Blue
        print(F("DEBUG: "));
        setTextColor(M1Shield.convertColor(0xFFFF)); // White
        print(F("Message #"));
        println(messageCount);
        break;

    case 3:
        setTextColor(M1Shield.convertColor(0xF800)); // Red
        print(F("ERROR: "));
        setTextColor(M1Shield.convertColor(0xFFFF)); // White
        println(F("Simulated error condition"));
        break;
    }

    // Add some sample data
    setTextColor(M1Shield.convertColor(0xBDF7)); // Light gray
    print(F("  Temp: "));
    print(20.0 + (messageCount % 10), 1);
    print(F("°C, Uptime: "));
    print(millis() / 1000);
    println(F("s"));
}

Screen *SimpleConsole::actionTaken(ActionTaken action, int8_t offsetX, int8_t offsetY)
{
    if (action & BUTTON_MENU)
    {
        // Clear screen and show welcome message
        cls();
        showWelcomeMessage();
        messageCount = 0;
        return nullptr;
    }

    if (action & BUTTON_LEFT)
    {
        // Show welcome message again
        setTextColor(M1Shield.convertColor(0x07FF)); // Cyan
        println();
        println(F("--- Welcome Message ---"));
        showWelcomeMessage();
        return nullptr;
    }

    if (action & BUTTON_RIGHT)
    {
        // Add a test message immediately
        setTextColor(M1Shield.convertColor(0xF81F)); // Magenta
        print(F("USER: "));
        setTextColor(M1Shield.convertColor(0xFFFF)); // White
        println(F("Manual test message triggered"));
        return nullptr;
    }

    if (action & BUTTON_UP)
    {
        // Show current status
        setTextColor(M1Shield.convertColor(0x07FF)); // Cyan
        print(F("STATUS: "));
        setTextColor(M1Shield.convertColor(0xFFFF)); // White
        print(F("Messages: "));
        print(messageCount);
        print(F(", Uptime: "));
        print(millis() / 1000);
        println(F("s"));
        return nullptr;
    }

    if (action & BUTTON_DOWN)
    {
        // Show memory info
        setTextColor(M1Shield.convertColor(0xFFE0)); // Yellow
        print(F("MEMORY: "));
        setTextColor(M1Shield.convertColor(0xFFFF)); // White
        println(F("Simulated memory status OK"));
        return nullptr;
    }

    return nullptr;
}
