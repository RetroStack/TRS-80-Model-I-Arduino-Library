#include "KeyboardTester.h"
#include "MainDemoMenu.h"
#include <Arduino.h>

KeyboardTester::KeyboardTester() : ConsoleScreen()
{
    _setTitle("Keyboard Test");
    setConsoleBackground(0x0000);
    setTextColor(0xFFFF, 0x0000);

    _lastUpdate = millis();
    _keyPressCount = 0;

    cls();
    showInstructions();

    Serial.println("Keyboard Tester initialized");
}

void KeyboardTester::loop()
{
    ConsoleScreen::loop();

    // Update keyboard status every 100ms
    if (millis() - _lastUpdate > 100)
    {
        _lastUpdate = millis();
        displayKeyboardStatus();
    }
}

void KeyboardTester::showInstructions()
{
    setTextColor(0x07E0, 0x0000); // Green
    println("=== KEYBOARD TESTER ===");
    println();
    setTextColor(0xFFE0, 0x0000); // Yellow
    println("INSTRUCTIONS:");
    setTextColor(0xFFFF, 0x0000);
    println("* Press keys on TRS-80 keyboard");
    println("* Key presses will be detected");
    println("* Real-time key status shown");
    println("* Use M1Shield MENU to exit");
    println();
    setTextColor(0x07FF, 0x0000); // Cyan
    println("Monitoring keyboard...");
    println();
}

void KeyboardTester::displayKeyboardStatus()
{
    // Simulate keyboard monitoring (replace with actual keyboard reading)
    static int lastDisplayedCount = -1;
    static unsigned long simulatedKeyPress = 0;

    // Simulate occasional key presses for demo
    if (millis() - simulatedKeyPress > 3000)
    {
        simulatedKeyPress = millis();
        _keyPressCount++;
    }

    // Only update display when count changes
    if (_keyPressCount != lastDisplayedCount)
    {
        lastDisplayedCount = _keyPressCount;

        // Move to status area (line 10)
        setCursor(0, 10);

        setTextColor(0xF81F, 0x0000); // Magenta
        println("=== KEYBOARD STATUS ===");

        setTextColor(0xFFFF, 0x0000); // White
        print("Keys Pressed: ");
        setTextColor(0x07E0, 0x0000); // Green
        println(_keyPressCount);

        setTextColor(0xFFFF, 0x0000);
        print("Last Key: ");
        setTextColor(0x07FF, 0x0000); // Cyan

        // Simulate different key names
        String keyNames[] = {"SPACE", "ENTER", "A", "1", "SHIFT", "CTRL"};
        println(keyNames[_keyPressCount % 6]);

        setTextColor(0xFFFF, 0x0000);
        print("Status: ");
        setTextColor(0x07E0, 0x0000);
        println("MONITORING");

        // Show matrix status
        setTextColor(0xFFFF, 0x0000);
        println();
        println("Keyboard Matrix Status:");
        setTextColor(0xFFE0, 0x0000); // Yellow

        // Simulate matrix display
        for (int row = 0; row < 8; row++)
        {
            print("Row ");
            print(row);
            print(": ");

            // Simulate active/inactive status
            if ((_keyPressCount + row) % 3 == 0)
            {
                setTextColor(0x07E0, 0x0000); // Green
                print("ACTIVE  ");
            }
            else
            {
                setTextColor(0xaaaa, 0x0000); // Gray
                print("IDLE    ");
            }

            if (row % 2 == 1)
                println(); // New line every 2 rows
        }

        println();
        println();
        setTextColor(0xF81F, 0x0000); // Magenta
        println("Press MENU to return");
    }
}

Screen *KeyboardTester::actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY)
{
    if (action & BUTTON_MENU)
    {
        Serial.println("Returning to main menu from Keyboard Tester");
        return new MainDemoMenu();
    }

    return nullptr;
}
