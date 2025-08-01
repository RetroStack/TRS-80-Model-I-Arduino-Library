#include "VideoFullScreenDemo.h"
#include "VideoFeaturesMenu.h"
#include <Arduino.h>
#include <Model1.h>
#include "logger.h"

VideoFullScreenDemo::VideoFullScreenDemo() : ConsoleScreen()
{
    _setTitle("Video Demo");
    setConsoleBackground(0x0000);
    setTextColor(0xFFFF, 0x0000);

    _startTime = millis();
    _demoStep = 0;
    _demoComplete = false;

    // Set button labels
    const char *buttons[] = {"M:Back", "", "", ""};
    _setButtonItems(buttons, 4);

    // Create instance variables instead of using static
    _video = new Video();
    _cassette = new Cassette();

    // Set up logger for debugging
    _video->setLogger(logger);
    _cassette->setLogger(logger);

    Serial.println(F("Video Full Screen Demo initialized"));
}

void VideoFullScreenDemo::_executeOnce()
{
    cls();
    setTextColor(0x07E0, 0x0000); // Green
    println(F("=== VIDEO FULL SCREEN DEMO ==="));
    println();
    setTextColor(0xFFFF, 0x0000);
    println(F("Demonstrating video features..."));
    println();
    Serial.println(F("=== VIDEO FULL SCREEN DEMO ==="));
    Serial.println(F("Demonstrating video features..."));
}

VideoFullScreenDemo::~VideoFullScreenDemo()
{
    // Clean up instance variables
    delete _video;
    delete _cassette;
    Serial.println(F("Video Full Screen Demo cleaned up"));
}

bool VideoFullScreenDemo::open()
{
    // Activate test signal to allow video hardware access
    Model1.activateTestSignal();

    // Call parent implementation
    return ConsoleScreen::open();
}

void VideoFullScreenDemo::close()
{
    // Deactivate test signal when screen is closed
    Model1.deactivateTestSignal();

    // Call parent implementation
    ConsoleScreen::close();
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
            println(F("Clearing screen with spaces..."));
            Serial.println(F("Clearing screen with spaces..."));
            _video->cls();
            _startTime = millis();
            _demoStep++;
        }
        break;

    case 1:
        if (elapsed > 2000)
        {
            setTextColor(0x07E0, 0x0000); // Green
            println(F("Clearing screen with '@' characters..."));
            Serial.println(F("Clearing screen with '@' characters..."));
            _video->cls('@');
            _startTime = millis();
            _demoStep++;
        }
        break;

    case 2:
        if (elapsed > 2000)
        {
            setTextColor(0x07E0, 0x0000); // Green
            println(F("Clearing screen with 'Hello World!'..."));
            Serial.println(F("Clearing screen with 'Hello World!'..."));
            _video->cls("Hello World! ");
            _startTime = millis();
            _demoStep++;
        }
        break;

    case 3:
        if (elapsed > 2000)
        {
            setTextColor(0xFFE0, 0x0000); // Yellow
            println(F("Testing text positioning..."));
            Serial.println(F("Testing text positioning..."));
            _video->cls();
            _video->print(F("UPPER LEFT"));
            _video->setXY(29, 7);
            _video->println(F("MIDDLE"));
            _video->print(F("START"));
            _video->print(60, 10, "CUT OFF");
            _video->print(50, 15, "RIGHT BOTTOM");
            _startTime = millis();
            _demoStep++;
        }
        break;

    case 4:
        if (elapsed > 2000)
        {
            setTextColor(0x07FF, 0x0000); // Cyan
            println(F("Testing character modes..."));
            Serial.println(F("Testing character modes..."));
            // Check current mode
            bool is64Mode = _cassette->is64CharacterMode();
            println(is64Mode ? "Currently in 64-char mode" : "Currently in 32-char mode");
            Serial.println(is64Mode ? "Currently in 64-char mode" : "Currently in 32-char mode");

            // Switch to 32-character mode
            println(F("Switching to 32-character mode..."));
            Serial.println(F("Switching to 32-character mode..."));
            _cassette->set32CharacterMode();
            _startTime = millis();
            _demoStep++;
        }
        break;

    case 5:
        if (elapsed > 2000)
        {
            setTextColor(0xF81F, 0x0000); // Magenta
            println(F("32-character mode active"));
            Serial.println(F("32-character mode active"));
            println(F("Switching back to 64-character mode..."));
            Serial.println(F("Switching back to 64-character mode..."));
            _cassette->set64CharacterMode();
            _startTime = millis();
            _demoStep++;
        }
        break;

    case 6:
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
    println(F("VIDEO DEMO COMPLETE!"));
    println();
    Serial.println(F("VIDEO DEMO COMPLETE!"));

    setTextColor(0xF81F, 0x0000); // Magenta
    println(F("Features demonstrated:"));
    Serial.println(F("Features demonstrated:"));
    setTextColor(0xFFFF, 0x0000);
    println(F("* Screen clearing (spaces, chars, strings)"));
    println(F("* Text positioning and cursor control"));
    println(F("* 32/64 character mode switching"));
    println(F("* Video memory direct access"));
    println();
    Serial.println(F("* Screen clearing (spaces, chars, strings)"));
    Serial.println(F("* Text positioning and cursor control"));
    Serial.println(F("* 32/64 character mode switching"));
    Serial.println(F("* Video memory direct access"));

    setTextColor(0x07FF, 0x0000); // Cyan
    println(F("Demo completed!"));
    Serial.println(F("Demo completed!"));
}

Screen *VideoFullScreenDemo::actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY)
{
    if (action & BUTTON_MENU)
    {
        Serial.println(F("Returning to Video Features Menu"));
        return new VideoFeaturesMenu();
    }

    return nullptr;
}
