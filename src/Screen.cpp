/**
 * @file Screen.cpp
 * @brief Implementation of the Screen base class for screen management
 *
 * This file provides the concrete implementation of the Screen class lifecycle methods.
 * The Screen class serves as the foundation for all user interface screens in the library.
 */

#include "Screen.h"
#include <Adafruit_GFX.h>

/**
 * @brief Default constructor - initializes screen in inactive state
 *
 * Creates a new Screen instance that is ready for activation but not yet
 * displaying content. The screen must be activated via open() before it
 * will respond to input or display content.
 */
Screen::Screen()
{
    _active = false;
}

/**
 * @brief Check the current activation state of the screen
 *
 * @return true if the screen is active and should be processing updates/input
 * @return false if the screen is inactive
 */
bool Screen::isActive() const
{
    return _active;
}

/**
 * @brief Activate the screen and perform initial rendering
 *
 * Transitions the screen from inactive to active state and triggers the
 * initial display setup. This method implements the standard activation
 * pattern used by all screens in the system.
 *
 * The activation process:
 * 1. Check if already active (safe to call multiple times)
 * 2. Set active flag to true
 * 3. Call _drawScreen() for initial rendering
 *
 * @return true if activation was successful
 * @return false if activation failed (not implemented in base class)
 *
 * @note Derived classes can override this method to add custom initialization
 *       logic, but should call the parent implementation for standard behavior.
 *
 * @example Custom Initialization
 * ```cpp
 * bool MyScreen::open() {
 *     // Custom setup logic
 *     initializeData();
 *
 *     // Call parent for standard activation
 *     return Screen::open();
 * }
 * ```
 */
bool Screen::open()
{
    if (_active)
    {
        return true; // Already active
    }

    _active = true;
    _drawScreen(); // Trigger initial rendering

    return true;
}

/**
 * @brief Deactivate the screen and perform cleanup
 *
 * Transitions the screen from active to inactive state. In the base
 * implementation, this simply clears the active flag. Derived classes
 * can override this method to perform custom cleanup operations.
 *
 * @note Safe to call multiple times on inactive screens
 * @note After calling close(), the screen should no longer respond to
 *       loop() or actionTaken() calls until open() is called again
 *
 * @example Custom Cleanup
 * ```cpp
 * void MyScreen::close() {
 *     // Custom cleanup logic
 *     releaseResources();
 *     saveState();
 *
 *     // Call parent for standard deactivation
 *     Screen::close();
 * }
 * ```
 */
void Screen::close()
{
    if (!_active)
    {
        return; // Already closed
    }

    _active = false;
}

/**
 * @brief Force a complete redraw of the screen content
 *
 * Triggers a full screen redraw by calling _drawScreen() if the screen
 * is currently active. This is useful when the display content may have
 * been corrupted or when a complete refresh is needed.
 *
 * Common use cases:
 * - Display corruption recovery
 * - External display state changes
 * - Manual refresh after bulk data updates
 *
 * @note Only active screens will be redrawn. Inactive screens ignore refresh calls.
 * @note This calls _drawScreen() directly, not the ongoing loop() updates
 *
 * @example Refresh After Data Update
 * ```cpp
 * void updateDisplayData() {
 *     // Bulk update of display data
 *     loadNewContent();
 *
 *     // Force immediate redraw
 *     myScreen.refresh();
 * }
 * ```
 */
void Screen::refresh()
{
    if (_active)
    {
        _drawScreen(); // Redraw the screen content
    }
}
