#include <Arduino.h>
#include <Model1.h>
#include <SerialLogger.h>

#define INIT_TEST_RESULT TestResult result = {}
#define UPDATE_ERRORS(diff)             \
    if (diff != 0)                      \
    {                                   \
        result.totalErrors++;           \
        for (uint8_t b = 0; b < 8; b++) \
        {                               \
            if (diff & (1 << b))        \
                result.bitErrors[b]++;  \
        }                               \
    }

struct TestResult
{
    uint32_t totalErrors;
    uint32_t bitErrors[8]; // bitErrors[0] = failures of bit 0, etc.
};

struct TestSuiteResult
{
    TestResult checkerboardNormal;
    TestResult checkerboardInverted;
    TestResult walkingOnes;
    TestResult walkingZeros;
    TestResult marchC;
    TestResult movingInversionZero;
    TestResult movingInversion55;
    TestResult movingInversionRandom;
    TestResult marchSS;
    TestResult marchLA;
    TestResult readDestructiveAA;
    TestResult readDestructive55;
    TestResult addressUniqueness55;
    TestResult addressUniquenessAA;
    TestResult retention;
};

// Define an accessible variable for the Model 1 instance
Model1 *model1;

// Define the serial logger
SerialLogger *logger;

void setup()
{
    // Setup the serial interface
    Serial.begin(500000); // baud rate

    // Initialize the serial logger
    logger = new SerialLogger();

    // Initialize a Model 1 instance to access it
    model1 = new Model1(logger);

    // Be sure to configure the DRAM refresh and setup the hardware pins
    // It uses the Timer 2, but you can set this to false and setup your own timer.
    model1->begin(2); // Timer 2

    // You can also use Timer 1, but this is often used for lots of other things
    // Timer 0 is not allowed and won't do anything. -1 turns this off.

    Serial.println("Started");

    // Give the system a bit of time to start up
    delay(5000);

    // Activate the test signal
    Serial.println("Activated");
    model1->activateTestSignal();
}

// Define a callback for Timer 2
ISR(TIMER2_COMPA_vect)
{
    // Trigger a refresh to happen
    model1->refreshNextMemoryRow();
}

TestResult runCheckerboardTest(uint16_t start, uint16_t length, bool toggleStart)
{
    uint8_t data;
    bool toggle;
    INIT_TEST_RESULT;

    Serial.print("Checkerboard Test");
    if (toggleStart)
    {
        Serial.print("(normal)");
    }
    else
    {
        Serial.print("(inverted)");
    }

    Serial.print(".");
    for (uint16_t i = 0, toggle = toggleStart; i < length; i++)
    {
        if (toggle)
        {
            model1->writeMemory(start + i, 0x55);
        }
        else
        {
            model1->writeMemory(start + i, 0xAA);
        }
    }
    Serial.print(".");
    for (uint16_t i = 0, toggle = toggleStart; i < length; i++)
    {
        data = model1->readMemory(start + i);
        if (toggle)
        {
            uint8_t diff = data ^ 0x55;
            UPDATE_ERRORS(diff);
        }
        else
        {
            uint8_t diff = data ^ 0xAA;
            UPDATE_ERRORS(diff);
        }
    }
    Serial.println();

    return result;
}

TestResult runWalkingOnesTest(uint16_t start, uint16_t length)
{
    uint8_t data;
    INIT_TEST_RESULT;

    Serial.print("Walking Ones Test");

    for (uint8_t bit = 0; bit < 8; bit++)
    {
        uint8_t pattern = 1 << bit;
        Serial.print(".");

        // Write
        for (uint16_t i = 0; i < length; i++)
        {
            model1->writeMemory(start + i, pattern);
        }

        // Read
        for (uint16_t i = 0; i < length; i++)
        {
            data = model1->readMemory(start + i);
            uint8_t diff = data ^ pattern;
            UPDATE_ERRORS(diff);
        }
    }
    Serial.println();

    return result;
}

