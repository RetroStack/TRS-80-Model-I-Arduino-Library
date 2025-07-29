/*
 * Display_ST7789_320x240.h - Factory for creating ST7789 display instances
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef DISPLAY_ST7789_320X240_H
#define DISPLAY_ST7789_320X240_H

#include <Adafruit_ST7789.h>
#include "DisplayProvider.h"

class Display_ST7789_320x240 : public DisplayProvider
{
public:
    Adafruit_GFX *create(int8_t cs, int8_t dc, int8_t rst) override
    {
        Adafruit_ST7789 *tft = new Adafruit_ST7789(cs, dc, rst);
        tft->init(240, 320, SPI_MODE0);
        tft->setRotation(3);
        return tft;
    }

    void destroy(Adafruit_GFX *display) override
    {
        delete display;
    }

    const char *name() const override
    {
        return "ST7789 320x240";
    }

    uint16_t width() const override
    {
        return 320; // After rotation 3
    }

    uint16_t height() const override
    {
        return 240; // After rotation 3
    }
};

#endif // DISPLAY_ST7789_320X240_H
