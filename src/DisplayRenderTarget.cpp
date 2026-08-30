/*
 * DisplayRenderTarget.cpp - Render target that wraps a DisplayProvider for graphics output
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#include "DisplayRenderTarget.h"

// Constructor
DisplayRenderTarget::DisplayRenderTarget(DisplayProvider *provider) 
    : _displayProvider(provider), _enabled(true)
{
}

// Destructor
DisplayRenderTarget::~DisplayRenderTarget()
{
    // We don't own the DisplayProvider, so we don't clean it up
}

// Set the display provider
void DisplayRenderTarget::setDisplayProvider(DisplayProvider *provider)
{
    _displayProvider = provider;
}

// Get the display provider
DisplayProvider *DisplayRenderTarget::getDisplayProvider() const
{
    return _displayProvider;
}

// Get render target name
const char *DisplayRenderTarget::getName() const
{
    if (_displayProvider)
    {
        return _displayProvider->name();
    }
    return "Display (No Provider)";
}

// Check if render target is enabled
bool DisplayRenderTarget::isEnabled() const
{
    return _enabled && _displayProvider != nullptr;
}

// Enable or disable the render target
void DisplayRenderTarget::setEnabled(bool enabled)
{
    _enabled = enabled;
}

// Get display width (similar to M1Shield::getScreenWidth())
uint16_t DisplayRenderTarget::getScreenWidth() const
{
    if (_displayProvider)
    {
        return _displayProvider->width();
    }
    return 0;
}

// Get display height (similar to M1Shield::getScreenHeight())
uint16_t DisplayRenderTarget::getScreenHeight() const
{
    if (_displayProvider)
    {
        return _displayProvider->height();
    }
    return 0;
}

// Check if display is small (similar to Screen::isSmallDisplay())
bool DisplayRenderTarget::isSmallDisplay() const
{
    return getScreenHeight() <= 128;
}

// Get GFX reference (similar to M1Shield::getGFX())
Adafruit_GFX &DisplayRenderTarget::getGFX() const
{
    if (_displayProvider)
    {
        return _displayProvider->getGFX();
    }
    
    // This should not happen if used correctly, but we need to return something
    // In a real implementation, you might want to throw an exception or return a null object
    static Adafruit_GFX *nullGfx = nullptr; // This will cause a crash if accessed - intentional for debugging
    return *nullGfx;
}

// Convert color (similar to M1Shield::convertColor())
uint16_t DisplayRenderTarget::convertColor(uint16_t color) const
{
    if (_displayProvider)
    {
        return _displayProvider->convertColor(color);
    }
    return color; // Return unchanged if no provider
}

// Update display (similar to M1Shield::display())
bool DisplayRenderTarget::display() const
{
    if (_displayProvider)
    {
        return _displayProvider->display();
    }
    return false;
}

// Check if display is initialized (similar to M1Shield::isDisplayInitialized())
bool DisplayRenderTarget::isDisplayInitialized() const
{
    return _displayProvider != nullptr;
}