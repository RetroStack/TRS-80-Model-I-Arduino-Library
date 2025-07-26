/*
 * ConsoleScreen Example - Demonstrates console functionality with auto-clear
 *
 * This example shows how ConsoleScreen provides terminal-like output with
 * automatic screen clearing when the bottom is reached (wrap-around behavior).
 * Features: scrolling, colors, tabs, various print methods, and memory-efficient auto-clear.
 */

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
        _setTitle("Debug Console - Auto-Clear Demo");
        setConsoleBackground(ST77XX_BLACK);
        setTextColor(ST77XX_GREEN, ST77XX_BLACK);
        setTextSize(1);
        setTabSize(8);

        // Clear screen and show initial message
        cls();

        // Explain the auto-clear behavior
        setTextColor(ST77XX_YELLOW, ST77XX_BLACK);
        println("=== CONSOLE AUTO-CLEAR DEMO ===");
        setTextColor(ST77XX_CYAN, ST77XX_BLACK);
        println("Console automatically clears when");
        println("text reaches the bottom of screen.");
        println("This saves memory and provides");
        println("continuous scrolling effect.");
        setTextColor(ST77XX_WHITE, ST77XX_BLACK);
        println();

        // Demonstrate tab formatting
        println("Column1\tColumn2\tColumn3");
        println("Data1\tData2\tData3");
        println("Info1\tInfo2\tInfo3");
        println();

        setTextColor(ST77XX_MAGENTA, ST77XX_BLACK);
        println("Watch console auto-clear when full!");
        setTextColor(ST77XX_WHITE, ST77XX_BLACK);
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
            setTextColor(ST77XX_CYAN, ST77XX_BLACK);
            print("[");
            print(millis() / 1000);
            print("s] ");

            // Add counter and status
            setTextColor(ST77XX_WHITE, ST77XX_BLACK);
            print("Line #");
            print(_counter);
            print("\tStatus: ");

            // Alternate between different status messages
            if (_counter % 4 == 0)
            {
                setTextColor(ST77XX_GREEN, ST77XX_BLACK);
                println("RUNNING");
            }
            else if (_counter % 4 == 1)
            {
                setTextColor(ST77XX_YELLOW, ST77XX_BLACK);
                println("WARNING");
            }
            else if (_counter % 4 == 2)
            {
                setTextColor(ST77XX_RED, ST77XX_BLACK);
                println("ERROR");
            }
            else
            {
                setTextColor(ST77XX_BLUE, ST77XX_BLACK);
                println("IDLE");
            }

            // Add some sample data
            setTextColor(ST77XX_WHITE, ST77XX_BLACK);
            print("Temp: ");
            print(20.0 + (_counter % 15), 1);
            print("°C\tMem: ");
            print(1024 - (_counter * 8));
            println(" bytes");

            // Show how many lines until auto-clear
            if (_counter % 5 == 0)
            {
                setTextColor(ST77XX_GRAY, ST77XX_BLACK);
                println("(Console will auto-clear soon...)");
                setTextColor(ST77XX_WHITE, ST77XX_BLACK);
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
            setTextColor(ST77XX_CYAN, ST77XX_BLACK);
            println(">> Manual clear requested by user");
            delay(500);
            cls();
            setTextColor(ST77XX_YELLOW, ST77XX_BLACK);
            println("=== MANUAL CLEAR (same as auto-clear) ===");
            setTextColor(ST77XX_WHITE, ST77XX_BLACK);
            println("Console cleared! Will auto-clear again");
            println("when it reaches the bottom.");
            println();
            return nullptr;
        }

        // Demonstrate different text sizes on UP/DOWN
        if (action & BUTTON_UP)
        {
            setTextColor(ST77XX_MAGENTA, ST77XX_BLACK);
            println(">> Switching to large text size");
            setTextSize(2);
            println("BIG TEXT!");
            setTextSize(1); // Back to normal
            setTextColor(ST77XX_WHITE, ST77XX_BLACK);
            return nullptr;
        }

        if (action & BUTTON_DOWN)
        {
            setTextColor(ST77XX_ORANGE, ST77XX_BLACK);
            println(">> Adding separator line");
            println("--------------------------------");
            setTextColor(ST77XX_WHITE, ST77XX_BLACK);
            return nullptr;
        }

        // Let parent handle other actions (like back button)
        return ConsoleScreen::actionTaken(action, offsetX, offsetY);
    }
};

// Usage in main Arduino sketch:
/*
void setup() {
    M1Shield.begin();

    DebugConsole* console = new DebugConsole();
    M1Shield.setScreen(console);
}

void loop() {
    M1Shield.loop();
}
*/
