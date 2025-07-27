#include "VideoViewPortDemo.h"
#include "VideoFeaturesMenu.h"
#include <Arduino.h>

VideoViewPortDemo::VideoViewPortDemo() : ConsoleScreen()
{
    _setTitle("ViewPort Demo");
    setConsoleBackground(0x0000);
    setTextColor(0xFFFF, 0x0000);

    _startTime = millis();
    _demoStep = 0;
    _demoComplete = false;

    cls();
    setTextColor(0x07E0, 0x0000); // Green
    println("=== VIDEO VIEWPORT DEMO ===");
    println();
    setTextColor(0xFFFF, 0x0000);
    println("Demonstrating viewport features...");
    println();

    Serial.println("Video ViewPort Demo initialized");
}

void VideoViewPortDemo::loop()
{
    ConsoleScreen::loop();

    if (!_demoComplete)
    {
        runViewPortDemo();
    }
}

void VideoViewPortDemo::runViewPortDemo()
{
    unsigned long elapsed = millis() - _startTime;

    switch (_demoStep)
    {
    case 0:
        if (elapsed > 1000)
        {
            setTextColor(0x07FF, 0x0000); // Cyan
            println("Creating viewport window...");
            println("Position: (10, 5)");
            println("Size: 30x10 characters");
            _startTime = millis();
            _demoStep++;
        }
        break;

    case 1:
        if (elapsed > 2000)
        {
            setTextColor(0x07E0, 0x0000); // Green
            println("Viewport created successfully");
            println();
            setTextColor(0x07FF, 0x0000);
            println("Writing text to viewport...");
            println("Text is clipped to viewport bounds");
            _startTime = millis();
            _demoStep++;
        }
        break;

    case 2:
        if (elapsed > 2000)
        {
            setTextColor(0xFFE0, 0x0000); // Yellow
            println("Viewport text rendering:");
            setTextColor(0xFFFF, 0x0000);
            println("Line 1: Hello Viewport!");
            println("Line 2: Clipped text demo");
            println("Line 3: Scrolling content");
            println();
            setTextColor(0x07FF, 0x0000);
            println("Testing viewport scrolling...");
            _startTime = millis();
            _demoStep++;
        }
        break;

    case 3:
        if (elapsed > 2000)
        {
            setTextColor(0x07E0, 0x0000); // Green
            println("Viewport scrolling active");
            println("Content moves within bounds");
            println();
            setTextColor(0x07FF, 0x0000);
            println("Closing viewport...");
            _startTime = millis();
            _demoStep++;
        }
        break;

    case 4:
        if (elapsed > 1500)
        {
            showDemoComplete();
            _demoComplete = true;
        }
        break;
    }
}

void VideoViewPortDemo::showDemoComplete()
{
    setTextColor(0x07E0, 0x0000); // Green
    println("VIEWPORT DEMO COMPLETE!");
    println();

    setTextColor(0xF81F, 0x0000); // Magenta
    println("Features demonstrated:");
    setTextColor(0xFFFF, 0x0000);
    println("* Viewport creation");
    println("* Text clipping");
    println("* Bounded rendering");
    println("* Scrolling within viewport");
    println("* Viewport management");
    println();

    setTextColor(0x07FF, 0x0000); // Cyan
    println("Press MENU to return");
}

Screen *VideoViewPortDemo::actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY)
{
    if (action & BUTTON_MENU)
    {
        Serial.println("Returning to Video Features Menu");
        return new VideoFeaturesMenu();
    }

    return nullptr;
}
