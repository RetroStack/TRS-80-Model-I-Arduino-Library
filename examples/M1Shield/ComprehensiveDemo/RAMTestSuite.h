#ifndef RAMTESTSUITE_H
#define RAMTESTSUITE_H

#include <ConsoleScreen.h>

class RAMTestSuite : public ConsoleScreen
{
private:
    bool _testsRunning;
    unsigned long _startTime;
    int _currentTest;
    int _totalTests;
    int _progressStep;

public:
    RAMTestSuite();
    void loop() override;
    Screen *actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY) override;

private:
    void runRAMTests();
    void displayTestResults();
};

#endif
