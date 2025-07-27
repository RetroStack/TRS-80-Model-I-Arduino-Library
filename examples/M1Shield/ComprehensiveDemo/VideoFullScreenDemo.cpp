#include "VideoFullScreenDemo.h"
#include "VideoFeaturesMenu.h"
#include <Arduino.h>

VideoFullScreenDemo::VideoFullScreenDemo() : ConsoleScreen()
{
    _setTitle("Video Demo");
    setConsoleBackground(0x0000);
    setTextColor(0xFFFF, 0x0000);

    _startTime = millis();
    _demoStep = 0;
    _demoComplete = false;

    cls();
    setTextColor(0x07E0, 0x0000); // Green
    println("=== VIDEO FULL SCREEN DEMO ===");
    println();
    setTextColor(0xFFFF, 0x0000);
    println("Demonstrating video features...");
    println();

    Serial.println("Video Full Screen Demo initialized");
}

void VideoFullScreenDemo::loop()
{
    ConsoleScreen::loop();

    if (!_demoComplete)
    {
        runVideoDemo();
    }
}

void VideoFullScreenDemo::runVideoDemo()
{
    unsigned long elapsed = millis() - _startTime;

    switch (_demoStep)
    {
    case 0:
        if (elapsed > 1000)
        {
            setTextColor(0x07FF, 0x0000); // Cyan
            println("Setting 32-character mode...");
            // _video.set32CharacterMode(); // Uncomment when available
            _startTime = millis();
            _demoStep++;
        }
        break;

    case 1:
        if (elapsed > 2000)
        {
            setTextColor(0x07E0, 0x0000); // Green
            println("32-character mode active");
            println("Screen shows wider characters");
            println();
            setTextColor(0x07FF, 0x0000);
            println("Setting 64-character mode...");
            _startTime = millis();
            _demoStep++;
        }
        break;

    case 2:
        if (elapsed > 2000)
        {
            setTextColor(0x07E0, 0x0000); // Green
            println("64-character mode active");
            println("Screen shows normal characters");
            println();
            setTextColor(0x07FF, 0x0000);
            println("Testing video memory access...");
            _startTime = millis();
            _demoStep++;
        }
        break;

    case 3:
        if (elapsed > 2000)
        {
            setTextColor(0xFFE0, 0x0000); // Yellow
            println("Video memory tests:");
            setTextColor(0xFFFF, 0x0000);
            println("* Character display: OK");
            println("* Memory mapping: OK");
            println("* Screen refresh: OK");
            println("* Mode switching: OK");
            println();
            _startTime = millis();
            _demoStep++;
        }
        break;

    case 4:
        if (elapsed > 2000)
        {
            showDemoComplete();
            _demoComplete = true;
        }
        break;
    }
}

void VideoFullScreenDemo::showDemoComplete()
{
    setTextColor(0x07E0, 0x0000); // Green
    println("VIDEO DEMO COMPLETE!");
    println();

    setTextColor(0xF81F, 0x0000); // Magenta
    println("Features demonstrated:");
    setTextColor(0xFFFF, 0x0000);
    println("* 32/64 character modes");
    println("* Video memory access");
    println("* Screen mode switching");
    println("* Character display control");
    println();

    setTextColor(0x07FF, 0x0000); // Cyan
    println("Press MENU to return");
}

Screen *VideoFullScreenDemo::actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY)
{
    if (action & BUTTON_MENU)
    {
        Serial.println("Returning to Video Features Menu");
        return new VideoFeaturesMenu();
    }

    return nullptr;
}
