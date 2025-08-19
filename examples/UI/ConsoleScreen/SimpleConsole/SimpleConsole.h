/*
 * SimpleConsole.h - Basic ConsoleScreen Implementation
 *
 * Demonstrates fundamental ConsoleScreen features:
 * - Text output with colors
 * - Automatic scrolling
 * - Basic user interaction
 * - Simple status updates
 */

#ifndef SIMPLE_CONSOLE_H
#define SIMPLE_CONSOLE_H

#include <ConsoleScreen.h>

class SimpleConsole : public ConsoleScreen
{
private:
    unsigned long lastUpdate;
    int messageCount;
    bool showingWelcome;

public:
    SimpleConsole();

    // Override ConsoleScreen methods
    void loop() override;
    Screen *actionTaken(ActionTaken action, int8_t offsetX, int8_t offsetY) override;

private:
    // Helper methods
    void showWelcomeMessage();
    void addStatusMessage();
    void showControls();
};

#endif // SIMPLE_CONSOLE_H
