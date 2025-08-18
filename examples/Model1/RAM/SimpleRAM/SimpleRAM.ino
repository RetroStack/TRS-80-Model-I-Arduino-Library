#include <Arduino.h>
#include <Model1.h>

void setup()
{
    // Initialize serial communication
    Serial.begin(115200);
    delay(1000);

    // Initialize Model 1 with DRAM refresh on Timer 2
    // This is essential for maintaining RAM contents
    Model1.begin(2);

    Serial.println(F("=== Simple RAM Example ==="));
    Serial.println(F("Basic RAM operations and refresh demonstration"));
    Serial.println();

    // Give the system time to stabilize
    delay(2000);
}

// Timer interrupt for DRAM refresh
ISR(TIMER2_COMPA_vect)
{
    // Critical: Refresh DRAM to prevent data loss
    Model1.nextUpdate();
}

void loop()
{
    // Basic memory operations
    demonstrateBasicMemoryOperations();
    delay(3000);

    // DRAM refresh demonstration
    demonstrateDRAMRefresh();
    delay(3000);

    // Simple memory test
    demonstrateSimpleMemoryTest();
    delay(3000);
}

void demonstrateBasicMemoryOperations()
{
    Serial.println(F("--- Basic Memory Operations ---"));

    // Take control of the bus
    Serial.println(F("Activating TEST signal..."));
    Model1.activateTestSignal();

    // Basic read/write operations
    uint16_t testAddress = 0x4000; // User RAM area
    uint8_t testValue = 0xAA;      // Test pattern

    Serial.print(F("Writing 0x"));
    Serial.print(testValue, HEX);
    Serial.print(F(" to address 0x"));
    Serial.println(testAddress, HEX);

    // Write test value
    Model1.writeMemory(testAddress, testValue);

    // Read back the value
    uint8_t readValue = Model1.readMemory(testAddress);

    Serial.print(F("Read back: 0x"));
    Serial.println(readValue, HEX);

    if (readValue == testValue)
    {
        Serial.println(F("Basic memory operation: SUCCESS"));
    }
    else
    {
        Serial.println(F("Basic memory operation: FAILED"));
    }

    // Release control of the bus
    Serial.println(F("Deactivating TEST signal..."));
    Model1.deactivateTestSignal();

    Serial.println();
}

void demonstrateDRAMRefresh()
{
    Serial.println(F("--- DRAM Refresh Demonstration ---"));

    Serial.println(F("DRAM requires periodic refresh to retain data"));
    Serial.println(F("Timer 2 interrupt handles automatic refresh"));
    Serial.println();

    // Take control with refresh active
    Model1.activateTestSignal();

    // Write test pattern
    uint16_t startAddr = 0x4000;
    uint8_t testPattern[] = {0x55, 0xAA, 0xFF, 0x00, 0x33, 0xCC};

    Serial.println(F("Writing test pattern..."));
    for (int i = 0; i < 6; i++)
    {
        Model1.writeMemory(startAddr + i, testPattern[i]);
    }

    // Wait while refresh continues in background
    Serial.println(F("Waiting 5 seconds (refresh active)..."));
    delay(5000);

    // Verify data integrity
    Serial.println(F("Verifying data integrity:"));
    bool dataIntact = true;

    for (int i = 0; i < 6; i++)
    {
        uint8_t readValue = Model1.readMemory(startAddr + i);
        Serial.print(F("Address 0x"));
        Serial.print(startAddr + i, HEX);
        Serial.print(F(": Expected 0x"));
        Serial.print(testPattern[i], HEX);
        Serial.print(F(", Read 0x"));
        Serial.print(readValue, HEX);

        if (readValue == testPattern[i])
        {
            Serial.println(F(" - OK"));
        }
        else
        {
            Serial.println(F(" - FAILED"));
            dataIntact = false;
        }
    }

    if (dataIntact)
    {
        Serial.println(F("DRAM refresh working correctly"));
    }
    else
    {
        Serial.println(F("DRAM refresh issue detected"));
    }

    Model1.deactivateTestSignal();
    Serial.println();
}

void demonstrateSimpleMemoryTest()
{
    Serial.println(F("--- Simple Memory Test ---"));

    Model1.activateTestSignal();

    // Test a small range of user RAM
    uint16_t testStart = 0x4000;
    uint16_t testLength = 256; // Test 256 bytes

    Serial.print(F("Testing "));
    Serial.print(testLength);
    Serial.print(F(" bytes starting at 0x"));
    Serial.println(testStart, HEX);

    // Write test pattern
    Serial.println(F("Writing test pattern..."));
    for (uint16_t i = 0; i < testLength; i++)
    {
        uint8_t pattern = (uint8_t)(i & 0xFF); // Use address low byte as pattern
        Model1.writeMemory(testStart + i, pattern);
    }

    // Verify test pattern
    Serial.println(F("Verifying test pattern..."));
    uint16_t errors = 0;

    for (uint16_t i = 0; i < testLength; i++)
    {
        uint8_t expected = (uint8_t)(i & 0xFF);
        uint8_t actual = Model1.readMemory(testStart + i);

        if (actual != expected)
        {
            errors++;
            if (errors <= 10) // Only show first 10 errors
            {
                Serial.print(F("Error at 0x"));
                Serial.print(testStart + i, HEX);
                Serial.print(F(": expected 0x"));
                Serial.print(expected, HEX);
                Serial.print(F(", got 0x"));
                Serial.println(actual, HEX);
            }
        }
    }

    Serial.print(F("Test complete: "));
    Serial.print(errors);
    Serial.print(F(" errors out of "));
    Serial.print(testLength);
    Serial.println(F(" bytes"));

    if (errors == 0)
    {
        Serial.println(F("Memory test: PASSED"));
    }
    else
    {
        Serial.print(F("Memory test: FAILED ("));
        Serial.print((float)errors / testLength * 100, 1);
        Serial.println(F("% error rate)"));
    }

    Model1.deactivateTestSignal();
    Serial.println();
}
