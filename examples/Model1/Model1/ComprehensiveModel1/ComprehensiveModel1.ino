#include <Arduino.h>
#include <Model1.h>
#include <SerialLogger.h>

// Logger for detailed hardware operation debugging
SerialLogger logger;

// System state tracking
struct SystemState
{
    bool busControlActive;
    bool waitSignalActive;
    bool resetSignalActive;
    unsigned long lastOperationTime;
};

SystemState systemState = {};

void setup()
{
    // Initialize serial communication at high speed
    Serial.begin(115200);
    delay(1000);

    // Initialize the Model 1 hardware interface
    Model1.begin();

    // Set up detailed logging
    Model1.setLogger(logger);

    // Take control of the TRS-80 bus
    Model1.activateTestSignal();
    systemState.busControlActive = true;

    Serial.println(F("=== Comprehensive Model1 Example ==="));
    Serial.println(F("Advanced hardware interface operations and diagnostics"));
    Serial.println();
}

void loop()
{
    // Advanced memory operations
    demonstrateAdvancedMemoryOperations();
    delay(4000);

    // Interrupt and timing operations
    demonstrateInterruptOperations();
    delay(4000);

    // Advanced bus control and timing
    demonstrateAdvancedBusControl();
    delay(4000);

    // System diagnostics and analysis
    demonstrateSystemDiagnostics();
    delay(4000);

    // Performance and timing analysis
    demonstratePerformanceAnalysis();
    delay(6000);
}

void demonstrateAdvancedMemoryOperations()
{
    Serial.println(F("--- Advanced Memory Operations ---"));

    // Memory region analysis
    Serial.println(F("TRS-80 Model I Memory Map Analysis:"));
    Serial.println(F("0x0000-0x1FFF: ROM (8KB) - System BASIC"));
    Serial.println(F("0x3C00-0x3FFF: Video RAM (1KB)"));
    Serial.println(F("0x4000-0x7FFF: User RAM (16KB)"));
    Serial.println();

    // Test different memory regions
    uint16_t testAddresses[] = {0x0000, 0x3C00, 0x4000, 0x7000};
    const char *regionNames[] = {"ROM", "Video RAM", "User RAM Low", "User RAM High"};

    for (int i = 0; i < 4; i++)
    {
        Serial.print(F("Testing "));
        Serial.print(regionNames[i]);
        Serial.print(F(" at 0x"));
        Serial.print(testAddresses[i], HEX);
        Serial.print(F(": "));

        uint8_t originalValue = Model1.readMemory(testAddresses[i]);

        if (i == 0) // ROM region - read-only
        {
            Serial.print(F("Read 0x"));
            Serial.print(originalValue, HEX);
            Serial.println(F(" (ROM - read only)"));
        }
        else // RAM regions - read/write
        {
            Model1.writeMemory(testAddresses[i], 0x55);
            uint8_t testRead = Model1.readMemory(testAddresses[i]);
            Model1.writeMemory(testAddresses[i], originalValue); // Restore

            if (testRead == 0x55)
            {
                Serial.println(F("R/W accessible"));
            }
            else
            {
                Serial.println(F("Access failed"));
            }
        }
    }

    // Advanced block operations with timing
    Serial.println(F("Advanced block operations:"));

    uint16_t blockAddr = 0x4000;
    uint16_t blockSize = 1024;

    // Timed large block write
    unsigned long startTime = micros();

    // Create test pattern
    uint8_t *testPattern = new uint8_t[blockSize];
    for (uint16_t i = 0; i < blockSize; i++)
    {
        testPattern[i] = (uint8_t)((i * 13) & 0xFF); // Pseudo-random pattern
    }

    Model1.writeMemory(blockAddr, testPattern, blockSize);
    unsigned long writeTime = micros() - startTime;

    // Timed large block read
    startTime = micros();
    uint8_t *readBack = Model1.readMemory(blockAddr, blockSize);
    unsigned long readTime = micros() - startTime;

    // Verify integrity
    bool dataIntact = true;
    for (uint16_t i = 0; i < blockSize; i++)
    {
        if (readBack[i] != testPattern[i])
        {
            dataIntact = false;
            break;
        }
    }

    Serial.print(F("Block write ("));
    Serial.print(blockSize);
    Serial.print(F(" bytes): "));
    Serial.print(writeTime);
    Serial.print(F(" μs ("));
    Serial.print((float)blockSize / writeTime * 1000000, 0);
    Serial.println(F(" bytes/sec)"));

    Serial.print(F("Block read ("));
    Serial.print(blockSize);
    Serial.print(F(" bytes): "));
    Serial.print(readTime);
    Serial.print(F(" μs ("));
    Serial.print((float)blockSize / readTime * 1000000, 0);
    Serial.println(F(" bytes/sec)"));

    Serial.print(F("Data integrity: "));
    Serial.println(dataIntact ? F("PASS") : F("FAIL"));

    delete[] testPattern;
    free(readBack);
}

