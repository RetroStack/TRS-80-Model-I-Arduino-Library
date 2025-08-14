/*
 * Display_ILI9325.h - Factory for creating ILI9325 display instances
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef DISPLAY_ILI9325_H
#define DISPLAY_ILI9325_H

#include <Adafruit_ILI9325.h>
#include "DisplayProvider.h"

class Display_ILI9325 : public DisplayProvider
{
private:
    Adafruit_ILI9325 *_display;

public:
    Display_ILI9325() : _display(nullptr) {}

    bool create(int8_t cs, int8_t dc, int8_t rst) override
    {
        if (_display)
        {
            delete _display;
        }
        // ILI9325 uses SPI interface
        _display = new Adafruit_ILI9325(cs, dc, rst);
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
        // ILI9325 uses 16-bit RGB565 format directly
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
        return "ILI9325 240x320";
    }

    uint16_t width() const override
    {
        return 320; // After rotation 3
    }

    uint16_t height() const override
    {
        return 240; // After rotation 3
    }

    ~Display_ILI9325() override
    {
        destroy();
    }
};

#endif // DISPLAY_ILI9325_H
