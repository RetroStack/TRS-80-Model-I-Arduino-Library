#include <Arduino.h>
#include <Model1.h>
#include <ROM.h>
#include <SerialLogger.h>

// Define an accessible variable for the Model 1 and ROM instance
ROM rom;
SerialLogger logger;

void setup()
{
    // Setup the serial interface
    Serial.begin(9600); // baud rate

    // Be sure to setup the hardware pins
    Model1.begin();

    Model1.setLogger(logger);
    rom.setLogger(logger);

    // Activate the test signal for the Arduino to take over the Model 1 system.
    // You need to activate it, otherwise the Z80 will continue to be in control.
    // The Arduino will also be blocked from accessing the bus without activation.
    Model1.activateTestSignal();

    // Print ROM A checksum
    Serial.print("ROM A = ");
    Serial.println(rom.getChecksum(0), HEX);

    // Print ROM B checksum
    Serial.print("ROM B = ");
    Serial.println(rom.getChecksum(1), HEX);

    // Print ROM C checksum
    Serial.print("ROM C = ");
    Serial.println(rom.getChecksum(2), HEX);

    // Print ROM D checksum, if there is one
    Serial.print("ROM D = ");
    Serial.println(rom.getChecksum(3), HEX);

    // Detect system ROMs
    const __FlashStringHelper *systemName = rom.identifyROM();
    if (systemName)
    {
        Serial.print("Detected system: ");
        Serial.println(systemName);
    }
    else
    {
        Serial.println("Unknown ROM combination.");
    }

    // Print ROM A contents to Serial logger in HEX editor view
    rom.printROMContents(0);
}

void loop()
{
}
