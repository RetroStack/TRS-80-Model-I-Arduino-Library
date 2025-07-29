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
    virtual bool create(int8_t cs, int8_t dc, int8_t rst) = 0;
    virtual void destroy() = 0;
    virtual Adafruit_GFX &getGFX() = 0;
    virtual bool display() = 0;
    virtual uint16_t convertColor(uint16_t color) = 0;
    virtual const char *name() const = 0;
    virtual uint16_t width() const = 0;
    virtual uint16_t height() const = 0;

    virtual ~DisplayProvider() = default;
};

#endif // DISPLAY_PROVIDER_H
