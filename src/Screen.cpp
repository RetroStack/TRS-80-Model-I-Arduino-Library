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
            getLogger()->errF(F("Screen: Failed to allocate memory for title"));
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

    // Get length and allocate buffer
    size_t len = strlen_P((const char *)title);
    char *buffer = (char *)malloc(len + 1);
    if (buffer == nullptr)
    {
        if (getLogger())
        {
            getLogger()->errF(F("Screen: Failed to allocate memory for flash title"));
        }
        clearTitle();
        return;
    }

    // Copy from flash to RAM buffer
    strcpy_P(buffer, (const char *)title);

    // Delegate to regular setTitle method
    setTitle(buffer);

    // Free temporary buffer
    free(buffer);
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
