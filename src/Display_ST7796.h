/*
 * Display_ST7796.h - Factory for creating ST7796 display instances
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef DISPLAY_ST7796_H
#define DISPLAY_ST7796_H

#include <Adafruit_ST7796S.h>
#include "DisplayProvider.h"

class Display_ST7796 : public DisplayProvider
{
private:
    Adafruit_ST7796S *_display;

public:
    // Constructor
    Display_ST7796() : _display(nullptr) {}

    // Create ST7796 display instance with specified pins
    bool create(int8_t cs, int8_t dc, int8_t rst) override
    {
        if (_display)
        {
            delete _display;
        }
        _display = new Adafruit_ST7796S(cs, dc, rst);
        _display->init(320, 480, 0, 0, ST7796S_BGR);
        _display->setRotation(1);
        _display->invertDisplay(true);
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
        // ST7796 uses 16-bit RGB565 format directly
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
        return "ST7796S 320x480";
    }

    uint16_t width() const override
    {
        return 480; // After rotation 1
    }

    uint16_t height() const override
    {
        return 320; // After rotation 1
    }

    ~Display_ST7796() override
    {
        destroy();
    }
};

#endif // DISPLAY_ST7796_H
