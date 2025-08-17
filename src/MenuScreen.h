/*
 * MenuScreen.h - Abstract base class for paginated menu screens
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef MENU_SCREEN_H
#define MENU_SCREEN_H

#include <Arduino.h>
#include "ContentScreen.h"

/**
 * @brief Abstract base class for paginated menu screens with navigation support
 *
 * MenuScreen provides a complete framework for creating navigable menu interfaces
 * with automatic pagination, visual selection feedback, and input handling.
 * Derived classes need only provide menu items and define the action for each item.
 *
 * The class handles:
 * - Automatic pagination when menu items exceed screen capacity
 * - Visual highlighting of the currently selected item
 * - Enabled/disabled item states
 * - Up/down navigation with wraparound
 * - Page navigation for large menu sets
 * - Consistent button labeling and action handling
 * - Dynamic memory management for menu items (always copies strings)
 *
 * @note This is an abstract class - derived classes must implement _getSelectedMenuItemScreen()
 *
 * @example Static Menu Items
 * @code
 * class SettingsMenu : public MenuScreen {
 * public:
 *     SettingsMenu() {
 *         // Option 1: Simple static items (C-strings)
 *         const char* items[] = {"Display", "Sound", "Input", "About"};
 *         setMenuItems(items, 4);
 *         setTitle("Settings");
 *     }
 *
 * protected:
 *     Screen* _getSelectedMenuItemScreen(int index) override {
 *         switch(index) {
 *             case 0: return new DisplaySettings();
 *             case 1: return new SoundSettings();
 *             // ...
 *         }
 *         return nullptr;
 *     }
 * };
 * @endcode
 *
 * @example Dynamic Menu Items
 * @code
 * class PlayerMenu : public MenuScreen {
 * private:
 *     String playerName;
 *     int playerScore;
 *     int playerLevel;
 *
 * public:
 *     PlayerMenu(String name, int score, int level)
 *         : playerName(name), playerScore(score), playerLevel(level) {
 *
 *         // Option 2: Dynamic items (String objects)
 *         String items[] = {
 *             String("Continue as ") + playerName,
 *             String("Score: ") + String(playerScore),
 *             String("Level: ") + String(playerLevel),
 *             "New Game",
 *             "Exit"
 *         };
 *         setMenuItems(items, 5);
 *         setTitle(String("Welcome ") + playerName);
 *     }
 *
 * protected:
 *     Screen* _getSelectedMenuItemScreen(int index) override {
 *         // Handle menu selection...
 *         return nullptr;
 *     }
 * };
 * @endcode
 *
 * @example Memory-Efficient Menu Items
 * @code
 * class StaticMenu : public MenuScreen {
 * public:
 *     StaticMenu() {
 *         // Option 3: Memory-efficient static items (F-strings)
 *         const __FlashStringHelper* items[] = {
 *             F("New Game"),
 *             F("Load Game"),
 *             F("Settings"),
 *             F("Exit")
 *         };
 *         setMenuItemsF(items, 4);
 *         setTitleF(F("Main Menu"));
 *     }
 *
 * protected:
 *     Screen* _getSelectedMenuItemScreen(int index) override {
 *         // Handle menu selection...
 *         return nullptr;
 *     }
 * };
 * @endcode
 */
class MenuScreen : public ContentScreen
{
private:
    uint8_t _currentPage; // Current page being displayed (0-based)

    char **_menuItems;      // Dynamically allocated array of menu item strings
    uint8_t _menuItemCount; // Total number of menu items across all pages

    uint8_t _selectedMenuItemIndex; // Currently selected menu item index (global, not page-relative)

    /**
     * @brief Calculate maximum items that can fit on one page based on available content height
     * @return Number of menu items that can be displayed per page
     */
    uint8_t _getItemsPerPage() const;

    /**
     * @brief Find the next enabled menu item starting from a given index
     * @param startIndex Index to start searching from (inclusive)
     * @param forward True to search forward, false to search backward
     * @return Index of next enabled item, or startIndex if none found
     */
    uint8_t _findNextEnabledItem(uint8_t startIndex, bool forward) const;

protected:
    /**
     * @brief Draw the menu content area with paginated item list
     *
     * Renders the current page of menu items with alternating row colors
     * and highlighting for the selected item. Handles pagination display
     * and visual feedback automatically.
     *
     * @note Called automatically by ContentScreen's render cycle
     * @note Overrides ContentScreen::_drawContent() to provide menu-specific rendering
     */
    void _drawContent();

    /**
     * @brief Set the currently selected menu item by index
     *
     * Updates the selected menu item and automatically handles page switching
     * if the selected item is on a different page than currently displayed.
     *
     * @param index Global menu item index (0-based, across all pages)
     *
     * @note Index is clamped to valid range [0, menuItemCount-1]
     * @note Automatically switches to the page containing the selected item
     */
    void _setSelectedMenuItemIndex(uint8_t index);

