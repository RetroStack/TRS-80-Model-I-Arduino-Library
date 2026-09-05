/*
 * Display_ST7735.h - Factory for creating ST7735 display instances
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef DISPLAY_ST7735_H
#define DISPLAY_ST7735_H

#include <Adafruit_ST7735.h>
#include "DisplayProviderTFT.h"

class Display_ST7735 : public DisplayProviderTFT<Adafruit_ST7735>
{
public:
    bool create(int8_t cs, int8_t dc, int8_t rst) override
    {
        if (!_reset(new Adafruit_ST7735(cs, dc, rst)))
        {
            return false;
        }

        _display->initR(INITR_BLACKTAB);
        _display->setRotation(3);
        return true;
    }

    const char *getName() const override
    {
        return "ST7735 128x160";
    }

    uint16_t getScreenWidth() const override
    {
        return 128;
    }

    uint16_t getScreenHeight() const override
    {
        return 160;
    }
};

#endif /* DISPLAY_ST7735_H */
