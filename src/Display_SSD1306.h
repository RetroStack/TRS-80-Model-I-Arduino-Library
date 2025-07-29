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
private:
    Adafruit_SSD1306 *_display;

public:
    Display_SSD1306() : _display(nullptr) {}

    bool create(int8_t cs, int8_t dc, int8_t rst) override
    {
        if (_display)
        {
            delete _display;
        }
        _display = new Adafruit_SSD1306(cs, dc, rst);
        _display->begin(SSD1306_SWITCHCAPVCC, 0x3C);
        _display->clearDisplay();
        return _display != nullptr;
    }

    Adafruit_GFX &getGFX() override
    {
        return *_display;
    }

    void destroy() override
    {
        if (_display)
        {
            delete _display;
            _display = nullptr;
        }
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

    ~Display_SSD1306() override
    {
        destroy();
    }
};

#endif // DISPLAY_SSD1306_H
