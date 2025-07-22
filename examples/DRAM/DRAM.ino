#include <Arduino.h>
#include <Model1.h>
#include <SerialLogger.h>

// Define the serial logger
SerialLogger logger;

void setup()
{
    // Setup the serial interface
    Serial.begin(500000); // baud rate

    // Be sure to configure the DRAM refresh and setup the hardware pins
    // It uses the Timer 2, but you can set this to false and setup your own timer.
    Model1.begin(2); // Timer 2
    Model1.setLogger(logger);

    // You can also use Timer 1, but this is often used for lots of other things
    // Timer 0 is not allowed and won't do anything. -1 turns this off.

    Serial.println("Started");

    // Give the system a bit of time to start up
    delay(5000);
}

// Define a callback for Timer 2
ISR(TIMER2_COMPA_vect)
{
    // Trigger a refresh to happen
    Model1.nextUpdate();
}

void loop()
{
    // Activate the test signal
    Serial.println("Activated");
    Model1.activateTestSignal();

    // At this point, the Arduino took over the execution of the Model 1
    // With the refresh interrupt above, we make sure that the DRAM is refreshed regularly

    // Let's keep it busy for a bit to show that the refresh works
    delay(5000);

    // Let's deactivate the control over the Model 1
    Model1.deactivateTestSignal();
    Serial.println("Deactivated");

    // At this point, you should be able to use the Model 1 again as normal

    // Let's give it 5 seconds before we go back in the loop and lock things up again.
    delay(5000);
}
