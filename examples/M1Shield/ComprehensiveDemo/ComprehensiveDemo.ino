/*
 * ===================================================================
 * M1Shield Comprehensive Demo - Complete Feature Showcase
 * ===================================================================
 *
 * This is the ULTIMATE M1Shield demonstration that combines ALL features
 * into one cohesive, interactive experience!
 *
 * DEMO FLOW:
 * 1. Welcome Console - Introduction and instructions
 * 2. Main Menu - Central navigation hub with TEST signal control
 * 3. M1TerminalScreen Demo - Full terminal emulation showcase
 * 4. ROM Features Menu - ROM detection and content display
 * 5. Video Features Menu - Full screen and viewport demonstrations
 * 6. RAM Testing Suite - Comprehensive memory tests with progress
 * 7. Keyboard Tester - Real-time key press detection
 * 8. Cassette Song Player - Interactive music selection and playback
 *
 * FEATURES DEMONSTRATED:
 * - MenuScreen with configuration items and sub-menus
 * - ConsoleScreen with progress bars and real-time output
 * - M1TerminalScreen with full TRS-80 terminal emulation
 * - Model1 TEST signal control and hardware interaction
 * - ROM detection and content reading
 * - Video mode switching and display features
 * - RAM testing with visual progress feedback
 * - Keyboard input monitoring and display
 * - Cassette interface with song selection and playback progress
 * - Seamless navigation between all features
 *
 * NAVIGATION:
 * - Use arrow buttons to navigate menus
 * - Press joystick to select menu items
 * - Use Menu button to go back/exit
 * - Follow on-screen instructions for each demo
 */

// First, tell the system which display you have
#include <Display_ST7789.h>
Display_ST7789 displayProvider;

// Include all necessary libraries
#include <M1Shield.h>
#include "WelcomeConsole.h"
#include "MainDemoMenu.h"

void setup()
{
    Serial.begin(115200);
    M1Shield.begin(displayProvider);

    // Enable joystick input for demo navigation
    // Uncomment to allow joystick control of menus and screens
    // M1Shield.activateJoystick();

    Serial.println("=== M1Shield Comprehensive Demo ===");
    Serial.println("Starting complete feature showcase...");

    // Start with the welcome console
    M1Shield.setScreen(new WelcomeConsole());
}

void loop()
{
    M1Shield.loop();
}
