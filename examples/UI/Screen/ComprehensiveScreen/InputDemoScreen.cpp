/*
 * InputDemoScreen.cpp
 *
 * Implementation of complete input handling demonstration for the Screen class.
 */

#include "InputDemoScreen.h"
#include <math.h>

InputDemoScreen::InputDemoScreen()
    : Screen()
{
    // Set the screen title
    setTitle("Input Demo");

    // Initialize input state
    memset(&inputState, 0, sizeof(inputState));

    // Initialize button colors (all white initially)
    for (int i = 0; i < 6; i++)
    {
        buttonTestColors[i] = M1Shield.convertColor(0xFFFF); // White
    }
    colorResetTime = 0;
}

bool InputDemoScreen::open()
{
    Serial.println("=== InputDemoScreen::open() ===");
    Serial.println("Ready to test ALL input capabilities!");
    Serial.println("Try pressing all buttons and moving the joystick!");

    // CRITICAL: Call base class open() first to set _active flag and perform initialization
    bool result = Screen::open();
    if (!result)
    {
        Serial.println("ERROR: Base Screen::open() failed!");
        return false;
    }

    // Reset input state
    memset(&inputState, 0, sizeof(inputState));
    inputState.lastInputTime = millis();

    // Reset button colors
    resetButtonColors();

    return true;
}

void InputDemoScreen::_drawScreen()
{
    if (!isActive())
        return;

    // Only do full screen clear and draw on initial load
    Adafruit_GFX &gfx = M1Shield.getGFX();
    gfx.fillScreen(M1Shield.convertColor(0x0000)); // Black background

    // Draw all components - each handles its own small/large display logic
    drawControllerLayout();
    drawJoystickArea();
    drawStatusArea();
}

void InputDemoScreen::loop()
{
    // Handle color reset timer for button feedback
    if (colorResetTime > 0 && millis() - colorResetTime > 1000)
    {
        resetButtonColors();
        colorResetTime = 0;

        // Update button visuals - function handles small/large display logic
        updateButtonVisuals();
    }
}

// Selective updates - each function handles its own small/large display logic
void InputDemoScreen::updateAfterInput(ActionTaken action, int8_t offsetX, int8_t offsetY)
{
    if (!isActive())
        return;

    // Update buttons if any button was pressed
    if (action & (BUTTON_MENU | BUTTON_LEFT | BUTTON_RIGHT | BUTTON_UP | BUTTON_DOWN | BUTTON_JOYSTICK))
    {
        updateButtonVisuals();
    }

    // Update joystick if joystick moved or was pressed
    if ((action & JOYSTICK_ANY) || (action & BUTTON_JOYSTICK) ||
        (abs(inputState.joystickX) > 0) || (abs(inputState.joystickY) > 0))
    {
        updateJoystickVisual();
    } // Always update status for input counts and timing
    updateStatusVisual();

    // Push changes to display
    M1Shield.display();
}

Screen *InputDemoScreen::actionTaken(ActionTaken action, int8_t offsetX, int8_t offsetY)
{
    // Record ALL input details for comprehensive demonstration
    inputState.lastInputTime = millis();
    inputState.totalInputs++;

    // Now offsetX and offsetY are already signed coordinates from M1Shield (-127 to +128)
    inputState.joystickX = offsetX;
    inputState.joystickY = offsetY; // Debug output to understand the values
    Serial.print("Raw joystick: X=");
    Serial.print(offsetX);
    Serial.print(" Y=");
    Serial.print(offsetY);
    Serial.print(" -> Converted: X=");
    Serial.print(inputState.joystickX);
    Serial.print(" Y=");
    Serial.println(inputState.joystickY);

    // Update input state based on action
    if (action & BUTTON_MENU)
    {
        inputState.menuPressed = true;
        buttonTestColors[0] = M1Shield.convertColor(0xF800); // Red
        inputState.lastAction = "MENU";
    }

    if (action & BUTTON_LEFT)
    {
        inputState.leftPressed = true;
        buttonTestColors[1] = M1Shield.convertColor(0x07E0); // Green
        inputState.lastAction = "LEFT";
    }

    if (action & BUTTON_RIGHT)
    {
        inputState.rightPressed = true;
        buttonTestColors[2] = M1Shield.convertColor(0x001F); // Blue
        inputState.lastAction = "RIGHT";
    }

    if (action & BUTTON_UP)
    {
        inputState.upPressed = true;
        buttonTestColors[3] = M1Shield.convertColor(0xFFE0); // Yellow
        inputState.lastAction = "UP";
    }

    if (action & BUTTON_DOWN)
    {
        inputState.downPressed = true;
        buttonTestColors[4] = M1Shield.convertColor(0xF81F); // Magenta
        inputState.lastAction = "DOWN";
    }

    if (action & BUTTON_JOYSTICK)
    {
        inputState.joystickPressed = true;
        buttonTestColors[5] = M1Shield.convertColor(0x07FF); // Cyan
        inputState.lastAction = "JOYSTICK";
    }

    if (action & JOYSTICK_ANY)
    {
        inputState.lastAction = "JOYSTICK_MOVE";
    }

    // Set color reset timer
    colorResetTime = millis();

    // Log detailed input information
    Serial.println("=== INPUT DETECTED ===");
    Serial.print("Action bits: ");
    Serial.print(action, BIN);
    Serial.print(" (");
    Serial.print(action);
    Serial.println(")");
    Serial.print("Joystick offset: X=");
    Serial.print(offsetX);
    Serial.print(", Y=");
    Serial.println(offsetY);
    Serial.print("Total inputs: ");
    Serial.println(inputState.totalInputs);

    // Force selective updates to show input feedback immediately
    updateAfterInput(action, offsetX, offsetY);

    return nullptr; // Stay on this screen
}

