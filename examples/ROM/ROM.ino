#include <Arduino.h>
#include <Model1.h>
#include <ROM.h>

// Define an accessible variable for the Model 1 and ROM instance
Model1 *model1;
ROM *rom;

void setup()
{
    // Initialize a Model 1 instance to access it
    model1 = new Model1();

    // Be sure to setup the hardware pins
    model1->begin();

    // Create an instance of the ROM subsystem, supplying the Model 1 connection
    rom = new ROM(model1);

    // Activate the test signal for the Arduino to take over the Model 1 system.
    // You need to activate it, otherwise the Z80 will continue to be in control.
    // The Arduino will also be blocked from accessing the bus without activation.
    model1->activateTestSignal();
}

void loop()
{
    // Request the full contents of the ROM
    // It reads all of the 12k of ROM and returns it as a byte array.
    uint8_t *contents = rom->getContent();

    // Make sure to free up that memory again
    free(contents);
}
