/*
 * ComprehensiveContent.cpp - Advanced ContentScreen Implementation
 *
 * This example demonstrates ALL ContentScreen features in a comprehensive way.
 */

#include "ComprehensiveContent.h"
#include <M1Shield.h>

ComprehensiveContent::ComprehensiveContent() : ContentScreen()
{
    currentMode = MODE_MAIN_MENU;
    currentOption = 0;
    maxOptions = 6;
    dataIndex = 0;
    lastDataUpdate = 0;
    lastDisplay = 0;
    startTime = 0;
    frameCount = 0;
    fps = 0.0;
    interactiveX = 50;
    interactiveY = 50;
    interactiveMode = false;
    showTimestamp = true;
    enableSound = false;
    refreshRate = 500;
    lastPerformanceUpdate = 0;
    memoryUsage = 0;
    cpuLoad = 0;

    // Initialize sensor data with random values
    for (int i = 0; i < 10; i++)
    {
        sensorData[i] = random(200, 800) / 10.0f;
    }
}

bool ComprehensiveContent::open()
{
    Serial.println("ComprehensiveContent::open() - Initializing advanced content screen");

    // Call parent implementation first
    if (!ContentScreen::open())
    {
        return false;
    }

    startTime = millis();

    // Set initial mode
    switchToMode(MODE_MAIN_MENU);

    // Show welcome message
    notifyF(F("Welcome to Comprehensive ContentScreen Demo!"));

    return true;
}

void ComprehensiveContent::_drawContent()
{
    // Clear the primary content area first
    Adafruit_GFX &gfx = M1Shield.getGFX();
    uint16_t contentTop = _getContentTop();
    uint16_t contentHeight = _getContentHeight();
    uint16_t contentWidth = _getContentWidth();
    gfx.fillRect(_getContentLeft(), contentTop, contentWidth, contentHeight, M1Shield.convertColor(0x0000));

    frameCount++;

    switch (currentMode)
    {
    case MODE_MAIN_MENU:
        drawMainMenu();
        break;
    case MODE_DATA_DISPLAY:
        drawDataDisplay();
        break;
    case MODE_CHART_VIEW:
        drawChartView();
        break;
    case MODE_SYSTEM_INFO:
        drawSystemInfo();
        break;
    case MODE_INTERACTIVE_DEMO:
        drawInteractiveDemo();
        break;
    case MODE_SETTINGS:
        drawSettings();
        break;
    default:
        drawAbout();
        break;
    }
}

void ComprehensiveContent::_drawSecondaryContent()
{
    // Clear the secondary content area first
    clearSecondaryContentArea();

    // Draw different secondary content based on current mode
    switch (currentMode)
    {
    case MODE_DATA_DISPLAY:
    case MODE_CHART_VIEW:
        drawSecondaryDataSummary();
        break;
    case MODE_SYSTEM_INFO:
        drawSecondarySystemInfo();
        break;
    default:
        drawSecondaryStatus();
        break;
    }
}

uint16_t ComprehensiveContent::_getSecondaryContentHeight() const
{
    return 50; // Secondary content area height in pixels
}

void ComprehensiveContent::refreshContent()
{
    // Optimized refresh using _drawMainContent() which handles both primary and secondary areas
    // without redrawing header/footer, then updates display
    // Use this for content-only updates. Use refresh() when header/footer changes.
    if (isActive())
    {
        // Use _drawMainContent() to handle both areas with proper clearing and borders
        // Each drawing method now handles its own area clearing
        _drawMainContent();
        M1Shield.display(); // Push changes to display
    }
}

