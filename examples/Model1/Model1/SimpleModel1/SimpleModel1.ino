#include <Arduino.h>
#include <Model1.h>

void setup()
{
    // Initialize serial communication
    Serial.begin(9600);
    delay(1000);

    // Initialize the Model 1 hardware interface
    Model1.begin();

    // Take control of the TRS-80 bus
    Model1.activateTestSignal();

    Serial.println(F("=== Simple Model1 Example ==="));
    Serial.println(F("Basic hardware interface operations"));
    Serial.println();
}

void loop()
{
    // Basic memory operations
    demonstrateBasicMemoryOperations();
    delay(3000);

    // Basic I/O operations
    demonstrateBasicIOOperations();
    delay(3000);

    // Memory block operations
    demonstrateMemoryBlockOperations();
    delay(3000);

    // Bus control demonstration
    demonstrateBusControl();
    delay(3000);
}

void demonstrateBasicMemoryOperations()
{
    Serial.println(F("--- Basic Memory Operations ---"));

    // Single byte read/write operations
    uint16_t testAddress = 0x3C00; // Video RAM start

    // Read current value
    uint8_t originalValue = Model1.readMemory(testAddress);
    Serial.print(F("Original value at 0x"));
    Serial.print(testAddress, HEX);
    Serial.print(F(": 0x"));
    Serial.println(originalValue, HEX);

    // Write a test value (space character)
    uint8_t testValue = 0x20; // Space character
    Model1.writeMemory(testAddress, testValue);
    Serial.print(F("Wrote 0x"));
    Serial.print(testValue, HEX);
    Serial.println(F(" (space character)"));

    // Read back to verify
    uint8_t readBack = Model1.readMemory(testAddress);
    Serial.print(F("Read back: 0x"));
    Serial.println(readBack, HEX);

    if (readBack == testValue)
    {
        Serial.println(F("Memory write/read: SUCCESS"));
    }
    else
    {
        Serial.println(F("Memory write/read: FAILED"));
    }

    // Restore original value
    Model1.writeMemory(testAddress, originalValue);
    Serial.println(F("Original value restored"));
    Serial.println();
}

void demonstrateBasicIOOperations()
{
    Serial.println(F("--- Basic I/O Operations ---"));

    // Read from I/O port (keyboard status)
    uint8_t keyboardStatus = Model1.readIO(0x3E);
    Serial.print(F("Keyboard status port (0x3E): 0x"));
    Serial.println(keyboardStatus, HEX);

    // Write to I/O port (cassette control)
    Serial.println(F("Writing to cassette control port (0x3F)"));
    Model1.writeIO(0x3F, 0x00); // Reset cassette control
    Serial.println(F("Cassette control reset"));

    // Check system reset signal
    bool isInReset = Model1.readSystemResetSignal();
    Serial.print(F("System reset signal: "));
    Serial.println(isInReset ? F("ACTIVE") : F("INACTIVE"));

    Serial.println();
}

void demonstrateMemoryBlockOperations()
{
    Serial.println(F("--- Memory Block Operations ---"));

    uint16_t baseAddress = 0x3C40; // Second line of video RAM

    // Write data array
    uint8_t testData[] = {'H', 'E', 'L', 'L', 'O'};
    Serial.println(F("Writing 'HELLO' to video memory..."));
    Model1.writeMemory(baseAddress, testData, 5);

    // Read back the data
    uint8_t *readData = Model1.readMemory(baseAddress, 5);
    Serial.print(F("Read back: "));
    for (int i = 0; i < 5; i++)
    {
        Serial.print((char)readData[i]);
    }
    Serial.println();
    free(readData); // Clean up allocated memory

    // Demonstrate fill operation
    Serial.println(F("Filling 10 bytes with 'A' character..."));
    Model1.fillMemory('A', baseAddress + 6, 10);

    // Demonstrate copy operation
    Serial.println(F("Copying first 5 bytes to position 20..."));
    Model1.copyMemory(baseAddress, baseAddress + 20, 5);

    // Clear the test area
    Serial.println(F("Clearing test area with spaces..."));
    Model1.fillMemory(' ', baseAddress, 30);

    Serial.println();
}

void demonstrateBusControl()
{
    Serial.println(F("--- Bus Control Operations ---"));

    // Demonstrate bus control cycling
    Serial.println(F("Current status: Arduino has bus control"));

    // Release control
    Serial.println(F("Releasing bus control to Z80..."));
    Model1.deactivateTestSignal();
    delay(1000);

    // Check if we can still access memory (we shouldn't be able to)
    Serial.println(F("Attempting memory access without bus control..."));
    // Note: This may not work reliably without bus control

    // Regain control
    Serial.println(F("Regaining bus control..."));
    Model1.activateTestSignal();

    // Verify we have control back
    uint16_t testAddr = 0x3C00;
    uint8_t testByte = Model1.readMemory(testAddr);
    Serial.print(F("Memory access with bus control - read 0x"));
    Serial.print(testByte, HEX);
    Serial.println(F(" from video RAM"));

    Serial.println(F("Bus control demonstration complete"));
    Serial.println();
}