TestResult runWalkingZerosTest(uint16_t start, uint16_t length)
{
    uint8_t data;
    INIT_TEST_RESULT;

    Serial.print("Walking Zeros Test");

    for (uint8_t bit = 0; bit < 8; bit++)
    {
        uint8_t pattern = ~(1 << bit);
        Serial.print(".");

        // Write
        for (uint16_t i = 0; i < length; i++)
        {
            model1->writeMemory(start + i, pattern);
        }

        // Read
        for (uint16_t i = 0; i < length; i++)
        {
            data = model1->readMemory(start + i);
            uint8_t diff = data ^ pattern;
            UPDATE_ERRORS(diff);
        }
    }
    Serial.println();

    return result;
}

TestResult runMarchCTest(uint16_t start, uint16_t length)
{
    uint8_t data;
    INIT_TEST_RESULT;

    Serial.print("March C- Test");

    Serial.print(".");
    for (uint16_t i = 0; i < length; i++)
    {
        model1->writeMemory(start + i, 0x00);
    }

    Serial.print(".");
    for (uint16_t i = 0; i < length; i++)
    {
        data = model1->readMemory(start + i);
        uint8_t diff = data ^ 0x00;
        UPDATE_ERRORS(diff);
        model1->writeMemory(start + i, 0xFF);
    }

    Serial.print(".");
    for (int16_t i = length - 1; i >= 0; i--)
    {
        data = model1->readMemory(start + i);
        uint8_t diff = data ^ 0xFF;
        UPDATE_ERRORS(diff);
        model1->writeMemory(start + i, 0x00);
    }

    Serial.print(".");
    for (int16_t i = length - 1; i >= 0; i--)
    {
        data = model1->readMemory(start + i);
        uint8_t diff = data ^ 0x00;
        UPDATE_ERRORS(diff);
    }
    Serial.println();

    return result;
}

TestResult runMovingInversionTest(uint16_t start, uint16_t length, uint8_t pattern)
{
    uint8_t data;
    INIT_TEST_RESULT;

    Serial.print("Moving Inversion Test (0x");
    Serial.print(pattern, HEX);
    Serial.print(")");

    // First pattern: 0x00
    uint8_t antipattern = ~pattern;

    Serial.print(".");
    for (uint16_t i = 0; i < length; i++)
    {
        model1->writeMemory(start + i, pattern);
    }

    Serial.print(".");
    for (uint16_t i = 0; i < length; i++)
    {
        data = model1->readMemory(start + i);
        uint8_t diff = data ^ pattern;
        UPDATE_ERRORS(diff);
        model1->writeMemory(start + i, antipattern);
    }

    Serial.print(".");
    for (uint16_t i = 0; i < length; i++)
    {
        data = model1->readMemory(start + i);
        uint8_t diff = data ^ antipattern;
        UPDATE_ERRORS(diff);
        model1->writeMemory(start + i, pattern);
    }

    Serial.print(".");
    for (uint16_t i = 0; i < length; i++)
    {
        data = model1->readMemory(start + i);
        uint8_t diff = data ^ pattern;
        UPDATE_ERRORS(diff);
    }
    Serial.println();

    return result;
}

TestResult runRetentionTest(uint16_t start, uint16_t length, uint8_t pattern, uint32_t delayMs, uint8_t repeatDelay)
{
    uint8_t data;
    INIT_TEST_RESULT;

    Serial.print("Retention Test (0x");
    Serial.print(pattern, HEX);
    Serial.print(")");

    // Fill with pattern
    Serial.print(".");
    for (uint16_t i = 0; i < length; i++)
    {
        model1->writeMemory(start + i, pattern);
    }

    for (uint16_t i = 0; i < repeatDelay; i++)
    {
        Serial.print(".");
        delay(delayMs);
    }
    // Verify
    Serial.print(".");
    for (uint16_t i = 0; i < length; i++)
    {
        data = model1->readMemory(start + i);
        uint8_t diff = data ^ pattern;
        UPDATE_ERRORS(diff);
    }
    Serial.println();

    return result;
}

