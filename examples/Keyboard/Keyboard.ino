#include <Arduino.h>
#include <Model1.h>
#include <Keyboard.h>

// Define an accessible variable for the Model 1 and keyboard subsystem instance
Model1 *model1;
Keyboard *keyboard;

void setup()
{
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
}

void loop()
{
    // Check wether a key is pressed
    if (keyboard->isKeyPressed())
    {

        // Scans the key that was pressed
        // If no key is pressed, then it will return 0.
        uint8_t pressedKey = keyboard->scanKey();
    }
}
