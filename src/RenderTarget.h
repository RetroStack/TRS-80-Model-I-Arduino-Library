/*
 * RenderTarget.h - Abstract interface for rendering targets
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef RENDER_TARGET_H
#define RENDER_TARGET_H

#include <Arduino.h>
#include <Adafruit_GFX.h>

// A destination that screens can be drawn to.
//
// A target owns a drawing surface, knows its own dimensions, converts colors
// into whatever format it needs, and can push what has been drawn to the
// physical device. Implementations are registered with a RenderManager, which
// drives them as a group.
class RenderTarget
{
public:
    virtual ~RenderTarget() = default;

    // Configuration and identification
    virtual const char *getName() const = 0; // Human-readable target name
    virtual bool isEnabled() const = 0;      // Whether this target should be drawn to and presented
    virtual void setEnabled(bool enabled) = 0;

    // Drawing surface
    virtual Adafruit_GFX &getGFX() = 0;                // Graphics context to draw into
    virtual uint16_t getScreenWidth() const = 0;       // Target width in pixels
    virtual uint16_t getScreenHeight() const = 0;      // Target height in pixels
    virtual uint16_t convertColor(uint16_t color) = 0; // Convert a color to this target's format

    // Push whatever has been drawn to the physical device. Buffered targets
    // (OLED) transfer their framebuffer; direct-write targets (TFT) validate.
    virtual bool display() = 0;

    // Single home for the small-display rule, shared by every target.
    bool isSmallDisplay() const { return getScreenHeight() <= 128 || getScreenWidth() <= 128; }
};

#endif /* RENDER_TARGET_H */