TestResult runMarchSSTest(uint16_t start, uint16_t length)
{
    uint8_t data;
    INIT_TEST_RESULT;

    Serial.print("March SS Test");

    // Phase 1: Up write 0
    Serial.print(".");
    for (uint16_t i = 0; i < length; i++)
    {
        model1->writeMemory(start + i, 0x00);
    }

    // Phase 2: Up read 0, write 1
    Serial.print(".");
    for (uint16_t i = 0; i < length; i++)
    {
        data = model1->readMemory(start + i);
        uint8_t diff = data ^ 0x00;
        UPDATE_ERRORS(diff);
        model1->writeMemory(start + i, 0xFF);
    }

    // Phase 3: Down read 1, write 0
    Serial.print(".");
    for (int16_t i = length - 1; i >= 0; i--)
    {
        data = model1->readMemory(start + i);
        uint8_t diff = data ^ 0xFF;
        UPDATE_ERRORS(diff);
        model1->writeMemory(start + i, 0x00);
    }

    // Phase 4: Down read 0, write 1
    Serial.print(".");
    for (int16_t i = length - 1; i >= 0; i--)
    {
        data = model1->readMemory(start + i);
        uint8_t diff = data ^ 0x00;
        UPDATE_ERRORS(diff);
        model1->writeMemory(start + i, 0xFF);
    }

    // Phase 5: Up read 1, write 0
    Serial.print(".");
    for (uint16_t i = 0; i < length; i++)
    {
        data = model1->readMemory(start + i);
        uint8_t diff = data ^ 0xFF;
        UPDATE_ERRORS(diff);
        model1->writeMemory(start + i, 0x00);
    }

    // Phase 6: Up read 0
    Serial.print(".");
    for (uint16_t i = 0; i < length; i++)
    {
        data = model1->readMemory(start + i);
        uint8_t diff = data ^ 0x00;
        UPDATE_ERRORS(diff);
    }
    Serial.println();

    return result;
}

TestResult runMarchLATest(uint16_t start, uint16_t length)
{
    uint8_t data;
    INIT_TEST_RESULT;

    Serial.print("March LA Test");

    // Phase 1: Up write 0
    Serial.print(".");
    for (uint16_t i = 0; i < length; i++)
    {
        model1->writeMemory(start + i, 0x00);
    }

    // Phase 2: Up read 0, write 1
    Serial.print(".");
    for (uint16_t i = 0; i < length; i++)
    {
        data = model1->readMemory(start + i);
        uint8_t diff = data ^ 0x00;
        UPDATE_ERRORS(diff);
        model1->writeMemory(start + i, 0xFF);
    }

    // Phase 3: Down read 1, write 0
    Serial.print(".");
    for (int32_t i = length - 1; i >= 0; i--)
    {
        data = model1->readMemory(start + i);
        uint8_t diff = data ^ 0xFF;
        UPDATE_ERRORS(diff);
        model1->writeMemory(start + i, 0x00);
    }

    // Phase 4: Down read 0
    Serial.print(".");
    for (int32_t i = length - 1; i >= 0; i--)
    {
        data = model1->readMemory(start + i);
        uint8_t diff = data ^ 0x00;
        UPDATE_ERRORS(diff);
    }
    Serial.println();

    return result;
}

TestResult runReadDestructiveTest(uint16_t start, uint16_t length, uint8_t pattern, uint8_t numReads)
{
    uint8_t data;
    INIT_TEST_RESULT;

    Serial.print("Read Destructive Fault Test (pattern 0x");
    Serial.print(pattern, HEX);
    Serial.print(", reads: ");
    Serial.print(numReads);
    Serial.print(")");

    // 1. Fill memory with pattern
    Serial.print(".");
    for (uint16_t i = 0; i < length; i++)
    {
        model1->writeMemory(start + i, pattern);
    }

    // 2. Repeatedly read the same cell without re-writing
    for (uint16_t i = 0; i < length; i++)
    {
        for (uint8_t r = 0; r < numReads; r++)
        {
            data = model1->readMemory(start + i);
            uint8_t diff = data ^ pattern;
            UPDATE_ERRORS(diff);
            if (diff != 0)
            {
                break;
            }
        }
    }
    Serial.println();

    return result;
}

