/*
 * ComprehensiveScreen.cpp
 *
 * Complete demonstration of Screen base class capabilities.
 * This shows all the features available in the Screen class.
 */

#include "ComprehensiveScreen.h"
#include "InputDemoScreen.h"
#include "LifecycleDemoScreen.h"
#include "AnimationDemoScreen.h"
#include <math.h>

ComprehensiveScreen::ComprehensiveScreen()
    : currentDemo(0), isActive(false), activationTime(0), screenActivations(0),
      lastFrameTime(0), frameCount(0), averageFPS(0.0)
{
    // Set the main screen title
    setTitle("Comprehensive Screen Demo");
    Serial.println("ComprehensiveScreen constructor called");
}

ComprehensiveScreen::~ComprehensiveScreen()
{
    // Clean screen lifecycle demonstration
}

bool ComprehensiveScreen::open()
{
    Serial.println("=== ComprehensiveScreen::open() called ===");

    // Call base class open() first to set _active and trigger initial draw
    bool result = Screen::open();
    Serial.print("Base Screen::open() returned: ");
    Serial.println(result);

    isActive = true;
    activationTime = millis();
    screenActivations++;

    // Reset performance tracking
    frameCount = 0;
    lastFrameTime = 0;
    averageFPS = 0.0;

    if (getLogger())
    {
        getLogger()->infoF(F("Screen activated - Count: %d"), screenActivations);
    }

    Serial.print("Screen activations: ");
    Serial.println(screenActivations);
    Serial.print("Activation time: ");
    Serial.println(activationTime);

    // Note: Don't call switchToNextDemo() here since base open() already calls _drawScreen()
    // The screen will start with demo 0 (Input Demo) as initialized
    Serial.println("ComprehensiveScreen::open() completed");

    return result;
}
void ComprehensiveScreen::close()
{
    Serial.println("=== ComprehensiveScreen::close() called ===");
    isActive = false;
    unsigned long duration = millis() - activationTime;

    if (getLogger())
    {
        getLogger()->infoF(F("Screen deactivated after %lu ms"), duration);
    }

    // Call base class close() to set _active = false
    Screen::close();
    Serial.println("ComprehensiveScreen::close() completed");
}

void ComprehensiveScreen::_drawScreen()
{
    Serial.println("=== ComprehensiveScreen::_drawScreen() called ===");

    // Get access to the display
    Adafruit_GFX &display = M1Shield.getGFX();
    Serial.println("Got display reference");

    // Clear the display
    display.fillScreen(M1Shield.convertColor(0x0000));
    display.setTextColor(M1Shield.convertColor(0xFFFF));
    Serial.println("Display cleared and text color set");

    // Draw title
    display.setTextSize(2);
    display.setCursor(10, 10);
    display.print("Screen Demos");
    Serial.println("Title drawn");

    // Draw menu options
    display.setTextSize(1);
    display.setCursor(10, 40);
    display.print("Available demos:");

    // Show current demo option highlighted
    const char *demos[] = {"Input Demo", "Lifecycle Demo", "Animation Demo"};

    for (int i = 0; i < 3; i++)
    {
        display.setCursor(20, 60 + (i * 15));

        if (i == currentDemo)
        {
            display.print(">> ");
            display.print(demos[i]);
            display.print(" <<");
        }
        else
        {
            display.print("   ");
            display.print(demos[i]);
        }
    }

    // Instructions
    display.setCursor(10, 120);
    display.print("MENU: Launch selected demo");
    display.setCursor(10, 135);
    display.print("UP/DOWN: Navigate");

    // Show statistics
    char buffer[30];
    sprintf(buffer, "Activations: %d", screenActivations);
    display.setCursor(10, 160);
    display.print(buffer);

    Serial.println("=== _drawScreen() completed ===");
}

void ComprehensiveScreen::loop()
{
    // This is a simple menu screen, so no continuous updates needed
    // Individual demo screens will handle their own loop() requirements
}

Screen *ComprehensiveScreen::actionTaken(ActionTaken action, int8_t offsetX, int8_t offsetY)
{
    Serial.print("=== actionTaken called with action: 0x");
    Serial.print(action, HEX);
    Serial.print(", offset: ");
    Serial.print(offsetX);
    Serial.print(",");
    Serial.println(offsetY);

    // Handle menu navigation
    if (action & BUTTON_UP)
    {
        Serial.println("UP button - previous demo");
        currentDemo = (currentDemo + 2) % 3; // Previous (with wrap-around)
        refresh();
        return nullptr;
    }

    if (action & BUTTON_DOWN)
    {
        Serial.println("DOWN button - next demo");
        currentDemo = (currentDemo + 1) % 3; // Next
        refresh();
        return nullptr;
    }

    // Launch selected demo
    if (action & BUTTON_MENU)
    {
        Serial.println("MENU button detected - launching selected demo");
        return launchSelectedDemo();
    }

    // Log other input for demonstration
    if (getLogger())
    {
        if (action & BUTTON_LEFT)
        {
            getLogger()->infoF(F("LEFT button pressed on menu"));
            Serial.println("LEFT button logged");
        }
        if (action & BUTTON_RIGHT)
        {
            getLogger()->infoF(F("RIGHT button pressed on menu"));
            Serial.println("RIGHT button logged");
        }
        if (action & BUTTON_JOYSTICK)
        {
            getLogger()->infoF(F("Joystick pressed at: %d,%d"), offsetX, offsetY);
            Serial.println("Joystick press logged");
        }
    }

    Serial.println("actionTaken returning nullptr");
    return nullptr; // Stay on this screen
}

Screen *ComprehensiveScreen::launchSelectedDemo()
{
    Serial.println("=== launchSelectedDemo() called ===");
    Serial.print("Selected demo: ");
    Serial.println(currentDemo);

    if (getLogger())
    {
        getLogger()->infoF(F("Launching demo %d: %s"), currentDemo, getDemoName());
    }

    Serial.print("Demo name: ");
    Serial.println(getDemoName());

    // Create and return new screen instance based on selected demo
    Screen *newScreen = nullptr;
    switch (currentDemo)
    {
    case 0:
        Serial.println("Creating new InputDemoScreen");
        newScreen = new InputDemoScreen();
        break;
    case 1:
        Serial.println("Creating new LifecycleDemoScreen");
        newScreen = new LifecycleDemoScreen();
        break;
    case 2:
        Serial.println("Creating new AnimationDemoScreen");
        newScreen = new AnimationDemoScreen();
        break;
    default:
        Serial.println("Unknown demo number - staying on current screen");
        return nullptr;
    }

    Serial.print("Created new screen instance: ");
    Serial.println((unsigned long)newScreen, HEX);
    Serial.println("=== launchSelectedDemo() returning new screen ===");
    return newScreen;
}

void ComprehensiveScreen::drawNavigationInfo()
{
    // This is called from draw() after each demo to show consistent navigation
    // Currently each demo shows its own navigation, so this can be minimal
    // or used for common status information
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