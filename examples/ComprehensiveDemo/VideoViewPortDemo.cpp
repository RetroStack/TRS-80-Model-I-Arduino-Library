#include "VideoViewPortDemo.h"
#include "VideoFeaturesMenu.h"
#include <Arduino.h>
#include <Model1.h>
#include "logger.h"

VideoViewPortDemo::VideoViewPortDemo() : ConsoleScreen()
{
    _setTitle("ViewPort Demo");
    setConsoleBackground(0x0000);
    setTextColor(0xFFFF, 0x0000);

    _startTime = millis();
    _demoStep = 0;
    _demoComplete = false;
    _viewPortSet = false;

    // Set button labels
    const char *buttons[] = {"M:Back", "", "", ""};
    _setButtonItems(buttons, 4);

    // Create instance variables instead of using static
    _video = new Video();
    _cassette = new Cassette();

    // Set up logger for debugging
    _video->setLogger(logger);
    _cassette->setLogger(logger);

    Serial.println(F("Video ViewPort Demo initialized"));
}

void VideoViewPortDemo::_executeOnce()
{
    cls();
    setTextColor(0x07E0, 0x0000); // Green
    println(F("=== VIDEO VIEWPORT DEMO ==="));
    println();
    setTextColor(0xFFFF, 0x0000);
    println(F("Demonstrating viewport features..."));
    println();
    Serial.println(F("=== VIDEO VIEWPORT DEMO ==="));
    Serial.println(F("Demonstrating viewport features..."));
}

VideoViewPortDemo::~VideoViewPortDemo()
{
    // Clean up instance variables
    delete _video;
    delete _cassette;
    Serial.println(F("Video ViewPort Demo cleaned up"));
}

bool VideoViewPortDemo::open()
{
    // Activate test signal to allow video hardware access
    Model1.activateTestSignal();

    // Call parent implementation
    return ConsoleScreen::open();
}

void VideoViewPortDemo::close()
{
    // Deactivate test signal when screen is closed
    Model1.deactivateTestSignal();

    // Call parent implementation
    ConsoleScreen::close();
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
            println(F("Creating viewport window..."));
            println(F("Position: (20, 5)"));
            println(F("Size: 40x5 characters"));
            Serial.println(F("Creating viewport window..."));
            Serial.println(F("Position: (20, 5)"));
            Serial.println(F("Size: 40x5 characters"));

            // Set up viewport
            ViewPort viewPort;
            viewPort.x = 20;
            viewPort.y = 5;
            viewPort.width = 40;
            viewPort.height = 5;
            _video->setViewPort(viewPort);
            _viewPortSet = true;

            _startTime = millis();
            _demoStep++;
        }
        break;

    case 1:
        if (elapsed > 2000)
        {
            setTextColor(0x07E0, 0x0000); // Green
            println(F("Viewport created successfully"));
            println();
            Serial.println(F("Viewport created successfully"));
            setTextColor(0x07FF, 0x0000);
            println(F("Clearing viewport with spaces..."));
            Serial.println(F("Clearing viewport with spaces..."));
            _video->cls();
            _startTime = millis();
            _demoStep++;
        }
        break;

    case 2:
        if (elapsed > 2000)
        {
            setTextColor(0xFFE0, 0x0000); // Yellow
            println(F("Writing text to viewport..."));
            Serial.println(F("Writing text to viewport..."));
            _video->print(F("UPPER LEFT"));
            _video->setXY(17, 2);
            _video->println(F("MIDDLE"));
            _video->print(F("START "));
            setTextColor(0xFFFF, 0x0000);
            println(F("Text is clipped to viewport bounds"));
            Serial.println(F("Text is clipped to viewport bounds"));
            _startTime = millis();
            _demoStep++;
        }
        break;

    case 3:
        if (elapsed > 2000)
        {
            setTextColor(0x07E0, 0x0000); // Green
            println(F("Reading text from viewport..."));
            Serial.println(F("Reading text from viewport..."));
            String readText = _video->read(18, 2, 5, false);
            _video->print(readText);
            println(F("Content read and displayed"));
            Serial.println(F("Content read and displayed"));
            setTextColor(0x07FF, 0x0000);
            println(F("Testing viewport boundaries..."));
            Serial.println(F("Testing viewport boundaries..."));
            _video->setAutoScroll(false);
            _video->print(28, 4, "RIGHT BOTTOM");
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
    println(F("VIEWPORT DEMO COMPLETE!"));
    println();
    Serial.println(F("VIEWPORT DEMO COMPLETE!"));

    setTextColor(0xF81F, 0x0000); // Magenta
    println(F("Features demonstrated:"));
    Serial.println(F("Features demonstrated:"));
    setTextColor(0xFFFF, 0x0000);
    println(F("* Viewport creation and setup"));
    println(F("* Bounded text rendering"));
    println(F("* Cursor positioning within viewport"));
    println(F("* Text reading from viewport"));
    println(F("* Auto-scroll control"));
    println(F("* Coordinate-based printing"));
    println();
    Serial.println(F("* Viewport creation and setup"));
    Serial.println(F("* Bounded text rendering"));
    Serial.println(F("* Cursor positioning within viewport"));
    Serial.println(F("* Text reading from viewport"));
    Serial.println(F("* Auto-scroll control"));
    Serial.println(F("* Coordinate-based printing"));

    setTextColor(0x07FF, 0x0000); // Cyan
    println(F("Demo completed!"));
    Serial.println(F("Demo completed!"));
}

Screen *VideoViewPortDemo::actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY)
{
    if (action & BUTTON_MENU)
    {
        Serial.println(F("Returning to Video Features Menu"));
        return new VideoFeaturesMenu();
    }

    return nullptr;
}