void InputDemoScreen::drawControllerLayout()
{
    Adafruit_GFX &display = M1Shield.getGFX();

    if (isSmallDisplay())
    {
        // Compact button layout for small displays
        int y = 25;
        display.setTextSize(1);
        display.setTextColor(M1Shield.convertColor(0xFFFF));

        // Show last action prominently
        display.setCursor(5, y);
        display.print("Last Action:");
        display.setTextColor(M1Shield.convertColor(0x07E0));
        display.setCursor(5, y + 12);
        display.print(inputState.lastAction);

        // Show simple button status
        display.setTextColor(M1Shield.convertColor(0xC618));
        display.setCursor(5, y + 30);
        display.print("Buttons: ");
        if (inputState.menuPressed)
            display.print("M ");
        if (inputState.leftPressed)
            display.print("L ");
        if (inputState.rightPressed)
            display.print("R ");
        if (inputState.upPressed)
            display.print("U ");
        if (inputState.downPressed)
            display.print("D ");
        if (inputState.joystickPressed)
            display.print("J ");
    }
    else
    {
        // Full controller layout for large displays
        int centerX = (display.width() / 2) - 40;
        int topY = 20;
        int buttonSize = 35;
        int spacing = 45;

        // D-Pad layout (LEFT, UP, RIGHT, DOWN in cross formation)
        // UP button (top center)
        int upX = centerX - buttonSize / 2;
        int upY = topY;
        drawButton(upX, upY, buttonSize, buttonSize, "UP", inputState.upPressed, buttonTestColors[3]);

        // LEFT button (left side)
        int leftX = centerX - spacing - buttonSize / 2;
        int leftY = topY + spacing;
        drawButton(leftX, leftY, buttonSize, buttonSize, "L", inputState.leftPressed, buttonTestColors[1]);

        // RIGHT button (right side)
        int rightX = centerX + spacing - buttonSize / 2;
        int rightY = topY + spacing;
        drawButton(rightX, rightY, buttonSize, buttonSize, "R", inputState.rightPressed, buttonTestColors[2]);

        // DOWN button (bottom center)
        int downX = centerX - buttonSize / 2;
        int downY = topY + spacing * 2;
        drawButton(downX, downY, buttonSize, buttonSize, "DN", inputState.downPressed, buttonTestColors[4]);

        // MENU button (top left)
        drawButton(10, topY, 40, 25, "MENU", inputState.menuPressed, buttonTestColors[0]);
    }
}

