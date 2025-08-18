/*
 * AnimationDemoScreen.cpp
 *
 * Implementation of real-time animation and performance demonstration.
 */

#include "AnimationDemoScreen.h"

AnimationDemoScreen::AnimationDemoScreen()
    : showPerformanceStats(true), animationSpeed(1)
{
    // Initialize animation state
    animationState.ballX = 50.0;
    animationState.ballY = 50.0;
    animationState.ballVelX = 2.0;
    animationState.ballVelY = 1.5;
    animationState.ballRadius = 8.0;
    animationState.ballColor = M1Shield.convertColor(0xFFFF00); // Yellow
    animationState.bounceCount = 0;
    animationState.animationPaused = false;

    // Initialize performance stats
    memset(&performanceStats, 0, sizeof(performanceStats));
    performanceStats.minFPS = 999.0;
}

bool AnimationDemoScreen::open()
{
    Serial.println("=== AnimationDemoScreen::open() ===");
    Serial.println("Starting animation and performance monitoring!");

    resetAnimation();
    performanceStats.frameCount = 0;
    performanceStats.lastFPSUpdate = millis();

    return true;
}

void AnimationDemoScreen::close()
{
    Serial.println("=== AnimationDemoScreen::close() ===");
    Serial.print("Final performance stats:");
    Serial.print(" FPS: ");
    Serial.print(performanceStats.currentFPS);
    Serial.print(" Max: ");
    Serial.print(performanceStats.maxFPS);
    Serial.print(" Min: ");
    Serial.print(performanceStats.minFPS);
    Serial.print(" Bounces: ");
    Serial.println(animationState.bounceCount);
}

void AnimationDemoScreen::_drawScreen()
{
    // Get access to the display
    Adafruit_GFX &display = M1Shield.getGFX();

    // Clear animation area (not the whole screen to preserve header)
    int startY = isSmallDisplay() ? 25 : 80;
    int animHeight = display.height() - startY - 40;

    display.fillRect(0, startY, display.width(), animHeight,
                     M1Shield.convertColor(0x0000)); // Black background

    // Draw animated ball
    if (!animationState.animationPaused)
    {
        drawAnimatedBall();
    }

    // Draw performance stats
    if (showPerformanceStats)
    {
        drawPerformanceStats();
    }

    // Draw controls
    drawControls();
}

void AnimationDemoScreen::loop()
{
    // This is where continuous animation happens!
    updateAnimation();
    updatePerformanceMonitoring();

    // Force screen refresh for animation
    if (!animationState.animationPaused)
    {
        refresh();
    }
}

Screen *AnimationDemoScreen::actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY)
{
    Serial.println("=== AnimationDemoScreen Input ===");
    Serial.print("Action during animation: ");
    Serial.println(action);

    if (action & BUTTON_UP)
    {
        adjustAnimationSpeed(true);
    }

    if (action & BUTTON_DOWN)
    {
        adjustAnimationSpeed(false);
    }

    if (action & BUTTON_LEFT)
    {
        showPerformanceStats = !showPerformanceStats;
        Serial.print("Performance stats: ");
        Serial.println(showPerformanceStats ? "ON" : "OFF");
    }

    if (action & BUTTON_RIGHT)
    {
        resetAnimation();
    }

    if (action & BUTTON_JOYSTICK)
    {
        pauseToggleAnimation();
    }

    refresh();

    return nullptr; // Stay on this screen
}

void AnimationDemoScreen::updateAnimation()
{
    if (animationState.animationPaused)
        return;

    // Get access to the display
    Adafruit_GFX &display = M1Shield.getGFX();

    // Update ball position
    animationState.ballX += animationState.ballVelX * animationSpeed;
    animationState.ballY += animationState.ballVelY * animationSpeed;

    // Get animation bounds
    int startY = isSmallDisplay() ? 25 : 80;
    int endY = display.height() - 40;

    // Bounce off walls
    bool bounced = false;

    if (animationState.ballX <= animationState.ballRadius ||
        animationState.ballX >= display.width() - animationState.ballRadius)
    {
        animationState.ballVelX = -animationState.ballVelX;
        bounced = true;
    }

    if (animationState.ballY <= startY + animationState.ballRadius ||
        animationState.ballY >= endY - animationState.ballRadius)
    {
        animationState.ballVelY = -animationState.ballVelY;
        bounced = true;
    }

    if (bounced)
    {
        animationState.bounceCount++;
        // Change color on bounce
        animationState.ballColor = M1Shield.convertColor(
            (random(128, 255) << 11) | (random(128, 255) << 5) | random(128, 255));
    }

    // Keep ball in bounds
    animationState.ballX = constrain(animationState.ballX, animationState.ballRadius,
                                     display.width() - animationState.ballRadius);
    animationState.ballY = constrain(animationState.ballY, startY + animationState.ballRadius,
                                     endY - animationState.ballRadius);
}

