#include <Arduino.h>
#include <Model1.h>
#include <Keyboard.h>

// Keyboard class instance for TRS-80 keyboard input
Keyboard keyboard;

void setup()
{
    // Initialize serial communication
    Serial.begin(9600);
    delay(1000);

    // Initialize the Model 1 hardware interface
    Model1.begin();

    // Take control of the TRS-80 bus
    Model1.activateTestSignal();

    // Initialize keyboard state
    keyboard.update();

    Serial.println(F("=== Simple Keyboard Example ==="));
    Serial.println(F("Basic keyboard input demonstration"));
    Serial.println(F("Press keys on the TRS-80 keyboard..."));
    Serial.println();
}

void loop()
{
    // Method 1: Simple key detection
    demonstrateSimpleKeyDetection();

    // Method 2: Change iterator approach
    demonstrateChangeIterator();

    // Small delay to prevent overwhelming the serial output
    delay(50);
}

void demonstrateSimpleKeyDetection()
{
    // Check for any key that was just pressed
    uint8_t pressedKey = keyboard.getFirstJustPressedKey();

    if (pressedKey != 0)
    {
        Serial.print(F("Key pressed (simple): "));

        // Display the character if it's printable
        if (pressedKey >= 32 && pressedKey <= 126)
        {
            Serial.print(F("'"));
            Serial.print((char)pressedKey);
            Serial.print(F("' "));
        }

        // Always show the hex value
        Serial.print(F("(0x"));
        if (pressedKey < 0x10)
            Serial.print('0');
        Serial.print(pressedKey, HEX);
        Serial.println(F(")"));

        // Provide helpful information for special keys
        interpretSpecialKey(pressedKey);
    }
}

void demonstrateChangeIterator()
{
    // Get all keyboard changes since the last scan
    KeyboardChangeIterator it = keyboard.changes();

    // Process all changes
    while (it.hasNext())
    {
        if (it.wasJustPressed())
        {
            uint8_t keyValue = it.keyValue();

            Serial.print(F("Key pressed (iterator): "));

            // Display the character if it's printable
            if (keyValue >= 32 && keyValue <= 126)
            {
                Serial.print(F("'"));
                Serial.print((char)keyValue);
                Serial.print(F("' "));
            }

            // Always show the hex value
            Serial.print(F("(0x"));
            if (keyValue < 0x10)
                Serial.print('0');
            Serial.print(keyValue, HEX);
            Serial.println(F(")"));

            // Provide helpful information for special keys
            interpretSpecialKey(keyValue);
        }

        // Move to the next change
        it.next();
    }
}

void interpretSpecialKey(uint8_t keyCode)
{
    // Provide helpful interpretation of special TRS-80 keys
    switch (keyCode)
    {
    case 0x0D:
        Serial.println(F("  -> ENTER key"));
        break;
    case 0x08:
        Serial.println(F("  -> BACKSPACE key"));
        break;
    case 0x20:
        Serial.println(F("  -> SPACE key"));
        break;
    case 0x1B:
        Serial.println(F("  -> BREAK key"));
        break;
    case 0x09:
        Serial.println(F("  -> TAB key"));
        break;
    case 0x0C:
        Serial.println(F("  -> CLEAR key"));
        break;
    case 0x1E:
        Serial.println(F("  -> UP ARROW"));
        break;
    case 0x1F:
        Serial.println(F("  -> DOWN ARROW"));
        break;
    case 0x1C:
        Serial.println(F("  -> RIGHT ARROW"));
        break;
    case 0x1D:
        Serial.println(F("  -> LEFT ARROW"));
        break;
    default:
        // Check for digits and letters
        if (keyCode >= '0' && keyCode <= '9')
        {
            Serial.println(F("  -> Number"));
        }
        else if (keyCode >= 'A' && keyCode <= 'Z')
        {
            Serial.println(F("  -> Letter (uppercase)"));
        }
        else if (keyCode >= 'a' && keyCode <= 'z')
        {
            Serial.println(F("  -> Letter (lowercase)"));
        }
        else if (keyCode >= 32 && keyCode <= 126)
        {
            Serial.println(F("  -> Printable character"));
        }
        else
        {
            Serial.print(F("  -> Special/Control key ("));
            Serial.print(keyCode);
            Serial.println(F(")"));
        }
        break;
    }
}
