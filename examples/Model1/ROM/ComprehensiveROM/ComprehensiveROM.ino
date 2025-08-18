#include <Arduino.h>
#include <Model1.h>
#include <ROM.h>
#include <SerialLogger.h>

// ROM class instance for reading TRS-80 system ROMs
ROM rom;

// Logger for detailed debugging information
SerialLogger logger;

// ROM analysis data structure
struct ROMAnalysis
{
    uint16_t checksum;
    bool isPresent;
    bool isValid;
    uint16_t startAddress;
    uint16_t endAddress;
    size_t size;
};

void setup()
{
    // Initialize serial communication
    Serial.begin(9600);
    delay(1000);

    // Initialize the Model 1 hardware interface
    Model1.begin();

    // Set up logging for detailed ROM operations
    Model1.setLogger(logger);
    rom.setLogger(logger);

    // Take control of the TRS-80 bus
    Model1.activateTestSignal();

    Serial.println(F("=== Comprehensive ROM Example ==="));
    Serial.println(F("Advanced ROM analysis and diagnostics"));
    Serial.println();
}

void loop()
{
    // Comprehensive system analysis
    demonstrateComprehensiveSystemAnalysis();
    delay(4000);

    // ROM content analysis
    demonstrateROMContentAnalysis();
    delay(4000);

    // ROM memory mapping
    demonstrateROMMemoryMapping();
    delay(4000);

    // ROM integrity verification
    demonstrateROMIntegrityCheck();
    delay(4000);

    // ROM comparison and differences
    demonstrateROMComparison();
    delay(6000);
}

void demonstrateComprehensiveSystemAnalysis()
{
    Serial.println(F("\\n--- Comprehensive System Analysis ---"));

    // Detailed system identification
    const __FlashStringHelper *systemName = rom.identifyROM();

    Serial.print(F("System identification: "));
    if (systemName)
    {
        Serial.println(systemName);
    }
    else
    {
        Serial.println(F("Unknown/Custom"));
    }

    // Analyze each ROM slot in detail
    ROMAnalysis analysis[4];

    for (int slot = 0; slot < 4; slot++)
    {
        analysis[slot].checksum = rom.getChecksum(slot);
        analysis[slot].startAddress = 0x0000 + (slot * 0x0800); // Each ROM is 2KB
        analysis[slot].endAddress = analysis[slot].startAddress + 0x07FF;
        analysis[slot].size = 0x0800; // 2KB per ROM
        analysis[slot].isPresent = (analysis[slot].checksum != 0x0000 &&
                                    analysis[slot].checksum != 0xFFFF);
        analysis[slot].isValid = analysis[slot].isPresent; // Basic validity check

        Serial.print(F("ROM "));
        Serial.print((char)('A' + slot));
        Serial.print(F(": "));
        Serial.print(F("0x"));
        Serial.print(analysis[slot].startAddress, HEX);
        Serial.print(F("-0x"));
        Serial.print(analysis[slot].endAddress, HEX);
        Serial.print(F(" | Checksum: 0x"));
        if (analysis[slot].checksum < 0x1000)
            Serial.print('0');
        if (analysis[slot].checksum < 0x100)
            Serial.print('0');
        if (analysis[slot].checksum < 0x10)
            Serial.print('0');
        Serial.print(analysis[slot].checksum, HEX);
        Serial.print(F(" | Status: "));
        Serial.println(analysis[slot].isPresent ? F("Present") : F("Missing"));
    }

    // System summary
    int presentROMs = 0;
    for (int i = 0; i < 4; i++)
    {
        if (analysis[i].isPresent)
            presentROMs++;
    }

    Serial.print(F("\\nSummary: "));
    Serial.print(presentROMs);
    Serial.println(F(" ROM(s) detected"));

    if (presentROMs >= 3)
    {
        Serial.println(F("System appears complete for operation"));
    }
    else if (presentROMs >= 1)
    {
        Serial.println(F("Partial system - may have limited functionality"));
    }
    else
    {
        Serial.println(F("No ROMs detected - system non-functional"));
    }
}

