/*
 * Display_ST7796.h - Factory for creating ST7796 display instances
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef DISPLAY_ST7796_H
#define DISPLAY_ST7796_H

#include <Adafruit_ST7796S.h>
#include "DisplayProviderTFT.h"

class Display_ST7796 : public DisplayProviderTFT<Adafruit_ST7796S>
{
public:
    bool create(int8_t cs, int8_t dc, int8_t rst) override
    {
        if (!_reset(new Adafruit_ST7796S(cs, dc, rst)))
        {
            return false;
        }

        _display->init(320, 480, 0, 0, ST7796S_BGR);
        _display->setRotation(1);
        _display->invertDisplay(true);
        return true;
    }

    const char *getName() const override
    {
        return "ST7796S 480x320";
    }

    uint16_t getScreenWidth() const override
    {
        return 480;
    }

    uint16_t getScreenHeight() const override
    {
        return 320;
    }
};

#endif /* DISPLAY_ST7796_H */
