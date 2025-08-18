#include <Arduino.h>
#include <Model1.h>
#include <ROM.h>

// ROM class instance for reading TRS-80 system ROMs
ROM rom;

void setup()
{
    // Initialize serial communication
    Serial.begin(9600);
    delay(1000);

    // Initialize the Model 1 hardware interface
    Model1.begin();

    // Take control of the TRS-80 bus
    Model1.activateTestSignal();

    Serial.println(F("=== Simple ROM Example ==="));
    Serial.println(F("Reading TRS-80 Model I system ROMs"));
    Serial.println();
}

void loop()
{
    // Basic ROM identification
    demonstrateSystemIdentification();
    delay(3000);

    // Basic checksum calculation
    demonstrateChecksumCalculation();
    delay(3000);

    // Basic ROM verification
    demonstrateROMVerification();
    delay(5000);
}

void demonstrateSystemIdentification()
{
    Serial.println(F("--- System Identification ---"));

    // Identify the system ROM combination
    const __FlashStringHelper *systemName = rom.identifyROM();

    if (systemName)
    {
        Serial.print(F("Detected system: "));
        Serial.println(systemName);
    }
    else
    {
        Serial.println(F("Unknown ROM combination"));
        Serial.println(F("This may be a modified or custom system"));
    }

    Serial.println();
}

void demonstrateChecksumCalculation()
{
    Serial.println(F("--- ROM Checksums ---"));

    // Calculate and display checksums for each ROM slot
    for (int romSlot = 0; romSlot < 4; romSlot++)
    {
        uint16_t checksum = rom.getChecksum(romSlot);

        Serial.print(F("ROM "));
        Serial.print((char)('A' + romSlot));
        Serial.print(F(" checksum: 0x"));
        if (checksum < 0x1000)
            Serial.print('0');
        if (checksum < 0x100)
            Serial.print('0');
        if (checksum < 0x10)
            Serial.print('0');
        Serial.println(checksum, HEX);
    }

    Serial.println();
}

void demonstrateROMVerification()
{
    Serial.println(F("--- ROM Verification ---"));

    // Check if ROMs are present and accessible
    bool hasValidROMs = false;

    for (int romSlot = 0; romSlot < 4; romSlot++)
    {
        uint16_t checksum = rom.getChecksum(romSlot);

        if (checksum != 0x0000 && checksum != 0xFFFF)
        {
            hasValidROMs = true;
            Serial.print(F("ROM "));
            Serial.print((char)('A' + romSlot));
            Serial.println(F(" appears valid"));
        }
        else
        {
            Serial.print(F("ROM "));
            Serial.print((char)('A' + romSlot));
            Serial.println(F(" appears empty or faulty"));
        }
    }

    if (hasValidROMs)
    {
        Serial.println(F("System has valid ROMs - ready for operation"));
    }
    else
    {
        Serial.println(F("Warning: No valid ROMs detected"));
    }

    Serial.println();
}
