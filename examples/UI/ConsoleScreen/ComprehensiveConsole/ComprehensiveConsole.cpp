/*
 * ComprehensiveConsole.cpp - Advanced ConsoleScreen Implementation
 *
 * This example demonstrates ALL ConsoleScreen features including
 * paging modes, auto-forward, one-time execution, complex formatting,
 * and interactive controls.
 */

#include "ComprehensiveConsole.h"
#include <M1Shield.h>

ComprehensiveConsole::ComprehensiveConsole() : ConsoleScreen()
{
    currentMode = MODE_WELCOME;
    lastUpdate = 0;
    modeStartTime = 0;
    messageCount = 0;
    dataStreamCount = 0;
    autoMode = false;
    startTime = 0;
    totalMessages = 0;
    averageMessageRate = 0.0;
    logLevel = 0;
    pagingDemoActive = false;
    pagingStep = 0;

    // Initialize sensor data
    for (int i = 0; i < 5; i++)
    {
        sensorData[i] = 20.0 + random(0, 200) / 10.0;
    }

    // Configure console with advanced settings
    setTitle(F("Comprehensive Console"));
    setConsoleBackground(M1Shield.convertColor(0x0008)); // Very dark blue
    setTextColor(M1Shield.convertColor(0xFFFF));         // White text
    setTextSize(1);
    setTabSize(8);

    // Start with PAGING_AUTO_CLEAR mode
    setPagingMode(PAGING_AUTO_CLEAR);

    // Enable auto-forward with 5 second delay
    setAutoForward(true, 5000);

    // Clear screen - welcome will be shown in _executeOnce
    cls();
}

void ComprehensiveConsole::_executeOnce()
{
    // This is called 1 second after the console opens
    startTime = millis();

    setTextColor(M1Shield.convertColor(0x07E0)); // Green
    println(F("=== One-Time Execution Triggered ==="));
    println();

    setTextColor(M1Shield.convertColor(0xFFFF)); // White
    println(F("System initialization complete."));
    println(F("Auto-forward enabled (5s delay)."));
    println(F("Starting comprehensive demo..."));
    println();

    switchToMode(MODE_WELCOME);
}

void ComprehensiveConsole::loop()
{
    unsigned long now = millis();

    // Mode-specific updates
    switch (currentMode)
    {
    case MODE_WELCOME:
        if (now - lastUpdate >= 2000)
            runWelcomeMode();
        break;

    case MODE_SYSTEM_LOG:
        if (now - lastUpdate >= 1500)
            runSystemLogMode();
        break;

    case MODE_DATA_STREAM:
        if (now - lastUpdate >= 800)
            runDataStreamMode();
        break;

    case MODE_INTERACTIVE:
        if (now - lastUpdate >= 3000)
            runInteractiveMode();
        break;

    case MODE_PAGING_DEMO:
        if (now - lastUpdate >= 1000)
            runPagingDemo();
        break;

    case MODE_PERFORMANCE:
        if (now - lastUpdate >= 2000)
            runPerformanceMode();
        break;

    case MODE_COLOR_DEMO:
        if (now - lastUpdate >= 1200)
            runColorDemo();
        break;
    }

    // Auto-mode progression
    if (autoMode && now - modeStartTime >= 15000) // 15 seconds per mode
    {
        ConsoleMode nextMode = (ConsoleMode)((currentMode + 1) % MODE_COUNT);
        switchToMode(nextMode);
    }
}

