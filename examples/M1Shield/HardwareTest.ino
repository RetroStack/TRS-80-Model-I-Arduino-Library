/*
 * ===================================================================
 * M1Shield Hardware Test - Complete Beginner's Guide
 * ===================================================================
 *
 * This is the PERFECT starting example for learning M1Shield!
 * Tests all hardware components with clear feedback.
 *
 * WHAT YOU'LL LEARN:
 * - How to initialize M1Shield hardware
 * - How to read buttons and joystick input
 * - How to control the LED with different colors
 * - How to draw basic graphics on the display
 * - How to use Serial Monitor for debugging
 * - How to handle different display types
 *
 * WHAT THIS EXAMPLE DOES:
 * - Tests display initialization and draws test pattern
 * - Monitors all button presses (Menu, Up, Down, Left, Right)
 * - Tracks joystick movement and button press
 * - Changes LED color based on current input
 * - Prints detailed feedback to Serial Monitor
 * - Works with multiple display types (ST7789, ST7735, ILI9341)
 *
 * CONTROLS & FEEDBACK:
 * - Menu Button: LED turns CYAN, Serial shows "Menu button: PRESSED"
 * - Arrow Buttons: LED turns BLUE, Serial shows which button
 * - Joystick Movement: LED turns MAGENTA, Serial shows direction & values
 * - Joystick Press: LED turns WHITE, Serial shows "Joystick button: PRESSED"
 * - No Input: LED turns OFF
 *
 * WHAT YOU NEED:
 * - M1Shield with working display
 *
 * DISPLAY SUPPORT:
 * This example works with multiple display types:
 * - ST7789 (320x240) - Default, most common
 * - ST7735 (128x160) - Smaller TFT display
 * - ILI9341 (240x320) - Alternative TFT display
 *
 * TROUBLESHOOTING FOR BEGINNERS:
 * - Blank screen? Check display type settings below
 * - No Serial output? Open Serial Monitor (Tools → Serial Monitor)
 * - Display wrong colors? Check display type selection
 *
 * TRY THIS STEP BY STEP:
 * 1. Upload this code
 * 2. Open Serial Monitor (Tools → Serial Monitor, set to 115200 baud)
 * 3. You should see initialization messages
 * 4. Display should show "M1Shield Test" with colored squares
 * 5. Press each button - watch Serial Monitor and LED
 * 6. Move joystick - watch LED turn magenta and Serial show directions
 * 7. Press joystick button - LED should turn white
 *
 * Authors: Marcel Erz (RetroStack)
 * License: MIT
 */

// ===================================================================
// STEP 1: Select your display type
// ===================================================================
// Uncomment ONE of these lines to match your display:

// #define USE_ST7735    // For 128x160 TFT displays
// #define USE_ILI9341   // For 240x320 TFT displays

// If you don't uncomment anything, it defaults to ST7789 (320x240)
// which is the most common display type for M1Shield

#include "M1Shield.h"