TestResult runAddressUniquenessTest(uint16_t start, uint16_t length, uint8_t pattern)
{
    uint8_t data;
    INIT_TEST_RESULT;

    Serial.print("Address Uniqueness Test (XOR pattern 0x");
    Serial.print(pattern, HEX);
    Serial.print(")");

    // Phase 1: Write unique XOR pattern
    Serial.print(".");
    for (uint16_t i = 0; i < length; i++)
    {
        uint8_t value = ((uint8_t)(i & 0xFF)) ^ pattern;
        model1->writeMemory(start + i, value);
    }

    // Phase 2: Verify
    Serial.print(".");
    for (uint16_t i = 0; i < length; i++)
    {
        uint8_t expected = ((uint8_t)(i & 0xFF)) ^ pattern;
        data = model1->readMemory(start + i);
        uint8_t diff = data ^ expected;
        UPDATE_ERRORS(diff);
    }
    Serial.println();

    return result;
}

TestSuiteResult runMemoryTestSuite(uint16_t start, uint16_t length)
{
    TestSuiteResult suite = {};

    suite.checkerboardNormal = runCheckerboardTest(start, length, true);
    suite.checkerboardInverted = runCheckerboardTest(start, length, false);
    suite.walkingOnes = runWalkingOnesTest(start, length);
    suite.walkingZeros = runWalkingZerosTest(start, length);
    suite.marchC = runMarchCTest(start, length);
    suite.movingInversionZero = runMovingInversionTest(start, length, 0x00);
    suite.movingInversion55 = runMovingInversionTest(start, length, 0x55);
    suite.movingInversionRandom = runMovingInversionTest(start, length, random(0, 255));
    suite.marchSS = runMarchSSTest(start, length);
    suite.marchLA = runMarchLATest(start, length);
    suite.readDestructiveAA = runReadDestructiveTest(start, length, 0xAA, 5);
    suite.readDestructive55 = runReadDestructiveTest(start, length, 0x55, 5);
    suite.addressUniqueness55 = runAddressUniquenessTest(start, length, 0x55);
    suite.addressUniquenessAA = runAddressUniquenessTest(start, length, 0xAA);
    suite.retention = runRetentionTest(start, length, 0xFF, 1000, 5);

    return suite;
}

void printResult(const char *name, const TestResult &r, const char *icRefs[8])
{
    Serial.print(name);
    Serial.print(" - Total Errors: ");
    Serial.println(r.totalErrors);
    for (uint8_t b = 0; b < 8; b++)
    {
        Serial.print("  Bit ");
        Serial.print(b);
        Serial.print(" (");
        Serial.print(icRefs[b]);
        Serial.print("): ");
        Serial.println(r.bitErrors[b]);
    }
}

