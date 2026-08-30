/*
 * RenderManager.cpp - Manages multiple render targets for M1Shield
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#include "RenderManager.h"
#include "Screen.h"

// Constructor - initialize empty target list
RenderManager::RenderManager() : _targetCount(0)
{
    // Initialize target array to nullptrs
    for (uint8_t i = 0; i < MAX_RENDER_TARGETS; i++)
    {
        _targets[i] = nullptr;
    }
}

// Destructor
RenderManager::~RenderManager()
{
    clearRenderTargets();
}

// Add a render target to the manager
bool RenderManager::addRenderTarget(RenderTarget *target)
{
    if (target == nullptr || _targetCount >= MAX_RENDER_TARGETS)
    {
        return false;
    }

    // Check if target is already registered
    for (uint8_t i = 0; i < _targetCount; i++)
    {
        if (_targets[i] == target)
        {
            return false; // Already registered
        }
    }

    // Add target to array
    _targets[_targetCount] = target;
    _targetCount++;
    return true;
}

// Remove a render target from the manager
bool RenderManager::removeRenderTarget(RenderTarget *target)
{
    if (target == nullptr || _targetCount == 0)
    {
        return false;
    }

    // Find target in array
    for (uint8_t i = 0; i < _targetCount; i++)
    {
        if (_targets[i] == target)
        {
            // Shift remaining targets down
            for (uint8_t j = i; j < _targetCount - 1; j++)
            {
                _targets[j] = _targets[j + 1];
            }
            _targets[_targetCount - 1] = nullptr;
            _targetCount--;
            return true;
        }
    }

    return false; // Target not found
}

// Clear all render targets
void RenderManager::clearRenderTargets()
{
    for (uint8_t i = 0; i < MAX_RENDER_TARGETS; i++)
    {
        _targets[i] = nullptr;
    }
    _targetCount = 0;
}

// Get number of registered render targets
uint8_t RenderManager::getRenderTargetCount() const
{
    return _targetCount;
}

// Get render target by index
RenderTarget *RenderManager::getRenderTarget(uint8_t index) const
{
    if (index < _targetCount)
    {
        return _targets[index];
    }
    return nullptr;
}