void InputDemoScreen::drawJoystickArea()
{
    Adafruit_GFX &display = M1Shield.getGFX();

    if (isSmallDisplay())
    {
        // Compact joystick display for small screens
        // Show joystick info if moved or pressed (since we only get updates when beyond deadzone)
        if (abs(inputState.joystickX) > 0 || abs(inputState.joystickY) > 0 || inputState.joystickPressed)
        {
            int y = 45;
            display.setTextSize(1);
            display.setTextColor(M1Shield.convertColor(0xFFFF));
            display.setCursor(5, y);
            display.print("Joy:");

            display.setTextColor(M1Shield.convertColor(0x07FF));
            display.print(inputState.joystickX);
            display.print(",");
            display.print(inputState.joystickY);

            if (inputState.joystickPressed)
            {
                display.setTextColor(M1Shield.convertColor(0xF800));
                display.print(" BTN");
            }
        }
    }
    else
    {
        // Large joystick visualization on the right side
        int centerX = display.width() - 60;
        int centerY = 100;
        int outerRadius = 40;
        int innerRadius = 6;

        // Draw outer circle (joystick boundary)
        display.drawCircle(centerX, centerY, outerRadius, M1Shield.convertColor(0x8410)); // Gray
        display.drawCircle(centerX, centerY, outerRadius - 1, M1Shield.convertColor(0x8410));

        // Draw center cross for reference
        display.drawFastHLine(centerX - 15, centerY, 30, M1Shield.convertColor(0x4208)); // Dark gray
        display.drawFastVLine(centerX, centerY - 15, 30, M1Shield.convertColor(0x4208));

        // Calculate joystick position (scale to radius)
        // joystickX and joystickY are now properly signed values (-127 to +128)
        int joyX = centerX + (inputState.joystickX * (outerRadius - 10) / 127);
        int joyY = centerY + (inputState.joystickY * (outerRadius - 10) / 127);

        // Clamp to circle bounds
        int deltaX = joyX - centerX;
        int deltaY = joyY - centerY;
        int distance = sqrt(deltaX * deltaX + deltaY * deltaY);
        if (distance > outerRadius - 10)
        {
            float ratio = (float)(outerRadius - 10) / distance;
            joyX = centerX + deltaX * ratio;
            joyY = centerY + deltaY * ratio;
        }

        // Draw joystick position
        uint16_t joyColor = inputState.joystickPressed ? M1Shield.convertColor(0xF800) : // Red when pressed
                                M1Shield.convertColor(0x07E0);                           // Green normal

        display.fillCircle(joyX, joyY, innerRadius, joyColor);
        display.drawCircle(joyX, joyY, innerRadius, M1Shield.convertColor(0xFFFF)); // White border

        // Joystick button indicator
        drawButton(centerX - 25, centerY + outerRadius + 10, 50, 20, "JOY BTN",
                   inputState.joystickPressed, buttonTestColors[5]);

        // Label
        display.setTextSize(1);
        display.setTextColor(M1Shield.convertColor(0xFFFF));
        display.setCursor(centerX - 20, centerY - outerRadius - 15);
        display.print("JOYSTICK");

        // Show coordinates
        display.setTextSize(1);
        display.setTextColor(M1Shield.convertColor(0xC618));
        display.setCursor(centerX - 25, centerY + outerRadius + 35);
        display.print(inputState.joystickX);
        display.print(",");
        display.print(inputState.joystickY);
    }
}

void InputDemoScreen::drawStatusArea()
{
    Adafruit_GFX &display = M1Shield.getGFX();

    if (isSmallDisplay())
    {
        // Compact status for small displays
        int y = 60;
        display.setTextSize(1);
        display.setTextColor(M1Shield.convertColor(0xFFE0)); // Yellow
        display.setCursor(5, y);
        display.print("Total: ");
        display.setTextColor(M1Shield.convertColor(0xFFFF));
        display.print(inputState.totalInputs);

        // Instructions
        display.setTextColor(M1Shield.convertColor(0x8410)); // Dark gray
        display.setCursor(5, display.height() - 10);
        display.print("Try all inputs!");
    }
    else
    {
        // Full status information at the bottom for large displays
        int bottomY = display.height() - 50;

        display.setTextSize(1);
        display.setTextColor(M1Shield.convertColor(0xFFE0)); // Yellow
        display.setCursor(10, bottomY);
        display.print("Total Inputs: ");
        display.setTextColor(M1Shield.convertColor(0xFFFF));
        display.print(inputState.totalInputs);

        // Time since last input
        unsigned long timeSince = millis() - inputState.lastInputTime;
        display.setTextColor(M1Shield.convertColor(0xC618)); // Light gray
        display.setCursor(10, bottomY + 15);
        display.print("Last: ");
        display.print(timeSince);
        display.print("ms ago");

        // Instructions
        display.setTextColor(M1Shield.convertColor(0x8410)); // Dark gray
        display.setCursor(10, bottomY + 30);
        display.print("Try all buttons & joystick!");
    }
}

// Helper method to draw a button with proper styling
void InputDemoScreen::drawButton(int x, int y, int w, int h, const char *label,
                                 bool pressed, uint16_t pressedColor)
{
    Adafruit_GFX &display = M1Shield.getGFX();

    // Button background
    uint16_t bgColor = pressed ? pressedColor : M1Shield.convertColor(0x2104); // Dark gray
    display.fillRect(x, y, w, h, bgColor);

    // Button border
    uint16_t borderColor = pressed ? M1Shield.convertColor(0xFFFF) : M1Shield.convertColor(0x8410);
    display.drawRect(x, y, w, h, borderColor);
    if (pressed)
    {
        display.drawRect(x + 1, y + 1, w - 2, h - 2, borderColor); // Double border when pressed
    }

    // Button label
    display.setTextSize(1);
    display.setTextColor(pressed ? M1Shield.convertColor(0x0000) : M1Shield.convertColor(0xFFFF));

    // Center text in button
    int textWidth = strlen(label) * 6; // Approximate width for size 1 text
    int textX = x + (w - textWidth) / 2;
    int textY = y + (h - 8) / 2; // 8 is height of size 1 text

    display.setCursor(textX, textY);
    display.print(label);
}

