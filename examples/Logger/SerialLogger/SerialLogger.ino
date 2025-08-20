#include <Arduino.h>
#include <Model1.h>
#include <SerialLogger.h>

// Define the serial logger
SerialLogger logger;

void setup()
{
    // Setup the serial interface
    Serial.begin(115200); // Standard baud rate for consistent user experience

    // Be sure to setup the hardware pins
    Model1.begin();
    Model1.setLogger(logger);

    // You can also directly use it
    logger.info("System is setup");

    // Instead of using "info", you can also use "warn" for warning and "err" for error
    // There is no distinction in it except a different initial in the Serial logs.
    logger.warn("System is activated!");
}

void loop()
{
    // This writes the value 128 to address (hex)3400.
    // However, this will result in an error as the Arduino does not yet have access to the
    // bus system as the TEST* signal hasn't been activated yet.
    Model1.writeMemory(0x3400, 128);

    // You can also mute output...
    logger.mute();

    // This will not be logged
    logger.info("Missing in action...");

    // ...and unmute again
    logger.unmute();

    // This will show up again
    logger.info("I am back!");

    // Log some debug states
    Model1.logState();

    // Avoid too many messages by delaying the loop
    delay(1000);
}