void AnimationDemoScreen::drawAnimatedBall()
{
    // Get access to the display
    Adafruit_GFX &display = M1Shield.getGFX();

    display.fillCircle((int)animationState.ballX, (int)animationState.ballY,
                       (int)animationState.ballRadius, animationState.ballColor);

    // Draw trail effect for better visibility
    display.drawCircle((int)animationState.ballX, (int)animationState.ballY,
                       (int)animationState.ballRadius + 1,
                       M1Shield.convertColor(0x4208)); // Dark gray
}

void AnimationDemoScreen::drawPerformanceStats()
{
    // Get access to the display
    Adafruit_GFX &display = M1Shield.getGFX();

    int y = isSmallDisplay() ? display.height() - 25 : display.height() - 35;

    display.setTextSize(1);
    display.setTextColor(M1Shield.convertColor(0xFFFF));

    // Current FPS
    display.setCursor(10, y);
    display.print("FPS: ");
    display.print(performanceStats.currentFPS, 1);

    if (!isSmallDisplay())
    {
        // Max/Min FPS
        display.setCursor(80, y);
        display.print("Max: ");
        display.print(performanceStats.maxFPS, 1);

        display.setCursor(150, y);
        display.print("Min: ");
        display.print(performanceStats.minFPS, 1);

        // Bounce count
        display.setCursor(220, y);
        display.print("Bounces: ");
        display.print(animationState.bounceCount);
    }
}

void AnimationDemoScreen::drawControls()
{
    // Get access to the display
    Adafruit_GFX &display = M1Shield.getGFX();

    int y = display.height() - 15;

    display.setTextSize(1);
    display.setTextColor(M1Shield.convertColor(0xFFE0)); // Yellow

    if (isSmallDisplay())
    {
        display.setCursor(10, y);
        display.print("Joy: Pause Speed: ");
        display.print(animationSpeed);
    }
    else
    {
        display.setCursor(10, y);
        display.print("UP/DOWN: Speed  LEFT: Stats  RIGHT: Reset  JOY: ");
        display.print(animationState.animationPaused ? "Resume" : "Pause");
        display.print("  Speed: ");
        display.print(animationSpeed);
    }
}

void AnimationDemoScreen::updatePerformanceMonitoring()
{
    unsigned long currentTime = millis();
    performanceStats.frameCount++;

    // Calculate FPS every 500ms
    if (currentTime - performanceStats.lastFPSUpdate >= 500)
    {
        performanceStats.currentFPS = performanceStats.frameCount * 1000.0 /
                                      (currentTime - performanceStats.lastFPSUpdate);

        // Update min/max
        if (performanceStats.currentFPS > performanceStats.maxFPS)
        {
            performanceStats.maxFPS = performanceStats.currentFPS;
        }
        if (performanceStats.currentFPS < performanceStats.minFPS && performanceStats.currentFPS > 0)
        {
            performanceStats.minFPS = performanceStats.currentFPS;
        }

        performanceStats.frameCount = 0;
        performanceStats.lastFPSUpdate = currentTime;
    }
}

void AnimationDemoScreen::resetAnimation()
{
    // Get access to the display
    Adafruit_GFX &display = M1Shield.getGFX();

    animationState.ballX = display.width() / 2.0;
    animationState.ballY = (isSmallDisplay() ? 40 : 120);
    animationState.ballVelX = 2.0 * animationSpeed;
    animationState.ballVelY = 1.5 * animationSpeed;
    animationState.bounceCount = 0;
    animationState.ballColor = M1Shield.convertColor(0xFFE0); // Yellow
    animationState.animationPaused = false;

    // Reset performance stats
    performanceStats.frameCount = 0;
    performanceStats.maxFPS = 0;
    performanceStats.minFPS = 999.0;
    performanceStats.lastFPSUpdate = millis();

    Serial.println("Animation reset!");
}

void AnimationDemoScreen::pauseToggleAnimation()
{
    animationState.animationPaused = !animationState.animationPaused;
    Serial.print("Animation ");
    Serial.println(animationState.animationPaused ? "PAUSED" : "RESUMED");
}

void AnimationDemoScreen::adjustAnimationSpeed(bool increase)
{
    if (increase)
    {
        animationSpeed++;
        if (animationSpeed > 5)
            animationSpeed = 5;
    }
    else
    {
        animationSpeed--;
        if (animationSpeed < 1)
            animationSpeed = 1;
    }

    // Update velocity to match new speed
    float speedMultiplier = animationSpeed / 1.0;
    animationState.ballVelX = (animationState.ballVelX > 0 ? 2.0 : -2.0) * speedMultiplier;
    animationState.ballVelY = (animationState.ballVelY > 0 ? 1.5 : -1.5) * speedMultiplier;

    Serial.print("Animation speed: ");
    Serial.println(animationSpeed);
}