void InputDemoScreen::resetButtonColors()
{
    for (int i = 0; i < 6; i++)
    {
        buttonTestColors[i] = M1Shield.convertColor(0xFFFF); // White
    }

    // Reset pressed states
    inputState.menuPressed = false;
    inputState.leftPressed = false;
    inputState.rightPressed = false;
    inputState.upPressed = false;
    inputState.downPressed = false;
    inputState.joystickPressed = false;
}

uint16_t InputDemoScreen::getActionColor(ActionTaken action)
{
    if (action & BUTTON_MENU)
        return M1Shield.convertColor(0xF800); // Red
    if (action & BUTTON_LEFT)
        return M1Shield.convertColor(0x07E0); // Green
    if (action & BUTTON_RIGHT)
        return M1Shield.convertColor(0x001F); // Blue
    if (action & BUTTON_UP)
        return M1Shield.convertColor(0xFFE0); // Yellow
    if (action & BUTTON_DOWN)
        return M1Shield.convertColor(0xF81F); // Magenta
    if (action & BUTTON_JOYSTICK)
        return M1Shield.convertColor(0x07FF); // Cyan
    return M1Shield.convertColor(0xFFFF);     // White
}

const char *InputDemoScreen::getActionName(ActionTaken action)
{
    if (action & BUTTON_MENU)
        return "MENU";
    if (action & BUTTON_LEFT)
        return "LEFT";
    if (action & BUTTON_RIGHT)
        return "RIGHT";
    if (action & BUTTON_UP)
        return "UP";
    if (action & BUTTON_DOWN)
        return "DOWN";
    if (action & BUTTON_JOYSTICK)
        return "JOYSTICK";
    if (action & JOYSTICK_ANY)
        return "JOYSTICK_MOVE";
    return "UNKNOWN";
}

// ===================================================================
// Selective Update Methods for Performance Optimization
// ===================================================================

void InputDemoScreen::updateButtonVisuals()
{
    Adafruit_GFX &display = M1Shield.getGFX();

    display.startWrite();

    if (isSmallDisplay())
    {
        // Clear and redraw controller area for small displays
        int y = 25;
        int height = 35; // Button status area
        display.fillRect(0, y, display.width(), height, M1Shield.convertColor(0x0000));
    }
    else
    {
        // Clear controller layout area for large displays
        int topY = 20;
        int height = 135; // Covers the entire button layout area
        display.fillRect(0, topY, display.width() - 120, height, M1Shield.convertColor(0x0000));
    }

    // Redraw controller layout (handles both small and large internally)
    drawControllerLayout();

    display.endWrite();
}

void InputDemoScreen::updateJoystickVisual()
{
    Adafruit_GFX &display = M1Shield.getGFX();

    display.startWrite();

    if (isSmallDisplay())
    {
        // Clear joystick text area for small displays
        int y = 45;
        int height = 15;
        display.fillRect(0, y, display.width(), height, M1Shield.convertColor(0x0000));
    }
    else
    {
        // Clear joystick area for large displays (right side of screen)
        int rightX = display.width() - 120;
        int topY = 20;
        int width = 120;
        int height = 180;
        display.fillRect(rightX, topY, width, height, M1Shield.convertColor(0x0000));
    }

    // Redraw joystick area (handles both small and large internally)
    drawJoystickArea();

    display.endWrite();
}

void InputDemoScreen::updateStatusVisual()
{
    Adafruit_GFX &display = M1Shield.getGFX();

    display.startWrite();

    if (isSmallDisplay())
    {
        // Clear status areas for small displays
        int y1 = 60;
        int height1 = 15;
        display.fillRect(0, y1, display.width(), height1, M1Shield.convertColor(0x0000));

        int y2 = display.height() - 10;
        int height2 = 10;
        display.fillRect(0, y2, display.width(), height2, M1Shield.convertColor(0x0000));
    }
    else
    {
        // Clear status area for large displays
        int bottomY = display.height() - 50;
        int height = 50;
        display.fillRect(0, bottomY, display.width(), height, M1Shield.convertColor(0x0000));
    }

    // Redraw status area (handles both small and large internally)
    drawStatusArea();

    display.endWrite();
}