void ComprehensiveConsole::switchToMode(ConsoleMode newMode)
{
    currentMode = newMode;
    modeStartTime = millis();
    lastUpdate = millis();
    messageCount = 0;

    // Mode-specific setup
    switch (newMode)
    {
    case MODE_WELCOME:
        setTextColor(M1Shield.convertColor(0x07FF)); // Cyan
        println(F("--- WELCOME MODE ---"));
        showModeMenu();
        break;

    case MODE_SYSTEM_LOG:
        setPagingMode(PAGING_AUTO_CLEAR);
        setTextColor(M1Shield.convertColor(0xFFE0)); // Yellow
        println(F("--- SYSTEM LOG MODE ---"));
        println(F("Simulating system log messages..."));
        break;

    case MODE_DATA_STREAM:
        setTextColor(M1Shield.convertColor(0x07E0)); // Green
        println(F("--- DATA STREAM MODE ---"));
        println(F("Real-time sensor data streaming..."));
        break;

    case MODE_INTERACTIVE:
        setTextColor(M1Shield.convertColor(0xF81F)); // Magenta
        println(F("--- INTERACTIVE MODE ---"));
        showControls();
        break;

    case MODE_PAGING_DEMO:
        setTextColor(M1Shield.convertColor(0x001F)); // Blue
        println(F("--- PAGING DEMO MODE ---"));
        demonstratePagingModes();
        break;

    case MODE_PERFORMANCE:
        setTextColor(M1Shield.convertColor(0xF800)); // Red
        println(F("--- PERFORMANCE MODE ---"));
        showPerformanceStats();
        break;

    case MODE_COLOR_DEMO:
        setTextColor(M1Shield.convertColor(0xFFFF)); // White
        println(F("--- COLOR DEMO MODE ---"));
        showColorPalette();
        break;
    }

    println();
}

void ComprehensiveConsole::showModeMenu()
{
    setTextColor(M1Shield.convertColor(0xBDF7)); // Light gray
    println(F("Available demonstration modes:"));
    println(F("1. System Log - Simulated log messages"));
    println(F("2. Data Stream - Real-time sensor data"));
    println(F("3. Interactive - User input examples"));
    println(F("4. Paging Demo - Different paging modes"));
    println(F("5. Performance - Speed and statistics"));
    println(F("6. Color Demo - Color palette showcase"));
    println();

    setTextColor(M1Shield.convertColor(0x07FF)); // Cyan
    println(F("Press UP/DOWN to navigate, MENU to select"));
    println(F("Press RIGHT to toggle auto-mode"));
    println();
}

void ComprehensiveConsole::showControls()
{
    setTextColor(M1Shield.convertColor(0x8410)); // Gray
    println(F("Interactive Controls:"));
    println(F("  UP/DOWN - Navigate modes"));
    println(F("  LEFT/RIGHT - Special functions"));
    println(F("  MENU - Select/Execute"));
    println(F("  Joystick - Enhanced navigation"));
    println();
}

void ComprehensiveConsole::runWelcomeMode()
{
    lastUpdate = millis();
    messageCount++;

    addTimestamp();
    setTextColor(M1Shield.convertColor(0x07E0)); // Green
    print(F("WELCOME: "));
    setTextColor(M1Shield.convertColor(0xFFFF)); // White
    print(F("Demo cycle "));
    print(messageCount);
    print(F(" - Auto-mode: "));
    println(autoMode ? F("ON") : F("OFF"));
}

void ComprehensiveConsole::runSystemLogMode()
{
    lastUpdate = millis();
    messageCount++;
    totalMessages++;

    // Simulate different log levels
    int level = messageCount % 4;

    const __FlashStringHelper *messages[] = {
        F("System boot sequence completed"),
        F("Warning: High CPU temperature detected"),
        F("Network connection established"),
        F("Error: Failed to write to log file")};

    addLogMessage(level, messages[level]);
}

void ComprehensiveConsole::runDataStreamMode()
{
    lastUpdate = millis();
    dataStreamCount++;
    totalMessages++;

    updateSensorData();
    addFormattedData();
}

void ComprehensiveConsole::runInteractiveMode()
{
    lastUpdate = millis();
    messageCount++;

    addTimestamp();
    setTextColor(M1Shield.convertColor(0xF81F)); // Magenta
    print(F("INTERACTIVE: "));
    setTextColor(M1Shield.convertColor(0xFFFF)); // White
    println(F("Waiting for user input..."));

    if (messageCount % 3 == 0)
    {
        showSystemInfo();
    }
}

