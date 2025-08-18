/*
 * MenuScreen.cpp - Complete menu framework with pagination and navigation
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#include "MenuScreen.h"
#include "M1Shield.h"
#include <Adafruit_GFX.h>

// Display Configuration Constants
constexpr uint8_t TEXT_SIZE_1_WIDTH = 6;       // Width of size-1 text characters
constexpr uint8_t TEXT_SIZE_1_HALF_HEIGHT = 3; // Half-height of size-1 text for centering

constexpr uint8_t TEXT_SIZE_2_WIDTH = 12;      // Width of size-2 text characters
constexpr uint8_t TEXT_SIZE_2_HALF_HEIGHT = 6; // Half-height of size-2 text for centering

constexpr uint8_t TEXT_SIZE_3_WIDTH = 18;      // Width of size-3 text characters
constexpr uint8_t TEXT_SIZE_3_HALF_HEIGHT = 8; // Half-height of size-3 text for centering

// Menu Layout Constants
constexpr uint16_t ROW_HEIGHT = 26;                       // Height of each menu row in pixels
constexpr uint16_t ROW_SMALL_HEIGHT = 10;                 // Small height of each menu row in pixels
constexpr uint16_t ROW_COLOR_BG1 = 0x4208;                // Background color for odd rows (dark blue)
constexpr uint16_t ROW_COLOR_FG1 = 0xFFFF;                // Foreground color for odd rows
constexpr uint16_t ROW_COLOR_BG2 = 0x39C7;                // Background color for even rows (darker blue)
constexpr uint16_t ROW_COLOR_FG2 = 0xFFFF;                // Foreground color for even rows
constexpr uint16_t SELECTED_ROW_COLOR_BG = 0xFFE0;        // Background color for selected row (bright yellow)
constexpr uint16_t SELECTED_ROW_COLOR_FG = 0x0000;        // Foreground color for selected row
constexpr uint16_t DISABLED_ROW_COLOR_BG1 = 0x2104;       // Background color for disabled odd rows (darker)
constexpr uint16_t DISABLED_ROW_COLOR_FG1 = 0x7BEF;       // Foreground color for disabled odd rows (gray)
constexpr uint16_t DISABLED_ROW_COLOR_BG2 = 0x18E3;       // Background color for disabled even rows (darker)
constexpr uint16_t DISABLED_ROW_COLOR_FG2 = 0x7BEF;       // Foreground color for disabled even rows (gray)
constexpr uint16_t NEXT_PAGE_INDICATOR_COLOR_FG = 0xFFFF; // Color for next page indicator text
constexpr uint16_t TABLE_COLOR_BG = 0x0000;               // Color for table background

// Constructor
MenuScreen::MenuScreen() : ContentScreen()
{
    _currentPage = 0;
    _selectedMenuItemIndex = 0;

    _menuItems = nullptr;
    _menuItemCount = 0;

    // Set default button labels - can be overridden by derived classes
    const char *buttonItems[2] = {
        "[M/<] Exit ", "[>] Select"};
    setButtonItems(buttonItems, 2);
}

// Destructor
MenuScreen::~MenuScreen()
{
    clearMenuItems();
}

// Calculate how many menu items fit on one page
uint8_t MenuScreen::_getItemsPerPage() const
{
    uint16_t contentHeight = _getContentHeight();
    uint8_t itemsPerPage = contentHeight / (isSmallDisplay() ? ROW_SMALL_HEIGHT : ROW_HEIGHT);

    // Ensure at least 1 item per page, even with very small screens
    return (itemsPerPage > 0) ? itemsPerPage : 1;
}

uint8_t MenuScreen::_findNextEnabledItem(uint8_t startIndex, bool forward) const
{
    if (_menuItemCount == 0)
        return 0;

    // Ensure start index is valid
    if (startIndex >= _menuItemCount)
    {
        startIndex = _menuItemCount - 1;
    }

    uint8_t currentIndex = startIndex;
    uint8_t attempts = 0;

    do
    {
        if (_isMenuItemEnabled(currentIndex))
        {
            return currentIndex;
        }

        if (forward)
        {
            currentIndex = (currentIndex + 1) % _menuItemCount;
        }
        else
        {
            currentIndex = (currentIndex == 0) ? _menuItemCount - 1 : currentIndex - 1;
        }
        attempts++;
    } while (attempts < _menuItemCount);

    // If no enabled items found, return the original index
    return startIndex;
}

// Input Handling and Navigation

// Handle user input actions and navigation
Screen *MenuScreen::actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY)
{
    if (!isActive())
    {
        return nullptr;
    }

    // Menu selection - activate the selected item (only if enabled)
    if (action & (BUTTON_RIGHT | BUTTON_JOYSTICK | JOYSTICK_RIGHT) ||
        ((action & (JOYSTICK_UP_RIGHT | JOYSTICK_DOWN_RIGHT)) && offsetX > offsetY))
    {
        uint8_t selectedIndex = _getSelectedMenuItemIndex();
        if (_isMenuItemEnabled(selectedIndex))
        {
            if (getLogger())
            {
                if (selectedIndex < _menuItemCount && _menuItems[selectedIndex])
                {
                    getLogger()->info(F("MenuScreen: Selecting menu item %d: '%s'"), selectedIndex, _menuItems[selectedIndex]);
                }
                else
                {
                    getLogger()->info(F("MenuScreen: Selecting menu item %d"), selectedIndex);
                }
            }
            return _getSelectedMenuItemScreen(selectedIndex);
        }
        // If current item is disabled, don't activate it
        if (getLogger())
        {
            getLogger()->warn(F("MenuScreen: Attempted to select disabled menu item %d"), selectedIndex);
        }
        return nullptr;
    }

    // Menu selection - activate the selected item (only if enabled)
    if (action & (BUTTON_LEFT | JOYSTICK_LEFT) ||
        ((action & (JOYSTICK_UP_LEFT | JOYSTICK_DOWN_LEFT)) && offsetX > offsetY))
    {
        uint8_t selectedIndex = _getSelectedMenuItemIndex();
        if (_isMenuItemEnabled(selectedIndex))
        {
            if (getLogger())
            {
                if (selectedIndex < _menuItemCount && _menuItems[selectedIndex])
                {
                    getLogger()->info(F("MenuScreen: Selecting menu item %d: '%s'"), selectedIndex, _menuItems[selectedIndex]);
                }
                else
                {
                    getLogger()->info(F("MenuScreen: Selecting menu item %d"), selectedIndex);
                }
            }
            return _getSelectedMenuItemScreen(selectedIndex);
        }
        // If current item is disabled, don't activate it
        if (getLogger())
        {
            getLogger()->warn(F("MenuScreen: Attempted to select disabled menu item %d"), selectedIndex);
        }
        return nullptr;
    }

    // Exit menu - return to previous screen
    if (action & BUTTON_MENU)
    {
        if (getLogger())
        {
            getLogger()->info(F("MenuScreen: Exiting menu via menu button"));
        }
        return _getSelectedMenuItemScreen(-1);
    }

    // Navigate menu items - handle directional input and skip disabled items
    if (action != NONE)
    {
        uint8_t currentSelection = _getSelectedMenuItemIndex();

        // Move up - find previous enabled item
        if (action & (BUTTON_UP | JOYSTICK_UP) ||
            ((action & (JOYSTICK_UP_LEFT | JOYSTICK_UP_RIGHT)) && offsetY > offsetX))
        {
            uint8_t nextSelection;
            if (currentSelection > 0)
            {
                nextSelection = _findNextEnabledItem(currentSelection - 1, false);
            }
            else
            {
                nextSelection = _findNextEnabledItem(_menuItemCount - 1, false); // Wrap to end
            }
            _setSelectedMenuItemIndex(nextSelection);
        }
        // Move down - find next enabled item
        else if (action & (BUTTON_DOWN | JOYSTICK_DOWN) ||
                 ((action & (JOYSTICK_DOWN_LEFT | JOYSTICK_DOWN_RIGHT)) && offsetY > offsetX))
        {
            uint8_t nextSelection;
            if (currentSelection < _menuItemCount - 1)
            {
                nextSelection = _findNextEnabledItem(currentSelection + 1, true);
            }
            else
            {
                nextSelection = _findNextEnabledItem(0, true); // Wrap to beginning
            }
            _setSelectedMenuItemIndex(nextSelection);
        }
    }

    // No new screen - stay on current screen
    return nullptr;
}

// Menu Content Rendering

void MenuScreen::_drawContent()
{
    if (!isActive())
        return;

    uint16_t top = _getContentTop();
    uint16_t left = _getContentLeft();
    uint16_t width = _getContentWidth();
    uint16_t height = _getContentHeight();
    uint8_t itemsPerPage = _getItemsPerPage();

    Adafruit_GFX &gfx = M1Shield.getGFX();

    // Start bulk drawing transaction for better performance
    gfx.startWrite();

    // Calculate starting item index for current page
    uint8_t itemIndex = _currentPage * itemsPerPage;
    uint8_t itemsDrawn = 0;

    bool isSmall = isSmallDisplay();
    uint16_t rowHeight;
    uint16_t textSizeWidth;
    uint16_t textSizeHalfHeight;
    uint16_t leftPadding;
    uint16_t configGap;
    uint16_t rightPadding;

    if (isSmall)
    {
        gfx.setTextSize(1);
        rowHeight = ROW_SMALL_HEIGHT;
        textSizeWidth = TEXT_SIZE_1_WIDTH;
        textSizeHalfHeight = 1;
        leftPadding = 0;
        configGap = 2;
        rightPadding = 1;
    }
    else
    {
        gfx.setTextSize(2);
        rowHeight = ROW_HEIGHT;
        textSizeWidth = TEXT_SIZE_2_WIDTH;
        textSizeHalfHeight = TEXT_SIZE_2_HALF_HEIGHT;
        leftPadding = 15;
        configGap = 10;
        rightPadding = 5;
    }

    // Render up to itemsPerPage items for current page
    for (uint8_t i = 0; i < itemsPerPage; i++, itemIndex++)
    {
        int y = top + (i * rowHeight);
        bool isEnabled = _isMenuItemEnabled(itemIndex);

        // Render selected item with highlight colors (only if enabled)
        if (!isEnabled)
        {
            uint16_t bgColor = (i % 2 == 0) ? DISABLED_ROW_COLOR_BG1 : DISABLED_ROW_COLOR_BG2;
            uint16_t fgColor = (i % 2 == 0) ? DISABLED_ROW_COLOR_FG1 : DISABLED_ROW_COLOR_FG2;

            gfx.fillRect(left, y, width, rowHeight, M1Shield.convertColor(bgColor));
            gfx.setTextColor(M1Shield.convertColor(fgColor));
        }
        // Render disabled items with grayed out colors
        else if (itemIndex == _selectedMenuItemIndex && isEnabled)
        {
            gfx.fillRect(left, y, width, rowHeight, M1Shield.convertColor(SELECTED_ROW_COLOR_BG));
            gfx.setTextColor(M1Shield.convertColor(SELECTED_ROW_COLOR_FG));
        }
        // Render normal enabled items with alternating row colors
        else
        {
            uint16_t bgColor = (i % 2 == 0) ? ROW_COLOR_BG1 : ROW_COLOR_BG2;
            uint16_t fgColor = (i % 2 == 0) ? ROW_COLOR_FG1 : ROW_COLOR_FG2;

            gfx.fillRect(left, y, width, rowHeight, M1Shield.convertColor(bgColor));
            gfx.setTextColor(M1Shield.convertColor(fgColor));
        }

        if (itemIndex < _menuItemCount)
        {
            // Calculate available space for menu text and config value
            // Check FlashString version first, then fall back to regular string
            const __FlashStringHelper *configValueF = _getMenuItemConfigValueF(itemIndex);
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
                else if (getLogger())
                {
                    getLogger()->errF(F("MenuScreen: Failed to allocate memory for config value display"));
                }
            }
            else
            {
                // Fall back to regular string method
                configValue = _getMenuItemConfigValue(itemIndex);
            }

            uint16_t configWidth = 0;
            uint16_t configX = 0;

            if (configValue != nullptr)
            {
                uint16_t configLen = strlen(configValue);
                configWidth = configLen * textSizeWidth;
                configX = left + width - configWidth - rightPadding;
            }

            // Calculate available space for menu text
            uint16_t menuTextStartX = left + leftPadding;
            uint16_t selectorWidth = (isSmall ? 1 : 2) * textSizeWidth; // "> " or "  "
            uint16_t menuTextX = menuTextStartX + selectorWidth;
            uint16_t availableWidth = width - (menuTextX - left) - configWidth - configGap; // gap between text and config

            // Truncate menu text if it would collide with config value
            String menuText = String(_menuItems[itemIndex]);
            uint16_t menuTextWidth = menuText.length() * textSizeWidth;

            if (configValue != nullptr && menuTextWidth > availableWidth)
            {
                // Calculate how many characters fit with "..."
                if (isSmall)
                {
                    uint16_t ellipsisWidth = 2 * textSizeWidth;
                    uint16_t maxCharsWidth = availableWidth - ellipsisWidth;
                    uint8_t maxChars = maxCharsWidth / textSizeWidth;

                    if (maxChars > 0)
                    {
                        menuText = menuText.substring(0, maxChars) + "..";
                    }
                    else
                    {
                        menuText = ".."; // Fallback if space is extremely limited
                    }
                }
                else
                {
                    uint16_t ellipsisWidth = 3 * textSizeWidth;
                    uint16_t maxCharsWidth = availableWidth - ellipsisWidth;
                    uint8_t maxChars = maxCharsWidth / textSizeWidth;

                    if (maxChars > 0)
                    {
                        menuText = menuText.substring(0, maxChars) + "...";
                    }
                    else
                    {
                        menuText = "..."; // Fallback if space is extremely limited
                    }
                }
            }

            // Draw selection indicator and menu item text
            gfx.setCursor(menuTextStartX, y + textSizeHalfHeight);
            if (isSmall)
            {
                gfx.print(itemIndex == _selectedMenuItemIndex ? ">" : " ");
            }
            else
            {
                gfx.print(itemIndex == _selectedMenuItemIndex ? "> " : "  ");
            }
            gfx.print(menuText);

            // Draw configuration value if available (right-aligned)
            if (configValue != nullptr)
            {
                gfx.setCursor(configX, y + textSizeHalfHeight);
                gfx.print(configValue);
            }

            // Clean up temporary buffer if it was allocated for FlashString
            if (tempConfigBuffer != nullptr)
            {
                free(tempConfigBuffer);
            }
        }

        itemsDrawn++;
    }

    // Check if we should show a simple "..." indicator for more pages
    uint16_t usedHeight = itemsDrawn * rowHeight;
    uint16_t remainingHeight = height - usedHeight;
    uint8_t totalPages = (_menuItemCount + itemsPerPage - 1) / itemsPerPage;

    // Show simple three-dot indicator if there are more pages and we have minimal space
    if (remainingHeight >= 5)
    {
        // Position dots below the last menu item with small gap
        uint16_t dotY = top + usedHeight + 1;
        uint16_t centerX = left + (width / 2);

        if (totalPages > 1 && _currentPage < totalPages - 1)
        {
            // Draw three small filled rectangles as dots (3x3 pixels each, spaced 6 pixels apart)
            gfx.fillRect(centerX - 9, dotY, 3, 3, M1Shield.convertColor(NEXT_PAGE_INDICATOR_COLOR_FG)); // Left dot
            gfx.fillRect(centerX - 1, dotY, 3, 3, M1Shield.convertColor(NEXT_PAGE_INDICATOR_COLOR_FG)); // Center dot
            gfx.fillRect(centerX + 7, dotY, 3, 3, M1Shield.convertColor(NEXT_PAGE_INDICATOR_COLOR_FG)); // Right dot
        }
        else
        {
            gfx.fillRect(centerX - 9, dotY, 3, 3, M1Shield.convertColor(TABLE_COLOR_BG)); // Left dot
            gfx.fillRect(centerX - 1, dotY, 3, 3, M1Shield.convertColor(TABLE_COLOR_BG)); // Center dot
            gfx.fillRect(centerX + 7, dotY, 3, 3, M1Shield.convertColor(TABLE_COLOR_BG)); // Right dot
        }
    }

    // End bulk drawing transaction
    gfx.endWrite();
}

// Menu Configuration and State Management

void MenuScreen::setMenuItemsF(const __FlashStringHelper **menuItems, uint8_t menuItemCount)
{
    if (menuItems == nullptr || menuItemCount == 0)
    {
        clearMenuItems();
        return;
    }

    // Create temporary array of const char* pointers
    const char **tempItems = (const char **)malloc(menuItemCount * sizeof(const char *));
    if (tempItems == nullptr)
    {
        if (getLogger())
        {
            getLogger()->err(F("MenuScreen: Failed to allocate memory for flash menu items array"));
        }
        clearMenuItems();
        return; // Allocation failed
    }

    // Initialize all pointers to nullptr
    for (uint8_t i = 0; i < menuItemCount; i++)
    {
        tempItems[i] = nullptr;
    }

    // Convert FlashString items to temporary strings
    for (uint8_t i = 0; i < menuItemCount; i++)
    {
        const __FlashStringHelper *menuItem = menuItems[i];
        if (menuItem != nullptr)
        {
            size_t len = strlen_P((const char *)menuItem);
            if (len > 0)
            {
                char *buffer = (char *)malloc(len + 1);
                if (buffer != nullptr)
                {
                    strcpy_P(buffer, (const char *)menuItem);
                    buffer[len] = '\0'; // Ensure null termination
                    tempItems[i] = buffer;
                }
                else if (getLogger())
                {
                    getLogger()->err(F("MenuScreen: Failed to allocate memory for flash menu item %d"), i);
                }
            }
        }
    }

    // Call setMenuItems to handle the rest (it will make its own copies)
    setMenuItems(tempItems, menuItemCount);

    // Clean up temporary allocations
    for (uint8_t i = 0; i < menuItemCount; i++)
    {
        if (tempItems[i] != nullptr)
        {
            free((void *)tempItems[i]);
        }
    }
    free(tempItems);
}

void MenuScreen::setMenuItems(const char **menuItems, uint8_t menuItemCount)
{
    // Clear any existing menu items first
    clearMenuItems();

    if (menuItems == nullptr || menuItemCount == 0)
    {
        return; // Just clear and exit
    }

    // Allocate array of string pointers
    _menuItems = (char **)malloc(menuItemCount * sizeof(char *));
    if (_menuItems == nullptr)
    {
        if (getLogger())
        {
            getLogger()->err(F("MenuScreen: Failed to allocate memory for menu items array"));
        }
        return; // Allocation failed
    }

    // Initialize all pointers to nullptr first
    for (uint8_t i = 0; i < menuItemCount; i++)
    {
        _menuItems[i] = nullptr;
    }

    // Allocate and copy each menu item string
    uint8_t successCount = 0;
    for (uint8_t i = 0; i < menuItemCount; i++)
    {
        if (menuItems[i] != nullptr)
        {
            size_t len = strlen(menuItems[i]);
            char *itemCopy = (char *)malloc(len + 1);
            if (itemCopy != nullptr)
            {
                strcpy(itemCopy, menuItems[i]);
                _menuItems[i] = itemCopy;
                successCount++;
            }
            else if (getLogger())
            {
                getLogger()->err(F("MenuScreen: Failed to allocate memory for menu item %d"), i);
            }
        }
    }

    // Update state
    _menuItemCount = successCount;

    // Set selection to first enabled item
    if (successCount > 0)
    {
        _selectedMenuItemIndex = _findNextEnabledItem(0, true);
    }
    else
    {
        _selectedMenuItemIndex = 0;
    }

    // Update display if active
    if (isActive())
    {
        _drawContent();
        M1Shield.display();
    }
}

void MenuScreen::setMenuItems(String *menuItems, uint8_t menuItemCount)
{
    // Clear any existing menu items first
    clearMenuItems();

    if (menuItems == nullptr || menuItemCount == 0)
    {
        return; // Just clear and exit
    }

    // Allocate array of string pointers
    _menuItems = (char **)malloc(menuItemCount * sizeof(char *));
    if (_menuItems == nullptr)
    {
        return; // Allocation failed
    }

    // Initialize all pointers to nullptr first
    for (uint8_t i = 0; i < menuItemCount; i++)
    {
        _menuItems[i] = nullptr;
    }

    // Allocate and copy each menu item string
    uint8_t successCount = 0;
    for (uint8_t i = 0; i < menuItemCount; i++)
    {
        const char *cstr = menuItems[i].c_str();
        if (cstr != nullptr)
        {
            size_t len = strlen(cstr);
            char *itemCopy = (char *)malloc(len + 1);
            if (itemCopy != nullptr)
            {
                strcpy(itemCopy, cstr);
                _menuItems[i] = itemCopy;
                successCount++;
            }
            else if (getLogger())
            {
                getLogger()->err(F("MenuScreen: Failed to allocate memory for menu item %d"), i);
            }
        }
    }

    // Update state
    _menuItemCount = successCount;

    // Set selection to first enabled item
    if (successCount > 0)
    {
        _selectedMenuItemIndex = _findNextEnabledItem(0, true);
    }
    else
    {
        _selectedMenuItemIndex = 0;
    }

    // Update display if active
    if (isActive())
    {
        _drawContent();
        M1Shield.display();
    }
}

void MenuScreen::_setSelectedMenuItemIndex(uint8_t index)
{
    if (index >= _menuItemCount)
    {
        index = _menuItemCount - 1; // Clamp to last item if out of bounds
    }

    // If the target index is disabled, find the nearest enabled item
    if (!_isMenuItemEnabled(index))
    {
        index = _findNextEnabledItem(index, true);
    }

    _selectedMenuItemIndex = index;

    // Calculate current page based on dynamic items per page
    uint8_t itemsPerPage = _getItemsPerPage();
    _currentPage = index / itemsPerPage;

    if (isActive())
    {
        _drawContent();
        M1Shield.display();
    }
}

uint8_t MenuScreen::_getSelectedMenuItemIndex() const
{
    return _selectedMenuItemIndex;
}

void MenuScreen::clearMenuItems()
{
    if (_menuItems != nullptr)
    {
        // Free individual string allocations
        for (uint8_t i = 0; i < _menuItemCount; i++)
        {
            if (_menuItems[i] != nullptr)
            {
                free(_menuItems[i]);
            }
        }

        // Free the array of pointers
        free(_menuItems);
    }

    // Reset state
    _menuItems = nullptr;
    _menuItemCount = 0;
    _selectedMenuItemIndex = 0;
    _currentPage = 0;

    // Update display if active
    if (isActive())
    {
        _drawContent();
        M1Shield.display();
    }
}
