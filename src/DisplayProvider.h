/*
 * DisplayProvider.h - Factory for creating display instances
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef DISPLAY_PROVIDER_H
#define DISPLAY_PROVIDER_H

#include <Adafruit_GFX.h>

class DisplayProvider
{
public:
    // Create display instance with specified pins
    virtual bool create(int8_t cs, int8_t dc, int8_t rst) = 0;

    // Destroy display instance and free resources
    virtual void destroy() = 0;

    // Get reference to Adafruit_GFX interface
    virtual Adafruit_GFX &getGFX() = 0;

    // Update physical display with current buffer contents
    virtual bool display() = 0;

    // Convert 16-bit color to display-specific format
    virtual uint16_t convertColor(uint16_t color) = 0;

    // Get display provider name
    virtual const char *name() const = 0;

    // Get display width in pixels
    virtual uint16_t width() const = 0;

    // Get display height in pixels
    virtual uint16_t height() const = 0;

    // Virtual destructor
    virtual ~DisplayProvider() = default;
};

#endif // DISPLAY_PROVIDER_H