void ComprehensiveContent::drawMainMenu()
{
    Adafruit_GFX &display = M1Shield.getGFX();
    int contentY = _getContentTop();

    display.setTextSize(1);
    display.setTextColor(M1Shield.convertColor(0xFFFF)); // White

    display.setCursor(10, contentY + 5);
    display.print(F("Select a demonstration:"));

    // Draw menu options
    for (int i = 0; i < maxOptions; i++)
    {
        int y = contentY + 25 + (i * 18);

        if (i == currentOption)
        {
            // Highlight selected option
            display.fillRect(5, y - 2, display.width() - 10, 14,
                             M1Shield.convertColor(0x4208));     // Dark gray
            display.setTextColor(M1Shield.convertColor(0xFFE0)); // Yellow
        }
        else
        {
            display.setTextColor(M1Shield.convertColor(0xBDF7)); // Light gray
        }

        display.setCursor(10, y);
        display.print(mainMenuOptions[i]);
    }

    // Show selection indicator (adjusted for secondary content area)
    display.setTextColor(M1Shield.convertColor(0x07E0)); // Green
    int contentBottom = _getContentTop() + _getContentHeight();
    display.setCursor(10, contentBottom - 25); // Move higher to avoid secondary content
    display.print(F("Option "));
    display.print(currentOption + 1);
    display.print(F(" of "));
    display.print(maxOptions);
}

void ComprehensiveContent::drawDataDisplay()
{
    Adafruit_GFX &display = M1Shield.getGFX();
    int contentY = _getContentTop();

    display.setTextSize(1);

    // Title
    display.setTextColor(M1Shield.convertColor(0xFFFF)); // White
    display.setCursor(10, contentY + 5);
    display.print(F("Real-time Data Display"));

    // Data grid
    for (int i = 0; i < 5; i++)
    {
        int y = contentY + 25 + (i * 16);

        // Sensor label
        display.setTextColor(M1Shield.convertColor(0x07E0)); // Green
        display.setCursor(10, y);
        display.print(F("Sensor "));
        display.print(i + 1);
        display.print(F(":"));

        // Sensor value with color coding
        float value = sensorData[i];
        uint16_t color;
        if (value > 70)
            color = M1Shield.convertColor(0xF800); // Red
        else if (value > 50)
            color = M1Shield.convertColor(0xFFE0); // Yellow
        else
            color = M1Shield.convertColor(0x07E0); // Green

        display.setTextColor(color);
        display.setCursor(80, y);
        display.print(value, 1);
        display.print(F("C"));

        // Value bar
        int barWidth = (int)((value / 100.0f) * 100);
        display.drawRect(120, y, 102, 8, M1Shield.convertColor(0x8410));
        display.fillRect(121, y + 1, barWidth, 6, color);
    }

    // Status information
    if (showTimestamp)
    {
        display.setTextColor(M1Shield.convertColor(0x8410)); // Gray
        int contentBottom = _getContentTop() + _getContentHeight();
        display.setCursor(10, contentBottom - 25); // Adjusted for secondary content
        display.print(F("Last update: "));
        display.print(millis() / 1000);
        display.print(F("s"));
    }
}

void ComprehensiveContent::drawChartView()
{
    Adafruit_GFX &display = M1Shield.getGFX();
    int contentY = _getContentTop();

    display.setTextSize(1);
    display.setTextColor(M1Shield.convertColor(0xFFFF)); // White
    display.setCursor(10, contentY + 5);
    display.print(F("Data Chart Visualization"));

    // Draw chart
    drawChart(10, contentY + 20, display.width() - 20, 80);

    // Chart legend (adjusted for secondary content)
    display.setTextColor(M1Shield.convertColor(0x07E0)); // Green
    int contentBottom = _getContentTop() + _getContentHeight();
    display.setCursor(10, contentBottom - 35); // Moved higher
    display.print(F("Sensor data over time"));

    display.setTextColor(M1Shield.convertColor(0x8410)); // Gray
    display.setCursor(10, contentBottom - 20);           // Moved higher
    display.print(F("Points: "));
    display.print(dataIndex);
    display.print(F("/10"));
}

