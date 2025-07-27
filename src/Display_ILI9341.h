/*
 * Display_ILI9341.h - Factory for creating ILI9341 display instances
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef DISPLAY_ILI9341_H
#define DISPLAY_ILI9341_H

#include <Adafruit_ILI9341.h>
#include "DisplayProvider.h"

class Display_ILI9341 : public DisplayProvider
{
public:
    Adafruit_GFX *create(int8_t cs, int8_t dc, int8_t rst) override
    {
        Adafruit_ILI9341 *tft = new Adafruit_ILI9341(cs, dc, rst);
        tft->begin();
        tft->setRotation(3);
        return tft;
    }

    void destroy(Adafruit_GFX *display) override
    {
        delete display;
    }

    const char *name() const override
    {
        return "ILI9341 240x320";
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

#endif // DISPLAY_ILI9341_H
