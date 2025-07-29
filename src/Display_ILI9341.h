/*
 * Display_ILI9341.h - Factory for creating ILI9341 display instances
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef DISPLAY_ILI9341_H
#define DISPLAY_ILI9341_H

#include <Adafruit_ILI9341.h>
#include "DisplayProvider.h"

class Display_ILI9341 : public DisplayProvider
{
private:
    Adafruit_ILI9341 *_display;

public:
    Display_ILI9341() : _display(nullptr) {}

    bool create(int8_t cs, int8_t dc, int8_t rst) override
    {
        if (_display)
        {
            delete _display;
        }
        _display = new Adafruit_ILI9341(cs, dc, rst);
        _display->begin();
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
        // ILI9341 uses 16-bit RGB565 format directly
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
        return "ILI9341 240x320";
    }

    uint16_t width() const override
    {
        return 320; // After rotation 3
    }

    uint16_t height() const override
    {
        return 240; // After rotation 3
    }

    ~Display_ILI9341() override
    {
        destroy();
    }
};

#endif // DISPLAY_ILI9341_H
