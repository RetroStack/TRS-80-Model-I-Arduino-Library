#ifndef VIDEOFULLSCREENDEMO_H
#define VIDEOFULLSCREENDEMO_H

#include <ConsoleScreen.h>
#include <Video.h>
#include <Cassette.h>

class VideoFullScreenDemo : public ConsoleScreen
{
private:
    unsigned long _startTime;
    int _demoStep;
    bool _demoComplete;
    Video *_video;
    Cassette *_cassette;

public:
    VideoFullScreenDemo();
    ~VideoFullScreenDemo();
    bool open() override;
    void close() override;
    void loop() override;
    Screen *actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY) override;

protected:
    void _executeOnce() override;

private:
    void runVideoDemo();
    void showDemoComplete();
};

#endif
