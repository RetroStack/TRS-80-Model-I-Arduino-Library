/*
 * ===================================================================
 * ConsoleScreen One-Time Execution Example
 * ===================================================================
 *
 * This example demonstrates the _executeOnce() method in ConsoleScreen!
 *
 * WHAT'S ONE-TIME EXECUTION:
 * - _executeOnce() - called exactly once, 1 second after console opens
 * - Perfect for initialization, data loading, or setup tasks
 * - Avoids the continuous execution overhead of loop() for one-time tasks
 * - Works alongside the existing auto-clear wrap-around behavior
 *
 * BEHAVIOR DEMONSTRATION:
 * 1. Console opens with "Initializing console..." message
 * 2. After 1 second: _executeOnce() runs automatically
 *    - Shows system initialization messages
 *    - Changes colors and displays status
 * 3. Then: loop() runs continuously for ongoing updates
 *    - Shows current time and status
 *    - Demonstrates auto-clear when console fills up
 *
 * HOW TO TEST:
 * - Upload and watch Serial Monitor for detailed logging
 * - Watch continuous time updates and auto-clear behavior
 * - Press joystick to restart and see _executeOnce() run again
 */

#include "M1Shield.h"
#include "ConsoleScreen.h"

class OneTimeExecutionDemo : public ConsoleScreen
{
private:
    bool _systemInitialized;
    unsigned long _lastTimeUpdate;
    int _messageCounter;

protected:
    void _executeOnce() override
    {
        // This runs exactly once, 1 second after console opens
        Serial.println("=== _executeOnce() called! ===");
        Serial.println("Running one-time console initialization...");

        // Clear console before initialization
        cls();

        // Show initialization process
        setTextColor(ST77XX_YELLOW, ST77XX_BLACK);
        println(">>> SYSTEM INITIALIZATION <<<");
        println();

        setTextColor(ST77XX_CYAN, ST77XX_BLACK);
        println("Loading configuration...");
        delay(200); // Simulate work

        println("Checking hardware...");
        delay(200); // Simulate work

        println("Starting services...");
        delay(200); // Simulate work

        // Show completion
        setTextColor(ST77XX_GREEN, ST77XX_BLACK);
        println("*** INITIALIZATION COMPLETE ***");
        println();

        setTextColor(ST77XX_WHITE, ST77XX_BLACK);
        println("Console ready for operation!");
        println("Watch continuous updates below:");
        println("--------------------------------");

        _systemInitialized = true;
        Serial.println("One-time initialization completed!");
        Serial.println("Now loop() will run continuously...");
    }

public:
    OneTimeExecutionDemo() : ConsoleScreen()
    {
        _setTitle("One-Time Exec");

        // Set initial colors and state
        setTextColor(ST77XX_WHITE, ST77XX_BLACK);
        setConsoleBackground(ST77XX_BLACK);
        setTextSize(1);

        _systemInitialized = false;
        _lastTimeUpdate = 0;
        _messageCounter = 0;

        Serial.println("Console created - _executeOnce() will run in 1 second");
    }

    void loop() override
    {
        // Call parent to handle one-time execution
        ConsoleScreen::loop();

        // Only show updates after initialization
        if (!_systemInitialized)
        {
            return;
        }

        // Update every 2 seconds
        if (millis() - _lastTimeUpdate > 2000)
        {
            _lastTimeUpdate = millis();
            _messageCounter++;

            // Show timestamp
            setTextColor(ST77XX_CYAN, ST77XX_BLACK);
            print("[");
            print(millis() / 1000);
            print("s] ");

            // Rotate through different types of messages
            switch (_messageCounter % 4)
            {
            case 0:
                setTextColor(ST77XX_GREEN, ST77XX_BLACK);
                print("STATUS: ");
                setTextColor(ST77XX_WHITE, ST77XX_BLACK);
                println("System operational");
                break;

            case 1:
                setTextColor(ST77XX_YELLOW, ST77XX_BLACK);
                print("TEMP: ");
                setTextColor(ST77XX_WHITE, ST77XX_BLACK);
                print(20.0 + (_messageCounter % 10), 1);
                println("°C");
                break;

            case 2:
                setTextColor(ST77XX_MAGENTA, ST77XX_BLACK);
                print("MSG #");
                print(_messageCounter);
                print(": ");
                setTextColor(ST77XX_WHITE, ST77XX_BLACK);
                println("Auto-clear demo running");
                break;

            case 3:
                setTextColor(ST77XX_BLUE, ST77XX_BLACK);
                print("MEM: ");
                setTextColor(ST77XX_WHITE, ST77XX_BLACK);
                print(2048 - (_messageCounter * 16));
                println(" bytes free");
                break;
            }

            // Add separator every 10 messages
            if (_messageCounter % 10 == 0)
            {
                setTextColor(0xaaa, ST77XX_BLACK);
                println("--- Console will auto-clear when full ---");
            }
        }
    }

    Screen *actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY) override
    {
        if (action & BUTTON_MENU)
        {
            Serial.println("Returning to previous screen...");
            return nullptr; // Will not actually do anything
        }

        if (action & BUTTON_JOYSTICK)
        {
            Serial.println("Restarting console demo...");

            // Reset state for demo restart
            _systemInitialized = false;
            _messageCounter = 0;

            setTextColor(ST77XX_CYAN, ST77XX_BLACK);
            cls();
            println("Restarting console...");
            println("_executeOnce() will run again!");

            // Reopen to restart timing
            close();
            open();

            return nullptr; // Stay on this screen
        }

        return nullptr; // No navigation
    }
};

void setup()
{
    Serial.begin(115200);
    M1Shield.begin();

    Serial.println("=== ConsoleScreen One-Time Execution Demo ===");
    Serial.println("Watch the console for initialization sequence!");

    M1Shield.setScreen(new OneTimeExecutionDemo());
}

void loop()
{
    M1Shield.loop();
}
