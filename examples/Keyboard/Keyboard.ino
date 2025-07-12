#include <Arduino.h>
#include <Model1.h>
#include <Keyboard.h>

// Define an accessible variable for the Model 1 and keyboard subsystem instance
Model1 *model1;
Keyboard *keyboard;

void setup()
{
    // Setup the serial interface
    Serial.begin(9600); // baud rate

    // Initialize a Model 1 instance to access it
    model1 = new Model1();

    // Be sure to setup the hardware pins
    model1->begin();

    // Create an instance of the Keyboard subsystem, supplying the Model 1 connection
    keyboard = new Keyboard(model1);

    // Activate the test signal for the Arduino to take over the Model 1 system.
    // You need to activate it, otherwise the Z80 will continue to be in control.
    // The Arduino will also be blocked from accessing the bus without activation.
    model1->activateTestSignal();

    // Read the latest state to establish a reference point (optional)
    keyboard->readState();
}

void loop()
{
    // Scans the key that was just pressed
    // If no key is pressed, then it will return 0.
    uint8_t pressedKey = keyboard->getFirstJustPressedKey();

    // Check if a key was pressed
    // You can also use keyboard->isKeyPressed() to check if any key is pressed
    if (pressedKey != 0)
    {
        Serial.print("Key pressed: ");
        Serial.print((char)pressedKey);
        Serial.print(" (0x");
        Serial.print(pressedKey, HEX);
        Serial.println(")");
    }

    // OR -----------------------------------------------------------------

    // Get the iterator with all keyboard changes compared to previous scan
    KeyboardChangeIterator it = keyboard->changes();

    // Iterate over all keyboard changeschanges
    while (it.hasNext())
    {
        if (it.wasJustPressed())
        {
            uint8_t value = it.keyValue();

            Serial.print("Key pressed: ");
            Serial.print((char)value);
            Serial.print(" (0x");
            Serial.print(value, HEX);
            Serial.println(")");
        }

        // Move to next change
        it.next();
    }
}
