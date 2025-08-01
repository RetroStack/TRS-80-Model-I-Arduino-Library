#ifndef VIDEOVIEWPORTDEMO_H
#define VIDEOVIEWPORTDEMO_H

#include <ConsoleScreen.h>
#include <Video.h>
#include <Cassette.h>

class VideoViewPortDemo : public ConsoleScreen
{
private:
    unsigned long _startTime;
    int _demoStep;
    bool _demoComplete;
    Video *_video;
    Cassette *_cassette;
    bool _viewPortSet;

public:
    VideoViewPortDemo();
    ~VideoViewPortDemo();
    bool open() override;
    void close() override;
    void loop() override;
    Screen *actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY) override;

protected:
    void _executeOnce() override;

private:
    void runViewPortDemo();
    void showDemoComplete();
};

#endif
