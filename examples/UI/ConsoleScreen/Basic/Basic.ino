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

#include "M1Shield.h"
#include "ConsoleScreen.h"

class DebugConsole : public ConsoleScreen
{
private:
    unsigned long _lastUpdate;
    int _counter;

public:
    DebugConsole()
    {
        // Configure the console appearance
        _setTitle("Debug Console");
        setConsoleBackground(0x0000);
        setTextColor(0x07E0, 0x0000);
        setTextSize(1);
        setTabSize(8);

        // Clear screen and show initial message
        cls();

        // Explain the auto-clear behavior
        setTextColor(0xFFE0, 0x0000);
        println("=== CONSOLE AUTO-CLEAR DEMO ===");
        setTextColor(0x07FF, 0x0000);
        println("Console automatically clears when");
        println("text reaches the bottom of screen.");
        println("This saves memory and provides");
        println("continuous scrolling effect.");
        setTextColor(0xFFFF, 0x0000);
        println();

        // Demonstrate tab formatting
        println("Column1\tColumn2\tColumn3");
        println("Data1\tData2\tData3");
        println("Info1\tInfo2\tInfo3");
        println();

        setTextColor(0xF81F, 0x0000);
        println("Watch console auto-clear when full!");
        setTextColor(0xFFFF, 0x0000);
        println();

        // Initialize timing
        _lastUpdate = millis();
        _counter = 0;
    }

    void loop() override
    {
        // Update console content periodically
        if (millis() - _lastUpdate > 1500)
        { // Every 1.5 seconds
            _lastUpdate = millis();
            _counter++;

            // Add timestamp
            setTextColor(0x07FF, 0x0000);
            print("[");
            print(millis() / 1000);
            print("s] ");

            // Add counter and status
            setTextColor(0xFFFF, 0x0000);
            print("Line #");
            print(_counter);
            print("\tStatus: ");

            // Alternate between different status messages
            if (_counter % 4 == 0)
            {
                setTextColor(0x07E0, 0x0000);
                println("RUNNING");
            }
            else if (_counter % 4 == 1)
            {
                setTextColor(0xFFE0, 0x0000);
                println("WARNING");
            }
            else if (_counter % 4 == 2)
            {
                setTextColor(0xF800, 0x0000);
                println("ERROR");
            }
            else
            {
                setTextColor(0x001F, 0x0000);
                println("IDLE");
            }

            // Add some sample data
            setTextColor(0xFFFF, 0x0000);
            print("Temp: ");
            print(20.0 + (_counter % 15), 1);
            print("°C\tMem: ");
            print(1024 - (_counter * 8));
            println(" bytes");

            // Show how many lines until auto-clear
            if (_counter % 5 == 0)
            {
                setTextColor(0xaaaa, 0x0000);
                println("(Console will auto-clear soon...)");
                setTextColor(0xFFFF, 0x0000);
            }

            println(); // Empty line for spacing

            // Note: Console will automatically clear when it reaches the bottom!
            // No manual clearing needed - this demonstrates the memory-efficient behavior
        }

        // Call parent loop for standard screen processing
        ConsoleScreen::loop();
    }

    Screen *actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY) override
    {
        // Handle manual clear on joystick press
        if (action & BUTTON_JOYSTICK)
        {
            setTextColor(0x07FF, 0x0000);
            println(">> Manual clear requested by user");
            delay(500);
            cls();
            setTextColor(0xFFE0, 0x0000);
            println("=== MANUAL CLEAR (same as auto-clear) ===");
            setTextColor(0xFFFF, 0x0000);
            println("Console cleared! Will auto-clear again");
            println("when it reaches the bottom.");
            println();
            return nullptr;
        }

        // Demonstrate different text sizes on UP/DOWN
        if (action & BUTTON_UP)
        {
            setTextColor(0xF81F, 0x0000);
            println(">> Switching to large text size");
            setTextSize(2);
            println("BIG TEXT!");
            setTextSize(1); // Back to normal
            setTextColor(0xFFFF, 0x0000);
            return nullptr;
        }

        if (action & BUTTON_DOWN)
        {
            setTextColor(0xFC00, 0x0000);
            println(">> Adding separator line");
            println("--------------------------------");
            setTextColor(0xFFFF, 0x0000);
            return nullptr;
        }

        // Let parent handle other actions (like back button)
        return ConsoleScreen::actionTaken(action, offsetX, offsetY);
    }
};

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
