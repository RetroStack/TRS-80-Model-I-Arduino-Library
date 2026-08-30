/*
 * DisplayRenderTarget.cpp - Render target backed by a DisplayProvider
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#include "DisplayRenderTarget.h"

// Constructor - the provider is required and is not owned by this target
DisplayRenderTarget::DisplayRenderTarget(DisplayProvider &provider)
    : _displayProvider(&provider), _enabled(true)
{
}

// Destructor - the DisplayProvider is owned by the caller, not by this target
DisplayRenderTarget::~DisplayRenderTarget()
{
}

// Reset and create the panel this target draws to
bool DisplayRenderTarget::begin(int8_t cs, int8_t dc, int8_t rst)
{
    // Same reset sequence M1Shield performs for the primary panel
    if (rst >= 0)
    {
        pinMode(rst, OUTPUT);
        digitalWrite(rst, LOW);
        delay(50);
        digitalWrite(rst, HIGH);
        delay(50);
    }

    return _displayProvider->create(cs, dc, rst);
}

// Swap the backing provider
void DisplayRenderTarget::setDisplayProvider(DisplayProvider &provider)
{
    _displayProvider = &provider;
}

// Get the backing provider
DisplayProvider &DisplayRenderTarget::getDisplayProvider() const
{
    return *_displayProvider;
}

// Get render target name
const char *DisplayRenderTarget::getName() const
{
    return _displayProvider->name();
}

// Check if render target is enabled
bool DisplayRenderTarget::isEnabled() const
{
    return _enabled;
}

// Enable or disable the render target
void DisplayRenderTarget::setEnabled(bool enabled)
{
    _enabled = enabled;
}

// Get the graphics context to draw into
Adafruit_GFX &DisplayRenderTarget::getGFX()
{
    return _displayProvider->getGFX();
}

// Get display width in pixels
uint16_t DisplayRenderTarget::getScreenWidth() const
{
    return _displayProvider->width();
}

// Get display height in pixels
uint16_t DisplayRenderTarget::getScreenHeight() const
{
    return _displayProvider->height();
}

// Convert a color into the panel's format
uint16_t DisplayRenderTarget::convertColor(uint16_t color)
{
    return _displayProvider->convertColor(color);
}

// Push the framebuffer to the panel
bool DisplayRenderTarget::display()
{
    return _displayProvider->display();
}
