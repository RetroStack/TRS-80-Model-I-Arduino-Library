#ifndef VIDEOFULLSCREENDEMO_H
#define VIDEOFULLSCREENDEMO_H

#include <ConsoleScreen.h>
#include <Video.h>

class VideoFullScreenDemo : public ConsoleScreen
{
private:
    Video _video;
    unsigned long _startTime;
    int _demoStep;
    bool _demoComplete;

public:
    VideoFullScreenDemo();
    void loop() override;
    Screen *actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY) override;

private:
    void runVideoDemo();
    void showDemoComplete();
};

#endif
