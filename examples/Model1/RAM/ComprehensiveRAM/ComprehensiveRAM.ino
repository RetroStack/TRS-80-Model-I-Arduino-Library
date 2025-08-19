#include <Arduino.h>
#include <Model1.h>
#include <SerialLogger.h>

// Logger for detailed memory operation debugging
SerialLogger logger;

// Memory test result structures
struct TestResult
{
    uint32_t totalErrors;
    uint32_t bitErrors[8]; // Track errors per bit position
};

struct MemoryRegion
{
    uint16_t start;
    uint16_t length;
    const char *name;
};

// Define memory regions for testing
MemoryRegion memoryRegions[] = {
    {0x3C00, 1024, "Video RAM"}, // Video memory
    {0x4000, 12288, "User RAM"}, // User memory (12KB)
    {0x7000, 4096, "High RAM"}   // High memory area
};

void setup()
{
    // Initialize serial communication at high speed for detailed output
    Serial.begin(115200);
    delay(1000);

    // Initialize Model 1 with DRAM refresh on Timer 2
    Model1.begin(2);
    Model1.setLogger(logger);

    Serial.println(F("=== Comprehensive RAM Example ==="));
    Serial.println(F("Advanced memory testing and analysis"));
    Serial.println();

    // System initialization delay
    delay(2000);
}

// Critical DRAM refresh interrupt
ISR(TIMER2_COMPA_vect)
{
    Model1.nextUpdate();
}

void loop()
{
    // Comprehensive memory analysis
    demonstrateMemoryMapping();
    delay(3000);

    // Advanced memory testing
    demonstrateAdvancedMemoryTests();
    delay(4000);

    // Memory region analysis
    demonstrateMemoryRegionAnalysis();
    delay(4000);

    // Performance and timing tests
    demonstrateMemoryPerformance();
    delay(4000);

    // Long-term stability test
    demonstrateLongTermStabilityTest();
    delay(6000);
}

void demonstrateMemoryMapping()
{
    Serial.println(F("--- TRS-80 Model I Memory Map Analysis ---"));

    Model1.activateTestSignal();

    Serial.println(F("Memory Region Analysis:"));
    Serial.println(F("Address Range | Size  | Type        | Status"));
    Serial.println(F("--------------|-------|-------------|--------"));

    // Analyze each defined memory region
    for (int i = 0; i < 3; i++)
    {
        MemoryRegion &region = memoryRegions[i];

        Serial.print(F("0x"));
        Serial.print(region.start, HEX);
        Serial.print(F("-0x"));
        Serial.print(region.start + region.length - 1, HEX);
        Serial.print(F(" | "));
        Serial.print(region.length);
        Serial.print(F("B | "));
        Serial.print(region.name);

        // Test if region is accessible
        uint8_t originalValue = Model1.readMemory(region.start);
        Model1.writeMemory(region.start, 0x55);
        uint8_t testRead = Model1.readMemory(region.start);
        Model1.writeMemory(region.start, originalValue); // Restore

        if (testRead == 0x55)
        {
            Serial.println(F(" | R/W"));
        }
        else
        {
            Serial.println(F(" | ERROR"));
        }
    }

    // Test ROM regions (should be read-only)
    Serial.println(F("ROM Regions (Read-Only):"));
    Serial.println(F("0x0000-0x1FFF | 8KB   | System ROM  | R/O"));

    Model1.deactivateTestSignal();
}

