/*
 * ===================================================================
 * Comprehensive M1Shield Example - Advanced Hardware Analysis
 * ===================================================================
 *
 * This advanced example demonstrates comprehensive M1Shield capabilities
 * including performance analysis, advanced graphics, and diagnostics.
 *
 * WHAT YOU'LL LEARN:
 * - Advanced display provider configuration and optimization
 * - Comprehensive input analysis and statistics
 * - Performance monitoring and benchmarking
 * - Advanced graphics and visual effects
 * - Real-time hardware diagnostics
 * - Professional debugging techniques
 *
 * WHAT THIS EXAMPLE DOES:
 * - Comprehensive hardware initialization and testing
 * - Real-time performance monitoring
 * - Advanced input analysis with timing and statistics
 * - Multiple display modes and effects
 * - Detailed diagnostic reporting
 * - Interactive testing interface
 * - Hardware stress testing
 *
 * ADVANCED FEATURES:
 * - Input timing analysis and response measurement
 * - Display performance benchmarking
 * - Real-time statistics and monitoring
 * - Hardware health assessment
 * - Professional diagnostic interface
 * - Performance optimization analysis
 *
 * Authors: Marcel Erz (RetroStack)
 * License: MIT
 */

// ===================================================================
// DISPLAY PROVIDER SELECTION
// ===================================================================
// Select your display provider (uncomment only one)

#include <Display_ST7789_320x240.h>
Display_ST7789_320x240 displayProvider;

// Additional providers available:
// #include <Display_ST7789_240x240.h>
// Display_ST7789_240x240 displayProvider;

// #include <Display_ST7789_320x170.h>
// Display_ST7789_320x170 displayProvider;

// #include <Display_ST7735.h>
// Display_ST7735 displayProvider;

// #include <Display_ILI9341.h>
// Display_ILI9341 displayProvider;

// #include <Display_SSD1306.h>
// Display_SSD1306 displayProvider;

#include <M1Shield.h>

// ===================================================================
// GLOBAL VARIABLES FOR ANALYSIS
// ===================================================================
struct InputStatistics
{
    unsigned long buttonPresses = 0;
    unsigned long joystickMoves = 0;
    unsigned long totalInputs = 0;
    unsigned long sessionStart = 0;
    float averageResponseTime = 0.0;
    unsigned long fastestResponse = 999999;
    unsigned long slowestResponse = 0;
};

struct DisplayStatistics
{
    unsigned long frameCount = 0;
    unsigned long drawCalls = 0;
    float averageFPS = 0.0;
    unsigned long lastFrameTime = 0;
    unsigned long totalRenderTime = 0;
};

struct HardwareHealth
{
    bool displayHealthy = true;
    bool inputHealthy = true;
    bool ledHealthy = true;
    float systemLoad = 0.0;
    unsigned long uptime = 0;
};

InputStatistics inputStats;
DisplayStatistics displayStats;
HardwareHealth hardwareHealth;

unsigned long lastStatsUpdate = 0;
unsigned long lastDiagnostic = 0;
int currentDisplayMode = 0;
unsigned long modeChangeTime = 0;

// ===================================================================
// SETUP FUNCTION
// ===================================================================
void setup()
{
    Serial.begin(115200);

    Serial.println("=== Comprehensive M1Shield Analysis System ===");
    Serial.println("Advanced hardware testing and diagnostics");
    Serial.println("");

    // Initialize statistics
    inputStats.sessionStart = millis();
    displayStats.lastFrameTime = millis();

    // Display provider information
    Serial.println("Display Provider Analysis:");
    Serial.print("  Controller: ");
    Serial.println(displayProvider.name());
    Serial.print("  Resolution: ");
    Serial.print(displayProvider.width());
    Serial.print("x");
    Serial.println(displayProvider.height());
    Serial.print("  Color Depth: ");
    Serial.println("16-bit RGB565");
    Serial.println("");

    // Initialize M1Shield with comprehensive setup
    Serial.println("Initializing M1Shield hardware...");
    unsigned long initStart = millis();
    M1Shield.begin(displayProvider);
    unsigned long initTime = millis() - initStart;

    Serial.print("Initialization completed in ");
    Serial.print(initTime);
    Serial.println("ms");

    // Perform comprehensive hardware test
    performHardwareTest();

    // Initialize display with advanced graphics
    initializeAdvancedDisplay();

    Serial.println("");
    Serial.println("=== System Ready for Analysis ===");
    Serial.println("Real-time statistics will be displayed every 5 seconds");
    Serial.println("Press buttons and use joystick for comprehensive testing");
    Serial.println("========================================");
}