void ComprehensiveContent::drawSystemInfo()
{
    Adafruit_GFX &display = M1Shield.getGFX();
    int contentY = _getContentTop();

    display.setTextSize(1);
    display.setTextColor(M1Shield.convertColor(0xFFFF)); // White
    display.setCursor(10, contentY + 5);
    display.print(F("System Information"));

    int y = contentY + 25;

    // Runtime
    display.setTextColor(M1Shield.convertColor(0x07E0)); // Green
    display.setCursor(10, y);
    display.print(F("Runtime:"));
    display.setTextColor(M1Shield.convertColor(0xFFFF));
    display.setCursor(80, y);
    display.print((millis() - startTime) / 1000);
    display.print(F("s"));
    y += 15;

    // FPS
    display.setTextColor(M1Shield.convertColor(0x07E0));
    display.setCursor(10, y);
    display.print(F("FPS:"));
    display.setTextColor(M1Shield.convertColor(0xFFFF));
    display.setCursor(80, y);
    display.print(fps, 1);
    y += 15;

    // Memory usage (simulated)
    display.setTextColor(M1Shield.convertColor(0x07E0));
    display.setCursor(10, y);
    display.print(F("Memory:"));
    display.setTextColor(M1Shield.convertColor(0xFFFF));
    display.setCursor(80, y);
    display.print(memoryUsage);
    display.print(F("%"));
    drawProgressIndicator(130, y, memoryUsage, 100);
    y += 15;

    // CPU load (simulated)
    display.setTextColor(M1Shield.convertColor(0x07E0));
    display.setCursor(10, y);
    display.print(F("CPU:"));
    display.setTextColor(M1Shield.convertColor(0xFFFF));
    display.setCursor(80, y);
    display.print(cpuLoad);
    display.print(F("%"));
    drawProgressIndicator(130, y, cpuLoad, 100);
    y += 15;

    // Frame count (adjusted for secondary content)
    display.setTextColor(M1Shield.convertColor(0x8410)); // Gray
    int contentBottom = _getContentTop() + _getContentHeight();
    display.setCursor(10, contentBottom - 20); // Moved higher
    display.print(F("Frames: "));
    display.print(frameCount);
}

void ComprehensiveContent::drawInteractiveDemo()
{
    Adafruit_GFX &display = M1Shield.getGFX();
    int contentY = _getContentTop();

    display.setTextSize(1);
    display.setTextColor(M1Shield.convertColor(0xFFFF)); // White
    display.setCursor(10, contentY + 5);
    display.print(F("Interactive Demo"));

    // Draw interactive element
    display.fillCircle(interactiveX, contentY + 40 + interactiveY, 8,
                       M1Shield.convertColor(0xF81F)); // Magenta

    // Draw bounds
    display.drawRect(10, contentY + 30, display.width() - 20, 60,
                     M1Shield.convertColor(0x8410));

    // Instructions (adjusted for secondary content)
    display.setTextColor(M1Shield.convertColor(0x07E0)); // Green
    int contentBottom = _getContentTop() + _getContentHeight();
    display.setCursor(10, contentBottom - 35); // Moved higher
    display.print(F("Use buttons to move circle"));

    display.setTextColor(M1Shield.convertColor(0x8410)); // Gray
    display.setCursor(10, contentBottom - 20);           // Moved higher
    display.print(F("Position: "));
    display.print(interactiveX);
    display.print(F(","));
    display.print(interactiveY);
}

void ComprehensiveContent::drawSettings()
{
    Adafruit_GFX &display = M1Shield.getGFX();
    int contentY = _getContentTop();

    display.setTextSize(1);
    display.setTextColor(M1Shield.convertColor(0xFFFF)); // White
    display.setCursor(10, contentY + 5);
    display.print(F("Settings Configuration"));

    // Draw settings options
    for (int i = 0; i < 4; i++)
    {
        int y = contentY + 25 + (i * 18);

        if (i == currentOption)
        {
            display.fillRect(5, y - 2, display.width() - 10, 14,
                             M1Shield.convertColor(0x4208));     // Dark gray
            display.setTextColor(M1Shield.convertColor(0xFFE0)); // Yellow
        }
        else
        {
            display.setTextColor(M1Shield.convertColor(0xBDF7)); // Light gray
        }

        display.setCursor(10, y);
        display.print(settingsOptions[i]);

        // Show current values
        if (i < 3)
        {
            display.setCursor(150, y);
            switch (i)
            {
            case 0:
                display.print(showTimestamp ? F("ON") : F("OFF"));
                break;
            case 1:
                display.print(enableSound ? F("ON") : F("OFF"));
                break;
            case 2:
                display.print(refreshRate);
                display.print(F("ms"));
                break;
            }
        }
    }
}

