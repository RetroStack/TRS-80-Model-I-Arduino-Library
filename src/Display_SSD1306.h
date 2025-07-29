/*
 * Display_SSD1306.h - Factory for creating SSD1306 display instances
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef DISPLAY_SSD1306_H
#define DISPLAY_SSD1306_H

#include <Adafruit_SSD1306.h>
#include "DisplayProvider.h"

class Display_SSD1306 : public DisplayProvider
{
public:
    Adafruit_GFX *create(int8_t cs, int8_t dc, int8_t rst) override
    {
        Adafruit_SSD1306 *display = new Adafruit_SSD1306(cs, dc, rst);
        display->begin(SSD1306_SWITCHCAPVCC, 0x3C);
        display->clearDisplay();
        return display;
    }

    void destroy(Adafruit_GFX *display) override
    {
        delete display;
    }

    const char *name() const override
    {
        return "SSD1306";
    }

    uint16_t width() const override
    {
        return 128;
    }

    uint16_t height() const override
    {
        return 64;
    }
};

#endif // DISPLAY_SSD1306_H