void demonstrateROMContentAnalysis()
{
    Serial.println(F("\\n--- ROM Content Analysis ---"));

    // Analyze ROM A content in detail (most critical ROM)
    Serial.println(F("Analyzing ROM A content:"));

    // Print first 64 bytes of ROM A as hex dump
    Serial.println(F("\\nFirst 64 bytes (hex dump):"));
    Serial.println(F("Addr  | 00 01 02 03 04 05 06 07 | ASCII"));
    Serial.println(F("------|-------------------------|--------"));

    for (int row = 0; row < 8; row++)
    {
        uint16_t address = 0x0000 + (row * 8);
        Serial.print(F("0x"));
        if (address < 0x1000)
            Serial.print('0');
        if (address < 0x100)
            Serial.print('0');
        if (address < 0x10)
            Serial.print('0');
        Serial.print(address, HEX);
        Serial.print(F(" | "));

        // Print hex bytes
        for (int col = 0; col < 8; col++)
        {
            uint8_t byte = Model1.readMemory(address + col);
            if (byte < 0x10)
                Serial.print('0');
            Serial.print(byte, HEX);
            Serial.print(' ');
        }

        Serial.print(F("| "));

        // Print ASCII representation
        for (int col = 0; col < 8; col++)
        {
            uint8_t byte = Model1.readMemory(address + col);
            if (byte >= 32 && byte <= 126)
                Serial.print((char)byte);
            else
                Serial.print('.');
        }

        Serial.println();
    }

    // Look for common ROM signatures
    Serial.println(F("\\nLooking for ROM signatures:"));

    // Check for copyright strings or version info
    uint8_t firstBytes[16];
    for (int i = 0; i < 16; i++)
    {
        firstBytes[i] = Model1.readMemory(0x0000 + i);
    }

    // Look for jump instructions or reset vectors
    if (firstBytes[0] == 0xC3 || firstBytes[0] == 0xF3)
    {
        Serial.println(F("ROM appears to start with valid Z80 instructions"));
    }
    else
    {
        Serial.println(F("ROM start does not match expected Z80 boot pattern"));
    }
}

void demonstrateROMMemoryMapping()
{
    Serial.println(F("\\n--- ROM Memory Mapping ---"));

    Serial.println(F("TRS-80 Model I ROM Memory Map:"));
    Serial.println(F("ROM A: 0x0000-0x07FF (2KB) - System Bootstrap"));
    Serial.println(F("ROM B: 0x0800-0x0FFF (2KB) - BASIC Interpreter"));
    Serial.println(F("ROM C: 0x1000-0x17FF (2KB) - BASIC Interpreter"));
    Serial.println(F("ROM D: 0x1800-0x1FFF (2KB) - BASIC Interpreter"));
    Serial.println();

    // Verify memory mapping by reading from different ROM areas
    Serial.println(F("Memory mapping verification:"));

    for (int rom = 0; rom < 4; rom++)
    {
        uint16_t testAddress = 0x0000 + (rom * 0x0800);
        uint8_t testByte = Model1.readMemory(testAddress);

        Serial.print(F("ROM "));
        Serial.print((char)('A' + rom));
        Serial.print(F(" @ 0x"));
        Serial.print(testAddress, HEX);
        Serial.print(F(": 0x"));
        if (testByte < 0x10)
            Serial.print('0');
        Serial.println(testByte, HEX);
    }
}

void demonstrateROMIntegrityCheck()
{
    Serial.println(F("\\n--- ROM Integrity Check ---"));

    // Perform multiple checksum calculations to verify consistency
    Serial.println(F("Multiple checksum verification:"));

    for (int rom = 0; rom < 4; rom++)
    {
        Serial.print(F("ROM "));
        Serial.print((char)('A' + rom));
        Serial.print(F(": "));

        uint16_t checksum1 = rom.getChecksum(rom);
        delay(10); // Small delay between reads
        uint16_t checksum2 = rom.getChecksum(rom);
        delay(10);
        uint16_t checksum3 = rom.getChecksum(rom);

        if (checksum1 == checksum2 && checksum2 == checksum3)
        {
            Serial.print(F("STABLE (0x"));
            if (checksum1 < 0x1000)
                Serial.print('0');
            if (checksum1 < 0x100)
                Serial.print('0');
            if (checksum1 < 0x10)
                Serial.print('0');
            Serial.print(checksum1, HEX);
            Serial.println(F(")"));
        }
        else
        {
            Serial.print(F("UNSTABLE - checksums vary: 0x"));
            Serial.print(checksum1, HEX);
            Serial.print(F(", 0x"));
            Serial.print(checksum2, HEX);
            Serial.print(F(", 0x"));
            Serial.println(checksum3, HEX);
        }
    }
}

void demonstrateROMComparison()
{
    Serial.println(F("\\n--- ROM Comparison Analysis ---"));

    // Compare ROM contents to identify patterns
    Serial.println(F("Analyzing ROM differences:"));

    uint16_t checksums[4];
    for (int i = 0; i < 4; i++)
    {
        checksums[i] = rom.getChecksum(i);
    }

    // Find identical ROMs
    for (int i = 0; i < 4; i++)
    {
        for (int j = i + 1; j < 4; j++)
        {
            if (checksums[i] == checksums[j] && checksums[i] != 0x0000 && checksums[i] != 0xFFFF)
            {
                Serial.print(F("ROM "));
                Serial.print((char)('A' + i));
                Serial.print(F(" and ROM "));
                Serial.print((char)('A' + j));
                Serial.println(F(" appear identical"));
            }
        }
    }

    // Display full ROM contents for ROM A (if present)
    if (checksums[0] != 0x0000 && checksums[0] != 0xFFFF)
    {
        Serial.println(F("\\nDisplaying ROM A contents:"));
        rom.printROMContents(0);
    }
    else
    {
        Serial.println(F("\\nROM A not present - skipping content display"));
    }
}