void ComprehensiveConsole::runPagingDemo()
{
    lastUpdate = millis();
    pagingStep++;

    if (!pagingDemoActive)
    {
        pagingDemoActive = true;
        setPagingMode(PAGING_WAIT_BUTTON);

        setTextColor(M1Shield.convertColor(0x001F)); // Blue
        println(F("PAGING: Switched to WAIT_BUTTON mode"));
        println(F("Press any button to continue..."));
        return;
    }

    addTimestamp();
    setTextColor(M1Shield.convertColor(0x001F)); // Blue
    print(F("PAGING: "));
    setTextColor(M1Shield.convertColor(0xFFFF)); // White
    print(F("Step "));
    print(pagingStep);
    println(F(" - Demonstrating paging control"));

    if (pagingStep >= 10)
    {
        setPagingMode(PAGING_AUTO_CLEAR);
        pagingDemoActive = false;
        pagingStep = 0;
    }
}

void ComprehensiveConsole::runPerformanceMode()
{
    lastUpdate = millis();
    messageCount++;
    totalMessages++;

    // Calculate performance metrics
    unsigned long uptime = millis() - startTime;
    if (uptime > 0)
    {
        averageMessageRate = (float)totalMessages / (uptime / 1000.0);
    }

    addTimestamp();
    setTextColor(M1Shield.convertColor(0xF800)); // Red
    print(F("PERF: "));
    setTextColor(M1Shield.convertColor(0xFFFF)); // White
    print(F("Rate: "));
    print(averageMessageRate, 2);
    print(F(" msg/s, Total: "));
    print(totalMessages);
    println(F(" messages"));
}

void ComprehensiveConsole::runColorDemo()
{
    lastUpdate = millis();
    messageCount++;

    // Cycle through different colors
    uint16_t colors[] = {0xF800, 0x07E0, 0x001F, 0xFFE0, 0xF81F, 0x07FF, 0xFFFF};
    int colorIndex = messageCount % 7;

    addTimestamp();
    setTextColor(M1Shield.convertColor(colors[colorIndex]));
    print(F("COLOR: "));
    setTextColor(M1Shield.convertColor(0xFFFF)); // White
    print(F("Demonstrating color #"));
    print(colorIndex + 1);
    print(F(" - RGB value: 0x"));
    print(colors[colorIndex], HEX);
    println();
}

void ComprehensiveConsole::addTimestamp()
{
    setTextColor(M1Shield.convertColor(0x8410)); // Gray
    print(F("["));
    print(millis() / 1000);
    print(F("s] "));
}

void ComprehensiveConsole::addLogMessage(int level, const __FlashStringHelper *message)
{
    addTimestamp();

    switch (level)
    {
    case 0:                                          // Info
        setTextColor(M1Shield.convertColor(0x07E0)); // Green
        print(F("INFO: "));
        break;
    case 1:                                          // Warning
        setTextColor(M1Shield.convertColor(0xFFE0)); // Yellow
        print(F("WARN: "));
        break;
    case 2:                                          // Debug
        setTextColor(M1Shield.convertColor(0x07FF)); // Cyan
        print(F("DEBUG: "));
        break;
    case 3:                                          // Error
        setTextColor(M1Shield.convertColor(0xF800)); // Red
        print(F("ERROR: "));
        break;
    }

    setTextColor(M1Shield.convertColor(0xFFFF)); // White
    println(message);
}

void ComprehensiveConsole::updateSensorData()
{
    for (int i = 0; i < 5; i++)
    {
        sensorData[i] += (random(-50, 51) / 100.0);
        sensorData[i] = constrain(sensorData[i], 0.0, 50.0);
    }
}

