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

    // Initialize a Model 1 instance to access it, supplying the logger
    // All other classes allow the supply of it as well to be able to track
    // issues across all of these subsystems.
    model1 = new Model1(logger);

    // Be sure to setup the hardware pins
    model1->begin();

    // You can also directly use it
    logger->info("System is setup");

    // Instead of using "info", you can also use "warn" for warning and "err" for error
    // There is no distinction in it except a different initial in the Serial logs.
    logger->warn("System is activated!");
}

void loop()
{
    // This writes the value 128 to address (hex)3400.
    // However, this will result in an error as the Arduino does not yet have access to the
    // bus system as the TEST* signal hasn't been activated yet.
    model1->writeMemory(0x3400, 128);

    // You can also mute output...
    logger->mute();

    // This will not be logged
    logger->info("Missing in action...");

    // ...and unmute again
    logger->unmute();

    // This will show up again
    logger->info("I am back!");

    // Log some debug states
    model1->logState();

    // Avoid too many messages by delaying the loop
    delay(1000);
}
