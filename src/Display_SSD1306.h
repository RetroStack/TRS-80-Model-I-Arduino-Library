/*
 * Display_SSD1306.h - Factory for creating SSD1306 display instances
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef DISPLAY_SSD1306_H
#define DISPLAY_SSD1306_H

#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include "DisplayProvider.h"

class Display_SSD1306 : public DisplayProvider
{
private:
    Adafruit_SSD1306 *_display;
    bool _isI2C;
    uint8_t _i2cAddress;

public:
    // Constructor
    Display_SSD1306() : _display(nullptr), _isI2C(false), _i2cAddress(0x3C) {}

    // Create SSD1306 display instance with SPI interface
    bool create(int8_t cs, int8_t dc, int8_t rst) override
    {
        if (_display)
        {
            delete _display;
        }
        _display = new Adafruit_SSD1306(128, 64, &SPI, dc, rst, cs);
        _isI2C = false;
        if (_display->begin(SSD1306_SWITCHCAPVCC))
        {
            _display->clearDisplay();
            return true;
        }
        return false;
    }

    // Create SSD1306 display instance with I2C interface
    bool createI2C(uint8_t i2cAddress = 0x3C, int8_t rst = -1)
    {
        if (_display)
        {
            delete _display;
        }
        _display = new Adafruit_SSD1306(128, 64, &Wire, rst);
        _isI2C = true;
        _i2cAddress = i2cAddress;
        if (_display->begin(SSD1306_SWITCHCAPVCC, _i2cAddress))
        {
            _display->clearDisplay();
            return true;
        }
        return false;
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
        // SSD1306 is monochrome: convert to black (0) or white (1)
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
        return (luminance >= 128) ? SSD1306_WHITE : SSD1306_BLACK;
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
        return "SSD1306";
    }

    uint16_t width() const override
    {
        return 128;
    }

    uint16_t height() const override
    {
        return 64;
    }

    ~Display_SSD1306() override
    {
        destroy();
    }
};

#endif // DISPLAY_SSD1306_H
