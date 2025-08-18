/*
 * SimpleContent.h - Basic ContentScreen Implementation
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

#ifndef SIMPLE_CONTENT_H
#define SIMPLE_CONTENT_H

#include <ContentScreen.h>

class SimpleContent : public ContentScreen
{
private:
    // Demo state variables
    int currentOption;
    int maxOptions;
    int progressValue;
    unsigned long lastUpdateTime;
    bool showingDemo;

    // Demo options
    const char *options[5] = {
        "Show Alert",
        "Show Confirm",
        "Show Notify",
        "Update Progress",
        "Toggle Demo"};

public:
    SimpleContent();

    // Override ContentScreen methods
    bool open() override;
    void _drawContent() override;
    Screen *actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY) override;
    void loop() override;

private:
    // Helper methods
    void updateDisplay();
    bool executeCurrentOption();
    void drawOptionsList();
    void drawDemoContent();
    void updateProgress(int progress);
    void startDemo();
};

#endif // SIMPLE_CONTENT_H
