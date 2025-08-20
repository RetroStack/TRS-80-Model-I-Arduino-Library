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
    virtual bool create(int8_t cs, int8_t dc, int8_t rst) = 0; // Create display instance with specified pins
    virtual void destroy() = 0;                                // Destroy display instance and free resources
    virtual ~DisplayProvider() = default;                      // Virtual destructor

    virtual Adafruit_GFX &getGFX() = 0; // Get reference to Adafruit_GFX interface

    virtual bool display() = 0;                        // Update physical display with current buffer contents
    virtual uint16_t convertColor(uint16_t color) = 0; // Convert 16-bit color to display-specific format

    virtual const char *name() const = 0; // Get display provider name
    virtual uint16_t width() const = 0;   // Get display width in pixels
    virtual uint16_t height() const = 0;  // Get display height in pixels
};

#endif // DISPLAY_PROVIDER_H
