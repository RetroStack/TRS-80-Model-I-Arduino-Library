#include "ROMDetectionDemo.h"
#include "ROMFeaturesMenu.h"
#include <Arduino.h>

ROMDetectionDemo::ROMDetectionDemo() : ConsoleScreen()
{
    _setTitle("ROM Detection");
    setConsoleBackground(0x0000);
    setTextColor(0xFFFF, 0x0000);

    _detectionComplete = false;
    _startTime = millis();
    _progressStep = 0;

    cls();
    setTextColor(0x07E0, 0x0000); // Green
    println("=== ROM DETECTION DEMO ===");
    println();
    setTextColor(0xFFFF, 0x0000);
    println("Analyzing ROM contents...");
    println();

    // Initialize progress bar
    drawProgressBar(10, 10);

    Serial.println("ROM Detection Demo initialized");
}

void ROMDetectionDemo::loop()
{
    ConsoleScreen::loop();

    if (!_detectionComplete)
    {
        runROMDetection();
    }
}

void ROMDetectionDemo::runROMDetection()
{
    unsigned long elapsed = millis() - _startTime;

    // Update progress bar every 200ms
    if (elapsed > (_progressStep + 1) * 200 && _progressStep < 50)
    {
        _progressStep++;
        drawProgressBar(_progressStep * 2, 10); // 0-100%

        // Show status messages during detection
        if (_progressStep == 10)
        {
            setTextColor(0x07FF, 0x0000); // Cyan
            println("Reading ROM header...");
        }
        else if (_progressStep == 25)
        {
            println("Analyzing ROM structure...");
        }
        else if (_progressStep == 40)
        {
            println("Detecting ROM version...");
        }
        else if (_progressStep == 50)
        {
            println("Detection complete!");
            println();
            displayResults();
            _detectionComplete = true;
        }
    }
}

void ROMDetectionDemo::displayResults()
{
    setTextColor(0xFFE0, 0x0000); // Yellow
    println("ROM DETECTION RESULTS:");
    println();

    setTextColor(0xFFFF, 0x0000); // White

    // Try to detect ROM type
    String romType = _rom.detectROMType();
    print("ROM Type: ");
    setTextColor(0x07E0, 0x0000); // Green
    println(romType);

    setTextColor(0xFFFF, 0x0000);
    print("ROM Size: ");
    setTextColor(0x07E0, 0x0000);
    println("12KB");

    setTextColor(0xFFFF, 0x0000);
    print("Base Address: ");
    setTextColor(0x07E0, 0x0000);
    println("0x0000");

    setTextColor(0xFFFF, 0x0000);
    print("Status: ");
    setTextColor(0x07E0, 0x0000);
    println("Valid ROM detected");

    println();
    setTextColor(0x07FF, 0x0000); // Cyan
    println("ROM Features available:");
    setTextColor(0xFFFF, 0x0000);
    println("* BASIC Interpreter");
    println("* System routines");
    println("* Character set data");
    println("* Boot loader");

    println();
    setTextColor(0xF81F, 0x0000); // Magenta
    println("Press MENU to go back");
}

Screen *ROMDetectionDemo::actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY)
{
    if (action & BUTTON_MENU)
    {
        Serial.println("Returning to ROM Features Menu");
        return new ROMFeaturesMenu();
    }

    return nullptr;
}
