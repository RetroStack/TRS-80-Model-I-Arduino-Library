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

/**
 * @brief Initialize menu screen with default configuration
 *
 * Sets up the menu screen with initial state including page counters,
 * empty menu item list, and default button labels for navigation.
 * Derived classes should call _setMenuItems() and _setTitle() to configure.
 */
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

/**
 * @brief Destructor frees dynamically allocated menu items
 *
 * Ensures proper cleanup of any dynamically allocated menu items
 * to prevent memory leaks when menu screens are destroyed.
 */
MenuScreen::~MenuScreen()
{
    clearMenuItems();
}

/**
 * @brief Calculate maximum items that can fit on one page based on available content height
 *
 * Dynamically calculates how many menu items can be displayed on one page
 * by dividing the available content area height by the row height.
 *
 * @return Number of menu items that can be displayed per page (minimum 1)
 *
 * @note Ensures at least 1 item per page even if content area is very small
 * @note Recalculated whenever screen layout changes
 */
uint8_t MenuScreen::_getItemsPerPage() const
{
    uint16_t contentHeight = _getContentHeight();
    uint8_t itemsPerPage = contentHeight / (isSmallDisplay() ? ROW_SMALL_HEIGHT : ROW_HEIGHT);

    // Ensure at least 1 item per page, even with very small screens
    return (itemsPerPage > 0) ? itemsPerPage : 1;
}

/**
 * @brief Find the next enabled menu item starting from a given index
 *
 * @param startIndex Index to start searching from (inclusive)
 * @param forward True to search forward, false to search backward
 * @return Index of next enabled item, or startIndex if none found
 */
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

/**
 * @brief Process user input actions for menu navigation and selection
 *
 * Handles all menu navigation including item selection, directional movement,
 * and exit actions. Supports both button and joystick input with intelligent
 * diagonal movement handling.
 *
 * @param action  Input action flags indicating what input occurred
 * @param offsetX Horizontal joystick offset for diagonal detection
 * @param offsetY Vertical joystick offset for diagonal detection
 * @return Screen* Screen to navigate to, or nullptr to stay on current screen
 *
 * @note Diagonal joystick movements prioritize the dominant axis
 */
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
            return _getSelectedMenuItemScreen(selectedIndex);
        }
        // If current item is disabled, don't activate it
        return nullptr;
    }

    // Menu selection - activate the selected item (only if enabled)
    if (action & (BUTTON_LEFT | JOYSTICK_LEFT) ||
        ((action & (JOYSTICK_UP_LEFT | JOYSTICK_DOWN_LEFT)) && offsetX > offsetY))
    {
        uint8_t selectedIndex = _getSelectedMenuItemIndex();
        if (_isMenuItemEnabled(selectedIndex))
        {
            return _getSelectedMenuItemScreen(selectedIndex);
        }
        // If current item is disabled, don't activate it
        return nullptr;
    }

    // Exit menu - return to previous screen
    if (action & BUTTON_MENU)
    {
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

/**
 * @brief Render the menu content area with paginated items and selection highlighting
 *
 * Draws the current page of menu items with alternating row colors and
 * highlights the currently selected item. Handles pagination automatically
 * and ensures proper visual feedback for navigation.
 *
 * @note Called automatically by ContentScreen's rendering system
 * @note Uses dynamic calculation to determine how many items to show per page
 * @note Selected item uses distinct colors for clear visual feedback
 * @note Shows pixel-based "..." indicator when additional pages exist
 */
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

/**
 * @brief Set menu items with dynamic memory allocation and string copying
 *
 * @param menuItems     Array of null-terminated strings for menu items
 * @param menuItemCount Number of items in the menuItems array
 *
 * @note Automatically frees any previously allocated menu items
 * @note Creates deep copies of all strings - original array can be freed
 * @note Automatically recalculates pagination when items change
 * @note Resets selection to first item when menu changes
 */
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

/**
 * @brief Set the menu items from an array of String objects
 * @param menuItems Array of String objects for menu items
 * @param menuItemCount Number of items in the menuItems array
 */
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

/**
 * @brief Set the currently selected menu item and update display
 *
 * Updates the selected menu item index and automatically switches
 * to the appropriate page if the selected item is not on the current page.
 * Triggers a redraw if the menu is currently active.
 *
 * @param index Global menu item index to select (0-based)
 *
 * @note Automatically calculates and switches to the correct page
 * @note Triggers immediate redraw if menu is active
 */
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

/**
 * @brief Get the currently selected menu item index
 *
 * Returns the global index of the currently selected menu item
 * across all pages of the menu.
 *
 * @return uint8_t Current selected menu item index (0-based)
 */
uint8_t MenuScreen::_getSelectedMenuItemIndex() const
{
    return _selectedMenuItemIndex;
}

/**
 * @brief Clear and free all dynamically allocated menu items
 *
 * Frees all memory associated with menu items and resets the menu
 * to an empty state. Safe to call multiple times or on empty menus.
 *
 * @note Safe to call multiple times or on empty menus
 * @note Automatically triggers display update if menu is active
 */
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