void demonstrateInterruptOperations()
{
    // TODO: Somehow doesn't work. Need to debug this
    Serial.println(F("--- Interrupt Operations ---"));

    // Test interrupt triggering
    Serial.println(F("Testing Z80 interrupt capabilities:"));

    Model1.deactivateTestSignal();

    uint8_t interruptVectors[] = {0x10, 0x18, 0x20, 0x28, 0x30, 0x38};
    const char *interruptNames[] = {"RST 10H", "RST 18H", "RST 20H", "RST 28H", "RST 30H", "RST 38H"};

    for (int i = 0; i < 6; i++)
    {
        Serial.print(F("Testing interrupt "));
        Serial.print(interruptNames[i]);
        Serial.print(F(" (0x"));
        Serial.print(interruptVectors[i], HEX);
        Serial.print(F("): "));

        unsigned long startTime = millis();
        bool wasTriggered = Model1.triggerInterrupt(interruptVectors[i]);
        unsigned long responseTime = millis() - startTime;

        if (wasTriggered)
        {
            Serial.print(F("SUCCESS ("));
            Serial.print(responseTime);
            Serial.println(F("ms response)"));
        }
        else
        {
            Serial.println(F("TIMEOUT"));
        }

        delay(100); // Brief pause between interrupt tests
    }

    Model1.activateTestSignal();
}

void demonstrateAdvancedBusControl()
{
    Serial.println(F("--- Advanced Bus Control ---"));

    // Test wait signal functionality
    Serial.println(F("Testing WAIT signal control:"));

    Serial.println(F("Activating WAIT signal (freezing Z80)..."));
    Model1.activateWaitSignal();
    systemState.waitSignalActive = true;
    delay(1000);

    Serial.println(F("Z80 should be frozen - testing memory access..."));
    uint8_t testValue = Model1.readMemory(0x3C00);
    Serial.print(F("Memory read successful: 0x"));
    Serial.println(testValue, HEX);

    Serial.println(F("Deactivating WAIT signal (releasing Z80)..."));
    Model1.deactivateWaitSignal();
    systemState.waitSignalActive = false;

    // Test rapid bus control switching
    Serial.println(F("Testing rapid bus control switching:"));

    for (int cycle = 0; cycle < 5; cycle++)
    {
        Serial.print(F("Cycle "));
        Serial.print(cycle + 1);
        Serial.print(F(": "));

        // Release control
        Model1.deactivateTestSignal();
        systemState.busControlActive = false;
        delayMicroseconds(100);

        // Regain control
        Model1.activateTestSignal();
        systemState.busControlActive = true;

        // Quick memory test to verify control
        uint8_t quickTest = Model1.readMemory(0x3C00);
        Serial.print(F("Read 0x"));
        Serial.println(quickTest, HEX);

        delay(200);
    }
}