void runAndEvaluate(uint16_t start, uint16_t length, const char *icRefs[8])
{
    Serial.println("=== START MEMORY TEST SUITE ===");

    TestSuiteResult suite = runMemoryTestSuite(start, length);

    // Serial.println();
    // Serial.println("--- Per-Test Results ---");

    // printResult("Checkerboard Normal", suite.checkerboardNormal, icRefs);
    // printResult("Checkerboard Inverted", suite.checkerboardInverted, icRefs);
    // printResult("Walking Ones", suite.walkingOnes, icRefs);
    // printResult("Walking Zeros", suite.walkingZeros, icRefs);
    // printResult("March C-", suite.marchC, icRefs);
    // printResult("Moving Inversion 0x00", suite.movingInversionZero, icRefs);
    // printResult("Moving Inversion 0x55", suite.movingInversion55, icRefs);
    // printResult("Moving Inversion Random", suite.movingInversionRandom, icRefs);
    // printResult("March SS", suite.marchSS, icRefs);
    // printResult("March LA", suite.marchLA, icRefs);
    // printResult("Read Destructive 0xAA", suite.readDestructiveAA, icRefs);
    // printResult("Read Destructive 0x55", suite.readDestructive55, icRefs);
    // printResult("Address Uniqueness 0x55", suite.addressUniqueness55, icRefs);
    // printResult("Address Uniqueness 0xAA", suite.addressUniquenessAA, icRefs);
    // printResult("Retention", suite.retention, icRefs);

    Serial.println();
    Serial.println("--- Aggregate Summary ---");

    // 1) Initialize totals
    uint32_t totalBitErrors[8] = {};
    uint32_t totalErrorsOverall = 0;

    // 2) Create a pointer array to all TestResults for iteration
    const TestResult *allResults[] = {
        &suite.checkerboardNormal,
        &suite.checkerboardInverted,
        &suite.walkingOnes,
        &suite.walkingZeros,
        &suite.marchC,
        &suite.movingInversionZero,
        &suite.movingInversion55,
        &suite.movingInversionRandom,
        &suite.marchSS,
        &suite.marchLA,
        &suite.readDestructiveAA,
        &suite.readDestructive55,
        &suite.addressUniqueness55,
        &suite.addressUniquenessAA,
        &suite.retention};

    // 3) Sum all bit errors
    for (uint8_t t = 0; t < sizeof(allResults) / sizeof(allResults[0]); t++)
    {
        const TestResult *r = allResults[t];
        totalErrorsOverall += r->totalErrors;
        for (uint8_t b = 0; b < 8; b++)
        {
            totalBitErrors[b] += r->bitErrors[b];
        }
    }

    // 4) Print per-bit totals
    for (uint8_t b = 0; b < 8; b++)
    {
        Serial.print("Bit ");
        Serial.print(b);
        Serial.print(" (");
        Serial.print(icRefs[b]);
        Serial.print("): ");
        Serial.println(totalBitErrors[b]);
    }

    Serial.print("Total Errors Across All Tests: ");
    Serial.println(totalErrorsOverall);

    Serial.println("=== END MEMORY TEST SUITE ===");
}

// DRAM
uint16_t dramStart = 0x4000;
uint16_t dramLength = 1024 * 16;

const char *dramICs[8] = {
    "Z17", "Z16", "Z18", "Z19", "Z15", "Z20", "Z14", "Z13"};

// VRAM
uint16_t vramStart = 0x3C00;
uint16_t vramLength = 1024;

const char *vramICs[8] = {
    "Z48", "Z47", "Z46", "Z45", "Z61", "Z62", "Z?", "Z63"};

void loop()
{
    delay(1000);

    model1->writeMemory(0x3C00, 0x61);
    uint8_t data = model1->readMemory(0x3C00);
    if (data == 0x21)
    { // Shifted in page due to circuitry
        Serial.println("Most likely a Model 1 without lower-case mod.");
    }
    else
    {
        Serial.println("A model 1 with lower-case mod.");
    }

    // SAF - Stuck At Fault (SAF0 or SAF1) -> Cell always reads 0 or 1
    // TF - Transition Fault -> Cell cannot change state in one or both directions
    // AF - Address Fault -> Wrong cell is accessed or modified
    // CF - Coupling Fault
    // - id  = Inversion-Dominating Coupling Fault - Writing aggressor inverts victim
    // - den = Dominating Coupling Fault - Dynamic EnableWriting aggressor forces victim to fixed value
    // SCF - State Coupling Fault - Aggressor's state holds victim in state

    Serial.println("====================================");
    Serial.println("VRAM Tests");
    runAndEvaluate(vramStart, vramLength, vramICs);

    Serial.println("====================================");
    Serial.println("DRAM Tests");
    runAndEvaluate(dramStart, dramLength, dramICs);

    Serial.println("====================================");
    Serial.println("====================================");
    Serial.println("====================================");
    Serial.println("====================================");
    Serial.println("====================================");
    Serial.println("====================================");
    Serial.println("====================================");
    Serial.println("====================================");
    Serial.println("====================================");
    Serial.println("====================================");
    Serial.println("====================================");
}
