#include <Arduino.h>
#include <Model1.h>

// Define an accessible variable for the Model 1 instance
Model1 *model1;

void setup()
{
    // Initialize a Model 1 instance to access it
    model1 = new Model1();

    // Be sure to configure the DRAM refresh and setup the hardware pins
    // It uses the Timer1, but you can set this to false and setup your own timer.
    model1->begin(true);

    // Activate the test signal for the Arduino to take over the Model 1 system.
    // You need to activate it, otherwise the Z80 will continue to be in control.
    // The Arduino will also be blocked from accessing the bus without activation.
    model1->activateTestSignal();
}

// Define a callback for the timer1
ISR(TIMER1_COMPA_vect)
{
    // Trigger a refresh to happen
    model1->refreshNextMemoryRow();
}

void loop()
{
    // ...
}