// ===================================================================
// SETUP FUNCTION - Runs once when Arduino starts
// ===================================================================
void setup()
{
    // Start serial communication for debug messages
    // Open "Serial Monitor" in Arduino IDE to see these messages
    Serial.begin(115200);

    Serial.println("=== M1Shield Hardware Test Starting ===");
    Serial.println("");
    Serial.println("This test verifies all M1Shield hardware components:");
    Serial.println("- Display initialization and graphics");
    Serial.println("- Button input detection");
    Serial.println("- Joystick movement and button");
    Serial.println("- LED color control");
    Serial.println("");

    // Show what display type we're using
    Serial.print("Display Type: ");
    Serial.println(DISPLAY_NAME);
    Serial.print("Resolution: ");
    Serial.print(DISPLAY_WIDTH);
    Serial.print("x");
    Serial.println(DISPLAY_HEIGHT);
    Serial.println("");

    // Initialize the M1Shield hardware
    // This sets up display, buttons, joystick, LED, and everything else
    Serial.println("Initializing M1Shield hardware...");
    M1Shield.begin();
    Serial.println("Hardware initialization complete!");

    // Verify that everything initialized correctly
    Serial.println("");
    Serial.println("Hardware Status Check:");
    Serial.print("   Display initialized: ");
    Serial.println(M1Shield.isDisplayInitialized() ? "YES" : "NO");

    Serial.print("   Screen dimensions: ");
    Serial.print(M1Shield.getScreenWidth());
    Serial.print("x");
    Serial.print(M1Shield.getScreenHeight());
    Serial.println(" pixels");

    // Draw a test pattern on the display to verify it's working
    Serial.println("");
    Serial.println("Drawing test pattern on display...");
    drawTestPattern();
    Serial.println("Test pattern displayed!");

    Serial.println("");
    Serial.println("=== Ready for Testing! ===");
    Serial.println("Try these inputs and watch the feedback:");
    Serial.println("Menu Button → LED: CYAN");
    Serial.println("Arrow Buttons → LED: BLUE");
    Serial.println("Joystick Movement → LED: MAGENTA");
    Serial.println("Joystick Press → LED: WHITE");
    Serial.println("No Input → LED: OFF");
    Serial.println("");
    Serial.println("Input feedback will appear below:");
    Serial.println("=====================================");
}

// LOOP FUNCTION - Runs continuously forever
void loop()
{
    // Test all input methods and show feedback
    // These functions monitor for changes and print to Serial Monitor
    testButtons();  // Check all button presses
    testJoystick(); // Check joystick movement and button

    // Update LED color based on current input
    // This gives immediate visual feedback for any input
    updateLED();

    // Small delay to prevent overwhelming the Serial Monitor
    // Without this, messages would scroll too fast to read
    delay(50);
}

// DISPLAY TEST PATTERN - Shows display is working properly
void drawTestPattern()
{
    Adafruit_GFX &gfx = M1Shield.getGFX();

    // Clear screen to black background
    gfx.fillScreen(ST77XX_BLACK);

    // Draw white border around entire screen
    gfx.drawRect(0, 0, M1Shield.getScreenWidth(), M1Shield.getScreenHeight(), ST77XX_WHITE);

    // Draw title in large text
    gfx.setTextColor(ST77XX_WHITE);
    gfx.setTextSize(2);
    gfx.setCursor(10, 10);
    gfx.print("M1Shield Test");

    // Show which display type was detected
    gfx.setTextSize(1);
    gfx.setCursor(10, 40);
    gfx.print("Display: ");
    gfx.print(DISPLAY_NAME);

    // Show screen resolution
    gfx.setCursor(10, 60);
    gfx.print("Resolution: ");
    gfx.print(DISPLAY_WIDTH);
    gfx.print("x");
    gfx.print(DISPLAY_HEIGHT);

    // Draw colored squares to test RGB display capability
    gfx.fillRect(10, 80, 20, 20, ST77XX_RED);   // Red square
    gfx.fillRect(35, 80, 20, 20, ST77XX_GREEN); // Green square
    gfx.fillRect(60, 80, 20, 20, ST77XX_BLUE);  // Blue square

    // Instructions for user
    gfx.setCursor(10, 110);
    gfx.print("Press buttons to test...");
}

