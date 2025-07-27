/**
 * ConfigMenuExample - Advanced MenuScreen Example with Configuration Values and Disabled Items
 *
 * This example demonstrates:
 * 1. Configuration values feature - each menu item displays its current setting
 * 2. Disabled menu items - some items are conditionally disabled based on other settings
 * 3. Visual feedback - disabled items appear grayed out and cannot be selected
 * 4. Logical dependencies - certain settings depend on others being enabled
 * 
 * Perfect for settings menus with complex interdependencies between options.
 */

#ifndef CONFIG_MENU_EXAMPLE_H
#define CONFIG_MENU_EXAMPLE_H

#include "MenuScreen.h"

class ConfigMenuExample : public MenuScreen
{
private:
    // Configuration settings that will be displayed
    bool _soundEnabled;
    uint8_t _difficulty; // 0=Easy, 1=Normal, 2=Hard
    uint16_t _timeout;   // Timeout in seconds
    uint8_t _brightness; // 0-100 brightness level
    bool _autoSave;
    uint8_t _language; // 0=English, 1=Spanish, 2=French

    // Buffer for numeric values (shared buffer to save memory)
    char _numericBuffer[12];

    // Menu items that will be displayed
    static constexpr const char *CONFIG_MENU_ITEMS[8] = {
        "Sound Effects",
        "Game Difficulty",
        "Screen Timeout",
        "Brightness",
        "Auto Save",
        "Language",
        "Reset to Defaults",
        "Back to Main Menu"};
    static constexpr uint8_t CONFIG_MENU_COUNT = 8;

protected:
    // Handle menu item selection
    Screen *_getSelectedMenuItemScreen(int index) override;

    // Return configuration values for each menu item
    const char *_getMenuItemConfigValue(int index) override;

    // Determine if menu items should be enabled/disabled
    bool _isMenuItemEnabled(int index) override;

public:
    ConfigMenuExample();
    virtual ~ConfigMenuExample() = default;

    // Helper methods for toggling settings
    void toggleSound();
    void cycleDifficulty();
    void adjustTimeout(int delta);
    void adjustBrightness(int delta);
    void toggleAutoSave();
    void cycleLanguage();
    void resetToDefaults();
};

#endif /* CONFIG_MENU_EXAMPLE_H */
