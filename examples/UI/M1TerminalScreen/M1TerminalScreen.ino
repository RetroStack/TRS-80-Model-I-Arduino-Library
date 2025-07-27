/*
 * M1Terminal.ino - TRS-80 Model I Terminal Screen Example
 *
 * This example demonstrates how to use the M1TerminalScreen to display and interact
 * with TRS-80 Model I video memory. The terminal provides an authentic recreation of
 * the original TRS-80 Model I 64x16 character display with modern enhancements.
 *
 * ## What This Example Does:
 *
 * 1. **Initializes Hardware**: Sets up the Model1 and M1Shield systems
 * 2. **Creates Terminal**: Displays a scrollable terminal screen
 * 3. **Shows Content**:
 *    - "No Content" message when Model1 is not active
 *    - Live TRS-80 video memory when Model1 test signal is active
 * 4. **Provides Controls**: Full 2D navigation and font switching
 *
 * ## User Controls:
 *
 * - **→ Arrow**: Scroll right (5-character steps)
 * - **← Arrow**: Scroll left (5-character steps)
 * - **↑ Arrow**: Scroll up (3-row steps)
 * - **↓ Arrow**: Scroll down (3-row steps)
 * - **Menu + Up**: Switch between font sets (Font 0 ↔ Font 1)
 * - **Menu**: Exit terminal (return to main menu - not implemented in this example)
 *
 * ## Technical Features:
 *
 * - **64x16 Display**: Full TRS-80 Model I resolution (1024 characters)
 * - **Dynamic Viewport**: Adapts to available screen space automatically
 * - **Incremental Rendering**: Smooth performance with change detection
 * - **Model1 Integration**: Real-time sync with TRS-80 hardware when available
 * - **Memory Efficient**: Only renders visible characters
 *
 * ## Expected Behavior:
 *
 * **Without TEST Signal Active:**
 * - Shows blue screen with centered "No Content" message
 *
 * **With TEST Signal Active:**
 * - Automatically loads live TRS-80 video memory (0x3C00-0x3FFF)
 * - Displays actual content that should be shown on the TRS-80 Model 1 video screen
 * - Scroll to view different parts of the 64-column display
 * - Real-time updates as TRS-80 content changes
 *
 * ## Hardware Requirements:
 *
 * - M1Shield board with display
 * - TRS-80 Model I system for live content
 *
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

// First, tell the system which display you have
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

#include <Model1.h>
#include <M1Shield.h>
#include <M1TerminalScreen.h>

void setup()
{
    // Initialize serial communication for debug output
    Serial.begin(115200);
    Serial.println("M1Terminal Example Starting...");

    // Initialize the Model1 system
    // This sets up communication with TRS-80 Model I hardware
    // and prepares for video memory reading when test signal is active
    Model1.begin();
    Serial.println("Model1 system initialized");

    // Initialize the M1Shield hardware
    // This sets up the display, input controls, and user interface
    M1Shield.begin(displayProvider);
    Serial.println("M1Shield initialized successfully");

    // Create and set the terminal screen
    // M1Shield takes ownership of this screen object and will manage its lifecycle
    // The terminal will automatically show "No Content" until Model1 provides data
    M1Shield.setScreen(new M1TerminalScreen());
    Serial.println("M1TerminalScreen created and activated");

    // Wait for 2 seconds to allow the user to see the "No Content" message
    // Not needed for real-world usage, just for demonstration
    delay(2000);

    // Activate test signal for Model1 to allow the Arduino access to the video memory
    Model1.activateTestSignal();

    Serial.println("Terminal ready!");
    Serial.println("- Use arrow keys for 2D scrolling");
    Serial.println("- Use Left + Right (same time) for font switching");
}

void loop()
{
    // Main processing loop
    // M1Shield handles all screen rendering, input processing, and Model1 integration
    // The terminal will automatically:
    // - Process user input (scrolling, font changes)
    // - Update display with incremental rendering
    // - Load Model1 video memory when test signal becomes active
    M1Shield.loop();
}
