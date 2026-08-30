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

// Manages multiple render targets
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
};

#endif /* RENDER_MANAGER_H */