void demonstrateAdvancedMemoryTests()
{
    Serial.println(F("--- Advanced Memory Tests ---"));

    Model1.activateTestSignal();

    // Test different patterns
    uint8_t testPatterns[] = {0x00, 0xFF, 0x55, 0xAA, 0x33, 0xCC, 0x0F, 0xF0};
    const char *patternNames[] = {"All 0s", "All 1s", "0101 Pattern", "1010 Pattern",
                                  "0011 Pattern", "1100 Pattern", "0000/1111", "1111/0000"};

    uint16_t testAddr = 0x4000;
    uint16_t testSize = 1024;

    Serial.println(F("Testing memory patterns:"));

    for (int p = 0; p < 8; p++)
    {
        Serial.print(F("Pattern "));
        Serial.print(patternNames[p]);
        Serial.print(F(" (0x"));
        Serial.print(testPatterns[p], HEX);
        Serial.print(F("): "));

        // Fill memory with pattern
        for (uint16_t i = 0; i < testSize; i++)
        {
            Model1.writeMemory(testAddr + i, testPatterns[p]);
        }

        // Verify pattern
        uint16_t errors = 0;
        for (uint16_t i = 0; i < testSize; i++)
        {
            uint8_t readValue = Model1.readMemory(testAddr + i);
            if (readValue != testPatterns[p])
            {
                errors++;
            }
        }

        if (errors == 0)
        {
            Serial.println(F("PASS"));
        }
        else
        {
            Serial.print(F("FAIL - "));
            Serial.print(errors);
            Serial.println(F(" errors"));
        }
    }

    // Walking bit test
    Serial.println(F("Walking bit test:"));
    performWalkingBitTest(testAddr, 64); // Test 64 bytes

    Model1.deactivateTestSignal();
}

void demonstrateMemoryRegionAnalysis()
{
    Serial.println(F("--- Memory Region Analysis ---"));

    Model1.activateTestSignal();

    for (int r = 0; r < 3; r++)
    {
        MemoryRegion &region = memoryRegions[r];

        Serial.print(F("Analyzing "));
        Serial.print(region.name);
        Serial.println(F(":"));

        // Sample memory content
        Serial.println(F("Memory sample (first 32 bytes):"));
        Serial.println(F("Addr  | 00 01 02 03 04 05 06 07 | ASCII"));
        Serial.println(F("------|-------------------------|--------"));

        for (int row = 0; row < 4; row++)
        {
            uint16_t addr = region.start + (row * 8);
            Serial.print(F("0x"));
            if (addr < 0x1000)
                Serial.print('0');
            if (addr < 0x100)
                Serial.print('0');
            if (addr < 0x10)
                Serial.print('0');
            Serial.print(addr, HEX);
            Serial.print(F(" | "));

            // Print hex bytes
            for (int col = 0; col < 8; col++)
            {
                uint8_t byte = Model1.readMemory(addr + col);
                if (byte < 0x10)
                    Serial.print('0');
                Serial.print(byte, HEX);
                Serial.print(' ');
            }

            Serial.print(F("| "));

            // Print ASCII representation
            for (int col = 0; col < 8; col++)
            {
                uint8_t byte = Model1.readMemory(addr + col);
                if (byte >= 32 && byte <= 126)
                    Serial.print((char)byte);
                else
                    Serial.print('.');
            }

            Serial.println();
        }

        // Quick integrity test
        TestResult result = performQuickTest(region.start, min(region.length, (uint16_t)256));

        Serial.print(F("Quick test (256 bytes): "));
        if (result.totalErrors == 0)
        {
            Serial.println(F("PASS"));
        }
        else
        {
            Serial.print(result.totalErrors);
            Serial.println(F(" errors"));
        }
    }

    Model1.deactivateTestSignal();
}

void demonstrateMemoryPerformance()
{
    Serial.println(F("--- Memory Performance Analysis ---"));

    Model1.activateTestSignal();

    uint16_t testAddr = 0x4000;
    uint16_t testSize = 1024;

    // Write performance test
    unsigned long startTime = micros();
    for (uint16_t i = 0; i < testSize; i++)
    {
        Model1.writeMemory(testAddr + i, (uint8_t)(i & 0xFF));
    }
    unsigned long writeTime = micros() - startTime;

    // Read performance test
    startTime = micros();
    for (uint16_t i = 0; i < testSize; i++)
    {
        volatile uint8_t dummy = Model1.readMemory(testAddr + i);
        (void)dummy; // Suppress unused variable warning
    }
    unsigned long readTime = micros() - startTime;

    Serial.print(F("Write performance: "));
    Serial.print((float)writeTime / testSize, 2);
    Serial.println(F(" μs/byte"));

    Serial.print(F("Read performance: "));
    Serial.print((float)readTime / testSize, 2);
    Serial.println(F(" μs/byte"));

    Serial.print(F("Total bandwidth: "));
    Serial.print((float)(testSize * 2) / (writeTime + readTime) * 1000000, 0);
    Serial.println(F(" bytes/second"));

    Model1.deactivateTestSignal();
}

