#include <Arduino.h>
#include <Model1.h>
#include <SerialLogger.h>

// Define an accessible variable for the Model 1 instance
Model1 *model1;

// Define the serial logger
SerialLogger *logger;

void setup()
{
    // Setup the serial interface
    Serial.begin(9600); // baud rate

    // Initialize the serial logger
    logger = new SerialLogger();

    // Initialize a Model 1 instance to access it
    model1 = new Model1(logger);

    // Be sure to configure the DRAM refresh and setup the hardware pins
    // It uses the Timer1, but you can set this to false and setup your own timer.
    model1->begin(true);

    // Give the system a bit of time to start up
    delay(5000);
}

// Define a callback for the timer1
ISR(TIMER1_COMPA_vect)
{
    // Trigger a refresh to happen
    model1->refreshNextMemoryRow();
}

void loop()
{
    // Activate the test signal
    model1->activateTestSignal();
    Serial.println("Activated");

    // At this point, the Arduino took over the execution of the Model 1
    // With the refresh interrupt above, we make sure that the DRAM is refreshed regularly

    // Let's keep it busy for a bit to show that the refresh works
    delay(5000);

    // Let's deactivate the control over the Model 1
    model1->deactivateTestSignal();
    Serial.println("Deactivated");

    // At this point, you should be able to use the Model 1 again as normal

    // Let's give it 20 seconds before we go back in the loop and lock things up again.
    delay(20000);
}
