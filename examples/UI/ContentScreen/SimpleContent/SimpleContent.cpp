/*
 * SimpleContent.cpp - Basic ContentScreen Implementation
 *
 * Demonstrates the core features of ContentScreen:
 * - Header with title and progress bar
 * - Footer with button hints
 * - Content area with custom display
 * - User feedback systems (alert, confirm, notify)
 * - Basic navigation and interaction
 *
 * Authors: Marcel Erz (RetroStack)
 * License: MIT
 */

#include "SimpleContent.h"
#include <M1Shield.h>

SimpleContent::SimpleContent() : ContentScreen()
{
    // Set the title for this screen
    setTitleF(F("ContentScreen Demo"));

    // Set up footer button labels to guide user interaction
    const char *buttons[] = {"Select", "Progress", "Back"};
    setButtonItems(buttons, 3);

    // Initialize demo state
    currentOption = 0;
    maxOptions = 5; // Updated to match options array in header
    progressValue = 0;
    lastUpdateTime = 0;
    showingDemo = false;
}

bool SimpleContent::open()
{
    Serial.println("SimpleContent::open() - Setting up content screen");

    // Show welcome notification
    notifyF(F("Welcome! Navigate with Up/Down, select with Menu"));

    return true;
}

void SimpleContent::_drawContent()
{
    if (showingDemo)
    {
        drawDemoContent();
    }
    else
    {
        drawOptionsList();
    }
}

void SimpleContent::drawOptionsList()
{
    // Draw title for content area
    drawTextF(10, 10, F("Select an option:"), M1Shield.convertColor(0xFFFF), 1);

    // Draw the options list using the options array from header
    for (int i = 0; i < maxOptions; i++)
    {
        uint16_t y = 35 + (i * 20);
        uint16_t color = (i == currentOption) ? M1Shield.convertColor(0x07E0) : M1Shield.convertColor(0xFFFF);

        // Draw selection indicator
        if (i == currentOption)
        {
            drawText(5, y, ">", color, 1);
        }

        drawText(20, y, options[i], color, 1);
    }

    // Show instructions at bottom
    drawTextF(10, _getContentHeight() - 20, F("Menu: Select | Up/Down: Navigate"), M1Shield.convertColor(0x07FF), 1);
}

void SimpleContent::drawDemoContent()
{
    drawTextF(10, 10, F("Demo Running..."), M1Shield.convertColor(0x07E0), 2);

    String progressText = "Progress: " + String(progressValue) + "%";
    drawText(10, 40, progressText.c_str(), M1Shield.convertColor(0xFFE0), 1);

    // Animate some demo content
    unsigned long currentTime = millis();
    int animationStep = (currentTime / 200) % 4;

    String animText = "Animation: ";
    for (int i = 0; i <= animationStep; i++)
    {
        animText += "*";
    }
    drawText(10, 60, animText.c_str(), M1Shield.convertColor(0xF81F), 1);

    drawTextF(10, 80, F("Press Menu to exit demo"), M1Shield.convertColor(0x07FF), 1);
}

void SimpleContent::loop()
{
    // Update demo animation if running
    if (showingDemo)
    {
        unsigned long currentTime = millis();
        if (currentTime - lastUpdateTime > 1000) // Update every second
        {
            progressValue += 10;
            if (progressValue > 100)
            {
                progressValue = 0;
            }

            setProgressValue(progressValue);
            lastUpdateTime = currentTime;

            // Change button hint when demo is running
            const char *demoButtons[] = {"Exit Demo"};
            setButtonItems(demoButtons, 1);
        }
    }
    else
    {
        // Reset button hints when not in demo
        const char *buttons[] = {"Select", "Progress", "Back"};
        setButtonItems(buttons, 3);
    }
}

Screen *SimpleContent::actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY)
{
    if (!isActive())
    {
        return nullptr;
    }

    if (showingDemo)
    {
        // Exit demo mode
        if (action & BUTTON_MENU)
        {
            showingDemo = false;
            setProgressValue(0);
            notifyF(F("Demo stopped"));
            return nullptr;
        }
    }
    else
    {
        if (action & (BUTTON_UP | JOYSTICK_UP))
        {
            currentOption = (currentOption - 1 + maxOptions) % maxOptions;
            return nullptr;
        }
        else if (action & (BUTTON_DOWN | JOYSTICK_DOWN))
        {
            currentOption = (currentOption + 1) % maxOptions;
            return nullptr;
        }
        else if (action & (BUTTON_MENU | BUTTON_JOYSTICK))
        {
            executeCurrentOption();
            return nullptr;
        }
    }

    return nullptr;
}

bool SimpleContent::executeCurrentOption()
{
    switch (currentOption)
    {
    case 0: // Show Alert
        alertF(F("This is a test alert dialog!"));
        notifyF(F("Alert dialog completed"));
        return true;

    case 1: // Show Confirm
    {
        ConfirmResult result = confirmF(F("Do you want to proceed?"), F("Cancel"), F("OK"));
        if (result == CONFIRM_RIGHT)
        {
            notifyF(F("You selected OK"));
        }
        else
        {
            notifyF(F("You selected Cancel"));
        }
    }
        return true;

    case 2: // Show Notify
        notifyF(F("This is a notification message!"), 5000);
        return true;

    case 3: // Update Progress
    {
        progressValue += 25;
        if (progressValue > 100)
        {
            progressValue = 0;
        }
        setProgressValue(progressValue);
        String msg = "Progress: " + String(progressValue) + "%";
        notify(msg);
    }
        return true;

    case 4: // Toggle Demo
        startDemo();
        return true;

    default:
        return false;
    }
}

void SimpleContent::updateProgress(int progress)
{
    progressValue = progress;
    setProgressValue(progress);
}

void SimpleContent::startDemo()
{
    showingDemo = true;
    progressValue = 0;
    lastUpdateTime = millis();
    notifyF(F("Demo mode started!"));

    setProgressValue(progressValue);
}

void SimpleContent::updateDisplay()
{
    // This method can be used to force screen refresh
    refresh();
}
