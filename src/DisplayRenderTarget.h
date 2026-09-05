/*
 * DisplayRenderTarget.h - Render target backed by a DisplayProvider
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef DISPLAY_RENDER_TARGET_H
#define DISPLAY_RENDER_TARGET_H

#include "RenderTarget.h"
#include "DisplayProvider.h"
#include <Adafruit_GFX.h>

// Render target that draws through a DisplayProvider - the panel attached to
// the shield. M1Shield creates and registers one of these in begin(), and its
// own display accessors delegate here.
//
// The provider is held by reference: a target without a surface to draw on has
// no valid meaning, so it cannot be constructed in that state.
class DisplayRenderTarget : public RenderTarget
{
private:
    DisplayProvider *_displayProvider; // Never null; pointer only so it can be reseated
    bool _enabled;

public:
    explicit DisplayRenderTarget(DisplayProvider &provider);
    virtual ~DisplayRenderTarget();

    // Reset and create the panel on the given pins. Pass rst = -1 when the
    // panel has no reset line of its own - sharing another panel's reset pin
    // would reset that panel too.
    bool begin(int8_t cs, int8_t dc, int8_t rst = -1);

    // Swap the backing provider (does not take ownership of either)
    void setDisplayProvider(DisplayProvider &provider);
    DisplayProvider &getDisplayProvider() const;
    DisplayProvider *getProvider() const override; // RenderTarget's optional accessor

    // RenderTarget interface
    const char *getName() const override;
    bool isEnabled() const override;
    void setEnabled(bool enabled) override;

    Adafruit_GFX &getGFX() override;
    uint16_t getScreenWidth() const override;
    uint16_t getScreenHeight() const override;
    uint16_t convertColor(uint16_t color) override;

    bool display() override;
};

#endif /* DISPLAY_RENDER_TARGET_H */
