/*
 * ComprehensiveConsole.h - Advanced ConsoleScreen Implementation
 *
 * Demonstrates ALL ConsoleScreen features:
 * - Multiple paging modes
 * - Auto-forward functionality
 * - One-time execution
 * - Complex formatting
 * - Interactive controls
 * - Performance monitoring
 * - Data logging simulation
 * - Color coding systems
 */

#ifndef COMPREHENSIVE_CONSOLE_H
#define COMPREHENSIVE_CONSOLE_H

#include <ConsoleScreen.h>

enum ConsoleMode
{
    MODE_WELCOME,
    MODE_SYSTEM_LOG,
    MODE_DATA_STREAM,
    MODE_INTERACTIVE,
    MODE_PAGING_DEMO,
    MODE_PERFORMANCE,
    MODE_COLOR_DEMO,
    MODE_COUNT
};

class ComprehensiveConsole : public ConsoleScreen
{
private:
    // State management
    ConsoleMode currentMode;
    unsigned long lastUpdate;
    unsigned long modeStartTime;
    int messageCount;
    int dataStreamCount;
    bool autoMode;

    // Performance tracking
    unsigned long startTime;
    int totalMessages;
    float averageMessageRate;

    // Data simulation
    float sensorData[5];
    int logLevel;

    // Paging demonstration
    bool pagingDemoActive;
    int pagingStep;

public:
    ComprehensiveConsole();

    // Override ConsoleScreen methods
    void loop() override;
    Screen *actionTaken(ActionTaken action, int8_t offsetX, int8_t offsetY) override;

protected:
    // Override one-time execution
    void _executeOnce() override;

private:
    // Mode-specific methods
    void switchToMode(ConsoleMode newMode);
    void runWelcomeMode();
    void runSystemLogMode();
    void runDataStreamMode();
    void runInteractiveMode();
    void runPagingDemo();
    void runPerformanceMode();
    void runColorDemo();

    // Helper methods
    void showModeMenu();
    void showControls();
    void updateSensorData();
    void addLogMessage(int level, const __FlashStringHelper *message);
    void addTimestamp();
    void showSystemInfo();
    void demonstratePagingModes();
    void showColorPalette();
    void addFormattedData();
    void showPerformanceStats();
};

#endif // COMPREHENSIVE_CONSOLE_H
