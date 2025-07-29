/*
 * LoggerScreen.ino - Example demonstrating visual on-screen logging
 *
 * This example shows how to use LoggerScreen as a visual logging destination
 * that displays log messages directly on the M1Shield display. It demonstrates
 * different log levels, formatting, timestamps, and color coding.
 *
 * ## What This Example Does:
 *
 * 1. **Creates Visual Logger**: Sets up LoggerScreen with formatted output
 * 2. **Demonstrates Log Levels**: Shows info, warn, and error messages
 * 3. **Real-Time Logging**: Continuous logging with timestamps
 * 4. **Color Coding**: Different colors for different log levels
 * 5. **System Monitoring**: Simulates real system monitoring scenarios
 *
 * ## Expected Behavior:
 *
 * The screen will display a scrolling log with:
 * - White text for INFO messages
 * - Yellow text for WARN messages
 * - Red text for ERROR messages
 * - Timestamps showing elapsed time
 * - Automatic scrolling when screen fills
 *
 * ## Controls:
 *
 * - **Menu Button**: Exit logger screen
 * - **Other Buttons**: No action (focus on logging display)
 *
 * ## Hardware Requirements:
 *
 * - M1Shield with display
 * - Arduino Mega 2560
 * - Any supported display (ST7789, ILI9341, etc.)
 *
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

// First, select your display provider
#include <Display_ST7789_320x240.h>
// For ST7789 240x320 displays (most common, landscape becomes 320x240)
#include <Display_ST7789_320x240.h>
Display_ST7789_320x240 displayProvider;

// For ST7789 240x240 square displays
// #include <Display_ST7789_240x240.h>
// Display_ST7789_240x240 displayProvider;

// For ST7789 320x170 wide displays (landscape)
// #include <Display_ST7789_320x170.h>
// Display_ST7789_320x170 displayProvider;

// For ST7789 320x240 alternative displays (landscape)
// #include <Display_ST7789_320x240.h>
// Display_ST7789_320x240 displayProvider;

// For smaller ST7735 128x160 displays
// #include <Display_ST7735.h>
// Display_ST7735 displayProvider;

// For large ST7796 320x480 displays (landscape becomes 480x320)
// #include <Display_ST7796.h>
// Display_ST7796 displayProvider;

// For parallel ILI9325 240x320 displays (landscape becomes 320x240)
// #include <Display_ILI9325.h>
// Display_ILI9325 displayProvider;

// For ILI9341 240x320 displays (landscape becomes 320x240)
// #include <Display_ILI9341.h>
// Display_ILI9341 displayProvider;

// For large HX8357 320x480 displays
// #include <Display_HX8357.h>
// Display_HX8357 displayProvider;

// For monochrome SSD1306 OLED displays (128x64)
// #include <Display_SSD1306.h>
// Display_SSD1306 displayProvider;

// For monochrome SH1106 OLED displays (128x64)
// #include <Display_SH1106.h>
// Display_SH1106 displayProvider;

#include <M1Shield.h>
#include <LoggerScreen.h>

// Create logger screen instance
LoggerScreen *systemLogger;

// Simulation variables
int loopCounter = 0;
int errorCounter = 0;
int warningCounter = 0;
unsigned long lastLogTime = 0;
unsigned long lastStatusTime = 0;

// Simulate system state
bool sensorOnline[4] = {true, true, true, false}; // Sensor 3 is offline
int systemLoad = 25;
int freeMemory = 1024;

void setup()
{
    // Initialize serial for additional debugging
    Serial.begin(115200);
    Serial.println("=== LoggerScreen Visual Logging Example ===");

    // Initialize M1Shield
    M1Shield.begin(displayProvider);

    // Optional: Enable joystick input for screen navigation
    // Uncomment the next line to allow joystick control
    // M1Shield.activateJoystick();

    Serial.println("M1Shield initialized");

    // Create logger screen with title
    systemLogger = new LoggerScreen("System Monitor");

    // Configure logger options
    systemLogger->setTimestampEnabled(true);   // Show timestamps
    systemLogger->setColorCodingEnabled(true); // Use color coding
    systemLogger->setTextSize(1);              // Small text for more lines

    // Set the logger screen as active
    M1Shield.setScreen(systemLogger);

    // Initial system messages
    systemLogger->info("System Monitor starting...");
    systemLogger->info(" LoggerScreen initialized");
    systemLogger->info("Monitoring 4 sensors");
    systemLogger->warn(" Sensor #3 offline at startup");

    Serial.println("Logger screen activated");
    Serial.println("Watch the display for real-time log messages!");
    Serial.println("Press Menu button on M1Shield to exit");
}

void loop()
{
    // Handle M1Shield processing
    M1Shield.loop();

    unsigned long currentTime = millis();

    // Log periodic status every 5 seconds
    if (currentTime - lastStatusTime >= 5000)
    {
        lastStatusTime = currentTime;

        systemLogger->info("System status check #%d", ++loopCounter);
        systemLogger->info("Free memory: %d bytes", freeMemory);
        systemLogger->info("System load: %d%%", systemLoad);

        // Simulate changing system conditions
        freeMemory -= random(10, 50);
        systemLoad = random(15, 85);

        if (freeMemory < 300)
        {
            systemLogger->warn(" Low memory warning: %d bytes", freeMemory);
            warningCounter++;
            freeMemory += 200; // Simulate memory cleanup
        }

        if (systemLoad > 75)
        {
            systemLogger->warn(" High system load: %d%%", systemLoad);
            warningCounter++;
        }
    }

    // Log sensor readings every 2 seconds
    if (currentTime - lastLogTime >= 2000)
    {
        lastLogTime = currentTime;

        systemLogger->info("Sensor scan initiated");

        for (int i = 0; i < 4; i++)
        {
            if (sensorOnline[i])
            {
                int value = random(20, 100);
                systemLogger->info("Sensor %d: %d°C", i, value);

                // Simulate sensor warnings and errors
                if (value > 85)
                {
                    systemLogger->warn(" High temperature on sensor %d: %d°C", i, value);
                    warningCounter++;
                }
                else if (value > 95)
                {
                    systemLogger->err("Critical temperature on sensor %d: %d°C", i, value);
                    errorCounter++;
                }
            }
            else
            {
                systemLogger->err("Sensor %d: No response", i);
                errorCounter++;

                // Occasionally bring sensor back online
                if (random(100) < 10)
                {
                    sensorOnline[i] = true;
                    systemLogger->info("Sensor %d: Back online", i);
                }
            }
        }

        // Simulate network events
        if (random(100) < 20)
        {
            systemLogger->info("Network heartbeat sent");
        }

        // Simulate occasional errors
        if (random(100) < 5)
        {
            const char *errors[] = {
                "Connection timeout",
                "Invalid checksum",
                "Buffer overflow",
                "Protocol error"};
            systemLogger->err("%s", errors[random(4)]);
            errorCounter++;
        }

        // Summary every 10th scan
        if (loopCounter % 10 == 0)
        {
            systemLogger->info("Summary: %d warnings, %d errors", warningCounter, errorCounter);
        }
    }

    // Simulate some processing delay
    delay(100);
}

/*
 * EXAMPLE OUTPUT ON SCREEN:
 *
 * System Monitor
 * ================
 * [00:01] [INFO] System Monitor starting...
 * [00:01] [INFO]  LoggerScreen initialized
 * [00:01] [INFO] Monitoring 4 sensors
 * [00:01] [WARN]  Sensor #3 offline at startup
 * [00:06] [INFO] System status check #1
 * [00:06] [INFO] Free memory: 987 bytes
 * [00:06] [INFO] System load: 42%
 * [00:08] [INFO] Sensor scan initiated
 * [00:08] [INFO] Sensor 0: 73°C
 * [00:08] [INFO] Sensor 1: 45°C
 * [00:08] [INFO] Sensor 2: 91°C
 * [00:08] [WARN]  High temperature on sensor 2: 91°C
 * [00:08] [ERR ] Sensor 3: No response
 * [00:08] [INFO] Network heartbeat sent
 * [00:10] [INFO] Sensor scan initiated
 * [00:10] [INFO] Sensor 0: 67°C
 * ...
 *
 * Colors:
 * - INFO messages appear in white
 * - WARN messages appear in yellow
 * - ERR messages appear in red
 * - Timestamps appear in light gray
 */
