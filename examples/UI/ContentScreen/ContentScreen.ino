/*
 * ===================================================================
 * ContentScreen Example
 * ===================================================================
 *
 * This is the easiest way to learn how ContentScreen works!
 * Perfect if you're new to Arduino or the M1Shield.
 *
 * WHAT YOU'LL LEARN:
 * - What is a ContentScreen and why use it
 * - How to create a simple menu interface
 * - How header, footer, and progress bar work automatically
 * - How to handle user input (buttons/joystick)
 * - How to use user feedback systems (alerts, confirmations, notifications)
 *
 * WHAT IT DOES:
 * - Demonstrates alert dialogs (cyan background, blocking)
 * - Demonstrates confirmation dialogs (magenta background, blocking with choice)
 * - Demonstrates notifications (yellow background, auto-expiring)
 * - Shows a menu with several options ("Back", "Settings", etc.)
 * - Up/Down buttons navigate through the menu
 * - Selected item is highlighted with white background
 * - Progress bar shows which item is selected (0-100%)
 * - Header shows "Simple Menu" title
 * - Footer shows "Back" and "Select" buttons
 * - LED changes color to give visual feedback
 *
 * CONTROLS:
 * - Up/Down Buttons (or Joystick): Navigate menu items
 * - Menu Button (or Joystick Press): Select current item
 * - Watch the progress bar change as you navigate!
 * - Watch the LED change colors!
 *
 * WHAT YOU NEED:
 * - M1Shield with working display (ST7789, ST7735, ILI9341, ILI9488, HX8357, ILI9325)
 * - Working buttons and joystick
 * - LED indicator
 *
 * TROUBLESHOOTING FOR BEGINNERS:
 * - Blank screen? Check display type below (USE_ST7789 vs USE_SSD1306)
 *
 * TRY THIS STEP BY STEP:
 * 1. Upload this code to your Arduino
 * 2. You'll see an alert dialog (cyan) - press any button to dismiss
 * 3. You'll see a confirmation dialog (magenta) - press LEFT/RIGHT to choose
 * 4. You should see "Simple Menu" at the top (header)
 * 5. You should see menu items in the middle (content area)
 * 6. You should see "Back Select" at the bottom (footer)
 * 7. You'll see a yellow notification that auto-disappears after 4 seconds
 * 8. Press Up/Down - selected item should change and LED should turn green
 * 9. Press Menu - LED should flash white
 * 10. Watch the progress bar at the bottom change as you navigate
 *
 * BEGINNER FRIENDLY APPROACH:
 * - Uses global SimpleContentScreen object (no pointers or -> needed)
 * - Simple dot notation: contentScreen.alert(), contentScreen.notify()
 * - Easy to understand and modify for beginners
 *
 * Authors: Marcel Erz (RetroStack)
 * License: MIT
 */

// STEP 1: Tell the system what display you have
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

// STEP 2: Include the libraries we need
#include <M1Shield.h>            // Main M1Shield functions
#include <ContentScreen.h>       // For ConfirmResult enum and user feedback systems
#include "SimpleContentScreen.h" // Our simple content screen

// STEP 3: Create our simple menu items
// This is just an array of text strings - the menu options
const char *menuItems[] = {
    "Back",     // Menu option 0
    "Settings", // Menu option 1
    "Games",    // Menu option 2
    "Tools",    // Menu option 3
    "About"     // Menu option 4
};
const int MENU_COUNT = 5;

// STEP 4: Create our content screen as a global object
// This avoids using pointers and -> notation
SimpleContentScreen contentScreen("Simple Menu", menuItems, MENU_COUNT);

void setup()
{
    // Start serial communication for debug messages
    // (Open "Serial Monitor" in Arduino IDE to see these)
    Serial.begin(115200);
    Serial.println("=== ContentScreen Tutorial Starting ===");

    // Initialize the M1Shield hardware
    // This sets up display, buttons, joystick, LED, etc.
    M1Shield.begin(displayProvider);

    // Optional: Enable joystick input for menu navigation
    // Uncomment the next line to allow joystick control of menus
    // M1Shield.activateJoystick();

    Serial.println("Hardware initialized!");

    // Set our content screen as the active screen
    // Using the global object we created above
    Serial.println("Setting content screen...");
    M1Shield.setScreen(&contentScreen);

    Serial.println("Content screen loaded!");

    // DEMO: User feedback system examples
    Serial.println("Demonstrating user feedback systems...");

    // Example 1: Alert dialog (cyan background)
    // This blocks until user presses any button
    Serial.println("Showing alert dialog...");
    contentScreen.alert("Welcome! This is an alert dialog example.");
    Serial.println("Alert dismissed by user");

    // Example 2: Confirmation dialog (magenta background)
    // This blocks until user chooses LEFT or RIGHT button
    Serial.println("Showing confirmation dialog...");
    ConfirmResult choice = contentScreen.confirm("Continue with demo?", "No", "Yes");
    if (choice == CONFIRM_LEFT)
    {
        Serial.println("User chose 'No' (left button)");
        contentScreen.alert("Demo cancelled. Starting menu anyway...");
    }
    else
    {
        Serial.println("User chose 'Yes' (right button)");
        contentScreen.alert("Great! Demo will continue...");
    }

    Serial.println();
    Serial.println("Ready to use! Try these controls:");
    Serial.println("- Up/Down buttons: Navigate menu");
    Serial.println("- Menu button: Select item");
    Serial.println("- Watch the progress bar and LED!");
    Serial.println("- Alert/Confirm dialogs demo at startup");
    Serial.println("- Notification demo shows before menu");
    Serial.println();
    Serial.println("You should see:");
    Serial.println("- Header: 'Simple Menu'");
    Serial.println("- Menu items in the middle");
    Serial.println("- Footer: 'Back Select' buttons");
    Serial.println("- Progress bar showing selection percentage");

    // Example 3: Notification (yellow background, auto-expires)
    // This doesn't block - it shows temporarily and disappears
    Serial.println("Showing notification example...");
    contentScreen.notify("Use UP/DOWN to navigate menu!", 4000); // Show for 4 seconds
    Serial.println("Notification will auto-dismiss in 4 seconds");
}

void loop()
{
    // This is all you need! M1Shield.loop() handles everything:
    // - Reading button presses and joystick movement
    // - Calling your screen's loop() method
    // - Calling your screen's actionTaken() when buttons pressed
    // - Managing the display updates
    // - Everything else!
    M1Shield.loop();

    // The magic happens in SimpleContentScreen.cpp!
}
