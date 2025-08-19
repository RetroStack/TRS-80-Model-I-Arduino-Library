/*
 * LifecycleDemoScreen.h
 *
 * Demonstrates Screen lifecycle management and state tracking:
 * - open() and close() method usage
 * - Active/inactive state management
 * - Screen transition handling
 * - Resource management and cleanup
 * - State persistence between activations
 */

#ifndef LIFECYCLE_DEMO_SCREEN_H
#define LIFECYCLE_DEMO_SCREEN_H

#include <Arduino.h>
#include <Screen.h>
#include <M1Shield.h>

class LifecycleDemoScreen : public Screen
{
private:
    // Lifecycle tracking
    struct LifecycleState
    {
        unsigned long openTime;
        unsigned long closeTime;
        unsigned long totalActiveTime;
        int openCount;
        int closeCount;
        bool isCurrentlyOpen;
        unsigned long lastStateChange;
        String currentState;
        bool needsFullRedraw;
        unsigned long lastDisplayUpdate;
    } lifecycleState;

    // Resource usage simulation
    int simulatedMemoryUsage;
    bool resourcesAllocated;
    unsigned long resourceAllocationTime;

public:
    LifecycleDemoScreen();

    void _drawScreen() override;
    void loop() override;
    Screen *actionTaken(ActionTaken action, int8_t offsetX, int8_t offsetY) override;
    bool open() override;
    void close() override;

private:
    void drawLifecycleStatus();
    void drawResourceUsage();
    void drawStateHistory();
    void updateLifecycleDisplay();
    void simulateResourceAllocation();
    void simulateResourceCleanup();
    void updateLifecycleStats();
};

#endif /* LIFECYCLE_DEMO_SCREEN_H */
