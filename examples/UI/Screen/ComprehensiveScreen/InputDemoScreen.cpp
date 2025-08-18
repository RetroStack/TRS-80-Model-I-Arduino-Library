/*
 * InputDemoScreen.cpp
 *
 * Implementation of complete input handling demonstration for the Screen class.
 */

#include "InputDemoScreen.h"

InputDemoScreen::InputDemoScreen()
{
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

    // Reset input state
    memset(&inputState, 0, sizeof(inputState));
    inputState.lastInputTime = millis();

    // Reset button colors
    resetButtonColors();

    return true;
}

void InputDemoScreen::_drawScreen()
{
    // Content area (avoiding header)
    int startY = isSmallDisplay() ? 25 : 75;

    if (isSmallDisplay())
    {
        // Compact layout for small displays
        drawInputStatus();
    }
    else
    {
        // Full layout for large displays
        drawInputStatus();
        drawJoystickVisualization();
        drawButtonStatus();
        drawInputHistory();
    }

    // Reset button colors after a delay
    if (colorResetTime > 0 && millis() - colorResetTime > 1000)
    {
        resetButtonColors();
        colorResetTime = 0;
    }
}

void InputDemoScreen::loop()
{
    // This demo doesn't need continuous updates in loop()
    // All updates happen in actionTaken() for input responsiveness
}

Screen *InputDemoScreen::actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY)
{
    // Record ALL input details for comprehensive demonstration
    inputState.lastInputTime = millis();
    inputState.totalInputs++;
    inputState.joystickX = offsetX;
    inputState.joystickY = offsetY;

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

    // Force screen refresh to show input feedback immediately
    refresh();

    return nullptr; // Stay on this screen
}

void InputDemoScreen::drawInputStatus()
{
    // Get access to the display
    Adafruit_GFX &display = M1Shield.getGFX();

    int y = isSmallDisplay() ? 25 : 80;

    display.setTextSize(1);
    display.setTextColor(M1Shield.convertColor(0xFFFF)); // White

    // Show last action
    display.setCursor(10, y);
    display.print("Last: ");
    display.setTextColor(getActionColor(static_cast<ActionTaken>(inputState.lastAction.c_str()[0])));
    display.print(inputState.lastAction);

    // Show total inputs
    display.setTextColor(M1Shield.convertColor(0xFFFF));
    display.setCursor(10, y + 15);
    display.print("Total: ");
    display.print(inputState.totalInputs);

    // Show joystick position
    if (inputState.joystickX != 0 || inputState.joystickY != 0)
    {
        display.setCursor(10, y + 30);
        display.print("Joy: ");
        display.print(inputState.joystickX);
        display.print(",");
        display.print(inputState.joystickY);
    }
}

void InputDemoScreen::drawJoystickVisualization()
{
    // Only draw on large displays
    if (isSmallDisplay())
        return;

    // Get access to the display
    Adafruit_GFX &display = M1Shield.getGFX();

    // Draw joystick visualization area
    int centerX = display.width() - 80;
    int centerY = 140;
    int radius = 30;

    // Draw joystick background circle
    display.drawCircle(centerX, centerY, radius, M1Shield.convertColor(0x6318)); // Gray

    // Draw joystick position
    int joyX = centerX + (inputState.joystickX * radius / 100);
    int joyY = centerY + (inputState.joystickY * radius / 100);

    uint16_t joyColor = inputState.joystickPressed ? M1Shield.convertColor(0xF800) : // Red when pressed
                            M1Shield.convertColor(0x07E0);                           // Green when not pressed

    display.fillCircle(joyX, joyY, 5, joyColor);

    // Label
    display.setTextSize(1);
    display.setTextColor(M1Shield.convertColor(0xFFFF));
    display.setCursor(centerX - 25, centerY + radius + 10);
    display.print("Joystick");
}

void InputDemoScreen::drawButtonStatus()
{
    // Only draw on large displays
    if (isSmallDisplay())
        return;

    // Get access to the display
    Adafruit_GFX &display = M1Shield.getGFX();

    // Draw button status indicators
    const char *buttonNames[] = {"MENU", "LEFT", "RIGHT", "UP", "DOWN", "JOY"};
    bool buttonStates[] = {
        inputState.menuPressed,
        inputState.leftPressed,
        inputState.rightPressed,
        inputState.upPressed,
        inputState.downPressed,
        inputState.joystickPressed};

    int startX = 10;
    int startY = 120;

    for (int i = 0; i < 6; i++)
    {
        int x = startX + (i % 3) * 80;
        int y = startY + (i / 3) * 25;

        // Draw button indicator
        uint16_t buttonColor = buttonStates[i] ? buttonTestColors[i] : M1Shield.convertColor(0x4208); // Dark gray when not pressed

        display.fillRect(x, y, 70, 15, buttonColor);
        display.drawRect(x, y, 70, 15, M1Shield.convertColor(0xFFFF)); // White

        // Button label
        display.setTextSize(1);
        display.setTextColor(M1Shield.convertColor(0x0000)); // Black text
        display.setCursor(x + 5, y + 4);
        display.print(buttonNames[i]);
    }
}

void InputDemoScreen::drawInputHistory()
{
    // Only draw on large displays
    if (isSmallDisplay())
        return;

    // Get access to the display
    Adafruit_GFX &display = M1Shield.getGFX();

    // Show input timing
    unsigned long timeSinceLastInput = millis() - inputState.lastInputTime;

    display.setTextSize(1);
    display.setTextColor(M1Shield.convertColor(0xC618)); // Light gray
    display.setCursor(10, display.height() - 40);
    display.print("Last input: ");
    display.print(timeSinceLastInput);
    display.print("ms ago");
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
