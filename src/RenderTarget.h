/*
 * RenderTarget.h - Abstract interface for rendering targets
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef RENDER_TARGET_H
#define RENDER_TARGET_H

#include <Arduino.h>

// Forward declarations
class Screen;

// Abstract base class for render targets
class RenderTarget
{
public:
    virtual ~RenderTarget() = default;

    // Configuration and identification
    virtual const char *getName() const = 0;
    virtual bool isEnabled() const = 0;
    virtual void setEnabled(bool enabled) = 0;
};

#endif /* RENDER_TARGET_H */