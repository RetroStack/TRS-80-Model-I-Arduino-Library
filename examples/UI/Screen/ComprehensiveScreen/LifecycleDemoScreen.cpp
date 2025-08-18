/*
 * LifecycleDemoScreen.cpp
 *
 * Implementation of Screen lifecycle management demonstration.
 */

#include "LifecycleDemoScreen.h"

LifecycleDemoScreen::LifecycleDemoScreen()
    : simulatedMemoryUsage(0), resourcesAllocated(false), resourceAllocationTime(0)
{
    // Initialize lifecycle state
    memset(&lifecycleState, 0, sizeof(lifecycleState));
    lifecycleState.currentState = "Created";
}

bool LifecycleDemoScreen::open()
{
    lifecycleState.openTime = millis();
    lifecycleState.openCount++;
    lifecycleState.isCurrentlyOpen = true;
    lifecycleState.lastStateChange = lifecycleState.openTime;
    lifecycleState.currentState = "Open";

    Serial.println("=== LifecycleDemoScreen::open() ===");
    Serial.print("Open count: ");
    Serial.println(lifecycleState.openCount);
    Serial.print("Opened at: ");
    Serial.print(lifecycleState.openTime);
    Serial.println("ms");

    // Simulate resource allocation
    simulateResourceAllocation();

    return true;
}

void LifecycleDemoScreen::close()
{
    lifecycleState.closeTime = millis();
    lifecycleState.closeCount++;
    lifecycleState.isCurrentlyOpen = false;
    lifecycleState.lastStateChange = lifecycleState.closeTime;
    lifecycleState.currentState = "Closed";

    // Update total active time
    if (lifecycleState.openTime > 0)
    {
        lifecycleState.totalActiveTime += (lifecycleState.closeTime - lifecycleState.openTime);
    }

    Serial.println("=== LifecycleDemoScreen::close() ===");
    Serial.print("Close count: ");
    Serial.println(lifecycleState.closeCount);
    Serial.print("Session duration: ");
    Serial.print(lifecycleState.closeTime - lifecycleState.openTime);
    Serial.println("ms");
    Serial.print("Total active time: ");
    Serial.print(lifecycleState.totalActiveTime);
    Serial.println("ms");

    // Simulate resource cleanup
    simulateResourceCleanup();
}

void LifecycleDemoScreen::_drawScreen()
{
    int startY = isSmallDisplay() ? 25 : 80;

    drawLifecycleStatus();

    if (!isSmallDisplay())
    {
        drawResourceUsage();
        drawStateHistory();
    }

    updateLifecycleStats();
}

void LifecycleDemoScreen::loop()
{
    // Update lifecycle statistics
    updateLifecycleStats();

    // Simulate some background processing
    if (resourcesAllocated)
    {
        simulatedMemoryUsage = (simulatedMemoryUsage + 1) % 1000;
    }
}

Screen *LifecycleDemoScreen::actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY)
{
    Serial.println("=== LifecycleDemoScreen Input ===");
    Serial.print("Action while state is: ");
    Serial.println(lifecycleState.currentState);
    Serial.print("Resources allocated: ");
    Serial.println(resourcesAllocated ? "Yes" : "No");

    // Demonstrate state-dependent behavior
    if (action & BUTTON_UP)
    {
        // Simulate increasing resource usage
        if (resourcesAllocated)
        {
            simulatedMemoryUsage += 100;
            if (simulatedMemoryUsage > 2000)
                simulatedMemoryUsage = 2000;
        }
    }

    if (action & BUTTON_DOWN)
    {
        // Simulate decreasing resource usage
        if (resourcesAllocated)
        {
            simulatedMemoryUsage -= 100;
            if (simulatedMemoryUsage < 0)
                simulatedMemoryUsage = 0;
        }
    }

    if (action & BUTTON_LEFT)
    {
        // Force resource cleanup and reallocation
        simulateResourceCleanup();
        delay(100); // Simulate cleanup time
        simulateResourceAllocation();
    }

    if (action & BUTTON_RIGHT)
    {
        // Toggle resource allocation
        if (resourcesAllocated)
        {
            simulateResourceCleanup();
        }
        else
        {
            simulateResourceAllocation();
        }
    }

    refresh();

    return nullptr; // Stay on this screen
}

