/*
 * Display_ST7796.h - Factory for creating ST7796 display instances
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef DISPLAY_ST7796_H
#define DISPLAY_ST7796_H

#include <Adafruit_ST7796S.h>
#include "DisplayProvider.h"

class Display_ST7796 : public DisplayProvider
{
public:
    Adafruit_GFX *create(int8_t cs, int8_t dc, int8_t rst) override
    {
        Adafruit_ST7796S *tft = new Adafruit_ST7796S(cs, dc, rst);
        tft->init(320, 480, 0, 0, ST7796S_BGR);
        tft->setRotation(1);
        tft->invertDisplay(true);
        return tft;
    }

    void destroy(Adafruit_GFX *display) override
    {
        delete display;
    }

    const char *name() const override
    {
        return "ST7796S 320x480";
    }

    uint16_t width() const override
    {
        return 480; // After rotation 1
    }

    uint16_t height() const override
    {
        return 320; // After rotation 1
    }
};

#endif // DISPLAY_ST7796_H
