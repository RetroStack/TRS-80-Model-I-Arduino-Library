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
public:
    Adafruit_GFX *create(int8_t cs, int8_t dc, int8_t rst) override
    {
        Adafruit_HX8357 *tft = new Adafruit_HX8357(cs, dc, rst);
        tft->begin(HX8357D);
        tft->setRotation(0);
        tft->invertDisplay(false);
        return tft;
    }

    void destroy(Adafruit_GFX *display) override
    {
        delete display;
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
};

#endif // DISPLAY_HX8357_H
