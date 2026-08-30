/*
 * RenderManager.h - Manages multiple render targets for M1Shield
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#include <Arduino.h>
#include "RenderTarget.h"

// Maximum number of render targets that can be registered
constexpr uint8_t MAX_RENDER_TARGETS = 8;

// Holds the registered render targets and drives them as a group.
//
// Registration order is significant: target 0 is the primary target, and is
// what M1Shield's display accessors read dimensions, colors and the graphics
// context from.
class RenderManager
{
private:
    RenderTarget *_targets[MAX_RENDER_TARGETS];
    uint8_t _targetCount;

public:
    RenderManager();
    ~RenderManager();

    // Render target management
    bool addRenderTarget(RenderTarget *target);
    bool removeRenderTarget(RenderTarget *target);
    void clearRenderTargets();
    uint8_t getRenderTargetCount() const;
    RenderTarget *getRenderTarget(uint8_t index) const;

    // The primary target - target 0, or nullptr when none is registered
    RenderTarget *getPrimaryRenderTarget() const;

    // Push every enabled target. Returns true only if all of them succeeded;
    // with no enabled targets there is nothing to push, which is not a failure.
    bool displayAll();
};

#endif /* RENDER_MANAGER_H */
