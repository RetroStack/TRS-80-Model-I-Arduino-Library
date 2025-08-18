/*
 * Display_ST7789_320x240.h - Factory for creating ST7789 display instances
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef DISPLAY_ST7789_320X240_H
#define DISPLAY_ST7789_320X240_H

#include <Adafruit_ST7789.h>
#include "DisplayProvider.h"

class Display_ST7789_320x240 : public DisplayProvider
{
private:
    Adafruit_ST7789 *_display;

public:
    // Constructor
    Display_ST7789_320x240() : _display(nullptr) {}

    // Create ST7789 display instance with specified pins
    bool create(int8_t cs, int8_t dc, int8_t rst) override
    {
        if (_display)
        {
            delete _display;
        }
        _display = new Adafruit_ST7789(cs, dc, rst);
        _display->init(240, 320, SPI_MODE0);
        _display->setRotation(3);
        return _display != nullptr;
    }

    // Get reference to Adafruit_GFX interface
    Adafruit_GFX &getGFX() override
    {
        return *_display;
    }

    // Update display (no-op for TFT displays)
    bool display() override
    {
        // TFT displays update immediately, no explicit display() call needed
        return (_display != nullptr);
    }

    // Convert color to display-specific format (ST7789 uses RGB565 directly)
    uint16_t convertColor(uint16_t color) override
    {
        // ST7789 uses 16-bit RGB565 format directly
        return color;
    }

    // Destroy display instance and free resources
    void destroy() override
    {
        if (_display)
        {
            delete _display;
            _display = nullptr;
        }
    }

    // Get display provider name
    const char *name() const override
    {
        return "ST7789 320x240";
    }

    // Get display width in pixels
    uint16_t width() const override
    {
        return 320; // After rotation 3
    }

    // Get display height in pixels
    uint16_t height() const override
    {
        return 240; // After rotation 3
    }

    // Destructor
    ~Display_ST7789_320x240() override
    {
        destroy();
    }
};

#endif // DISPLAY_ST7789_320X240_H