void ComprehensiveContent::drawAbout()
{
    Adafruit_GFX &display = M1Shield.getGFX();
    int contentY = _getContentTop();

    display.setTextSize(1);
    display.setTextColor(M1Shield.convertColor(0xFFFF)); // White
    display.setCursor(10, contentY + 5);
    display.print(F("About This Demo"));

    display.setTextColor(M1Shield.convertColor(0x07E0)); // Green
    display.setCursor(10, contentY + 25);
    display.print(F("Comprehensive ContentScreen"));
    display.setCursor(10, contentY + 40);
    display.print(F("Example v1.0"));

    display.setTextColor(M1Shield.convertColor(0x8410)); // Gray
    display.setCursor(10, contentY + 60);
    display.print(F("Demonstrates ALL features:"));
    display.setCursor(10, contentY + 75);
    display.print(F("- Layout management"));
    display.setCursor(10, contentY + 90);
    display.print(F("- User feedback systems"));
    display.setCursor(10, contentY + 105);
    display.print(F("- Real-time data display"));

    display.setTextColor(M1Shield.convertColor(0xF81F)); // Magenta
    int contentBottom = _getContentTop() + _getContentHeight();
    display.setCursor(10, contentBottom - 20); // Adjusted for secondary content
    display.print(F("Press Menu to return"));
}

Screen *ComprehensiveContent::actionTaken(ActionTaken action, int8_t offsetX, int8_t offsetY)
{
    if (currentMode == MODE_INTERACTIVE_DEMO)
    {
        // Handle interactive demo movement
        if (action & BUTTON_UP)
        {
            interactiveY = max(0, interactiveY - 5);
            refreshContent();
            return nullptr;
        }
        if (action & BUTTON_DOWN)
        {
            interactiveY = min(40, interactiveY + 5);
            refreshContent();
            return nullptr;
        }
        if (action & BUTTON_LEFT)
        {
            interactiveX = max(20, interactiveX - 5);
            refreshContent();
            return nullptr;
        }
        if (action & BUTTON_RIGHT)
        {
            interactiveX = min(M1Shield.getDisplayProvider().width() - 30, interactiveX + 5);
            refreshContent();
            return nullptr;
        }
        if (action & BUTTON_MENU)
        {
            switchToMode(MODE_MAIN_MENU);
            return nullptr;
        }
    }
    else
    {
        // Handle menu navigation
        if (action & BUTTON_UP)
        {
            currentOption = (currentOption - 1 + maxOptions) % maxOptions;
            updateModeProgress();
            refreshContent();
            return nullptr;
        }

        if (action & BUTTON_DOWN)
        {
            currentOption = (currentOption + 1) % maxOptions;
            updateModeProgress();
            refreshContent();
            return nullptr;
        }

        if (action & BUTTON_MENU)
        {
            executeCurrentOption();
            return nullptr;
        }

        if (action & BUTTON_LEFT)
        {
            if (currentMode != MODE_MAIN_MENU)
            {
                switchToMode(MODE_MAIN_MENU);
                return nullptr;
            }
        }
    }

    return nullptr;
}

void ComprehensiveContent::executeCurrentOption()
{
    if (currentMode == MODE_MAIN_MENU)
    {
        handleMainMenuSelection();
    }
    else if (currentMode == MODE_SETTINGS)
    {
        handleSettingsSelection();
    }
    else
    {
        switchToMode(MODE_MAIN_MENU);
    }
}

void ComprehensiveContent::handleMainMenuSelection()
{
    switch (currentOption)
    {
    case 0:
        switchToMode(MODE_DATA_DISPLAY);
        break;
    case 1:
        switchToMode(MODE_CHART_VIEW);
        break;
    case 2:
        switchToMode(MODE_SYSTEM_INFO);
        break;
    case 3:
        switchToMode(MODE_INTERACTIVE_DEMO);
        break;
    case 4:
        switchToMode(MODE_SETTINGS);
        break;
    case 5:
        alertF(F("Comprehensive ContentScreen Demo v1.0\n\nDemonstrates all ContentScreen features including layout management, user feedback, real-time updates, and interactive elements."));
        break;
    }
}