// BUTTON TESTING - Check all 6 buttons (Menu + 4 directional + joystick)
void testButtons()
{
    // Static variables remember the last state of each button
    // This lets us detect when a button changes from pressed to released
    static bool lastMenuState = false;
    static bool lastLeftState = false;
    static bool lastRightState = false;
    static bool lastUpState = false;
    static bool lastDownState = false;
    static bool lastJoyState = false;

    // Test menu button (center button on M1Shield)
    bool menuPressed = M1Shield.isMenuPressed();
    if (menuPressed != lastMenuState)
    {
        Serial.print("Menu button: ");
        Serial.println(menuPressed ? "PRESSED" : "RELEASED");
        lastMenuState = menuPressed;
    }

    // Test all four directional buttons
    bool leftPressed = M1Shield.isLeftPressed();
    if (leftPressed != lastLeftState)
    {
        Serial.print("Left button: ");
        Serial.println(leftPressed ? "PRESSED" : "RELEASED");
        lastLeftState = leftPressed;
    }

    bool rightPressed = M1Shield.isRightPressed();
    if (rightPressed != lastRightState)
    {
        Serial.print("Right button: ");
        Serial.println(rightPressed ? "PRESSED" : "RELEASED");
        lastRightState = rightPressed;
    }

    bool upPressed = M1Shield.isUpPressed();
    if (upPressed != lastUpState)
    {
        Serial.print("Up button: ");
        Serial.println(upPressed ? "PRESSED" : "RELEASED");
        lastUpState = upPressed;
    }

    bool downPressed = M1Shield.isDownPressed();
    if (downPressed != lastDownState)
    {
        Serial.print("Down button: ");
        Serial.println(downPressed ? "PRESSED" : "RELEASED");
        lastDownState = downPressed;
    }

    // Test joystick button (press down on joystick)
    bool joyPressed = M1Shield.isJoystickPressed();
    if (joyPressed != lastJoyState)
    {
        Serial.print("Joystick button: ");
        Serial.println(joyPressed ? "PRESSED" : "RELEASED");
        lastJoyState = joyPressed;
    }
}

// JOYSTICK TESTING - Check analog joystick movement and direction
void testJoystick()
{
    // Remember the last direction to detect changes
    static JoystickDirection lastDirection = CENTER;
    static unsigned long lastPrint = 0;

    // Get current joystick direction (CENTER, UP, DOWN, LEFT, RIGHT, etc.)
    JoystickDirection direction = M1Shield.getJoystickDirection();

    // Print when direction changes
    if (direction != lastDirection)
    {
        Serial.print("Joystick: ");
        Serial.print(M1Shield.joystickDirectionToString(direction));
        Serial.print(" (X: ");
        Serial.print(M1Shield.getJoystickX()); // Raw X value (0-1023)
        Serial.print(", Y: ");
        Serial.print(M1Shield.getJoystickY()); // Raw Y value (0-1023)
        Serial.println(")");
        lastDirection = direction;
    }

    // Print raw analog values every second when joystick is moved
    // This helps you see the exact values your joystick produces
    if (direction != CENTER && millis() - lastPrint > 1000)
    {
        Serial.print("Joystick raw values - X: ");
        Serial.print(M1Shield.getJoystickX());
        Serial.print(", Y: ");
        Serial.println(M1Shield.getJoystickY());
        lastPrint = millis();
    }
}

// LED FEEDBACK - Shows visual feedback based on current input
void updateLED()
{
    // Change LED color based on what input is active
    // This gives immediate visual feedback for any input

    if (M1Shield.isJoystickPressed())
    {
        // White when joystick button is pressed
        M1Shield.setLEDColor(LEDColor::COLOR_WHITE);
    }
    else if (M1Shield.isMenuPressed())
    {
        // Cyan when menu button is pressed
        M1Shield.setLEDColor(LEDColor::COLOR_CYAN);
    }
    else if (M1Shield.isLeftPressed() || M1Shield.isRightPressed() ||
             M1Shield.isUpPressed() || M1Shield.isDownPressed())
    {
        // Blue when any directional button is pressed
        M1Shield.setLEDColor(LEDColor::COLOR_BLUE);
    }
    else if (!M1Shield.isJoystickCentered())
    {
        // Magenta when joystick is moved but not pressed
        M1Shield.setLEDColor(LEDColor::COLOR_MAGENTA);
    }
    else
    {
        // LED off when no input is active
        M1Shield.setLEDColor(LEDColor::COLOR_OFF);
    }
}
