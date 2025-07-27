/*
 * ===================================================================
 * M1Shield Hardware Test - Complete Beginner's Guide
 * ===================================================================
 *
 * This is the PERFECT starting example for learning M1Shield!
 * Tests all hardware components with clear feedback.
 *
 * WHAT YOU'LL LEARN:
 * - How to initialize M1Shield hardware with DisplayProvider system
 * - How to select and configure different display types
 * - How to read buttons and joystick input
 * - How to control the LED with different colors
 * - How to draw basic graphics on the display
 * - How to use Serial Monitor for debugging
 * - How to use the new display provider architecture
 *
 * WHAT THIS EXAMPLE DOES:
 * - Tests display initialization using DisplayProvider system
 * - Demonstrates proper display provider selection and configuration
 * - Monitors all button presses (Menu, Up, Down, Left, Right)
 * - Tracks joystick movement and button press
 * - Changes LED color based on current input
 * - Prints detailed feedback to Serial Monitor
 * - Shows display information from the provider
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
 * - Knowledge of your display type (see DISPLAY PROVIDERS section below)
 *
 * DISPLAY PROVIDERS - NEW SYSTEM:
 * This example uses the new DisplayProvider system for better display support.
 * Each display type has its own provider class with optimized settings:
 *
 * - Display_ST7789: Most common 240x320 TFT (landscape: 320x240)
 * - Display_ST7789_240x240: Square 240x240 TFT displays
 * - Display_ST7735: Smaller 128x160 TFT displays
 * - Display_ILI9341: Popular 240x320 TFT (landscape: 320x240)
 * - Display_ST7796: Large 320x480 TFT (landscape: 480x320)
 * - Display_HX8357: Large 320x480 TFT displays
 * - Display_ILI9325: Parallel interface 240x320 displays
 *
 * Each provider automatically handles:
 * - Proper initialization sequence
 * - Optimal rotation settings
 * - Color space configuration
 * - Display-specific features
 *
 * HOW TO SELECT YOUR DISPLAY:
 * 1. Identify your display controller chip (usually printed on display)
 * 2. Uncomment the matching #include and provider declaration below
 * 3. Comment out all other display providers
 * 4. Upload and test - display should work perfectly
 *
 * TROUBLESHOOTING FOR BEGINNERS:
 * - Blank screen? Wrong display provider selected
 * - Wrong colors? Try different provider for same size display
 * - Upside down? Provider handles rotation automatically
 * - No Serial output? Open Serial Monitor (Tools → Serial Monitor)
 * - Still issues? Check wiring and power connections
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
// STEP 1: Select your display provider
// ===================================================================
// IMPORTANT: Uncomment ONLY ONE display provider that matches your hardware!
// Each provider is optimized for specific display controllers.

// For ST7789 240x320 displays (most common, landscape becomes 320x240)
#include <Display_ST7789.h>
Display_ST7789 displayProvider;

// For ST7789 240x240 square displays
// #include <Display_ST7789_240x240.h>
// Display_ST7789_240x240 displayProvider;

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

// ===================================================================

#include <M1Shield.h>

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

    // Show what display type we're using from the provider
    Serial.print("Display Provider: ");
    Serial.println(displayProvider.name());
    Serial.print("Resolution: ");
    Serial.print(displayProvider.width());
    Serial.print("x");
    Serial.println(displayProvider.height());
    Serial.println("");

    // Initialize the M1Shield hardware
    // This sets up display, buttons, joystick, LED, and everything else
    Serial.println("Initializing M1Shield hardware...");
    M1Shield.begin(displayProvider);
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
    gfx.fillScreen(0x0000);

    // Draw white border around entire screen
    gfx.drawRect(0, 0, M1Shield.getScreenWidth(), M1Shield.getScreenHeight(), 0xFFFF);

    // Draw title in large text
    gfx.setTextColor(0xFFFF);
    gfx.setTextSize(2);
    gfx.setCursor(10, 10);
    gfx.print("M1Shield Test");

    // Show which display provider was used
    gfx.setTextSize(1);
    gfx.setCursor(10, 40);
    gfx.print("Provider: ");
    gfx.print(displayProvider.name());

    // Show screen resolution from provider
    gfx.setCursor(10, 60);
    gfx.print("Resolution: ");
    gfx.print(displayProvider.width());
    gfx.print("x");
    gfx.print(displayProvider.height());

    // Draw colored squares to test RGB display capability
    gfx.fillRect(10, 80, 20, 20, 0xF800); // Red square
    gfx.fillRect(35, 80, 20, 20, 0x07E0); // Green square
    gfx.fillRect(60, 80, 20, 20, 0x001F); // Blue square

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
        Serial.print("(X: ");
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
