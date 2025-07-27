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
public:
    Adafruit_GFX *create(int8_t cs, int8_t dc, int8_t rst) override
    {
        Adafruit_ST7735 *tft = new Adafruit_ST7735(cs, dc, rst);
        tft->initR(INITR_BLACKTAB);
        tft->setRotation(3);
        return tft;
    }

    void destroy(Adafruit_GFX *display) override
    {
        delete display;
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
};

#endif // DISPLAY_ST7735_H