void ComprehensiveConsole::addFormattedData()
{
    addTimestamp();
    setTextColor(M1Shield.convertColor(0x07E0)); // Green
    print(F("DATA: "));

    setTextColor(M1Shield.convertColor(0xFFFF)); // White
    for (int i = 0; i < 5; i++)
    {
        print(F("S"));
        print(i + 1);
        print(F(":"));
        print(sensorData[i], 1);
        if (i < 4)
            print(F("\t"));
    }
    println();
}

void ComprehensiveConsole::showSystemInfo()
{
    setTextColor(M1Shield.convertColor(0x8410)); // Gray
    print(F("SYS: Uptime "));
    print((millis() - startTime) / 1000);
    print(F("s, Messages: "));
    print(totalMessages);
    print(F(", Mode: "));
    print(currentMode);
    println();
}

void ComprehensiveConsole::demonstratePagingModes()
{
    setTextColor(M1Shield.convertColor(0xBDF7)); // Light gray
    println(F("Paging Modes Available:"));
    println(F("• PAGING_AUTO_CLEAR - Automatic clearing"));
    println(F("• PAGING_WAIT_BUTTON - Wait for button"));
    println(F("• PAGING_WAIT_TIMEOUT - Wait with timeout"));
    println(F("• PAGING_WAIT_BOTH - Wait for both"));
    println();
}

void ComprehensiveConsole::showColorPalette()
{
    setTextColor(M1Shield.convertColor(0xFFFF)); // White
    println(F("Color Palette Demonstration:"));

    struct ColorDemo
    {
        uint16_t color;
        const __FlashStringHelper *name;
    };

    ColorDemo colors[] = {
        {0xF800, F("Red")},
        {0x07E0, F("Green")},
        {0x001F, F("Blue")},
        {0xFFE0, F("Yellow")},
        {0xF81F, F("Magenta")},
        {0x07FF, F("Cyan")},
        {0xFFFF, F("White")}};

    for (int i = 0; i < 7; i++)
    {
        setTextColor(M1Shield.convertColor(colors[i].color));
        print(F("■ "));
        print(colors[i].name);
        print(F(" "));
    }
    println();
}

void ComprehensiveConsole::showPerformanceStats()
{
    setTextColor(M1Shield.convertColor(0xBDF7)); // Light gray
    println(F("Performance Statistics:"));
    print(F("• Total Messages: "));
    println(totalMessages);
    print(F("• Average Rate: "));
    print(averageMessageRate, 2);
    println(F(" msg/s"));
    print(F("• Uptime: "));
    print((millis() - startTime) / 1000);
    println(F(" seconds"));
    println();
}

Screen *ComprehensiveConsole::actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY)
{
    if (action & BUTTON_UP)
    {
        // Previous mode
        ConsoleMode prevMode = (ConsoleMode)((currentMode - 1 + MODE_COUNT) % MODE_COUNT);
        switchToMode(prevMode);
        return nullptr;
    }

    if (action & BUTTON_DOWN)
    {
        // Next mode
        ConsoleMode nextMode = (ConsoleMode)((currentMode + 1) % MODE_COUNT);
        switchToMode(nextMode);
        return nullptr;
    }

    if (action & BUTTON_LEFT)
    {
        // Clear screen
        cls();
        setTextColor(M1Shield.convertColor(0x07FF)); // Cyan
        println(F("Screen cleared by user"));
        println();
        return nullptr;
    }

    if (action & BUTTON_RIGHT)
    {
        // Toggle auto-mode
        autoMode = !autoMode;
        setTextColor(M1Shield.convertColor(0xFFE0)); // Yellow
        print(F("Auto-mode "));
        println(autoMode ? F("ENABLED") : F("DISABLED"));
        return nullptr;
    }

    if (action & BUTTON_MENU)
    {
        // Manual continue for paging mode
        if (isWaitingForPaging())
        {
            continuePaging();
            setTextColor(M1Shield.convertColor(0x07E0)); // Green
            println(F("Paging continued manually"));
        }
        else
        {
            // Show current status
            showSystemInfo();
        }
        return nullptr;
    }

    return nullptr;
}
