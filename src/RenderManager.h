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
    RenderTarget *_current; // Target being drawn during a render pass; nullptr otherwise

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

    // Render pass state.
    //
    // During a pass the manager nominates one target as active, and M1Shield's
    // display accessors resolve to it - which is how existing drawing code
    // reaches every target without changing a single call site.
    //
    // Outside a pass the active target is the primary, whether or not it is
    // enabled: target 0 is the layout authority, so disabling the panel must
    // not silently reflow the UI to whatever target 1 happens to be.
    RenderTarget *getActiveTarget() const; // Current target, or the primary outside a pass
    RenderTarget *getPassTarget() const;   // Current target, or nullptr outside a pass
    bool inRenderPass() const;             // True while a pass is running
    void beginRenderPass(RenderTarget *target);
    void endRenderPass();
};

#endif /* RENDER_MANAGER_H */
