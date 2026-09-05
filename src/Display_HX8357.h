/*
 * Display_HX8357.h - Factory for creating HX8357 display instances
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef DISPLAY_HX8357_H
#define DISPLAY_HX8357_H

#include <Adafruit_HX8357.h>
#include "DisplayProviderTFT.h"

class Display_HX8357 : public DisplayProviderTFT<Adafruit_HX8357>
{
public:
    bool create(int8_t cs, int8_t dc, int8_t rst) override
    {
        if (!_reset(new Adafruit_HX8357(cs, dc, rst)))
        {
            return false;
        }

        _display->begin(HX8357D);
        _display->setRotation(0);
        _display->invertDisplay(false);
        return true;
    }

    const char *getName() const override
    {
        return "HX8357D 320x480";
    }

    uint16_t getScreenWidth() const override
    {
        return 320;
    }

    uint16_t getScreenHeight() const override
    {
        return 480;
    }
};

#endif /* DISPLAY_HX8357_H */
