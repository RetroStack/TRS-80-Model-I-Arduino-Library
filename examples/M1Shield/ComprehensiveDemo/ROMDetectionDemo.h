#ifndef ROMDETECTIONDEMO_H
#define ROMDETECTIONDEMO_H

#include <ConsoleScreen.h>
#include <ROM.h>

class ROMDetectionDemo : public ConsoleScreen
{
private:
    ROM _rom;
    bool _detectionComplete;
    unsigned long _startTime;
    int _progressStep;

public:
    ROMDetectionDemo();
    void loop() override;
    Screen *actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY) override;

private:
    void runROMDetection();
    void displayResults();
};

#endif
