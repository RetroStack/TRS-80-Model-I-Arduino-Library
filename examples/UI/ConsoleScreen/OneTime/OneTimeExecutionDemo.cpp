#include "OneTimeExecutionDemo.h"
#include <Arduino.h>

void OneTimeExecutionDemo::_executeOnce()
{
    // This runs exactly once, 1 second after console opens
    Serial.println("=== _executeOnce() called! ===");
    Serial.println("Running one-time console initialization...");

    // Clear console before initialization
    cls();

    // Show initialization process
    setTextColor(0xFFE0, 0x0000);
    println(">>> SYSTEM INITIALIZATION <<<");
    println();

    setTextColor(0x07FF, 0x0000);
    println("Loading configuration...");
    delay(200); // Simulate work

    println("Checking hardware...");
    delay(200); // Simulate work

    println("Starting services...");
    delay(200); // Simulate work

    // Show completion
    setTextColor(0x07E0, 0x0000);
    println("*** INITIALIZATION COMPLETE ***");
    println();

    setTextColor(0xFFFF, 0x0000);
    println("Console ready for operation!");
    println("Watch continuous updates below:");
    println("--------------------------------");

    _systemInitialized = true;
    Serial.println("One-time initialization completed!");
    Serial.println("Now loop() will run continuously...");
}

OneTimeExecutionDemo::OneTimeExecutionDemo() : ConsoleScreen()
{
    _setTitle("One-Time Exec");

    // Set initial colors and state
    setTextColor(0xFFFF, 0x0000);
    setConsoleBackground(0x0000);
    setTextSize(1);

    _systemInitialized = false;
    _lastTimeUpdate = 0;
    _messageCounter = 0;

    Serial.println("Console created - _executeOnce() will run in 1 second");
}

void OneTimeExecutionDemo::loop()
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
        setTextColor(0x07FF, 0x0000);
        print("[");
        print(millis() / 1000);
        print("s] ");

        // Rotate through different types of messages
        switch (_messageCounter % 4)
        {
        case 0:
            setTextColor(0x07E0, 0x0000);
            print("STATUS: ");
            setTextColor(0xFFFF, 0x0000);
            println("System operational");
            break;

        case 1:
            setTextColor(0xFFE0, 0x0000);
            print("TEMP: ");
            setTextColor(0xFFFF, 0x0000);
            print(20.0 + (_messageCounter % 10), 1);
            println("°C");
            break;

        case 2:
            setTextColor(0xF81F, 0x0000);
            print("MSG #");
            print(_messageCounter);
            print(": ");
            setTextColor(0xFFFF, 0x0000);
            println("Auto-clear demo running");
            break;

        case 3:
            setTextColor(0x001F, 0x0000);
            print("MEM: ");
            setTextColor(0xFFFF, 0x0000);
            print(2048 - (_messageCounter * 16));
            println(" bytes free");
            break;
        }

        // Add separator every 10 messages
        if (_messageCounter % 10 == 0)
        {
            setTextColor(0xaaa, 0x0000);
            println("--- Console will auto-clear when full ---");
        }
    }
}

Screen *OneTimeExecutionDemo::actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY)
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

        setTextColor(0x07FF, 0x0000);
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
