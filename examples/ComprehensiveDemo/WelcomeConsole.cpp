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

    // Set button labels
    const char *buttons[] = {"", "JB:Start", "", ""};
    _setButtonItems(buttons, 4);

    Serial.println(F("Welcome Console initialized"));
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
        println(F("M1SHIELD"));
        setTextSize(1);
        setTextColor(0xFFE0, 0x0000); // Yellow
        println(F("COMPREHENSIVE DEMO"));
        println();
        _animationStep = 1;
    }
    else if (elapsed > 2500 && _animationStep == 1)
    {
        setTextColor(0x07FF, 0x0000); // Cyan
        println(F("Welcome to the"));
        println(F("M1Shield feature showcase!"));
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
    println(F("FEATURES INCLUDED:"));
    setTextColor(0xFFFF, 0x0000); // White
    println(F("* Model1 TEST Signal Control"));
    println(F("* M1TerminalScreen Demo"));
    println(F("* ROM Detection & Reading"));
    println(F("* Video Mode Switching"));
    println(F("* RAM Testing Suite"));
    println(F("* Keyboard Input Monitor"));
    println(F("* Cassette Song Player"));
    println();
}

void WelcomeConsole::showInstructions()
{
    setTextColor(0x07E0, 0x0000); // Green
    println(F("INSTRUCTIONS:"));
    setTextColor(0xFFFF, 0x0000); // White
    println(F("* Use arrow buttons to navigate"));
    println(F("* Press joystick to select"));
    println(F("* Use Menu button to go back"));
    println();
    setTextColor(0xFFE0, 0x0000); // Yellow
    println(F("Ready to start!"));
}

Screen *WelcomeConsole::actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY)
{
    if (_introComplete && ((action & BUTTON_JOYSTICK) || (action & BUTTON_RIGHT)))
    {
        Serial.println(F("Moving to main demo menu"));
        return new MainDemoMenu();
    }

    // Skip animation on any button press or joystick input
    if (!_introComplete)
    {
        _introComplete = true;
        cls();
        showFeatureList();
        showInstructions();
    }

    return nullptr;
}
