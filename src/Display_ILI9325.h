/*
 * Display_ILI9325.h - Factory for creating ILI9325 display instances
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef DISPLAY_ILI9325_H
#define DISPLAY_ILI9325_H

#include <Adafruit_TFTLCD.h>
#include "DisplayProvider.h"

class Display_ILI9325 : public DisplayProvider
{
public:
    Adafruit_GFX *create(int8_t cs, int8_t dc, int8_t rst) override
    {
        // ILI9325 typically uses parallel interface, cs pin is used as chip select
        Adafruit_TFTLCD *tft = new Adafruit_TFTLCD(cs, dc, rst);
        tft->begin(0x9325); // ILI9325 identifier
        tft->setRotation(3);
        return tft;
    }

    void destroy(Adafruit_GFX *display) override
    {
        delete display;
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
};

#endif // DISPLAY_ILI9325_H
