/*
 * Display_ST7789.h - Factory for creating ST7789 display instances
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef DISPLAY_ST7789_H
#define DISPLAY_ST7789_H

#include <Adafruit_ST7789.h>
#include "DisplayProvider.h"

class Display_ST7789 : public DisplayProvider
{
private:
    Adafruit_ST7789 *_display;

public:
    Display_ST7789() : _display(nullptr) {}

    bool create(int8_t cs, int8_t dc, int8_t rst) override
    {
        if (_display)
        {
            delete _display;
        }
        _display = new Adafruit_ST7789(cs, dc, rst);
        _display->init(240, 320, SPI_MODE0);
        _display->setRotation(3);
        return _display != nullptr;
    }

    Adafruit_GFX &getGFX() override
    {
        return *_display;
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
        return "ST7789 240x320";
    }

    uint16_t width() const override
    {
        return 320; // After rotation 3
    }

    uint16_t height() const override
    {
        return 240; // After rotation 3
    }

    ~Display_ST7789() override
    {
        destroy();
    }
};

#endif // DISPLAY_ST7789_H
