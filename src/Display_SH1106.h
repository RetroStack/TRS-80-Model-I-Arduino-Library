/*
 * Display_SSD1306.h - Factory for creating SSD1306 display instances
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef DISPLAY_SH1106_H
#define DISPLAY_SH1106_H

#include <Wire.h>
#include <Adafruit_SH110X.h>
#include "DisplayProvider.h"

class Display_SH1106 : public DisplayProvider
{
private:
    Adafruit_SH1106G *_display;

public:
    Display_SH1106() : _display(nullptr) {}

    bool create(int8_t cs, int8_t dc, int8_t rst) override
    {
        if (_display)
        {
            delete _display;
        }
        _display = new Adafruit_SH1106G(128, 64, &Wire, -1);
        delay(1000);
        _display->begin(0x3C, true); // Default I2C address for SH1106
        _display->clearDisplay();
        return _display != nullptr;
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
        return "SH1106";
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