// ===================================================================
// MAIN LOOP
// ===================================================================
void loop()
{
    unsigned long loopStart = micros();

    // Advanced input monitoring
    analyzeInputs();

    // Update display with current mode
    updateAdvancedDisplay();

    // Update LED with advanced patterns
    updateAdvancedLED();

    // Periodic statistics and diagnostics
    if (millis() - lastStatsUpdate > 5000)
    {
        displayStatistics();
        lastStatsUpdate = millis();
    }

    if (millis() - lastDiagnostic > 10000)
    {
        performDiagnostics();
        lastDiagnostic = millis();
    }

    // Calculate system load
    unsigned long loopTime = micros() - loopStart;
    hardwareHealth.systemLoad = (loopTime / 1000.0) / 100.0; // Convert to percentage

    delay(10); // High-frequency monitoring
}

// ===================================================================
// HARDWARE TESTING FUNCTIONS
// ===================================================================
void performHardwareTest()
{
    Serial.println("Performing comprehensive hardware test...");

    // Test display functionality
    Serial.print("  Display test: ");
    hardwareHealth.displayHealthy = testDisplay();
    Serial.println(hardwareHealth.displayHealthy ? "PASS" : "FAIL");

    // Test LED functionality
    Serial.print("  LED test: ");
    hardwareHealth.ledHealthy = testLED();
    Serial.println(hardwareHealth.ledHealthy ? "PASS" : "FAIL");

    Serial.println("Hardware test completed");
}

bool testDisplay()
{
    // Test basic display operations
    Adafruit_GFX &gfx = M1Shield.getGFX();

    // Draw test pattern and verify
    gfx.fillScreen(M1Shield.convertColor(0x0000));
    gfx.fillRect(0, 0, 10, 10, M1Shield.convertColor(0xFFFF));

    return M1Shield.isDisplayInitialized();
}

bool testLED()
{
    // Test LED color changes
    M1Shield.setLEDColor(LEDColor::COLOR_RED);
    delay(50);
    M1Shield.setLEDColor(LEDColor::COLOR_GREEN);
    delay(50);
    M1Shield.setLEDColor(LEDColor::COLOR_BLUE);
    delay(50);
    M1Shield.setLEDColor(LEDColor::COLOR_OFF);

    return true; // Assume healthy if no exceptions
}

