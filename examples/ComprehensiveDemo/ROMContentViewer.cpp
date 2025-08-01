#include "ROMContentViewer.h"
#include "ROMFeaturesMenu.h"
#include <Arduino.h>
#include "logger.h"

ROMContentViewer::ROMContentViewer() : ConsoleScreen()
{
    _setTitle("ROM Viewer");
    setConsoleBackground(0x0000);
    setTextColor(0xFFFF, 0x0000);
    _rom.setLogger(logger);

    _currentAddress = 0x0000;

    // Set button labels for navigation
    const char *buttons[] = {"M:Exit", "", "UP:Prev", "DN:Next"};
    _setButtonItems(buttons, 4);

    Serial.println(F("ROM Content Viewer initialized"));
}

void ROMContentViewer::_executeOnce()
{
    displayROMContent();
}

void ROMContentViewer::displayROMContent()
{
    cls();
    setTextColor(0xFFFF, 0x0000); // White

    // Get dynamic lines per page based on available screen space
    uint16_t linesPerPage = getLinesPerPage();

    // Display hex dump
    Model1.activateTestSignal();
    for (int line = 0; line < linesPerPage; line++)
    {
        uint16_t lineAddress = _currentAddress + (line * BYTES_PER_LINE);

        // Build complete line as a single string
        String completeLine = "";

        // Address column (magenta will be set before printing)
        String addressPart = "";
        if (lineAddress < 0x1000)
            addressPart += "0";
        if (lineAddress < 0x0100)
            addressPart += "0";
        if (lineAddress < 0x0010)
            addressPart += "0";
        addressPart += String(lineAddress, HEX);
        addressPart += ": ";

        // Hex bytes
        String hexPart = "";
        for (int byte = 0; byte < BYTES_PER_LINE; byte++)
        {
            uint16_t address = lineAddress + byte;
            if (address < 0x3000) // ROM size limit
            {
                uint8_t value = Model1.readMemory(address);
                if (value < 0x10)
                    hexPart += "0";
                hexPart += String(value, HEX);
                hexPart += " ";
            }
            else
            {
                hexPart += "-- ";
            }
        }

        // ASCII representation
        String asciiPart = " ";
        for (int byte = 0; byte < BYTES_PER_LINE; byte++)
        {
            uint16_t address = lineAddress + byte;
            if (address < 0x3000)
            {
                uint8_t value = Model1.readMemory(address);
                if (value >= 32 && value <= 126)
                {
                    asciiPart += (char)value;
                }
                else
                {
                    asciiPart += ".";
                }
            }
            else
            {
                asciiPart += " ";
            }
        }

        // Print address in magenta
        setTextColor(0xF81F, 0x0000); // Magenta
        print(addressPart);

        // Print hex bytes in cyan
        setTextColor(0x07FF, 0x0000); // Cyan
        print(hexPart);

        // Print ASCII in yellow
        setTextColor(0xFFE0, 0x0000); // Yellow
        println(asciiPart);
    }
    Model1.deactivateTestSignal();
}

Screen *ROMContentViewer::actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY)
{
    if (action & BUTTON_MENU)
    {
        Serial.println(F("Returning to ROM Features Menu"));
        return new ROMFeaturesMenu();
    }

    if (action & UP_ANY)
    {
        uint16_t linesPerPage = getLinesPerPage();
        if (_currentAddress >= BYTES_PER_LINE * linesPerPage)
        {
            _currentAddress -= BYTES_PER_LINE * linesPerPage;
            displayROMContent();
        }
        return nullptr;
    }

    if (action & DOWN_ANY)
    {
        uint16_t linesPerPage = getLinesPerPage();
        if (_currentAddress + (BYTES_PER_LINE * linesPerPage * 2) < 0x3000)
        {
            _currentAddress += BYTES_PER_LINE * linesPerPage;
            displayROMContent();
        }
        return nullptr;
    }

    return nullptr;
}

uint16_t ROMContentViewer::getLinesPerPage() const
{
    // Account for header (2 lines: title + blank line)
    // Use the ConsoleScreen's internal dimensions and line height
    uint16_t availableHeight = _getContentHeight();
    uint16_t headerHeight = 2 * 8; // Assuming default text size (8 pixels per line)

    // Use current text size to calculate line height
    uint16_t lineHeight = 8; // Default size 1 text height

    // Calculate usable height after header
    uint16_t usableHeight = availableHeight - headerHeight;

    // Calculate how many lines fit, with a minimum of 5 lines
    uint16_t calculatedLines = usableHeight / lineHeight;

    // Ensure we have at least 5 lines and don't exceed reasonable limits
    if (calculatedLines < 5)
        calculatedLines = 5;
    // if (calculatedLines > 20)
    //     calculatedLines = 20;

    return calculatedLines;
}
