#include <Arduino.h>
#include <Model1.h>
#include <Cassette.h>

// Define an accessible variable for the Model 1 and Cassette subsystem instance
Model1 *model1;
Cassette *cassette;

void setup()
{
    // Initialize a Model 1 instance to access it
    model1 = new Model1();

    // Be sure to setup the hardware pins
    model1->begin();

    // Create an instance of the Cassette subsystem, supplying the Model 1 connection
    cassette = new Cassette(model1);

    // Activate the test signal for the Arduino to take over the Model 1 system.
    // You need to activate it, otherwise the Z80 will continue to be in control.
    // The Arduino will also be blocked from accessing the bus without activation.
    model1->activateTestSignal();
}

void loop()
{
    // Sets the output to low and high
    cassette->writeRaw(false, true);

    // Reads from the cassette interface
    if (cassette->read() == HIGH)
    {
        // ...
    }

    // Activates the remote driver
    cassette->activateRemote();

    // Deactivates the remote driver
    cassette->deactivateRemote();
}