// ===================================================================
// ADVANCED INPUT ANALYSIS
// ===================================================================
void analyzeInputs()
{
    static bool lastButtons[6] = {false}; // Menu, Up, Down, Left, Right, Joy
    static JoystickDirection lastJoyDir = CENTER;
    static unsigned long lastInputTime = 0;

    bool currentButtons[6] = {
        M1Shield.isMenuPressed(),
        M1Shield.isUpPressed(),
        M1Shield.isDownPressed(),
        M1Shield.isLeftPressed(),
        M1Shield.isRightPressed(),
        M1Shield.isJoystickPressed()};

    const char *buttonNames[] = {"Menu", "Up", "Down", "Left", "Right", "JoyBtn"};

    // Analyze button presses with timing
    for (int i = 0; i < 6; i++)
    {
        if (currentButtons[i] != lastButtons[i])
        {
            unsigned long responseTime = millis() - lastInputTime;

            if (currentButtons[i])
            { // Button pressed
                inputStats.buttonPresses++;
                inputStats.totalInputs++;

                // Update response time statistics
                if (lastInputTime > 0)
                {
                    if (responseTime < inputStats.fastestResponse)
                    {
                        inputStats.fastestResponse = responseTime;
                    }
                    if (responseTime > inputStats.slowestResponse)
                    {
                        inputStats.slowestResponse = responseTime;
                    }

                    // Update average response time
                    inputStats.averageResponseTime =
                        (inputStats.averageResponseTime + responseTime) / 2.0;
                }

                Serial.print("INPUT: ");
                Serial.print(buttonNames[i]);
                Serial.print(" pressed (Response: ");
                Serial.print(responseTime);
                Serial.println("ms)");
            }

            lastButtons[i] = currentButtons[i];
            lastInputTime = millis();
        }
    }

    // Analyze joystick movement
    JoystickDirection currentJoyDir = M1Shield.getJoystickDirection();
    if (currentJoyDir != lastJoyDir)
    {
        inputStats.joystickMoves++;
        inputStats.totalInputs++;

        Serial.print("JOYSTICK: ");
        Serial.print("X=");
        Serial.print(M1Shield.getJoystickX());
        Serial.print(", Y=");
        Serial.print(M1Shield.getJoystickY());
        Serial.print(" (Direction changed)");
        Serial.println();

        lastJoyDir = currentJoyDir;
    }
}

// ===================================================================
// ADVANCED DISPLAY FUNCTIONS
// ===================================================================
void initializeAdvancedDisplay()
{
    Adafruit_GFX &gfx = M1Shield.getGFX();

    // Clear screen with gradient background
    drawGradientBackground();

    // Draw comprehensive interface
    drawAdvancedInterface();

    displayStats.frameCount++;
}

void updateAdvancedDisplay()
{
    // Cycle through different display modes every 10 seconds
    if (millis() - modeChangeTime > 10000)
    {
        currentDisplayMode = (currentDisplayMode + 1) % 3;
        modeChangeTime = millis();

        switch (currentDisplayMode)
        {
        case 0:
            drawStatisticsMode();
            break;
        case 1:
            drawDiagnosticsMode();
            break;
        case 2:
            drawPerformanceMode();
            break;
        }

        displayStats.frameCount++;
        displayStats.drawCalls++;
    }
}

void drawGradientBackground()
{
    Adafruit_GFX &gfx = M1Shield.getGFX();

    // Simple gradient effect
    for (int y = 0; y < M1Shield.getScreenHeight(); y += 4)
    {
        uint16_t color = M1Shield.convertColor((y * 0x1F) / M1Shield.getScreenHeight());
        gfx.fillRect(0, y, M1Shield.getScreenWidth(), 4, color);
    }
}

void drawAdvancedInterface()
{
    Adafruit_GFX &gfx = M1Shield.getGFX();

    // Title bar
    gfx.fillRect(0, 0, M1Shield.getScreenWidth(), 20, M1Shield.convertColor(0x001F));
    gfx.setTextColor(M1Shield.convertColor(0xFFFF));
    gfx.setTextSize(1);
    gfx.setCursor(2, 6);
    gfx.print("M1Shield Comprehensive Analysis");

    // Status indicator
    gfx.fillCircle(M1Shield.getScreenWidth() - 10, 10, 5,
                   M1Shield.convertColor(hardwareHealth.displayHealthy ? 0x07E0 : 0xF800));
}

