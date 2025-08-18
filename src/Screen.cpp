/*
 * Screen.cpp - Abstract base class for all UI screens
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#include "Screen.h"
#include "M1Shield.h"
#include <Adafruit_GFX.h>

Screen::Screen()
{
    _active = false;
}

bool Screen::isActive() const
{
    return _active;
}

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

void Screen::close()
{
    if (!_active)
    {
        return; // Already closed
    }

    _active = false;
}

void Screen::refresh()
{
    if (_active)
    {
        _drawScreen();      // Redraw the screen content
        M1Shield.display(); // Push changes to display
    }
}

bool Screen::isSmallDisplay() const
{
    return M1Shield.getScreenHeight() <= 128;
}