void ComprehensiveContent::handleSettingsSelection()
{
    switch (currentOption)
    {
    case 0:
        showTimestamp = !showTimestamp;
        // Demonstrate custom notification colors: green for enabled, red for disabled
        notifyF(showTimestamp ? F("Timestamp enabled") : F("Timestamp disabled"), 3000, showTimestamp ? 0x07E0 : 0xF800);
        refreshContent();
        break;
    case 1:
        enableSound = !enableSound;
        // Demonstrate custom notification colors: green for enabled, red for disabled
        notifyF(enableSound ? F("Sound enabled") : F("Sound disabled"), 3000, enableSound ? 0x07E0 : 0xF800);
        refreshContent();
        break;
    case 2:
        refreshRate = (refreshRate == 500) ? 1000 : ((refreshRate == 1000) ? 250 : 500);
        // Demonstrate custom notification colors: cyan for updates
        notifyF(F("Refresh rate updated"), 3000, 0x07FF);
        refreshContent();
        break;
    case 3:
        switchToMode(MODE_MAIN_MENU);
        break;
    }
}

void ComprehensiveContent::switchToMode(ContentMode newMode)
{
    currentMode = newMode;
    currentOption = 0;

    switch (newMode)
    {
    case MODE_MAIN_MENU:
        maxOptions = 6;
        break;
    case MODE_SETTINGS:
        maxOptions = 4;
        break;
    default:
        maxOptions = 1;
        break;
    }

    setTitle(getModeTitle(newMode));
    // Note: ContentScreen doesn't have setFooter - this should be handled through button items
    updateModeProgress();
    refresh(); // Use full refresh when changing modes to update title in header
}

void ComprehensiveContent::updateModeProgress()
{
    if (maxOptions > 1)
    {
        int progress = (currentOption * 100) / (maxOptions - 1);
        setProgressValue(progress);
    }
    else
    {
        setProgressValue(50); // Default for non-menu modes
    }
}

const char *ComprehensiveContent::getModeTitle(ContentMode mode)
{
    switch (mode)
    {
    case MODE_MAIN_MENU:
        return "Main Menu";
    case MODE_DATA_DISPLAY:
        return "Data Display";
    case MODE_CHART_VIEW:
        return "Chart View";
    case MODE_SYSTEM_INFO:
        return "System Info";
    case MODE_INTERACTIVE_DEMO:
        return "Interactive";
    case MODE_SETTINGS:
        return "Settings";
    default:
        return "About";
    }
}

const char *ComprehensiveContent::getModeFooter(ContentMode mode)
{
    switch (mode)
    {
    case MODE_MAIN_MENU:
        return "Up/Down: Navigate | Menu: Select";
    case MODE_INTERACTIVE_DEMO:
        return "Arrows: Move | Menu: Exit";
    case MODE_SETTINGS:
        return "Up/Down: Navigate | Menu: Toggle";
    default:
        return "Left: Back | Menu: Select";
    }
}

void ComprehensiveContent::updateData()
{
    // Simulate sensor data updates
    for (int i = 0; i < 10; i++)
    {
        sensorData[i] += random(-50, 51) / 100.0f;
        sensorData[i] = constrain(sensorData[i], 0, 100);
    }
    dataIndex = (dataIndex + 1) % 10;
}

void ComprehensiveContent::updateSystemMetrics()
{
    // Simulate system metrics
    memoryUsage = 40 + random(0, 40);
    cpuLoad = 20 + random(0, 60);

    // Calculate FPS
    unsigned long now = millis();
    if (now - lastPerformanceUpdate >= 1000)
    {
        fps = frameCount * 1000.0f / (now - lastPerformanceUpdate);
        frameCount = 0;
        lastPerformanceUpdate = now;
    }
}