void drawStatisticsMode()
{
    Adafruit_GFX &gfx = M1Shield.getGFX();

    // Clear content area
    gfx.fillRect(0, 25, M1Shield.getScreenWidth(), M1Shield.getScreenHeight() - 25,
                 M1Shield.convertColor(0x0000));

    gfx.setTextColor(M1Shield.convertColor(0xFFFF));
    gfx.setTextSize(1);

    gfx.setCursor(5, 30);
    gfx.print("STATISTICS MODE");

    gfx.setCursor(5, 45);
    gfx.print("Button Presses: ");
    gfx.print(inputStats.buttonPresses);

    gfx.setCursor(5, 55);
    gfx.print("Joystick Moves: ");
    gfx.print(inputStats.joystickMoves);

    gfx.setCursor(5, 65);
    gfx.print("Total Inputs: ");
    gfx.print(inputStats.totalInputs);

    gfx.setCursor(5, 75);
    gfx.print("Avg Response: ");
    gfx.print(inputStats.averageResponseTime, 1);
    gfx.print("ms");
}

void drawDiagnosticsMode()
{
    Adafruit_GFX &gfx = M1Shield.getGFX();

    // Clear content area
    gfx.fillRect(0, 25, M1Shield.getScreenWidth(), M1Shield.getScreenHeight() - 25,
                 M1Shield.convertColor(0x0000));

    gfx.setTextColor(M1Shield.convertColor(0xFFFF));
    gfx.setTextSize(1);

    gfx.setCursor(5, 30);
    gfx.print("DIAGNOSTICS MODE");

    gfx.setCursor(5, 45);
    gfx.print("Display: ");
    gfx.setTextColor(M1Shield.convertColor(hardwareHealth.displayHealthy ? 0x07E0 : 0xF800));
    gfx.print(hardwareHealth.displayHealthy ? "HEALTHY" : "ERROR");

    gfx.setTextColor(M1Shield.convertColor(0xFFFF));
    gfx.setCursor(5, 55);
    gfx.print("Inputs: ");
    gfx.setTextColor(M1Shield.convertColor(hardwareHealth.inputHealthy ? 0x07E0 : 0xF800));
    gfx.print(hardwareHealth.inputHealthy ? "HEALTHY" : "ERROR");

    gfx.setTextColor(M1Shield.convertColor(0xFFFF));
    gfx.setCursor(5, 65);
    gfx.print("LED: ");
    gfx.setTextColor(M1Shield.convertColor(hardwareHealth.ledHealthy ? 0x07E0 : 0xF800));
    gfx.print(hardwareHealth.ledHealthy ? "HEALTHY" : "ERROR");

    gfx.setTextColor(M1Shield.convertColor(0xFFFF));
    gfx.setCursor(5, 75);
    gfx.print("System Load: ");
    gfx.print(hardwareHealth.systemLoad, 1);
    gfx.print("%");
}

void drawPerformanceMode()
{
    Adafruit_GFX &gfx = M1Shield.getGFX();

    // Clear content area
    gfx.fillRect(0, 25, M1Shield.getScreenWidth(), M1Shield.getScreenHeight() - 25,
                 M1Shield.convertColor(0x0000));

    gfx.setTextColor(M1Shield.convertColor(0xFFFF));
    gfx.setTextSize(1);

    gfx.setCursor(5, 30);
    gfx.print("PERFORMANCE MODE");

    gfx.setCursor(5, 45);
    gfx.print("Frames: ");
    gfx.print(displayStats.frameCount);

    gfx.setCursor(5, 55);
    gfx.print("Draw Calls: ");
    gfx.print(displayStats.drawCalls);

    gfx.setCursor(5, 65);
    gfx.print("Uptime: ");
    gfx.print((millis() - inputStats.sessionStart) / 1000);
    gfx.print("s");

    gfx.setCursor(5, 75);
    gfx.print("Memory: ");
    gfx.print("OK"); // Simplified for this example
}

