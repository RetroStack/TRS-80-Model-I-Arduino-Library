/*
 * Screen.cpp - Abstract base class for all UI screens
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#include "Screen.h"
#include "M1Shield.h"
#include <Adafruit_GFX.h>

// Constructor - initialize screen as inactive
Screen::Screen()
{
    _active = false;
    _logger = nullptr;
}

// Check if screen is currently active
bool Screen::isActive() const
{
    return _active;
}

// Set logger for debugging output
void Screen::setLogger(ILogger &logger)
{
    _logger = &logger;
}

// Get logger instance
ILogger *Screen::getLogger() const
{
    return _logger;
}

// Activate screen and perform initial setup
bool Screen::open()
{
    if (_active)
    {
        return true; // Already active
    }

    _active = true;
    _drawScreen();      // Trigger initial rendering
    M1Shield.display(); // Push changes to display

    return true;
}

// Deactivate screen and perform cleanup
void Screen::close()
{
    if (!_active)
    {
        return; // Already closed
    }

    _active = false;
}

// Force complete redraw of screen content
void Screen::refresh()
{
    if (_active)
    {
        _drawScreen();      // Redraw the screen content
        M1Shield.display(); // Push changes to display
    }
}

// Check if current display is small (height <= 128 pixels)
bool Screen::isSmallDisplay() const
{
    return M1Shield.getScreenHeight() <= 128;
}