void demonstrateLongTermStabilityTest()
{
    Serial.println(F("--- Long-Term Stability Test ---"));

    Model1.activateTestSignal();

    uint16_t testAddr = 0x4000;
    uint16_t testSize = 512;

    Serial.println(F("Writing test pattern and monitoring for 30 seconds..."));

    // Write initial pattern
    for (uint16_t i = 0; i < testSize; i++)
    {
        uint8_t pattern = (uint8_t)((i * 7) & 0xFF); // Pseudo-random pattern
        Model1.writeMemory(testAddr + i, pattern);
    }

    // Monitor for 30 seconds
    for (int second = 1; second <= 30; second++)
    {
        delay(1000);

        // Verify pattern integrity
        uint16_t errors = 0;
        for (uint16_t i = 0; i < testSize; i++)
        {
            uint8_t expected = (uint8_t)((i * 7) & 0xFF);
            uint8_t actual = Model1.readMemory(testAddr + i);
            if (actual != expected)
            {
                errors++;
            }
        }

        if (second % 5 == 0 || errors > 0) // Report every 5 seconds or on error
        {
            Serial.print(F("Time: "));
            Serial.print(second);
            Serial.print(F("s, Errors: "));
            Serial.println(errors);
        }

        if (errors > 0)
        {
            Serial.println(F("Data corruption detected - refresh may be failing"));
            break;
        }
    }

    Serial.println(F("Stability test complete"));

    Model1.deactivateTestSignal();
}

// Helper function for walking bit test
void performWalkingBitTest(uint16_t address, uint16_t length)
{
    for (int bit = 0; bit < 8; bit++)
    {
        uint8_t pattern = (1 << bit);
        Serial.print(F("Walking bit "));
        Serial.print(bit);
        Serial.print(F(" (0x"));
        Serial.print(pattern, HEX);
        Serial.print(F("): "));

        // Write pattern
        for (uint16_t i = 0; i < length; i++)
        {
            Model1.writeMemory(address + i, pattern);
        }

        // Verify pattern
        uint16_t errors = 0;
        for (uint16_t i = 0; i < length; i++)
        {
            uint8_t readValue = Model1.readMemory(address + i);
            if (readValue != pattern)
            {
                errors++;
            }
        }

        if (errors == 0)
        {
            Serial.println(F("PASS"));
        }
        else
        {
            Serial.print(F("FAIL - "));
            Serial.print(errors);
            Serial.println(F(" errors"));
        }
    }
}

// Helper function for quick memory test
TestResult performQuickTest(uint16_t address, uint16_t length)
{
    TestResult result = {};

    // Simple alternating pattern test
    for (uint16_t i = 0; i < length; i++)
    {
        uint8_t pattern = (i & 1) ? 0xAA : 0x55;
        Model1.writeMemory(address + i, pattern);
    }

    for (uint16_t i = 0; i < length; i++)
    {
        uint8_t expected = (i & 1) ? 0xAA : 0x55;
        uint8_t actual = Model1.readMemory(address + i);

        if (actual != expected)
        {
            result.totalErrors++;
            uint8_t diff = actual ^ expected;
            for (int bit = 0; bit < 8; bit++)
            {
                if (diff & (1 << bit))
                {
                    result.bitErrors[bit]++;
                }
            }
        }
    }

    return result;
}
