/*
 * ===================================================================
 * MenuScreen Configuration Values Example
 * ===================================================================
 *
 * This example demonstrates both configuration values AND disabled menu items!
 * - Configuration values are displayed on the right side of each menu item
 * - Some menu items are conditionally disabled based on other settings
 * - Disabled items appear grayed out and provide helpful feedback
 *
 * WHAT'S NEW IN THIS EXAMPLE:
 * - Configuration values displayed on the right side of each menu item
 * - Disabled items feature with visual graying and logical dependencies
 * - Real-time updates when settings change enable/disable other options
 * - Feedback messages explaining why items are disabled
 * - Perfect for complex settings menus with interdependencies
 *
 * HOW TO TEST DISABLED ITEMS:
 * 1. Use "Reset to Defaults" to see several items become grayed out
 * 2. Turn Sound OFF → Game Difficulty becomes disabled (grayed)
 * 3. Set Difficulty to Easy → Auto Save becomes disabled
 * 4. Set Timeout below 10s → Brightness becomes disabled
 * 5. Turn Auto Save ON → Language becomes disabled
 * 6. Try selecting disabled (grayed) items to see feedback messages
 *
 * WHAT YOU'LL SEE:
 * ┌─────────────────────────────────┐
 * │ Configuration Menu              │
 * ├─────────────────────────────────┤
 * │ > Sound Effects            Off  │  ← When OFF...
 * │   Game Difficulty       Normal  │  ← This gets disabled (gray)
 * │   Screen Timeout            5s  │  ← When < 10s...
 * │   Brightness               80%  │  ← This gets disabled (gray)
 * │   Auto Save                 On  │  ← When ON...
 * │   Language             English  │  ← This gets disabled (gray)
 * │   Reset to Defaults             │  ← Always enabled
 * │   Back to Main Menu             │  ← Always enabled
 * ├─────────────────────────────────┤
 * │ [M/<] Back    [>] Change        │
 * └─────────────────────────────────┘
 *
 * WHAT YOU'LL LEARN:
 * - How to override _getMenuItemConfigValue() to show setting values
 * - How to create interactive configuration menus
 * - How to handle different types of settings (boolean, numeric, enum)
 * - How to update display when settings change
 * - Best practices for settings menu design
 *
 * CONTROLS:
 * - Up/Down: Navigate through settings
 * - Menu Button: Select setting to change its value
 * - Watch the configuration values update in real-time!
 * - LED changes color when you modify settings
 *
 * EXAMPLE BEHAVIOR:
 * - Select "Sound Effects" → toggles between "On" and "Off"
 * - Select "Game Difficulty" → cycles through "Easy", "Normal", "Hard"
 * - Select "Screen Timeout" → increases timeout (wraps around)
 * - Select "Brightness" → increases brightness (wraps around)
 * - Select "Auto Save" → toggles between "On" and "Off"
 * - Select "Language" → cycles through "English", "Spanish", "French"
 * - Select "Reset to Defaults" → resets all settings
 *
 * TECHNICAL DETAILS:
 * The key to this feature is implementing _getMenuItemConfigValue():
 *
 * const char* _getMenuItemConfigValue(int index) override {
 *     switch(index) {
 *         case 0: return _soundEnabled ? "On" : "Off";
 *         case 1: return _difficultyNames[_difficulty];
 *         // ... etc
 *     }
 * }
 *
 * The MenuScreen framework automatically:
 * 1. Calls this method for each menu item when drawing
 * 2. Right-aligns the returned string on each row
 * 3. Updates the display when refresh() is called
 * 4. Handles nullptr (no config value) gracefully
 *
 * Authors: Marcel Erz (RetroStack)
 * License: MIT
 */

// STEP 1: Configure your display type
#include <Display_ST7789.h>
Display_ST7789 displayProvider;

// #include <Display_ST7789_240x240.h>
// Display_ST7789_240x240 displayProvider;

// #include <Display_ST7735.h>
// Display_ST7735 displayProvider;

// #include <Display_ST7796.h>
// Display_ST7796 displayProvider;

// #include <Display_ILI9325.h>
// Display_ILI9325 displayProvider;

// #include <Display_ILI9341.h>
// Display_ILI9341 displayProvider;

// #include <Display_HX8357.h>
// Display_HX8357 displayProvider;

// ----------------------------------

// STEP 2: Include required libraries
#include "ConfigMenuExample.h" // Our configuration menu implementation
#include <M1Shield.h>          // Main M1Shield functionality

void setup()
{
    // Start serial communication for debug output
    Serial.begin(115200);
    Serial.println("=== MenuScreen Configuration Values Example ===");
    Serial.println("");
    Serial.println("This example demonstrates the new configuration values feature:");
    Serial.println("- Each menu item shows its current setting on the right");
    Serial.println("- Values update in real-time when changed");
    Serial.println("- Perfect for settings menus and status displays");
    Serial.println("");
    Serial.println("Try this:");
    Serial.println("1. Navigate with Up/Down buttons");
    Serial.println("2. Press Menu button to change setting values");
    Serial.println("3. Watch the right-side values update instantly!");
    Serial.println("4. Notice how LED colors change with your selections");
    Serial.println("");

    // Initialize the M1Shield hardware
    M1Shield.begin(displayProvider);

    // Create and display our configuration menu
    M1Shield.setScreen(new ConfigMenuExample());

    // Set initial LED color to indicate ready state
    M1Shield.setLEDColor(LEDColor::COLOR_GREEN);

    Serial.println("Configuration menu is now active!");
    Serial.println("Open Serial Monitor to see setting changes...");
    Serial.println("");
}

void loop()
{
    // The M1Shield framework handles everything automatically:
    // - Input processing (buttons and joystick)
    // - Screen updates and navigation
    // - Configuration value display
    // - Menu item selection and actions
    M1Shield.loop();
}