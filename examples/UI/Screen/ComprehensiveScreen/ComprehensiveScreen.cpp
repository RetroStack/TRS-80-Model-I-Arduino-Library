/*
 * ComprehensiveScreen.cpp
 *
 * Implementation of the main controller for the complete Screen base class demonstration.
 * This coordinates three separate demo screens showing different Screen capabilities.
 */

#include "ComprehensiveScreen.h"

ComprehensiveScreen::ComprehensiveScreen()
    : currentDemo(0), isActive(false), activationTime(0), screenActivations(0),
      lastFrameTime(0), frameCount(0), averageFPS(0.0)
{
    // Create our three demo screens
    inputDemo = new InputDemoScreen();
    lifecycleDemo = new LifecycleDemoScreen();
    animationDemo = new AnimationDemoScreen();
}

ComprehensiveScreen::~ComprehensiveScreen()
{
    // Proper cleanup - important for embedded systems!
    delete inputDemo;
    delete lifecycleDemo;
    delete animationDemo;
}

bool ComprehensiveScreen::open()
{
    // Screen lifecycle demonstration
    isActive = true;
    activationTime = millis();
    screenActivations++;

    Serial.println("=== ComprehensiveScreen::open() ===");
    Serial.print("Screen activation #");
    Serial.println(screenActivations);
    Serial.print("Opened at: ");
    Serial.print(activationTime);
    Serial.println("ms");

    // Open the current demo screen
    switch (currentDemo)
    {
    case 0:
        inputDemo->open();
        break;
    case 1:
        lifecycleDemo->open();
        break;
    case 2:
        animationDemo->open();
        break;
    }

    return true;
}

void ComprehensiveScreen::close()
{
    // Screen lifecycle demonstration
    isActive = false;
    unsigned long closeTime = millis();
    unsigned long sessionDuration = closeTime - activationTime;

    Serial.println("=== ComprehensiveScreen::close() ===");
    Serial.print("Session duration: ");
    Serial.print(sessionDuration);
    Serial.println("ms");
    Serial.print("Average FPS during session: ");
    Serial.println(averageFPS);

    // Close the current demo screen
    switch (currentDemo)
    {
    case 0:
        inputDemo->close();
        break;
    case 1:
        lifecycleDemo->close();
        break;
    case 2:
        animationDemo->close();
        break;
    }
}

void ComprehensiveScreen::_drawScreen()
{
    // Get access to the display
    Adafruit_GFX &display = M1Shield.getGFX();

    // Clear screen with adaptive background color
    uint16_t bgColor = isSmallDisplay() ? M1Shield.convertColor(0x0008) : // Dark blue for small displays
                           M1Shield.convertColor(0x0010);                 // Brighter blue for large displays

    display.fillScreen(bgColor);

    // Draw common header
    drawHeader();

    // Draw the current demo
    drawCurrentDemo();

    updatePerformanceStats();
}

void ComprehensiveScreen::loop()
{
    // Delegate to current demo screen
    switch (currentDemo)
    {
    case 0:
        inputDemo->loop();
        break;
    case 1:
        lifecycleDemo->loop();
        break;
    case 2:
        animationDemo->loop();
        break;
    }

    // Update our own performance monitoring
    frameCount++;
}

Screen *ComprehensiveScreen::actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY)
{
    // Handle navigation between demos
    if (action & BUTTON_MENU)
    {
        switchToNextDemo();
        return nullptr; // Stay on this screen
    }

    // Delegate all other actions to current demo
    switch (currentDemo)
    {
    case 0:
        inputDemo->actionTaken(action, offsetX, offsetY);
        break;
    case 1:
        lifecycleDemo->actionTaken(action, offsetX, offsetY);
        break;
    case 2:
        animationDemo->actionTaken(action, offsetX, offsetY);
        break;
    }

    return nullptr; // Stay on this screen
}