void ComprehensiveContent::drawChart(int x, int y, int width, int height)
{
    Adafruit_GFX &display = M1Shield.getGFX();

    // Draw chart border
    display.drawRect(x, y, width, height, M1Shield.convertColor(0x8410));

    // Draw data points
    for (int i = 0; i < 9; i++)
    {
        int x1 = x + 5 + (i * (width - 10)) / 9;
        int y1 = y + height - 5 - (int)((sensorData[i] / 100.0f) * (height - 10));
        int x2 = x + 5 + ((i + 1) * (width - 10)) / 9;
        int y2 = y + height - 5 - (int)((sensorData[i + 1] / 100.0f) * (height - 10));

        display.drawLine(x1, y1, x2, y2, M1Shield.convertColor(0x07E0)); // Green
        display.fillCircle(x1, y1, 2, M1Shield.convertColor(0xFFE0));    // Yellow
    }
}

void ComprehensiveContent::drawProgressIndicator(int x, int y, int value, int maxValue)
{
    Adafruit_GFX &display = M1Shield.getGFX();

    int width = 80;
    int height = 8;

    display.drawRect(x, y, width, height, M1Shield.convertColor(0x8410));

    int fillWidth = (value * (width - 2)) / maxValue;
    uint16_t color = (value > 75) ? M1Shield.convertColor(0xF800) : // Red
                         (value > 50) ? M1Shield.convertColor(0xFFE0)
                                      :                 // Yellow
                         M1Shield.convertColor(0x07E0); // Green

    display.fillRect(x + 1, y + 1, fillWidth, height - 2, color);
}

void ComprehensiveContent::loop()
{
    // Call parent ContentScreen::loop() to handle notification timeouts
    ContentScreen::loop();

    unsigned long now = millis();

    // Update data at specified refresh rate
    if (now - lastDataUpdate >= refreshRate)
    {
        lastDataUpdate = now;
        updateData();
        updateSystemMetrics();

        // Refresh display if showing real-time content
        // Use optimized content-only refresh instead of full screen refresh
        if (currentMode == MODE_DATA_DISPLAY ||
            currentMode == MODE_CHART_VIEW ||
            currentMode == MODE_SYSTEM_INFO)
        {
            refreshContent(); // Use optimized refresh with _drawMainContent()
        }
        else
        {
            // For other modes, use clearSecondaryContentArea() and redraw secondary content
            // This leverages the built-in ContentScreen infrastructure
            clearSecondaryContentArea();
            _drawSecondaryContent();
            M1Shield.display();
        }
    }
}

// =====================================================================================
// Secondary Content Area Drawing Methods
// =====================================================================================

void ComprehensiveContent::drawSecondaryStatus()
{
    Adafruit_GFX &display = M1Shield.getGFX();
    uint16_t x = _getSecondaryContentLeft();
    uint16_t y = _getSecondaryContentTop();

    // Title with direct graphics
    display.setTextSize(1);
    display.setTextColor(M1Shield.convertColor(0xFFFF));
    display.setCursor(x + 5, y + 5);
    display.print("STATUS");

    // Current mode indicator with colored background
    display.fillRect(x + 5, y + 18, 60, 12, M1Shield.convertColor(0x07E0));
    display.setTextColor(M1Shield.convertColor(0x0000));
    display.setCursor(x + 8, y + 20);
    display.print(getModeTitle(currentMode));

    // Runtime progress bar
    unsigned long elapsed = millis() - startTime;
    int seconds = (elapsed / 1000) % 60;
    int minutes = (elapsed / 60000) % 60;

    // Time display
    display.setTextColor(M1Shield.convertColor(0xFFE0));
    display.setCursor(x + 75, y + 5);
    if (minutes < 10)
        display.print("0");
    display.print(minutes);
    display.print(":");
    if (seconds < 10)
        display.print("0");
    display.print(seconds);

    // FPS indicator with visual bar
    if (fps > 0)
    {
        display.setTextColor(M1Shield.convertColor(0x8410));
        display.setCursor(x + 150, y + 5);
        display.print("FPS: ");
        display.print(fps, 1);

        // FPS bar (target 30 FPS)
        int fpsBarWidth = (fps > 30) ? 40 : (fps * 40 / 30);
        uint16_t fpsColor = (fps > 25) ? M1Shield.convertColor(0x07E0) : (fps > 15) ? M1Shield.convertColor(0xFFE0)
                                                                                    : M1Shield.convertColor(0xF800);
        display.fillRect(x + 150, y + 18, fpsBarWidth, 6, fpsColor);
        display.drawRect(x + 150, y + 18, 40, 6, M1Shield.convertColor(0x8410));
    }

    // Activity indicator (blinking dot)
    uint16_t dotColor = ((millis() / 500) % 2) ? M1Shield.convertColor(0x07E0) : M1Shield.convertColor(0x0000);
    display.fillCircle(x + 220, y + 8, 3, dotColor);
}

