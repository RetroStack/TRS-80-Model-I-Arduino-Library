/*
 * Screen.cpp - Abstract base class for all UI screens
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#include "Screen.h"
#include "M1Shield.h"
#include "utils.h"
#include <Adafruit_GFX.h>

// Constructor - initialize screen as inactive
Screen::Screen()
{
    _active = false;
    _logger = nullptr;
    _title = nullptr;
}

// Destructor - cleanup title memory
Screen::~Screen()
{
    clearTitle();
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
    _redrawAll(); // Trigger initial rendering on every enabled target

    return true;
}

// Draw the screen to every enabled render target and push each one
void Screen::_redrawAll()
{
    M1Shield.renderAll([this]
                       {
                           _drawScreen();
                           M1Shield.display();
                       });
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
        _redrawAll();
    }
}

// Check if current display is small (either dimension <= 128 pixels)
bool Screen::isSmallDisplay() const
{
    // Testing height alone classified the 128x160 ST7735 as large, so it took
    // the full 320x240 template: a size-3 title on a 128 pixel width leaves
    // room for six characters, three of which the ellipsis then consumes.
    return M1Shield.getScreenHeight() <= 128 || M1Shield.getScreenWidth() <= 128;
}

// Set screen title/name
void Screen::setTitle(const char *title)
{
    clearTitle();

    // Allocate and copy new title if provided
    if (title != nullptr && title[0] != '\0')
    {
        size_t titleLen = strlen(title);
        _title = (char *)malloc(titleLen + 1); // +1 for null terminator
        if (_title != nullptr)
        {
            strcpy(_title, title); // Safe because we allocated exact size needed
        }
        else if (getLogger())
        {
            const char *currentTitle = getTitle();
            getLogger()->errF(F("Screen[%s]: Failed to allocate memory for title"),
                              currentTitle ? currentTitle : "Unknown");
        }
    }
}

// Set screen title from Arduino String object
void Screen::setTitle(String title)
{
    setTitle(title.c_str());
}

// Set screen title from FlashString (F() macro)
void Screen::setTitleF(const __FlashStringHelper *title)
{
    if (title == nullptr)
    {
        clearTitle();
        return;
    }

    FlashBuffer copy(title);
    if (!copy.valid())
    {
        if (getLogger())
        {
            const char *currentTitle = getTitle();
            getLogger()->errF(F("Screen[%s]: Failed to allocate memory for flash title"),
                              currentTitle ? currentTitle : "Unknown");
        }
        clearTitle();
        return;
    }

    setTitle(copy.c_str());
}

// Clear the current title
void Screen::clearTitle()
{
    if (_title != nullptr)
    {
        free(_title);
        _title = nullptr;
    }
}

// Get current screen title
const char *Screen::getTitle() const
{
    return _title; // May be nullptr if no title set
}
