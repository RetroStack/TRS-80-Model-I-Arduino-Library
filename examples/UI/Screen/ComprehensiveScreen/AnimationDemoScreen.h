/*
 * AnimationDemoScreen.h
 *
 * Demonstrates real-time updates and performance optimization:
 * - Continuous screen updates in loop()
 * - Frame rate monitoring and optimization
 * - Smooth animations and transitions
 * - Performance best practices
 * - Visual feedback systems
 */

#ifndef ANIMATION_DEMO_SCREEN_H
#define ANIMATION_DEMO_SCREEN_H

#include <Arduino.h>
#include <Screen.h>
#include <M1Shield.h>

class AnimationDemoScreen : public Screen
{
private:
    // Animation state
    struct AnimationState
    {
        float ballX, ballY;
        float ballVelX, ballVelY;
        float ballRadius;
        uint16_t ballColor;
        unsigned long animationStartTime;
        int bounceCount;
        bool animationPaused;
    } animationState;

    // Performance monitoring
    struct PerformanceStats
    {
        unsigned long frameCount;
        unsigned long lastFPSUpdate;
        float currentFPS;
        float maxFPS;
        float minFPS;
        unsigned long frameTime;
        unsigned long maxFrameTime;
    } performanceStats;

    // User controls
    bool showPerformanceStats;
    int animationSpeed;

public:
    AnimationDemoScreen();

    void _drawScreen() override;
    void loop() override;
    Screen *actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY) override;
    bool open() override;
    void close() override;

private:
    void updateAnimation();
    void drawAnimatedBall();
    void drawPerformanceStats();
    void drawControls();
    void updatePerformanceMonitoring();
    void resetAnimation();
    void pauseToggleAnimation();
    void adjustAnimationSpeed(bool increase);
};

#endif /* ANIMATION_DEMO_SCREEN_H */