void ComprehensiveContent::drawSecondarySystemInfo()
{
    Adafruit_GFX &display = M1Shield.getGFX();
    uint16_t x = _getSecondaryContentLeft();
    uint16_t y = _getSecondaryContentTop();

    // Title
    display.setTextSize(1);
    display.setTextColor(M1Shield.convertColor(0xFFFF));
    display.setCursor(x + 5, y + 5);
    display.print("SYSTEM METRICS");

    // Memory usage gauge (circular gauge)
    int centerX = x + 40;
    int centerY = y + 30;
    int radius = 12;

    // Draw memory gauge background
    display.drawCircle(centerX, centerY, radius, M1Shield.convertColor(0x8410));

    // Fill memory gauge based on usage
    float memAngle = (memoryUsage * 270.0f / 100.0f) - 135; // -135 to 135 degrees
    int endX = centerX + (radius - 2) * cos(memAngle * PI / 180);
    int endY = centerY + (radius - 2) * sin(memAngle * PI / 180);
    display.drawLine(centerX, centerY, endX, endY, M1Shield.convertColor(0x07E0));

    // Memory percentage in center
    display.setTextColor(M1Shield.convertColor(0xFFFF));
    display.setCursor(centerX - 6, centerY - 3);
    display.print(memoryUsage);

    // CPU load with LED-style indicators
    int cpuX = x + 80;
    int cpuY = y + 18;

    display.setTextColor(M1Shield.convertColor(0xFFFF));
    display.setCursor(cpuX, cpuY);
    display.print("CPU");

    // LED bar for CPU (10 LEDs)
    for (int i = 0; i < 10; i++)
    {
        int ledX = cpuX + i * 8;
        int ledY = cpuY + 12;
        uint16_t ledColor;

        if (i < (cpuLoad / 10))
        {
            // Active LED color based on level
            if (i < 6)
                ledColor = M1Shield.convertColor(0x07E0); // Green
            else if (i < 8)
                ledColor = M1Shield.convertColor(0xFFE0); // Yellow
            else
                ledColor = M1Shield.convertColor(0xF800); // Red
        }
        else
        {
            ledColor = M1Shield.convertColor(0x2104); // Dark gray
        }

        display.fillRect(ledX, ledY, 6, 8, ledColor);
        display.drawRect(ledX, ledY, 6, 8, M1Shield.convertColor(0x8410));
    }

    // Refresh rate indicator with animated bar
    display.setTextColor(M1Shield.convertColor(0x8410));
    display.setCursor(x + 180, y + 5);
    display.print("Refresh");
    display.setCursor(x + 180, y + 15);
    display.print(refreshRate);
    display.print("ms");

    // Animated refresh indicator
    int refreshBarX = x + 180;
    int refreshBarY = y + 28;
    int animPhase = (millis() / refreshRate) % 4;
    for (int i = 0; i < 4; i++)
    {
        uint16_t barColor = (i == animPhase) ? M1Shield.convertColor(0x07E0) : M1Shield.convertColor(0x2104);
        display.fillRect(refreshBarX + i * 8, refreshBarY, 6, 4, barColor);
    }
}

