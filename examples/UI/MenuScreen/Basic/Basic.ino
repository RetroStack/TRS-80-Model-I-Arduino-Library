/*
 * ===================================================================
 * MenuScreen Example
 * ===================================================================
 *
 * This example shows you how to create a professional-looking menu system
 * using the MenuScreen base class. Perfect for learning advanced M1Shield features!
 *
 * WHAT YOU'LL LEARN:
 * - How to use the MenuScreen base class (more advanced than ContentScreen)
 * - How to create a multi-option menu with proper navigation
 * - How menu item selection works and screen transitions
 * - How to handle user input in a menu system
 * - The difference between MenuScreen and ContentScreen
 *
 * WHAT IS MENUSCREEN?
 * MenuScreen is a specialized type of ContentScreen that automatically:
 * - Creates a scrollable menu with highlighted selection
 * - Handles up/down navigation automatically
 * - Manages menu item selection and screen transitions
 * - Provides consistent menu appearance and behavior
 * - Includes built-in header, footer, and progress bar like ContentScreen
 *
 * MENUSCREEN vs CONTENTSCREEN vs SCREEN:
 * - Screen: Basic screen class - you do everything yourself
 * - ContentScreen: Adds header/footer/progress bar automatically
 * - MenuScreen: Adds menu navigation on top of ContentScreen features
 *
 * WHAT THIS EXAMPLE DOES:
 * - Shows a "TRS-80 Main Menu" with 11 menu options
 * - Up/Down buttons navigate through menu items
 * - Selected item is highlighted automatically
 * - Menu button selects the current item
 * - LED changes color based on what you select
 * - Serial output shows what happens when you select items
 *
 * THE MENU ITEMS:
 * 1. Run BASIC        - Would start a BASIC interpreter
 * 2. Load Program     - Would open a file browser
 * 3. Save Program     - Would open a save dialog
 * 4. Font: A/B        - Would toggle between font styles
 * 5. View: Left       - Would change display view
 * 6. Settings         - Would open settings screen
 * 7. Games            - Would show games menu
 * 8. Utilities        - Would show utilities menu
 * 9. About            - Would show about information
 * 10. Help            - Would show help screen
 * 11. Debug Mode      - Would enable debug features
 * 12. Exit            - Would exit the menu
 *
 * CONTROLS:
 * - Up/Down Buttons (or Joystick): Navigate through menu items
 * - Menu Button (or Joystick Press): Select the highlighted item
 * - Watch the LED change colors when you select different items!
 * - Open Serial Monitor to see selection messages
 *
 * WHAT YOU NEED:
 * - M1Shield with working display (ST7789, ST7735, ILI9341, ILI9488, HX8357, ILI9325)
 * - Arduino IDE with Serial Monitor (to see selection messages)
 *
 * TROUBLESHOOTING:
 * - Blank screen? Make sure display type is correct below
 *
 * TRY THIS STEP BY STEP:
 * 1. Upload this code and open Serial Monitor (Tools → Serial Monitor)
 * 2. You should see "TRS-80 Main Menu" at the top
 * 3. You should see menu items in the middle with one highlighted
 * 4. Press Up/Down - highlight should move between items
 * 5. Press Menu button - LED should change color and Serial Monitor should show selection
 * 6. Try selecting different items and watch LED colors change!
 *
 * LEARNING PATH:
 * 1. Start with Screen example (basic screen functionality)
 * 2. Try ContentScreen example (header/footer/progress bar)
 * 3. Study this MenuScreen example (automatic menu handling)
 * 4. Create your own menu screens for your projects!
 *
 * Authors: Marcel Erz (RetroStack)
 * License: MIT
 */

// STEP 1: Configure your display type
// For ST7789 240x320 displays (most common, landscape becomes 320x240)
#include <Display_ST7789_320x240.h>
Display_ST7789_320x240 displayProvider;

// For ST7789 240x240 square displays
// #include <Display_ST7789_240x240.h>
// Display_ST7789_240x240 displayProvider;

// For ST7789 320x170 wide displays (landscape)
// #include <Display_ST7789_320x170.h>
// Display_ST7789_320x170 displayProvider;

// For ST7789 320x240 alternative displays (landscape)
// #include <Display_ST7789_320x240.h>
// Display_ST7789_320x240 displayProvider;

// For smaller ST7735 128x160 displays
// #include <Display_ST7735.h>
// Display_ST7735 displayProvider;

// For large ST7796 320x480 displays (landscape becomes 480x320)
// #include <Display_ST7796.h>
// Display_ST7796 displayProvider;

// For parallel ILI9325 240x320 displays (landscape becomes 320x240)
// #include <Display_ILI9325.h>
// Display_ILI9325 displayProvider;

// For ILI9341 240x320 displays (landscape becomes 320x240)
// #include <Display_ILI9341.h>
// Display_ILI9341 displayProvider;

// For large HX8357 320x480 displays
// #include <Display_HX8357.h>
// Display_HX8357 displayProvider;

// For monochrome SSD1306 OLED displays (128x64)
// #include <Display_SSD1306.h>
// Display_SSD1306 displayProvider;

// For monochrome SH1106 OLED displays (128x64)
// #include <Display_SH1106.h>
// Display_SH1106 displayProvider;

// ----------------------------------

// STEP 2: Include the required libraries
#include "SimpleMenuScreen.h" // Our menu screen implementation
#include <M1Shield.h>         // Main M1Shield functionality

void setup()
{
    // Start serial communication for debug output
    // Open "Serial Monitor" in Arduino IDE to see menu selection messages
    Serial.begin(115200);
    Serial.println("=== MenuScreen Tutorial Starting ===");
    Serial.println("");
    Serial.println("This example demonstrates the MenuScreen framework:");
    Serial.println("- Automatic menu navigation");
    Serial.println("- Item selection handling");
    Serial.println("- Screen transition framework");
    Serial.println("- LED visual feedback");
    Serial.println("");

    // Initialize the M1Shield hardware
    // This sets up display, buttons, joystick, LED, and all other components
    M1Shield.begin(displayProvider);

    // Optional: Enable joystick input for menu navigation
    // Uncomment the next line to allow joystick control of menus
    // M1Shield.activateJoystick();

    Serial.println("M1Shield hardware initialized");

    // Create and attach screen instance
    // This uses the SimpleMenuScreen class we defined in SimpleMenuScreen.h
    M1Shield.setScreen(new SimpleMenuScreen());
    Serial.println("Menu screen loaded and active");

    // Set LED to green to indicate successful initialization
    M1Shield.setLEDColor(LEDColor::COLOR_GREEN);

    Serial.println("");
    Serial.println("=== Ready! Try these controls: ===");
    Serial.println("Up/Down Buttons: Navigate menu items");
    Serial.println("Menu Button: Select highlighted item");
    Serial.println("Watch the LED change colors when you select items!");
    Serial.println("");
    Serial.println("You should see:");
    Serial.println("- Header: 'TRS-80 Main Menu'");
    Serial.println("- 12 menu items with one highlighted");
    Serial.println("- Footer with navigation hints");
    Serial.println("- Progress bar at the bottom");
    Serial.println("");
    Serial.println("Selection messages will appear below:");
    Serial.println("=====================================");
}

void loop()
{
    M1Shield.loop();
    // This automatically:
    // - Calls the active screen's loop() method
    // - Handles button input processing
    // - Manages screen transitions
    // - Updates the display
    // - Everything else!
}
