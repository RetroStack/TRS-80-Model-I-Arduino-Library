/*
 * ===================================================================
 * Simple M1Shield Example - Basic Hardware Testing
 * ===================================================================
 *
 * This is the perfect starting example for learning M1Shield basics!
 * Learn fundamental hardware interaction with clear, simple code.
 *
 * WHAT YOU'LL LEARN:
 * - How to initialize M1Shield with DisplayProvider system
 * - How to select the correct display type for your hardware
 * - How to read button and joystick input
 * - How to control the LED with different colors
 * - How to draw basic graphics on the display
 * - How to use Serial Monitor for debugging
 *
 * WHAT THIS EXAMPLE DOES:
 * - Tests basic display initialization
 * - Monitors button presses (Menu, Up, Down, Left, Right)
 * - Reads joystick movement and button press
 * - Provides LED feedback for different inputs
 * - Shows clear Serial Monitor output
 *
 * CONTROLS & FEEDBACK:
 * - Menu Button: LED turns CYAN
 * - Arrow Buttons: LED turns BLUE
 * - Joystick Movement: LED turns GREEN
 * - Joystick Press: LED turns WHITE
 * - No Input: LED turns OFF
 *
 * Authors: Marcel Erz (RetroStack)
 * License: MIT
 */

// ===================================================================
// STEP 1: Select your display provider
// ===================================================================
// IMPORTANT: Uncomment ONLY ONE display provider that matches your hardware!

// For ST7789 240x320 displays (most common, landscape becomes 320x240)
#include <Display_ST7789_320x240.h>
Display_ST7789_320x240 displayProvider;

// For ST7789 240x240 square displays
// #include <Display_ST7789_240x240.h>
// Display_ST7789_240x240 displayProvider;

// For ST7789 320x170 wide displays (landscape)
// #include <Display_ST7789_320x170.h>
// Display_ST7789_320x170 displayProvider;

// For smaller ST7735 128x160 displays
// #include <Display_ST7735.h>
// Display_ST7735 displayProvider;

// For monochrome SSD1306 OLED displays (128x64)
// #include <Display_SSD1306.h>
// Display_SSD1306 displayProvider;

#include <M1Shield.h>

// ===================================================================
// SETUP FUNCTION - Runs once when Arduino starts
// ===================================================================
void setup()
{
    // Start serial communication for debug messages
    Serial.begin(115200);

    Serial.println("=== Simple M1Shield Test ===");
    Serial.println("Learning M1Shield basics...");
    Serial.println("");

    // Show display information
    Serial.print("Display: ");
    Serial.print(displayProvider.name());
    Serial.print(" (");
    Serial.print(displayProvider.width());
    Serial.print("x");
    Serial.print(displayProvider.height());
    Serial.println(")");

    // Initialize M1Shield hardware
    Serial.println("Initializing M1Shield...");
    M1Shield.begin(displayProvider);

    // Check for SD card
    Serial.print("SD Card Status: ");
    if (M1Shield.isSDCardInserted())
    {
        Serial.println("Detected and initialized");
    }
    else
    {
        Serial.println("Not detected or initialization failed");
    }

    // Draw simple test display
    drawSimpleDisplay();

    Serial.println("M1Shield ready!");
    Serial.println("Press buttons and move joystick...");
    Serial.println("=====================================");
}

// ===================================================================
// LOOP FUNCTION - Runs continuously
// ===================================================================
void loop()
{
    // Check for input and provide feedback
    checkButtons();
    checkJoystick();
    updateLED();

    // Small delay for readability
    delay(100);
}

// ===================================================================
// DISPLAY FUNCTIONS
// ===================================================================
void drawSimpleDisplay()
{
    Adafruit_GFX &gfx = M1Shield.getGFX();

    // Clear screen
    gfx.fillScreen(M1Shield.convertColor(0x0000));

    // Title
    gfx.setTextColor(M1Shield.convertColor(0xFFFF));
    gfx.setTextSize(2);
    gfx.setCursor(10, 10);
    gfx.print("Simple M1Shield");

    // Instructions
    gfx.setTextSize(1);
    gfx.setCursor(10, 40);
    gfx.print("Press buttons to test");

    // Status area
    gfx.setCursor(10, 60);
    gfx.print("Status: Ready");
}

// ===================================================================
// INPUT FUNCTIONS
// ===================================================================
void checkButtons()
{
    static bool lastMenu = false, lastUp = false, lastDown = false;
    static bool lastLeft = false, lastRight = false;

    // Menu button
    bool menu = M1Shield.isMenuPressed();
    if (menu != lastMenu)
    {
        Serial.print("Menu: ");
        Serial.println(menu ? "PRESSED" : "RELEASED");
        lastMenu = menu;
    }

    // Directional buttons
    bool up = M1Shield.isUpPressed();
    if (up != lastUp)
    {
        Serial.print("Up: ");
        Serial.println(up ? "PRESSED" : "RELEASED");
        lastUp = up;
    }

    bool down = M1Shield.isDownPressed();
    if (down != lastDown)
    {
        Serial.print("Down: ");
        Serial.println(down ? "PRESSED" : "RELEASED");
        lastDown = down;
    }

    bool left = M1Shield.isLeftPressed();
    if (left != lastLeft)
    {
        Serial.print("Left: ");
        Serial.println(left ? "PRESSED" : "RELEASED");
        lastLeft = left;
    }

    bool right = M1Shield.isRightPressed();
    if (right != lastRight)
    {
        Serial.print("Right: ");
        Serial.println(right ? "PRESSED" : "RELEASED");
        lastRight = right;
    }
}

void checkJoystick()
{
    static bool lastJoyButton = false;
    static JoystickDirection lastDirection = CENTER;

    // Joystick button
    bool joyButton = M1Shield.isJoystickPressed();
    if (joyButton != lastJoyButton)
    {
        Serial.print("Joystick Button: ");
        Serial.println(joyButton ? "PRESSED" : "RELEASED");
        lastJoyButton = joyButton;
    }

    // Joystick direction
    JoystickDirection direction = M1Shield.getJoystickDirection();
    if (direction != lastDirection)
    {
        Serial.print("Joystick: ");
        switch (direction)
        {
        case UP:
            Serial.println("UP");
            break;
        case DOWN:
            Serial.println("DOWN");
            break;
        case LEFT:
            Serial.println("LEFT");
            break;
        case RIGHT:
            Serial.println("RIGHT");
            break;
        case CENTER:
            Serial.println("CENTER");
            break;
        default:
            Serial.println("DIAGONAL");
            break;
        }
        lastDirection = direction;
    }
}

void updateLED()
{
    // LED feedback based on input
    if (M1Shield.isJoystickPressed())
    {
        M1Shield.setLEDColor(LEDColor::COLOR_WHITE);
    }
    else if (M1Shield.isMenuPressed())
    {
        M1Shield.setLEDColor(LEDColor::COLOR_CYAN);
    }
    else if (M1Shield.isUpPressed() || M1Shield.isDownPressed() ||
             M1Shield.isLeftPressed() || M1Shield.isRightPressed())
    {
        M1Shield.setLEDColor(LEDColor::COLOR_BLUE);
    }
    else if (!M1Shield.isJoystickCentered())
    {
        M1Shield.setLEDColor(LEDColor::COLOR_GREEN);
    }
    else
    {
        M1Shield.setLEDColor(LEDColor::COLOR_OFF);
    }
}