void ComprehensiveContent::drawSecondaryDataSummary()
{
    Adafruit_GFX &display = M1Shield.getGFX();
    uint16_t x = _getSecondaryContentLeft();
    uint16_t y = _getSecondaryContentTop();

    // Title
    display.setTextSize(1);
    display.setTextColor(M1Shield.convertColor(0xFFFF));
    display.setCursor(x + 5, y + 5);
    display.print("DATA SUMMARY");

    // Calculate data statistics
    float sum = 0, minVal = sensorData[0], maxVal = sensorData[0];
    for (int i = 0; i < 10; i++)
    {
        sum += sensorData[i];
        if (sensorData[i] < minVal)
            minVal = sensorData[i];
        if (sensorData[i] > maxVal)
            maxVal = sensorData[i];
    }
    float avg = sum / 10.0f;

    // Mini sparkline chart (10 data points in 60 pixels width)
    int chartX = x + 5;
    int chartY = y + 18;
    int chartW = 60;
    int chartH = 20;

    // Chart background
    display.drawRect(chartX, chartY, chartW, chartH, M1Shield.convertColor(0x8410));

    // Plot data points
    for (int i = 0; i < 9; i++)
    {
        int x1 = chartX + (i * chartW / 9);
        int y1 = chartY + chartH - ((sensorData[i] - minVal) * chartH / (maxVal - minVal));
        int x2 = chartX + ((i + 1) * chartW / 9);
        int y2 = chartY + chartH - ((sensorData[i + 1] - minVal) * chartH / (maxVal - minVal));

        display.drawLine(x1, y1, x2, y2, M1Shield.convertColor(0x07E0));
        display.fillCircle(x1, y1, 1, M1Shield.convertColor(0xFFE0));
    }

    // Last point
    int lastX = chartX + chartW - 1;
    int lastY = chartY + chartH - ((sensorData[9] - minVal) * chartH / (maxVal - minVal));
    display.fillCircle(lastX, lastY, 1, M1Shield.convertColor(0xFFE0));

    // Statistics with visual indicators
    display.setTextColor(M1Shield.convertColor(0x07E0));
    display.setCursor(x + 75, y + 18);
    display.print("Avg: ");
    display.print(avg, 1);

    // Min/Max range indicator
    display.setTextColor(M1Shield.convertColor(0x8410));
    display.setCursor(x + 75, y + 28);
    display.print("Range: ");
    display.print(maxVal - minVal, 1);

    // Trend arrow indicator
    bool upTrend = sensorData[9] > sensorData[0];
    uint16_t trendColor = upTrend ? M1Shield.convertColor(0x07E0) : M1Shield.convertColor(0xF800);

    display.setTextColor(trendColor);
    display.setCursor(x + 150, y + 18);
    display.print("Trend");

    // Draw trend arrow
    int arrowX = x + 185;
    int arrowY = y + 23;
    if (upTrend)
    {
        // Up arrow
        display.drawLine(arrowX, arrowY, arrowX + 5, arrowY - 5, trendColor);
        display.drawLine(arrowX + 5, arrowY - 5, arrowX + 10, arrowY, trendColor);
        display.drawLine(arrowX + 5, arrowY - 5, arrowX + 5, arrowY + 3, trendColor);
    }
    else
    {
        // Down arrow
        display.drawLine(arrowX, arrowY, arrowX + 5, arrowY + 5, trendColor);
        display.drawLine(arrowX + 5, arrowY + 5, arrowX + 10, arrowY, trendColor);
        display.drawLine(arrowX + 5, arrowY + 5, arrowX + 5, arrowY - 3, trendColor);
    }

    // Sample count with dots
    display.setTextColor(M1Shield.convertColor(0xFFE0));
    display.setCursor(x + 150, y + 32);
    display.print("Samples: 10");

    // Draw 10 sample dots
    for (int i = 0; i < 10; i++)
    {
        int dotX = x + 200 + (i * 3);
        int dotY = y + 35;
        display.fillCircle(dotX, dotY, 1, M1Shield.convertColor(0xFFE0));
    }
}
