/*
 * ComprehensiveScreen.h
 *
 * Main controller for the complete Screen base class demonstration.
 * This coordinates three separate demo screens showing different Screen capabilities.
 */

#ifndef COMPREHENSIVE_SCREEN_H
#define COMPREHENSIVE_SCREEN_H

#include <Arduino.h>
#include <Screen.h>
#include <M1Shield.h>
#include "InputDemoScreen.h"
#include "LifecycleDemoScreen.h"
#include "AnimationDemoScreen.h"

/*
 * ComprehensiveScreen
 *
 * Main controller that demonstrates all Screen capabilities by cycling
 * through three specialized demo screens, each showing different aspects
 * of the Screen base class functionality.
 *
 * This screen acts as both:
 * 1. A demonstration of navigation between multiple screens
 * 2. A controller that manages the three demo sub-screens
 * 3. An example of proper screen lifecycle management
 */
class ComprehensiveScreen : public Screen
{
private:
    // Track which demo we're currently showing
    int currentDemo;

    // Our three demonstration screens
    InputDemoScreen *inputDemo;
    LifecycleDemoScreen *lifecycleDemo;
    AnimationDemoScreen *animationDemo;

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
    Screen *actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY) override;

    // Lifecycle methods that screens can override for advanced behavior
    bool open() override;
    void close() override;

    // Utility methods for demo functionality
    void switchToNextDemo();
    void drawHeader();
    void drawCurrentDemo();
    void updatePerformanceStats();
    const char *getDemoName();
    const char *getDemoDescription();
};

#endif /* COMPREHENSIVE_SCREEN_H */
