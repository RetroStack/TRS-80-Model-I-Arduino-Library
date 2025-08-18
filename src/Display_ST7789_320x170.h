/*
 * Display_ST7789_320x170.h - Factory for creating ST7789 320x170 display instances
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef DISPLAY_ST7789_320X170_H
#define DISPLAY_ST7789_320X170_H

#include <Adafruit_ST7789.h>
#include "DisplayProvider.h"

class Display_ST7789_320x170 : public DisplayProvider
{
private:
    Adafruit_ST7789 *_display;

public:
    // Constructor
    Display_ST7789_320x170() : _display(nullptr) {}

    // Create ST7789 320x170 display instance with specified pins
    bool create(int8_t cs, int8_t dc, int8_t rst) override
    {
        if (_display)
        {
            delete _display;
        }
        _display = new Adafruit_ST7789(cs, dc, rst);
        _display->init(170, 320, SPI_MODE0);
        _display->setRotation(3);
        return _display != nullptr;
    }

    Adafruit_GFX &getGFX() override
    {
        return *_display;
    }

    bool display() override
    {
        // TFT displays update immediately, no explicit display() call needed
        return (_display != nullptr);
    }

    uint16_t convertColor(uint16_t color) override
    {
        // ST7789 uses 16-bit RGB565 format directly
        return color;
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
        return "ST7789 320x170";
    }

    uint16_t width() const override
    {
        return 320;
    }

    uint16_t height() const override
    {
        return 170;
    }

    ~Display_ST7789_320x170() override
    {
        destroy();
    }
};

#endif // DISPLAY_ST7789_320X170_H