void ComprehensiveScreen::switchToNextDemo()
{
    // Close current demo
    switch (currentDemo)
    {
    case 0:
        inputDemo->close();
        break;
    case 1:
        lifecycleDemo->close();
        break;
    case 2:
        animationDemo->close();
        break;
    }

    // Move to next demo
    currentDemo = (currentDemo + 1) % 3;

    Serial.println("=== SWITCHING DEMO ===");
    Serial.print("New demo: ");
    Serial.print(currentDemo);
    Serial.print(" - ");
    Serial.println(getDemoName());

    // Open new demo
    switch (currentDemo)
    {
    case 0:
        inputDemo->open();
        break;
    case 1:
        lifecycleDemo->open();
        break;
    case 2:
        animationDemo->open();
        break;
    }

    // Force screen redraw
    refresh();
}

void ComprehensiveScreen::drawHeader()
{
    // Get access to the display
    Adafruit_GFX &display = M1Shield.getGFX();

    // Adaptive header for different display sizes
    if (isSmallDisplay())
    {
        // Compact header for small displays (128x64 OLED)
        display.setTextColor(M1Shield.convertColor(0xFFE0)); // Yellow
        display.setTextSize(1);
        display.setCursor(0, 0);
        display.print("Screen Demo ");
        display.print(currentDemo + 1);
        display.print("/3");

        // Demo name on second line
        display.setCursor(0, 10);
        display.setTextColor(M1Shield.convertColor(0xFFFF)); // White
        display.print(getDemoName());
    }
    else
    {
        // Full header for large displays
        display.setTextColor(M1Shield.convertColor(0xFFE0)); // Yellow
        display.setTextSize(2);
        display.setCursor(10, 10);
        display.print("Screen Complete Demo");

        // Demo indicator
        display.setTextSize(1);
        display.setCursor(10, 35);
        display.setTextColor(M1Shield.convertColor(0x07FF)); // Cyan
        display.print("Demo ");
        display.print(currentDemo + 1);
        display.print(" of 3: ");
        display.setTextColor(M1Shield.convertColor(0xFFFF)); // White
        display.print(getDemoName());

        // Description
        display.setCursor(10, 50);
        display.setTextColor(M1Shield.convertColor(0xC618)); // Light gray
        display.print(getDemoDescription());

        // Navigation hint
        display.setCursor(10, display.height() - 20);
        display.setTextColor(M1Shield.convertColor(0xFFE0)); // Yellow
        display.print("MENU: Next Demo");
    }
}

void ComprehensiveScreen::drawCurrentDemo()
{
    // Set viewport for demo content (below header)
    int contentY = isSmallDisplay() ? 25 : 75;

    // Draw the active demo screen content
    switch (currentDemo)
    {
    case 0:
        inputDemo->_drawScreen();
        break;
    case 1:
        lifecycleDemo->_drawScreen();
        break;
    case 2:
        animationDemo->_drawScreen();
        break;
    }
}

void ComprehensiveScreen::updatePerformanceStats()
{
    unsigned long currentTime = millis();
    if (lastFrameTime == 0)
    {
        lastFrameTime = currentTime;
        return;
    }

    // Calculate FPS every second
    if (currentTime - lastFrameTime >= 1000)
    {
        averageFPS = frameCount * 1000.0 / (currentTime - lastFrameTime);
        lastFrameTime = currentTime;
        frameCount = 0;
    }
}

const char *ComprehensiveScreen::getDemoName()
{
    switch (currentDemo)
    {
    case 0:
        return "Input Demo";
    case 1:
        return "Lifecycle Demo";
    case 2:
        return "Animation Demo";
    default:
        return "Unknown";
    }
}

const char *ComprehensiveScreen::getDemoDescription()
{
    switch (currentDemo)
    {
    case 0:
        return "Test all buttons and joystick input";
    case 1:
        return "Screen lifecycle and state management";
    case 2:
        return "Real-time updates and performance";
    default:
        return "";
    }
}