    /**
     * @brief Get the index of the currently selected menu item
     *
     * @return uint8_t Global index of selected menu item (0-based)
     */
    uint8_t _getSelectedMenuItemIndex() const;

    /**
     * @brief Abstract method to get the screen for a selected menu item
     *
     * Derived classes must implement this method to define what happens
     * when each menu item is selected. Return nullptr to stay on current screen.
     *
     * @param index Global index of the selected menu item
     * @return Screen* Pointer to screen to navigate to, or nullptr to stay current
     *
     * @note The returned screen will be managed by the M1Shield - do not delete manually
     * @note This is a pure virtual method that must be implemented by derived classes
     *
     * @example
     * @code
     * Screen* MyMenu::_getSelectedMenuItemScreen(int index) {
     *     switch(index) {
     *         case 0: return new GameScreen();
     *         case 1: return new SettingsScreen();
     *         default: return nullptr;  // Stay on current screen
     *     }
     * }
     * @endcode
     */
    virtual Screen *_getSelectedMenuItemScreen(int index) = 0;

    /**
     * @brief Get configuration value string for a menu item (optional override)
     *
     * Derived classes can override this method to provide additional configuration
     * values that will be displayed right-aligned on each menu row. This is useful
     * for showing current settings, status indicators, or numeric values.
     *
     * @param index Global index of the menu item (0-based)
     * @return const char* Configuration string to display, or nullptr for no value
     *
     * @note Default implementation returns nullptr (no configuration values)
     * @note Returned strings should be short to fit within the row width
     * @note Memory management is handled by the derived class
     *
     * @example
     * @code
     * const char* SettingsMenu::_getMenuItemConfigValue(uint8_t index) {
     *     switch(index) {
     *         case 0: return _soundEnabled ? "On" : "Off";    // Sound setting
     *         case 1: return _difficultyNames[_difficulty];   // Difficulty level
     *         case 2: return _timeoutBuffer;                  // Numeric value as string
     *         default: return nullptr;                        // No value for other items
     *     }
     * }
     * @endcode
     */
    virtual const char *_getMenuItemConfigValue(uint8_t index) { return nullptr; }

    /**
     * @brief Get configuration value FlashString for a menu item (optional override)
     *
     * Derived classes can override this method to provide memory-efficient configuration
     * values using FlashString (F() macro) that will be displayed right-aligned on each
     * menu row. This is useful for static configuration text that doesn't change.
     *
     * @param index Global index of the menu item (0-based)
     * @return const __FlashStringHelper* FlashString to display, or nullptr for no value
     *
     * @note Default implementation returns nullptr (no configuration values)
     * @note Takes precedence over _getMenuItemConfigValue() if both are overridden
     * @note More memory efficient for static configuration text
     * @note Returned FlashStrings should be short to fit within the row width
     *
     * @example
     * @code
     * const __FlashStringHelper* SettingsMenu::_getMenuItemConfigValueF(uint8_t index) {
     *     switch(index) {
     *         case 0: return _soundEnabled ? F("On") : F("Off");    // Flash-stored text
     *         case 1: return F("Easy");                             // Static difficulty
     *         case 2: return F("Auto");                             // Static mode
     *         default: return nullptr;                              // No value for other items
     *     }
     * }
     * @endcode
     */
    virtual const __FlashStringHelper *_getMenuItemConfigValueF(uint8_t index) { return nullptr; }

    /**
     * @brief Check if a menu item is enabled/selectable (optional override)
     *
     * Derived classes can override this method to control which menu items are
     * enabled or disabled. Disabled items are visually grayed out and cannot
     * be selected during navigation - the selection will skip over them.
     *
     * @param index Global index of the menu item (0-based)
     * @return bool True if the menu item is enabled and selectable, false if disabled
     *
     * @note Default implementation returns true (all items enabled)
     * @note Disabled items are rendered with reduced contrast/grayed out appearance
     * @note Navigation automatically skips over disabled items
     *
     * @example
     * @code
     * bool GameMenu::_isMenuItemEnabled(uint8_t index) const {
     *     switch(index) {
     *         case 0: return true;                     // New Game - always enabled
     *         case 1: return _hasSaveFile;             // Load Game - only if save exists
     *         case 2: return _gameInProgress;          // Resume Game - only if game active
     *         case 3: return true;                     // Settings - always enabled
     *         default: return true;
     *     }
     * }
     * @endcode
     */
    virtual bool _isMenuItemEnabled(uint8_t index) const { return true; }

public:
    /**
     * @brief Constructor
     *
     * Initializes menu state with default values and sets up standard
     * button labels for menu navigation. Derived classes should call
     * _setMenuItems() and _setTitle() to configure the specific menu.
     */
    MenuScreen();