void demonstrateSystemDiagnostics()
{
    Serial.println(F("--- System Diagnostics ---"));

    // System state analysis
    Serial.println(F("Current system state:"));
    Serial.print(F("Bus control: "));
    Serial.println(systemState.busControlActive ? F("ACTIVE") : F("INACTIVE"));
    Serial.print(F("Wait signal: "));
    Serial.println(systemState.waitSignalActive ? F("ACTIVE") : F("INACTIVE"));

    // Reset signal monitoring
    bool resetState = Model1.readSystemResetSignal();
    Serial.print(F("Reset signal: "));
    Serial.println(resetState ? F("ACTIVE") : F("INACTIVE"));

    // Memory map verification
    Serial.println(F("Memory map verification:"));

    // Test ROM area (should be readable but not writable)
    uint8_t romTest = Model1.readMemory(0x0000);
    Serial.print(F("ROM @ 0x0000: 0x"));
    Serial.print(romTest, HEX);
    Serial.println(F(" (should be readable)"));

    // Test video RAM (should be readable and writable)
    uint8_t originalVideo = Model1.readMemory(0x3C00);
    Model1.writeMemory(0x3C00, 0x55);
    uint8_t videoTest = Model1.readMemory(0x3C00);
    Model1.writeMemory(0x3C00, originalVideo);

    Serial.print(F("Video RAM test: "));
    if (videoTest == 0x55)
    {
        Serial.println(F("PASS (R/W accessible)"));
    }
    else
    {
        Serial.println(F("FAIL (not writable)"));
    }

    // Test user RAM
    uint8_t originalRAM = Model1.readMemory(0x4000);
    Model1.writeMemory(0x4000, 0xAA);
    uint8_t ramTest = Model1.readMemory(0x4000);
    Model1.writeMemory(0x4000, originalRAM);

    Serial.print(F("User RAM test: "));
    if (ramTest == 0xAA)
    {
        Serial.println(F("PASS (R/W accessible)"));
    }
    else
    {
        Serial.println(F("FAIL (not writable)"));
    }
}

void demonstratePerformanceAnalysis()
{
    Serial.println(F("--- Performance Analysis ---"));

    // Memory access timing analysis
    Serial.println(F("Memory access timing analysis:"));

    uint16_t testAddr = 0x4000;
    const int iterations = 1000;

    // Single byte write timing
    unsigned long startTime = micros();
    for (int i = 0; i < iterations; i++)
    {
        Model1.writeMemory(testAddr, (uint8_t)(i & 0xFF));
    }
    unsigned long writeTime = micros() - startTime;

    // Single byte read timing
    startTime = micros();
    for (int i = 0; i < iterations; i++)
    {
        volatile uint8_t dummy = Model1.readMemory(testAddr);
        (void)dummy; // Suppress unused variable warning
    }
    unsigned long readTime = micros() - startTime;

    Serial.print(F("Single byte write ("));
    Serial.print(iterations);
    Serial.print(F(" ops): "));
    Serial.print((float)writeTime / iterations, 2);
    Serial.println(F(" μs/op"));

    Serial.print(F("Single byte read ("));
    Serial.print(iterations);
    Serial.print(F(" ops): "));
    Serial.print((float)readTime / iterations, 2);
    Serial.println(F(" μs/op"));

    // I/O timing analysis
    Serial.println(F("I/O operation timing:"));

    startTime = micros();
    for (int i = 0; i < iterations; i++)
    {
        Model1.writeIO(0x3F, (uint8_t)(i & 0xFF));
    }
    unsigned long ioWriteTime = micros() - startTime;

    startTime = micros();
    for (int i = 0; i < iterations; i++)
    {
        volatile uint8_t dummy = Model1.readIO(0x3E);
        (void)dummy;
    }
    unsigned long ioReadTime = micros() - startTime;

    Serial.print(F("I/O write ("));
    Serial.print(iterations);
    Serial.print(F(" ops): "));
    Serial.print((float)ioWriteTime / iterations, 2);
    Serial.println(F(" μs/op"));

    Serial.print(F("I/O read ("));
    Serial.print(iterations);
    Serial.print(F(" ops): "));
    Serial.print((float)ioReadTime / iterations, 2);
    Serial.println(F(" μs/op"));

    // Calculate theoretical bandwidth
    float memoryBandwidth = 1000000.0 / ((float)(writeTime + readTime) / (iterations * 2));
    Serial.print(F("Theoretical memory bandwidth: "));
    Serial.print(memoryBandwidth, 0);
    Serial.println(F(" bytes/second"));

    systemState.lastOperationTime = millis();
}
