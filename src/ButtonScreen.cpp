/*
 * ButtonScreen.cpp - Button screen with footer-based horizontal navigation
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#include "ButtonScreen.h"
#include "M1Shield.h"
#include <Adafruit_GFX.h>

// Display Configuration Constants
constexpr uint8_t TEXT_SIZE_2_WIDTH = 12;      // Width of size-2 text characters
constexpr uint8_t TEXT_SIZE_2_HEIGHT = 16;     // Height of size-2 text characters
constexpr uint8_t TEXT_SIZE_2_HALF_HEIGHT = 8; // Half-height of size-2 text for centering

// Horizontal Button Layout Constants
constexpr uint16_t BUTTON_FOOTER_HEIGHT = 26;       // Height of expanded footer for horizontal buttons (2 rows of size-2 text)
constexpr uint16_t BUTTON_SMALL_FOOTER_HEIGHT = 20; // Height of expanded footer for small displays
constexpr uint16_t BUTTON_ITEM_HEIGHT = 16;         // Height of each button item row
constexpr uint16_t BUTTON_CONFIG_HEIGHT = 16;       // Height of config value row
constexpr uint16_t BUTTON_PADDING = 2;              // Padding between button items horizontally
constexpr uint16_t BUTTON_MIN_WIDTH = 100;          // Minimum width of button items to calculate number of buttons

// Color Constants
constexpr uint16_t BUTTON_COLOR_BG = 0x0000;          // Background color for horizontal buttons
constexpr uint16_t BUTTON_COLOR_FG = 0xFFFF;          // Foreground color for horizontal buttons
constexpr uint16_t BUTTON_SELECTED_COLOR_BG = 0xFFE0; // Background color for selected item (bright yellow)
constexpr uint16_t BUTTON_SELECTED_COLOR_FG = 0x0000; // Foreground color for selected item
constexpr uint16_t BUTTON_DISABLED_COLOR_BG = 0x2104; // Background color for disabled items
constexpr uint16_t BUTTON_DISABLED_COLOR_FG = 0x7BEF; // Foreground color for disabled items (gray)
constexpr uint16_t BUTTON_CONFIG_COLOR = 0xCCCC;      // Color for config values

// Constructor
ButtonScreen::ButtonScreen() : ContentScreen()
{
    _selectedButtonItemIndex = 0;
    _viewStartIndex = 0;
}

// Open the button screen
bool ButtonScreen::open()
{
    bool result = ContentScreen::open();

    // Find first enabled item when screen opens
    uint8_t buttonItemCount = _getButtonItemCount();
    if (buttonItemCount > 0)
    {
        _selectedButtonItemIndex = _findNextEnabledItem(0, true);
        _adjustViewWindow();
    }

    return result;
}

// Destructor
ButtonScreen::~ButtonScreen()
{
    // ContentScreen will handle cleanup of button items
}

// Calculate maximum items that can fit horizontally
uint8_t ButtonScreen::_getMaxVisibleItems() const
{
    return M1Shield.getScreenWidth() / BUTTON_MIN_WIDTH;
}

// Find the next enabled button item
uint8_t ButtonScreen::_findNextEnabledItem(uint8_t startIndex, bool forward) const
{
    uint8_t buttonItemCount = _getButtonItemCount();
    if (buttonItemCount == 0)
        return 0;

    // Ensure start index is valid
    if (startIndex >= buttonItemCount)
    {
        startIndex = buttonItemCount - 1;
    }

    uint8_t currentIndex = startIndex;
    uint8_t attempts = 0;

    do
    {
        if (_isButtonItemEnabled(currentIndex))
        {
            return currentIndex;
        }

        if (forward)
        {
            currentIndex = (currentIndex + 1) % buttonItemCount;
        }
        else
        {
            currentIndex = (currentIndex == 0) ? buttonItemCount - 1 : currentIndex - 1;
        }
        attempts++;
    } while (attempts < buttonItemCount);

    // If no enabled items found, return the original index
    return startIndex;
}

// Adjust view window to ensure selected item is visible
void ButtonScreen::_adjustViewWindow()
{
    uint8_t buttonItemCount = _getButtonItemCount();
    if (buttonItemCount == 0)
        return;

    uint8_t maxVisible = _getMaxVisibleItems();

    // If all items fit, start from 0
    if (buttonItemCount <= maxVisible)
    {
        _viewStartIndex = 0;
        return;
    }

    // If selected item is before view window, move window left
    if (_selectedButtonItemIndex < _viewStartIndex)
    {
        _viewStartIndex = _selectedButtonItemIndex;
    }
    // If selected item is after view window, move window right
    else if (_selectedButtonItemIndex >= _viewStartIndex + maxVisible)
    {
        _viewStartIndex = _selectedButtonItemIndex - maxVisible + 1;
    }
    // Ensure view doesn't go past the end
    if (_viewStartIndex + maxVisible > buttonItemCount)
    {
        _viewStartIndex = buttonItemCount - maxVisible;
    }
}

// Set the currently selected button item by index
void ButtonScreen::_setSelectedButtonItemIndex(uint8_t index)
{
    uint8_t buttonItemCount = _getButtonItemCount();
    if (index < buttonItemCount)
    {
        _selectedButtonItemIndex = index;
        _adjustViewWindow();

        if (getLogger())
        {
            const char *title = getTitle();
            getLogger()->infoF(F("ButtonScreen[%s]: Selected item %d, view starts at %d"),
                               title ? title : "Unknown", _selectedButtonItemIndex, _viewStartIndex);
        }
    }
}

// Get the index of the currently selected button item
uint8_t ButtonScreen::_getSelectedButtonItemIndex() const
{
    return _selectedButtonItemIndex;
}

// Get the height needed for horizontal button footer
uint16_t ButtonScreen::_getFooterHeight() const
{
    return isSmallDisplay() ? BUTTON_SMALL_FOOTER_HEIGHT : BUTTON_FOOTER_HEIGHT;
}

// Main loop for horizontal button screen
void ButtonScreen::loop()
{
    ContentScreen::loop(); // Handle notification timeouts
}

// Handle user input actions and navigation
Screen *ButtonScreen::actionTaken(ActionTaken action, int8_t offsetX, int8_t offsetY)
{
    if (!isActive())
    {
        return nullptr;
    }

    // Menu selection - activate the selected item (only if enabled)
    if (action & (BUTTON_UP | BUTTON_DOWN | BUTTON_JOYSTICK | JOYSTICK_UP | JOYSTICK_DOWN) ||
        ((action & (JOYSTICK_UP_LEFT | JOYSTICK_UP_RIGHT | JOYSTICK_DOWN_LEFT | JOYSTICK_DOWN_RIGHT)) && offsetY > offsetX))
    {
        uint8_t selectedIndex = _getSelectedButtonItemIndex();
        if (_isButtonItemEnabled(selectedIndex))
        {
            uint8_t selectedIndex = _getSelectedButtonItemIndex();
            if (getLogger())
            {
                const char *title = getTitle();
                getLogger()->infoF(F("ButtonScreen[%s]: Selecting button item %d"),
                                   title ? title : "Unknown", selectedIndex);
            }
            return _getSelectedButtonItemScreen(selectedIndex);
        }
        // If current item is disabled, don't activate it
        if (getLogger())
        {
            const char *title = getTitle();
            getLogger()->warnF(F("ButtonScreen[%s]: Attempted to select disabled button item %d"),
                               title ? title : "Unknown", selectedIndex);
        }
        return nullptr;
    }

    // Exit menu - return to previous screen
    if (action & BUTTON_MENU)
    {
        if (getLogger())
        {
            const char *title = getTitle();
            getLogger()->infoF(F("ButtonScreen[%s]: Exiting button screen via menu button"),
                               title ? title : "Unknown");
        }
        return _getSelectedButtonItemScreen(-1);
    }

    // Navigate button items horizontally - handle directional input and skip disabled items
    if (action != NONE)
    {
        uint8_t currentSelection = _getSelectedButtonItemIndex();
        uint8_t buttonItemCount = _getButtonItemCount();

        // Move left - find previous enabled item
        if (action & (BUTTON_LEFT | JOYSTICK_LEFT) ||
            ((action & (JOYSTICK_UP_LEFT | JOYSTICK_DOWN_LEFT)) && offsetX > offsetY))
        {
            uint8_t nextSelection;
            if (currentSelection > 0)
            {
                nextSelection = _findNextEnabledItem(currentSelection - 1, false);
            }
            else
            {
                nextSelection = _findNextEnabledItem(buttonItemCount - 1, false); // Wrap to end
            }
            _setSelectedButtonItemIndex(nextSelection);
            refreshButtons();
        }
        // Move right - find next enabled item
        else if (action & (BUTTON_RIGHT | JOYSTICK_RIGHT) ||
                 ((action & (JOYSTICK_UP_RIGHT | JOYSTICK_DOWN_RIGHT)) && offsetX > offsetY))
        {
            uint8_t nextSelection;
            if (currentSelection < buttonItemCount - 1)
            {
                nextSelection = _findNextEnabledItem(currentSelection + 1, true);
            }
            else
            {
                nextSelection = _findNextEnabledItem(0, true); // Wrap to beginning
            }
            _setSelectedButtonItemIndex(nextSelection);
            refreshButtons();
        }
    }

    // No new screen - stay on current screen
    return nullptr;
}

// Draw horizontal button display in expanded footer area
void ButtonScreen::_drawFooter()
{
    uint8_t buttonItemCount = _getButtonItemCount();
    if (buttonItemCount == 0)
        return;

    uint16_t screenWidth = M1Shield.getScreenWidth();
    uint16_t footerHeight = _getFooterHeight();
    uint16_t footerY = _getFooterTop();

    Adafruit_GFX &gfx = M1Shield.getGFX();

    // Clear footer background
    gfx.fillRect(0, footerY, screenWidth, footerHeight, M1Shield.convertColor(BUTTON_COLOR_BG));

    uint8_t maxVisible = _getMaxVisibleItems();
    uint8_t itemsToShow = min(maxVisible, buttonItemCount - _viewStartIndex);

    if (itemsToShow == 0)
        return;

    // Calculate item width (equal distribution)
    uint16_t itemWidth = screenWidth / maxVisible;

    bool isSmall = isSmallDisplay();
    uint8_t textSize = isSmall ? 1 : 2;
    uint8_t textHeight = isSmall ? 8 : 16;
    uint8_t configTextHeight = isSmall ? 8 : 16;

    gfx.setTextSize(textSize);

    // Draw each visible button item
    for (uint8_t i = 0; i < itemsToShow; i++)
    {
        uint8_t itemIndex = _viewStartIndex + i;
        uint16_t itemX = i * itemWidth;
        bool isSelected = (itemIndex == _selectedButtonItemIndex);
        bool isEnabled = _isButtonItemEnabled(itemIndex);

        // Choose colors based on state
        uint16_t bgColor, fgColor;
        if (!isEnabled)
        {
            bgColor = BUTTON_DISABLED_COLOR_BG;
            fgColor = BUTTON_DISABLED_COLOR_FG;
        }
        else if (isSelected)
        {
            bgColor = BUTTON_SELECTED_COLOR_BG;
            fgColor = BUTTON_SELECTED_COLOR_FG;
        }
        else
        {
            bgColor = BUTTON_COLOR_BG;
            fgColor = BUTTON_COLOR_FG;
        }

        // Draw item background
        gfx.fillRect(itemX, footerY, itemWidth, footerHeight, M1Shield.convertColor(bgColor));

        // Draw button item text (first row)
        const char *buttonItemText = _getButtonItem(itemIndex);
        if (buttonItemText != nullptr)
        {
            gfx.setTextColor(M1Shield.convertColor(fgColor));

            // Truncate text to fit in item width
            String itemText = String(buttonItemText);
            uint8_t charWidth = isSmall ? 6 : 12;
            uint8_t maxChars = (itemWidth - 4) / charWidth; // 4 pixels margin

            if (itemText.length() > maxChars && maxChars > 3)
            {
                itemText = itemText.substring(0, maxChars - 3) + "...";
            }
            else if (itemText.length() > maxChars)
            {
                itemText = itemText.substring(0, maxChars);
            }

            // Center text horizontally in item
            uint16_t textWidth = itemText.length() * charWidth;
            uint16_t textX = itemX + (itemWidth - textWidth) / 2;
            uint16_t textY = footerY + 1; // Minimal margin from top

            gfx.setCursor(textX, textY);
            gfx.print(itemText);
        }

        // Draw config value (second row)
        const __FlashStringHelper *configValueF = _getButtonItemConfigValueF(itemIndex);
        const char *configValue = nullptr;
        char *tempConfigBuffer = nullptr;

        if (configValueF != nullptr)
        {
            // Convert FlashString to regular string for display
            size_t len = strlen_P((const char *)configValueF);
            tempConfigBuffer = (char *)malloc(len + 1);
            if (tempConfigBuffer != nullptr)
            {
                strcpy_P(tempConfigBuffer, (const char *)configValueF);
                configValue = tempConfigBuffer;
            }
            else
            {
                // Log malloc failure with screen title context
                if (getLogger())
                {
                    const char *title = getTitle();
                    getLogger()->errF(F("ButtonScreen[%s]: Failed to allocate %d bytes for config value display"),
                                      title ? title : "Unknown", len + 1);
                }
            }
        }
        else
        {
            configValue = _getButtonItemConfigValue(itemIndex);
        }

        if (configValue != nullptr)
        {
            // Set text size to 1 for config values (second row)
            gfx.setTextSize(1);
            gfx.setTextColor(M1Shield.convertColor(isSelected ? fgColor : BUTTON_CONFIG_COLOR));

            // Truncate config text to fit (use size-1 character width)
            String configText = String(configValue);
            uint8_t configCharWidth = 6; // Always 6 pixels for size-1 text
            uint8_t maxChars = (itemWidth - 4) / configCharWidth;

            if (configText.length() > maxChars && maxChars > 3)
            {
                configText = configText.substring(0, maxChars - 3) + "...";
            }
            else if (configText.length() > maxChars)
            {
                configText = configText.substring(0, maxChars);
            }

            // Center config text horizontally in item (use size-1 text width)
            uint16_t configTextWidth = configText.length() * configCharWidth;
            uint16_t textX = itemX + (itemWidth - configTextWidth) / 2;
            uint16_t textY = footerY + textHeight + 1; // Minimal gap between rows

            gfx.setCursor(textX, textY);
            gfx.print(configText);

            // Reset text size back to original for next iteration
            gfx.setTextSize(textSize);
        }

        // Free temporary config buffer
        if (tempConfigBuffer != nullptr)
        {
            free(tempConfigBuffer);
        }

        // Draw border between items (except last one)
        if (i < itemsToShow - 1 && !isSmall)
        {
            uint16_t borderX = itemX + itemWidth - 1;
            gfx.drawFastVLine(borderX, footerY, footerHeight, M1Shield.convertColor(0x4444));
        }
    }
}

// Refresh the horizontal button display
void ButtonScreen::refreshButtons()
{
    if (isActive())
    {
        // Redraw just the horizontal button area
        Adafruit_GFX &gfx = M1Shield.getGFX();
        gfx.startWrite();
        _drawFooter();
        gfx.endWrite();
    }
}