void LifecycleDemoScreen::drawLifecycleStatus()
{
    // Get access to the display
    Adafruit_GFX &display = M1Shield.getGFX();

    int y = isSmallDisplay() ? 25 : 80;

    display.setTextSize(1);
    display.setTextColor(M1Shield.convertColor(0xFFFF));

    // Current state
    display.setCursor(10, y);
    display.print("State: ");
    uint16_t stateColor = lifecycleState.isCurrentlyOpen ? M1Shield.convertColor(0x07E0) : // Green for open
                              M1Shield.convertColor(0xF800);                               // Red for closed
    display.setTextColor(stateColor);
    display.print(lifecycleState.currentState);

    // Open/close counts
    display.setTextColor(M1Shield.convertColor(0xFFFF));
    display.setCursor(10, y + 15);
    display.print("Opens: ");
    display.print(lifecycleState.openCount);
    display.print(" Closes: ");
    display.print(lifecycleState.closeCount);

    // Total active time
    display.setCursor(10, y + 30);
    display.print("Active: ");
    display.print(lifecycleState.totalActiveTime);
    display.print("ms");

    // Current session time
    if (lifecycleState.isCurrentlyOpen)
    {
        unsigned long currentSessionTime = millis() - lifecycleState.openTime;
        display.setCursor(10, y + 45);
        display.print("Session: ");
        display.print(currentSessionTime);
        display.print("ms");
    }
}

void LifecycleDemoScreen::drawResourceUsage()
{
    // Only draw on large displays
    if (isSmallDisplay())
        return;

    // Get access to the display
    Adafruit_GFX &display = M1Shield.getGFX();

    int y = 150;

    display.setTextSize(1);
    display.setTextColor(M1Shield.convertColor(0xFFFF));

    // Resource allocation status
    display.setCursor(10, y);
    display.print("Resources: ");
    uint16_t resourceColor = resourcesAllocated ? M1Shield.convertColor(0x07E0) : // Green for allocated
                                 M1Shield.convertColor(0xF800);                   // Red for not allocated
    display.setTextColor(resourceColor);
    display.print(resourcesAllocated ? "Allocated" : "Not Allocated");

    // Simulated memory usage
    if (resourcesAllocated)
    {
        display.setTextColor(M1Shield.convertColor(0xFFFF));
        display.setCursor(10, y + 15);
        display.print("Memory: ");
        display.print(simulatedMemoryUsage);
        display.print(" bytes");

        // Memory usage bar
        int barWidth = (simulatedMemoryUsage * 200) / 2000;
        display.drawRect(10, y + 30, 200, 10, M1Shield.convertColor(0xFFFF));
        display.fillRect(10, y + 30, barWidth, 10, M1Shield.convertColor(0x07E0));
    }
}

void LifecycleDemoScreen::drawStateHistory()
{
    // Only draw on large displays
    if (isSmallDisplay())
        return;

    // Get access to the display
    Adafruit_GFX &display = M1Shield.getGFX();

    // Controls hint
    display.setTextSize(1);
    display.setTextColor(M1Shield.convertColor(0xFFE0));
    display.setCursor(10, display.height() - 40);
    display.print("UP/DOWN: Memory  LEFT: Reset  RIGHT: Toggle");
}

void LifecycleDemoScreen::simulateResourceAllocation()
{
    if (!resourcesAllocated)
    {
        resourcesAllocated = true;
        resourceAllocationTime = millis();
        simulatedMemoryUsage = 500; // Start with some base usage

        Serial.println("Resources allocated!");
        Serial.print("Initial memory usage: ");
        Serial.print(simulatedMemoryUsage);
        Serial.println(" bytes");
    }
}

void LifecycleDemoScreen::simulateResourceCleanup()
{
    if (resourcesAllocated)
    {
        resourcesAllocated = false;
        unsigned long allocationDuration = millis() - resourceAllocationTime;
        simulatedMemoryUsage = 0;

        Serial.println("Resources cleaned up!");
        Serial.print("Allocation duration: ");
        Serial.print(allocationDuration);
        Serial.println("ms");
    }
}

void LifecycleDemoScreen::updateLifecycleStats()
{
    // Update current session time if open
    if (lifecycleState.isCurrentlyOpen)
    {
        unsigned long currentTime = millis();
        // Update stats every second to avoid too frequent updates
        static unsigned long lastUpdate = 0;
        if (currentTime - lastUpdate > 1000)
        {
            lastUpdate = currentTime;
            // Could update average session time, etc.
        }
    }
}
