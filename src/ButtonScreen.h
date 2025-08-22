/*
 * ButtonScreen.h - Button screen with footer-based horizontal navigation
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef BUTTON_SCREEN_H
#define BUTTON_SCREEN_H

#include <Arduino.h>
#include "ContentScreen.h"

// Button screen class that displays button items horizontally in an expanded footer
class ButtonScreen : public ContentScreen
{
private:
    uint8_t _selectedButtonItemIndex; // Currently selected button item index (0-based)
    uint8_t _viewStartIndex;          // Index of the leftmost visible button item

    uint8_t _getMaxVisibleItems() const;                                  // Calculate maximum items that can fit horizontally
    uint8_t _findNextEnabledItem(uint8_t startIndex, bool forward) const; // Find the next enabled button item
    void _adjustViewWindow();                                             // Adjust the view window to ensure selected item is visible

protected:
    void _drawContent() override = 0;           // Pure virtual method for content area rendering
    void _drawFooter() override;                // Draw horizontal button display in expanded footer area
    uint16_t _getFooterHeight() const override; // Override to provide expanded footer height for horizontal buttons

    void _setSelectedButtonItemIndex(uint8_t index); // Set the currently selected button item by index
    uint8_t _getSelectedButtonItemIndex() const;     // Get the index of the currently selected button item

    // Abstract method to get the screen for a selected button item
    virtual Screen *_getSelectedButtonItemScreen(int index) = 0;

    // Get configuration value string for a button item (optional override)
    virtual const char *_getButtonItemConfigValue(uint8_t index)
    {
        (void)index;
        return nullptr;
    }

    // Get configuration value FlashString for a button item (optional override)
    virtual const __FlashStringHelper *_getButtonItemConfigValueF(uint8_t index)
    {
        (void)index;
        return nullptr;
    }

    // Check if a button item is enabled/selectable (optional override)
    virtual bool _isButtonItemEnabled(uint8_t index) const
    {
        (void)index;
        return true;
    }

public:
    ButtonScreen();          // Constructor
    virtual ~ButtonScreen(); // Destructor - ContentScreen handles button items cleanup

    bool open() override; // Initialize view when screen opens

    void loop() override; // Main loop processing for button screen updates

    // Handle user input actions and navigate accordingly
    Screen *actionTaken(ActionTaken action, int8_t offsetX, int8_t offsetY) override;

    void refreshButtons(); // Refresh the horizontal button display
};

#endif /* BUTTON_SCREEN_H */
