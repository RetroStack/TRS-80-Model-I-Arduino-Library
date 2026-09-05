/*
 * DisplayProviderTFT.h - Shared base for the SPI TFT display providers
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef DISPLAY_PROVIDER_TFT_H
#define DISPLAY_PROVIDER_TFT_H

#include "DisplayProvider.h"

// Every SPI TFT provider owns one Adafruit driver object and answers the same
// five members identically; only the concrete type, the initialisation calls
// and the reported geometry differ. Those stay in the provider, everything
// else lives here.
//
// Each sketch activates one provider, so this instantiates once or twice per
// binary rather than once per provider -- no flash cost against the
// hand-written copies it replaces.
template <typename TDisplay>
class DisplayProviderTFT : public DisplayProvider
{
protected:
    TDisplay *_display;

    // Replaces any existing instance and reports whether the new one was
    // allocated. Call this first in create(): the hand-written providers tested
    // the pointer only after already calling init() through it, so the check
    // could never have caught anything.
    bool _reset(TDisplay *created)
    {
        destroy();
        _display = created;
        return _display != nullptr;
    }

public:
    DisplayProviderTFT() : _display(nullptr) {}

    Adafruit_GFX &getGFX() override
    {
        return *_display;
    }

    bool display() override
    {
        // TFT displays update immediately, no explicit display() call needed
        return (_display != nullptr);
    }

    uint16_t convertColor(uint16_t color) override
    {
        // These panels take 16-bit RGB565 directly
        return color;
    }

    void destroy() override
    {
        if (_display)
        {
            delete _display;
            _display = nullptr;
        }
    }

    ~DisplayProviderTFT() override
    {
        destroy();
    }
};

#endif /* DISPLAY_PROVIDER_TFT_H */
