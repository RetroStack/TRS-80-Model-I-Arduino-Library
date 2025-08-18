/*
 * Display_SH1106.h - Factory for creating SH1106 display instances
 * Supports both SPI and I2C communication protocols
 *
 * Usage:
 * - SPI mode: Display_SH1106(true) - specify SPI in constructor
 * - I2C mode: Display_SH1106(false) or Display_SH1106() - default is I2C
 *
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef DISPLAY_SH1106_H
#define DISPLAY_SH1106_H

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_SH110X.h>
#include "DisplayProvider.h"

class Display_SH1106 : public DisplayProvider
{
private:
    Adafruit_SH1106G *_display;
    bool _useSPI;

public:
    // Constructor with SPI/I2C mode selection
    Display_SH1106(bool useSPI = false) : _display(nullptr), _useSPI(useSPI) {}

    // Create SH1106 display instance with specified pins
    bool create(int8_t cs, int8_t dc, int8_t rst) override
    {
        if (_display)
        {
            delete _display;
        }

        if (_useSPI)
        {
            // SPI constructor - using hardware SPI
            _display = new Adafruit_SH1106G(128, 64, &SPI, dc, rst, cs);
        }
        else
        {
            // I2C constructor
            _display = new Adafruit_SH1106G(128, 64, &Wire, rst);

            Wire.begin();

            // Give display a *long* power-on delay
            delay(500);

            Wire.beginTransmission(0x3C);
            Wire.write(0x00); // Control byte
            Wire.write(0xAE); // Display OFF
            Wire.endTransmission();
            delay(50);
        }

        delay(1000);

        if (_useSPI)
        {
            // SPI initialization - no I2C address needed
            if (!_display->begin())
            {
                return false;
            }
        }
        else
        {
            // I2C initialization with default address
            if (!_display->begin(0x3C, true))
            {
                return false;
            }
        }

        _display->clearDisplay();
        return true;
    }

    Adafruit_GFX &getGFX() override
    {
        return *_display;
    }

    bool display() override
    {
        if (_display)
        {
            _display->display();
            return true;
        }
        return false;
    }

    uint16_t convertColor(uint16_t color) override
    {
        // SH1106 is monochrome: convert to black (0) or white (1)
        // Extract RGB components from RGB565 format
        uint8_t r = (color >> 11) & 0x1F; // 5 bits red
        uint8_t g = (color >> 5) & 0x3F;  // 6 bits green
        uint8_t b = color & 0x1F;         // 5 bits blue

        // Scale to 8-bit values
        r = (r * 255) / 31;
        g = (g * 255) / 63;
        b = (b * 255) / 31;

        // Calculate luminance using standard weights (ITU-R BT.709)
        uint16_t luminance = (uint16_t)(0.2126 * r + 0.7152 * g + 0.0722 * b);

        // Use 50% threshold (127.5, rounded to 128)
        return (luminance >= 128) ? SH110X_WHITE : SH110X_BLACK;
    }

    void destroy() override
    {
        if (_display)
        {
            delete _display;
            _display = nullptr;
        }
    }

    const char *name() const override
    {
        return _useSPI ? "SH1106 (SPI)" : "SH1106 (I2C)";
    }

    uint16_t width() const override
    {
        return 128;
    }

    uint16_t height() const override
    {
        return 64;
    }

    ~Display_SH1106() override
    {
        destroy();
    }
};

#endif // DISPLAY_SH1106_H
