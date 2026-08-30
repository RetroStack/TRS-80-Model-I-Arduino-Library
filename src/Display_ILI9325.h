/*
 * Display_ILI9325.h - Factory for creating ILI9325 display instances
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef DISPLAY_ILI9325_H
#define DISPLAY_ILI9325_H

#include <Adafruit_ILI9325.h>
#include "DisplayProviderTFT.h"

class Display_ILI9325 : public DisplayProviderTFT<Adafruit_ILI9325>
{
public:
    bool create(int8_t cs, int8_t dc, int8_t rst) override
    {
        if (!_reset(new Adafruit_ILI9325(cs, dc, rst)))
        {
            return false;
        }

        _display->begin();
        _display->setRotation(3);
        return true;
    }

    const char *name() const override
    {
        return "ILI9325 320x240";
    }

    uint16_t width() const override
    {
        return 320;
    }

    uint16_t height() const override
    {
        return 240;
    }
};

#endif /* DISPLAY_ILI9325_H */
