/*
 * ComprehensiveContent.h - Advanced ContentScreen Implementation
 *
 * Demonstrates ALL ContentScreen features:
 * - Multiple content modes and layouts
 * - Advanced user feedback systems
 * - Real-time data visualization
 * - Custom graphics and charts
 * - Dynamic progress tracking
 * - Complex state management
 * - Performance monitoring
 * - Interactive elements
 *
 * Authors: Marcel Erz (RetroStack)
 * License: MIT
 */

#ifndef COMPREHENSIVE_CONTENT_H
#define COMPREHENSIVE_CONTENT_H

#include <ContentScreen.h>

enum ContentMode
{
    MODE_MAIN_MENU,
    MODE_DATA_DISPLAY,
    MODE_CHART_VIEW,
    MODE_SYSTEM_INFO,
    MODE_INTERACTIVE_DEMO,
    MODE_SETTINGS,
    MODE_COUNT
};

class ComprehensiveContent : public ContentScreen
{
private:
    // State management
    ContentMode currentMode;
    int currentOption;
    int maxOptions;

    // Data simulation
    float sensorData[10];
    int dataIndex;
    unsigned long lastDataUpdate;
    unsigned long lastDisplay;

    // System monitoring
    unsigned long startTime;
    int frameCount;
    float fps;

    // Interactive demo state
    int interactiveX, interactiveY;
    bool interactiveMode;

    // Settings
    bool showTimestamp;
    bool enableSound;
    int refreshRate;

    // Performance tracking
    unsigned long lastPerformanceUpdate;
    int memoryUsage;
    int cpuLoad;

    // Menu options for each mode - using regular arrays for now
    const char *mainMenuOptions[6] = {
        "Data Display",
        "Chart View",
        "System Info",
        "Interactive Demo",
        "Settings",
        "About"};

    const char *settingsOptions[4] = {
        "Show Timestamp",
        "Enable Sound",
        "Refresh Rate",
        "Back to Main"};

public:
    ComprehensiveContent();

    // Override ContentScreen methods
    bool open() override;
    void _drawContent() override;
    Screen *actionTaken(ActionTaken action, int8_t offsetX, int8_t offsetY) override;
    void loop() override;

    // Optimized content refresh (content area only)
    void refreshContent();

private:
    // Mode-specific drawing methods
    void drawMainMenu();
    void drawDataDisplay();
    void drawChartView();
    void drawSystemInfo();
    void drawInteractiveDemo();
    void drawSettings();
    void drawAbout();

    // Helper methods
    void updateData();
    void updateSystemMetrics();
    void executeCurrentOption();
    void switchToMode(ContentMode newMode);
    void updateModeProgress();
    void drawChart(int x, int y, int width, int height);
    void drawProgressIndicator(int x, int y, int value, int maxValue);
    void handleMainMenuSelection();
    void handleSettingsSelection();

    // Utility methods
    const char *getModeTitle(ContentMode mode);
    const char *getModeFooter(ContentMode mode);
    uint16_t getRandomColor();
    void drawTextWithBackground(int x, int y, const char *text, uint16_t textColor, uint16_t bgColor);
};

#endif // COMPREHENSIVE_CONTENT_H
