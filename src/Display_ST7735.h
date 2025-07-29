/*
 * Display_ST7735.h - Factory for creating ST7735 display instances
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef DISPLAY_ST7735_H
#define DISPLAY_ST7735_H

#include <Adafruit_ST7735.h>
#include "DisplayProvider.h"

class Display_ST7735 : public DisplayProvider
{
private:
    Adafruit_ST7735 *_display;

public:
    Display_ST7735() : _display(nullptr) {}

    bool create(int8_t cs, int8_t dc, int8_t rst) override
    {
        if (_display)
        {
            delete _display;
        }
        _display = new Adafruit_ST7735(cs, dc, rst);
        _display->initR(INITR_BLACKTAB);
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
        return "ST7735 128x160";
    }

    uint16_t width() const override
    {
        return 128;
    }

    uint16_t height() const override
    {
        return 160;
    }

    ~Display_ST7735() override
    {
        destroy();
    }
};

#endif // DISPLAY_ST7735_H
