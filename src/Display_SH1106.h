/*
 * Display_SSD1306.h - Factory for creating SSD1306 display instances
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef DISPLAY_SH1106_H
#define DISPLAY_SH1106_H

#include <Wire.h>
#include <Adafruit_SH110X.h>
#include "DisplayProvider.h"

class Display_SH1106 : public DisplayProvider
{
public:
    Adafruit_GFX *create(int8_t cs, int8_t dc, int8_t rst) override
    {
        Adafruit_SH1106G *display = new Adafruit_SH1106G(128, 64, &Wire, -1);
        delay(1000);
        display->begin(0x3C, true); // Default I2C address for SH1106
        display->clearDisplay();
        return display;
    }

    void destroy(Adafruit_GFX *display) override
    {
        delete display;
    }

    const char *name() const override
    {
        return "SH1106";
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

#endif // DISPLAY_SH1106_H