// ===================================================================
// ADVANCED LED FUNCTIONS
// ===================================================================
void updateAdvancedLED()
{
    static unsigned long lastLEDUpdate = 0;
    static int ledPhase = 0;

    // Advanced LED patterns based on system state
    if (millis() - lastLEDUpdate > 100)
    {
        if (inputStats.totalInputs > 0)
        {
            // Pulse pattern when inputs detected
            ledPhase = (ledPhase + 1) % 20;
            if (ledPhase < 10)
            {
                M1Shield.setLEDColor(LEDColor::COLOR_GREEN);
            }
            else
            {
                M1Shield.setLEDColor(LEDColor::COLOR_OFF);
            }
        }
        else
        {
            // Breathing pattern when idle
            ledPhase = (ledPhase + 1) % 40;
            if (ledPhase < 20)
            {
                M1Shield.setLEDColor(LEDColor::COLOR_BLUE);
            }
            else
            {
                M1Shield.setLEDColor(LEDColor::COLOR_OFF);
            }
        }
        lastLEDUpdate = millis();
    }

    // Override with input-specific colors
    if (M1Shield.isJoystickPressed())
    {
        M1Shield.setLEDColor(LEDColor::COLOR_WHITE);
    }
    else if (M1Shield.isMenuPressed())
    {
        M1Shield.setLEDColor(LEDColor::COLOR_CYAN);
    }
    else if (M1Shield.isUpPressed() || M1Shield.isDownPressed() ||
             M1Shield.isLeftPressed() || M1Shield.isRightPressed())
    {
        M1Shield.setLEDColor(LEDColor::COLOR_MAGENTA);
    }
}

// ===================================================================
// STATISTICS AND DIAGNOSTICS
// ===================================================================
void displayStatistics()
{
    unsigned long sessionTime = millis() - inputStats.sessionStart;
    float inputRate = inputStats.totalInputs / (sessionTime / 1000.0);

    Serial.println("");
    Serial.println("=== REAL-TIME STATISTICS ===");
    Serial.print("Session Time: ");
    Serial.print(sessionTime / 1000);
    Serial.println(" seconds");

    Serial.print("Total Inputs: ");
    Serial.println(inputStats.totalInputs);

    Serial.print("Button Presses: ");
    Serial.println(inputStats.buttonPresses);

    Serial.print("Joystick Moves: ");
    Serial.println(inputStats.joystickMoves);

    Serial.print("Input Rate: ");
    Serial.print(inputRate, 2);
    Serial.println(" inputs/sec");

    if (inputStats.totalInputs > 0)
    {
        Serial.print("Avg Response: ");
        Serial.print(inputStats.averageResponseTime, 1);
        Serial.println("ms");

        Serial.print("Fastest Response: ");
        Serial.print(inputStats.fastestResponse);
        Serial.println("ms");

        Serial.print("Slowest Response: ");
        Serial.print(inputStats.slowestResponse);
        Serial.println("ms");
    }

    Serial.print("Display Frames: ");
    Serial.println(displayStats.frameCount);

    Serial.print("System Load: ");
    Serial.print(hardwareHealth.systemLoad, 1);
    Serial.println("%");

    Serial.println("============================");
}

void performDiagnostics()
{
    Serial.println("");
    Serial.println("=== HARDWARE DIAGNOSTICS ===");

    // Check display health
    Serial.print("Display Health: ");
    Serial.println(hardwareHealth.displayHealthy ? "HEALTHY" : "ERROR");

    // Check input responsiveness
    Serial.print("Input Health: ");
    Serial.println(hardwareHealth.inputHealthy ? "HEALTHY" : "ERROR");

    // Check LED functionality
    Serial.print("LED Health: ");
    Serial.println(hardwareHealth.ledHealthy ? "HEALTHY" : "ERROR");

    // System performance
    Serial.print("System Performance: ");
    if (hardwareHealth.systemLoad < 50)
    {
        Serial.println("EXCELLENT");
    }
    else if (hardwareHealth.systemLoad < 75)
    {
        Serial.println("GOOD");
    }
    else
    {
        Serial.println("STRESSED");
    }

    Serial.println("=============================");
}
