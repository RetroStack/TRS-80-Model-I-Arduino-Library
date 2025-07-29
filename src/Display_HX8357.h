/*
 * Display_HX8357.h - Factory for creating HX8357 display instances
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef DISPLAY_HX8357_H
#define DISPLAY_HX8357_H

#include <Adafruit_HX8357.h>
#include "DisplayProvider.h"

class Display_HX8357 : public DisplayProvider
{
private:
    Adafruit_HX8357 *_display;

public:
    Display_HX8357() : _display(nullptr) {}

    bool create(int8_t cs, int8_t dc, int8_t rst) override
    {
        if (_display)
        {
            delete _display;
        }
        _display = new Adafruit_HX8357(cs, dc, rst);
        _display->begin(HX8357D);
        _display->setRotation(0);
        _display->invertDisplay(false);
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
        return "HX8357D 320x480";
    }

    uint16_t width() const override
    {
        return 320;
    }

    uint16_t height() const override
    {
        return 480;
    }

    ~Display_HX8357() override
    {
        destroy();
    }
};

#endif // DISPLAY_HX8357_H
