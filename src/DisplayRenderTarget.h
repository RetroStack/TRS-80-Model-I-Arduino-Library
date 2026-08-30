/*
 * DisplayRenderTarget.h - Render target that wraps a DisplayProvider for graphics output
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef DISPLAY_RENDER_TARGET_H
#define DISPLAY_RENDER_TARGET_H

#include "RenderTarget.h"
#include "DisplayProvider.h"
#include <Adafruit_GFX.h>

// Render target that provides display functionality using a DisplayProvider
// This moves display-related functionality from M1Shield into a render target
class DisplayRenderTarget : public RenderTarget
{
private:
    DisplayProvider *_displayProvider;
    bool _enabled;

public:
    DisplayRenderTarget(DisplayProvider *provider = nullptr);
    virtual ~DisplayRenderTarget();

    // Set/get the display provider
    void setDisplayProvider(DisplayProvider *provider);
    DisplayProvider *getDisplayProvider() const;

    // RenderTarget interface
    const char *getName() const override;
    bool isEnabled() const override;
    void setEnabled(bool enabled) override;

    // Display properties (similar to M1Shield interface)
    uint16_t getScreenWidth() const;
    uint16_t getScreenHeight() const;
    bool isSmallDisplay() const; // Check if display height <= 128 pixels
    
    // Graphics access (similar to M1Shield interface)
    Adafruit_GFX &getGFX() const;
    uint16_t convertColor(uint16_t color) const;
    
    // Display update (similar to M1Shield interface)
    bool display() const; // Update the display (push framebuffer for OLED, validate for TFT)
    
    // Display initialization status
    bool isDisplayInitialized() const;
};

#endif /* DISPLAY_RENDER_TARGET_H */