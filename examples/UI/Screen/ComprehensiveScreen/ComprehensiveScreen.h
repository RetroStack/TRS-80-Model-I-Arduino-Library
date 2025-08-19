/*
 * ComprehensiveScreen.h
 *
 * Complete demonstration of Screen base class capabilities.
 * This shows all the features available in the Screen class and acts as a menu
 * to switch to individual demo screens.
 */

#ifndef COMPREHENSIVE_SCREEN_H
#define COMPREHENSIVE_SCREEN_H

#include <Arduino.h>
#include <Screen.h>
#include <M1Shield.h>

/*
 * ComprehensiveScreen
 *
 * A menu screen that demonstrates Screen capabilities and allows switching
 * to individual demo screens. This follows the proper pattern of creating
 * new Screen instances when needed, rather than managing them as member variables.
 */
class ComprehensiveScreen : public Screen
{
private:
    // Track which demo we're currently showing
    int currentDemo;

    // Track screen state for lifecycle demonstration
    bool isActive;
    unsigned long activationTime;
    int screenActivations;

    // Performance tracking
    unsigned long lastFrameTime;
    int frameCount;
    float averageFPS;

public:
    ComprehensiveScreen();
    ~ComprehensiveScreen();

    // Core Screen methods that EVERY screen must implement
    void _drawScreen() override;
    void loop() override;
    Screen *actionTaken(ActionTaken action, int8_t offsetX, int8_t offsetY) override;

    // Lifecycle methods that screens can override for advanced behavior
    bool open() override;
    void close() override;

    // Utility methods for demo functionality
    Screen *launchSelectedDemo();
    const char *getDemoName();
    void drawNavigationInfo();
};

#endif /* COMPREHENSIVE_SCREEN_H */
