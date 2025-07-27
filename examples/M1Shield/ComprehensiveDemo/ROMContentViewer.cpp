#include "ROMContentViewer.h"
#include "ROMFeaturesMenu.h"
#include <Arduino.h>

ROMContentViewer::ROMContentViewer() : ConsoleScreen()
{
    _setTitle("ROM Viewer");
    setConsoleBackground(0x0000);
    setTextColor(0xFFFF, 0x0000);

    _currentAddress = 0x0000;

    cls();
    setTextColor(0x07E0, 0x0000); // Green
    println("=== ROM CONTENT VIEWER ===");
    println();

    displayInstructions();
    displayROMContent();

    Serial.println("ROM Content Viewer initialized");
}

void ROMContentViewer::loop()
{
    ConsoleScreen::loop();
}

void ROMContentViewer::displayInstructions()
{
    setTextColor(0xFFE0, 0x0000); // Yellow
    println("Navigation:");
    setTextColor(0xFFFF, 0x0000);
    println("UP/DOWN: Scroll through ROM");
    println("MENU: Return to ROM menu");
    println();
}

void ROMContentViewer::displayROMContent()
{
    setTextColor(0x07FF, 0x0000); // Cyan
    print("ROM Content at 0x");
    if (_currentAddress < 0x1000)
        print("0");
    if (_currentAddress < 0x0100)
        print("0");
    if (_currentAddress < 0x0010)
        print("0");
    print(_currentAddress, HEX);
    println(":");
    println();

    setTextColor(0xFFFF, 0x0000); // White

    // Display hex dump
    for (int line = 0; line < LINES_PER_PAGE; line++)
    {
        uint16_t lineAddress = _currentAddress + (line * BYTES_PER_LINE);

        // Address column
        setTextColor(0xF81F, 0x0000); // Magenta
        if (lineAddress < 0x1000)
            print("0");
        if (lineAddress < 0x0100)
            print("0");
        if (lineAddress < 0x0010)
            print("0");
        print(lineAddress, HEX);
        print(": ");

        // Hex bytes
        setTextColor(0x07FF, 0x0000); // Cyan
        for (int byte = 0; byte < BYTES_PER_LINE; byte++)
        {
            uint16_t address = lineAddress + byte;
            if (address < 0x3000) // ROM size limit
            {
                uint8_t value = _rom.readByte(address);
                if (value < 0x10)
                    print("0");
                print(value, HEX);
                print(" ");
            }
            else
            {
                print("-- ");
            }
        }

        // ASCII representation
        setTextColor(0xFFE0, 0x0000); // Yellow
        print(" ");
        for (int byte = 0; byte < BYTES_PER_LINE; byte++)
        {
            uint16_t address = lineAddress + byte;
            if (address < 0x3000)
            {
                uint8_t value = _rom.readByte(address);
                if (value >= 32 && value <= 126)
                {
                    print((char)value);
                }
                else
                {
                    print(".");
                }
            }
            else
            {
                print(" ");
            }
        }

        println();
    }

    println();
    setTextColor(0x07E0, 0x0000); // Green
    println("Use UP/DOWN to scroll");
}

Screen *ROMContentViewer::actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY)
{
    if (action & BUTTON_MENU)
    {
        Serial.println("Returning to ROM Features Menu");
        return new ROMFeaturesMenu();
    }

    if (action & BUTTON_UP)
    {
        if (_currentAddress >= BYTES_PER_LINE * LINES_PER_PAGE)
        {
            _currentAddress -= BYTES_PER_LINE * LINES_PER_PAGE;
            cls();
            displayInstructions();
            displayROMContent();
        }
        return nullptr;
    }

    if (action & BUTTON_DOWN)
    {
        if (_currentAddress + (BYTES_PER_LINE * LINES_PER_PAGE * 2) < 0x3000)
        {
            _currentAddress += BYTES_PER_LINE * LINES_PER_PAGE;
            cls();
            displayInstructions();
            displayROMContent();
        }
        return nullptr;
    }

    return nullptr;
}
