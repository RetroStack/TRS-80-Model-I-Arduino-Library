/*
 * Display_ST7789_320x170.h - Factory for creating ST7789 320x170 display instances
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef DISPLAY_ST7789_320X170_H
#define DISPLAY_ST7789_320X170_H

#include <Adafruit_ST7789.h>
#include "DisplayProviderTFT.h"

class Display_ST7789_320x170 : public DisplayProviderTFT<Adafruit_ST7789>
{
public:
    bool create(int8_t cs, int8_t dc, int8_t rst) override
    {
        if (!_reset(new Adafruit_ST7789(cs, dc, rst)))
        {
            return false;
        }

        _display->init(170, 320, SPI_MODE0);
        _display->setRotation(3);
        return true;
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
};

#endif /* DISPLAY_ST7789_320X170_H */
