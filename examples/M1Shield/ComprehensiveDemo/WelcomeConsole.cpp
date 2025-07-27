#include "WelcomeConsole.h"
#include "MainDemoMenu.h"
#include <Arduino.h>

WelcomeConsole::WelcomeConsole() : ConsoleScreen()
{
    _setTitle("M1Shield Demo");
    setConsoleBackground(0x0000);
    setTextColor(0xFFFF, 0x0000);
    setTextSize(1);

    _startTime = millis();
    _introComplete = false;
    _animationStep = 0;

    Serial.println("Welcome Console initialized");
}

void WelcomeConsole::loop()
{
    ConsoleScreen::loop();

    if (!_introComplete)
    {
        showWelcomeAnimation();
    }
}

void WelcomeConsole::showWelcomeAnimation()
{
    unsigned long elapsed = millis() - _startTime;

    if (elapsed > 1000 && _animationStep == 0)
    {
        cls();
        setTextColor(0x07E0, 0x0000); // Green
        setTextSize(2);
        println("M1SHIELD");
        setTextSize(1);
        setTextColor(0xFFE0, 0x0000); // Yellow
        println("COMPREHENSIVE DEMO");
        println();
        _animationStep = 1;
    }
    else if (elapsed > 2500 && _animationStep == 1)
    {
        setTextColor(0x07FF, 0x0000); // Cyan
        println("Welcome to the complete");
        println("M1Shield feature showcase!");
        println();
        _animationStep = 2;
    }
    else if (elapsed > 4000 && _animationStep == 2)
    {
        showFeatureList();
        _animationStep = 3;
    }
    else if (elapsed > 6000 && _animationStep == 3)
    {
        showInstructions();
        _introComplete = true;
    }
}

void WelcomeConsole::showFeatureList()
{
    setTextColor(0xF81F, 0x0000); // Magenta
    println("FEATURES INCLUDED:");
    setTextColor(0xFFFF, 0x0000); // White
    println("* Model1 TEST Signal Control");
    println("* M1TerminalScreen Demo");
    println("* ROM Detection & Reading");
    println("* Video Mode Switching");
    println("* RAM Testing Suite");
    println("* Keyboard Input Monitor");
    println("* Cassette Song Player");
    println();
}

void WelcomeConsole::showInstructions()
{
    setTextColor(0x07E0, 0x0000); // Green
    println("INSTRUCTIONS:");
    setTextColor(0xFFFF, 0x0000); // White
    println("* Use arrow buttons to navigate");
    println("* Press joystick to select");
    println("* Use Menu button to go back");
    println();
    setTextColor(0xFFE0, 0x0000); // Yellow
    println("Press joystick to continue...");
}

Screen *WelcomeConsole::actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY)
{
    if (_introComplete && (action & BUTTON_JOYSTICK))
    {
        Serial.println("Moving to main demo menu");
        return new MainDemoMenu();
    }

    // Skip animation on any button press
    if (!_introComplete)
    {
        _introComplete = true;
        cls();
        showFeatureList();
        showInstructions();
    }

    return nullptr;
}
