#ifndef RAMTESTSUITE_H
#define RAMTESTSUITE_H

#include <ConsoleScreen.h>

struct TestResult
{
    uint32_t totalErrors;
    uint32_t bitErrors[8]; // bitErrors[0] = failures of bit 0, etc.
};

struct TestSuiteResult
{
    TestResult repeatedWriteNormal;
    TestResult repeatedWriteInverted;
    TestResult repeatedReadNormal;
    TestResult repeatedReadInverted;
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

class RAMTestSuite : public ConsoleScreen
{
public:
    RAMTestSuite();

    // Public method for specialized test suites
    void runSpecializedTest(uint16_t start, uint16_t length, const char *const icRefs[]);

protected:
    void runAndEvaluate(uint16_t start, uint16_t length, const char *const icRefs[]);

private:
    TestResult runRepeatedWriteTest(uint16_t start, uint16_t length, bool toggleStart);
    TestResult runRepeatedReadTest(uint16_t start, uint16_t length, bool toggleStart);
    TestResult runCheckerboardTest(uint16_t start, uint16_t length, bool toggleStart);
    TestResult runWalkingOnesTest(uint16_t start, uint16_t length);
    TestResult runWalkingZerosTest(uint16_t start, uint16_t length);
    TestResult runMarchCTest(uint16_t start, uint16_t length);
    TestResult runMovingInversionTest(uint16_t start, uint16_t length, uint8_t pattern);
    TestResult runRetentionTest(uint16_t start, uint16_t length, uint8_t pattern, uint32_t delayMs, uint8_t repeatDelay);
    TestResult runMarchSSTest(uint16_t start, uint16_t length);
    TestResult runMarchLATest(uint16_t start, uint16_t length);
    TestResult runReadDestructiveTest(uint16_t start, uint16_t length, uint8_t pattern, uint8_t numReads);
    TestResult runAddressUniquenessTest(uint16_t start, uint16_t length, uint8_t pattern);
};

#endif
