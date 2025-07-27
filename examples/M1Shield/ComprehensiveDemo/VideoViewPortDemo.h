#ifndef VIDEOVIEWPORTDEMO_H
#define VIDEOVIEWPORTDEMO_H

#include <ConsoleScreen.h>
#include <Video.h>

class VideoViewPortDemo : public ConsoleScreen
{
private:
    Video _video;
    unsigned long _startTime;
    int _demoStep;
    bool _demoComplete;

public:
    VideoViewPortDemo();
    void loop() override;
    Screen *actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY) override;

private:
    void runViewPortDemo();
    void showDemoComplete();
};

#endif