    /**
     * @brief Destructor - frees dynamically allocated menu items
     *
     * Ensures proper cleanup of any dynamically allocated menu items
     * to prevent memory leaks when menu screens are destroyed.
     */
    virtual ~MenuScreen();

    // Screen Interface Implementation

    /**
     * @brief Main loop processing for menu screen updates
     *
     * @note Provides MenuScreen-specific loop implementation since ContentScreen::loop() is pure virtual
     * @note Override this method in derived classes for custom periodic processing
     */
    virtual void loop() {}

    /**
     * @brief Handle user input actions and navigate accordingly
     *
     * Processes user input for menu navigation including:
     * - Up/Down: Navigate between menu items with wraparound
     * - Right/Select: Activate selected menu item
     * - Left/Back: Return to previous screen
     * - Page navigation for multi-page menus
     *
     * @param action  The input action that occurred (button press, joystick movement)
     * @param offsetX Horizontal offset for analog joystick input
     * @param offsetY Vertical offset for analog joystick input
     * @return Screen* Pointer to screen to navigate to, or nullptr to stay on current
     *
     * @note Diagonal joystick movements are handled intelligently based on dominant axis
     *
     * @example
     * Navigation behaviors:
     * - BUTTON_UP/JOYSTICK_UP: Move selection up (with wraparound to bottom)
     * - BUTTON_DOWN/JOYSTICK_DOWN: Move selection down (with wraparound to top)
     * - BUTTON_RIGHT/JOYSTICK_RIGHT: Activate selected item via _getSelectedMenuItemScreen()
     * - BUTTON_LEFT/JOYSTICK_LEFT/BUTTON_MENU: Return to previous screen (handled by base class)
     */
    Screen *actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY) override;

    /**
     * @brief Set the menu items to be displayed and navigated
     *
     * @param menuItems     Array of null-terminated strings for menu items
     * @param menuItemCount Number of items in the menuItems array
     *
     * @note Automatically frees any previously allocated menu items
     * @note Creates deep copies of all strings - original array can be freed
     * @note Automatically recalculates pagination when items change
     * @note Resets selection to first item when menu changes
     *
     * @example
     * @code
     * const char* items[] = {"New Game", "Load Game", "Settings", "Exit"};
     * _setMenuItems(items, 4);
     * @endcode
     */
    void setMenuItems(const char **menuItems, uint8_t menuItemCount);

    /**
     * @brief Set the menu items from an array of String objects
     *
     * Convenient version for setting menu items from Arduino String objects.
     * Useful when menu items are built dynamically at runtime.
     *
     * @param menuItems     Array of String objects for menu items
     * @param menuItemCount Number of items in the menuItems array
     *
     * @note Automatically frees any previously allocated menu items
     * @note Creates deep copies of all strings - original array can be freed
     * @note Automatically recalculates pagination when items change
     * @note Resets selection to first item when menu changes
     *
     * @example
     * @code
     * String items[] = {"New Game", "Load Game", "Settings", "Exit"};
     * setMenuItems(items, 4);
     *
     * // Or with dynamic content:
     * String dynamicItems[] = {
     *     "Player: " + playerName,
     *     "Score: " + String(score),
     *     "Level: " + String(level)
     * };
     * setMenuItems(dynamicItems, 3);
     * @endcode
     */
    void setMenuItems(String *menuItems, uint8_t menuItemCount);

    /**
     * @brief Set the menu items to be displayed and navigated (Flash version)
     *
     * @param menuItems     Array of flash strings for menu items
     * @param menuItemCount Number of items in the menuItems array
     *
     * @note Automatically frees any previously allocated menu items
     * @note Creates deep copies of all strings - original array can be freed
     * @note Automatically recalculates pagination when items change
     * @note Resets selection to first item when menu changes
     *
     * @example
     * @code
     * const __FlashStringHelper* items[] = {F("New Game"), F("Load Game"), F("Settings"), F("Exit")};
     * _setMenuItemsF(items, 4);
     * @endcode
     */
    void setMenuItemsF(const __FlashStringHelper **menuItems, uint8_t menuItemCount);

    /**
     * @brief Clear and free all dynamically allocated menu items
     *
     * Frees all memory associated with menu items and resets the menu
     * to an empty state. Safe to call multiple times or on empty menus.
     *
     * @note Safe to call multiple times or on empty menus
     * @note Automatically triggers display update if menu is active
     */
    void clearMenuItems();
};

#endif /* MENU_SCREEN_H */