/*
 * Display_ST7789_320x240.h - Factory for creating ST7789 display instances
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef DISPLAY_ST7789_320X240_H
#define DISPLAY_ST7789_320X240_H

#include <Adafruit_ST7789.h>
#include "DisplayProviderTFT.h"

class Display_ST7789_320x240 : public DisplayProviderTFT<Adafruit_ST7789>
{
public:
    bool create(int8_t cs, int8_t dc, int8_t rst) override
    {
        if (!_reset(new Adafruit_ST7789(cs, dc, rst)))
        {
            return false;
        }

        _display->init(240, 320, SPI_MODE0);
        _display->setRotation(3);
        return true;
    }

    const char *getName() const override
    {
        return "ST7789 320x240";
    }

    uint16_t getScreenWidth() const override
    {
        return 320;
    }

    uint16_t getScreenHeight() const override
    {
        return 240;
    }
};

#endif /* DISPLAY_ST7789_320X240_H */
