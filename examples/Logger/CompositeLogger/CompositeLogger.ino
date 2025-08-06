/*
 * CompositeLogger.ino - Example demonstrating multi-destination logging
 *
 * This example shows how to use CompositeLogger to send log messages to multiple
 * destinations simultaneously. It demonstrates logger registration, different log
 * levels, and dynamic logger management.
 *
 * ## What This Example Does:
 *
 * 1. **Creates Multiple Loggers**: Sets up SerialLogger and LoggerScreen, and demonstrates concepts
 * 2. **Registers Loggers**: Shows how to add multiple loggers to CompositeLogger
 * 3. **Multi-Destination Logging**: Demonstrates logging to multiple outputs
 *
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

// Select your display provider
#include <Display_ST7789.h>
Display_ST7789 displayProvider;

#include <M1Shield.h>
#include <CompositeLogger.h>
#include <SerialLogger.h>
#include <LoggerScreen.h>

// Create logger instances
SerialLogger serialLogger;
LoggerScreen *screenLogger;
CompositeLogger multiLogger;

// Application state
int deviceCount = 0;
int connectionAttempts = 0;
int successfulConnections = 0;
unsigned long lastScanTime = 0;
unsigned long lastStatusTime = 0;

// Simulate device registry
struct Device
{
    int id;
    const char *name;
    bool online;
    int signalStrength;
};

Device devices[] = {
    {1, "Temperature Sensor", true, 85},
    {2, "Humidity Sensor", false, 0},
    {3, "Motion Detector", true, 92},
    {4, "Light Sensor", true, 78},
    {5, "Door Sensor", false, 0}};

const int DEVICE_COUNT = sizeof(devices) / sizeof(devices[0]);

void setup()
{
    // Initialize serial communication
    Serial.begin(115200);
    delay(1000); // Allow serial to stabilize

    Serial.println("=== Multi-Destination Logging Example ===");
    Serial.println("Messages will appear in both Serial Monitor and on-screen display");
    Serial.println();

    // Initialize M1Shield
    M1Shield.begin(displayProvider);

    // Optional: Enable joystick input
    // M1Shield.activateJoystick();

    // Create screen logger
    screenLogger = new LoggerScreen("Multi-Log Demo");
    screenLogger->setTimestampEnabled(true);
    screenLogger->setColorCodingEnabled(true);

    // Set up composite logger with multiple destinations
    Serial.println("Setting up multi-destination logging...");

    if (multiLogger.addLogger(&serialLogger))
    {
        Serial.println("Serial logger registered");
    }

    if (multiLogger.addLogger(screenLogger->asLogger()))
    {
        Serial.println("Screen logger registered");
    }

    Serial.print(F("Total loggers active: "));
    Serial.println(multiLogger.getLoggerCount());
    
    // Activate the screen
    M1Shield.setScreen(screenLogger);

    // Start logging through composite logger
    multiLogger.info("Multi-destination logging system started");
    multiLogger.info("Active destinations: Serial + Screen");
    multiLogger.info("Scanning for %d devices...", DEVICE_COUNT);

    // Initial device scan
    scanDevices();

    multiLogger.info("System initialization complete");

    Serial.println("Starting main monitoring loop...");
    Serial.println("Watch both Serial Monitor and display for synchronized output");
    Serial.println();
}

void loop()
{
    // Handle M1Shield processing
    M1Shield.loop();

    unsigned long currentTime = millis();

    // Perform device scan every 8 seconds
    if (currentTime - lastScanTime >= 8000)
    {
        lastScanTime = currentTime;
        scanDevices();
    }

    // Log system status every 15 seconds
    if (currentTime - lastStatusTime >= 15000)
    {
        lastStatusTime = currentTime;
        logSystemStatus();
    }

    // Simulate some background activity
    delay(500);
}

void scanDevices()
{
    multiLogger.info("Device scan initiated");

    int onlineCount = 0;
    int offlineCount = 0;

    for (int i = 0; i < DEVICE_COUNT; i++)
    {
        Device &device = devices[i];

        // Simulate device state changes
        if (!device.online && random(100) < 30)
        {
            // Bring offline device online
            device.online = true;
            device.signalStrength = random(70, 95);
            multiLogger.info("%s (ID:%d) came online", device.name, device.id);
            successfulConnections++;
        }
        else if (device.online && random(100) < 10)
        {
            // Take online device offline
            device.online = false;
            device.signalStrength = 0;
            multiLogger.warn("%s (ID:%d) went offline", device.name, device.id);
        }

        // Log device status
        if (device.online)
        {
            // Simulate signal strength changes
            device.signalStrength += random(-5, 6);
            device.signalStrength = constrain(device.signalStrength, 50, 95);

            multiLogger.info("%s: Signal %d%%", device.name, device.signalStrength);

            if (device.signalStrength < 60)
            {
                multiLogger.warn("Weak signal on %s: %d%%", device.name, device.signalStrength);
            }

            onlineCount++;
        }
        else
        {
            multiLogger.err("%s: No response", device.name);
            offlineCount++;
        }

        connectionAttempts++;
    }

    // Summary
    multiLogger.info("Scan complete: %d online, %d offline", onlineCount, offlineCount);

    if (offlineCount > 0)
    {
        multiLogger.warn("%d devices need attention", offlineCount);
    }
}

void logSystemStatus()
{
    multiLogger.info("System Status Report");
    multiLogger.info("Uptime: %lu seconds", millis() / 1000);
    multiLogger.info("Connection attempts: %d", connectionAttempts);
    multiLogger.info("Successful connections: %d", successfulConnections);

    int successRate = (connectionAttempts > 0) ? (successfulConnections * 100 / connectionAttempts) : 0;
    multiLogger.info("Success rate: %d%%", successRate);

    if (successRate < 70)
    {
        multiLogger.warn("Low success rate detected");
    }

    // Simulate memory and performance metrics
    int freeMemory = random(400, 1200);
    int cpuUsage = random(15, 45);

    multiLogger.info("Free memory: %d bytes", freeMemory);
    multiLogger.info("CPU usage: %d%%", cpuUsage);

    if (freeMemory < 500)
    {
        multiLogger.warn("Low memory: %d bytes", freeMemory);
    }

    if (cpuUsage > 80)
    {
        multiLogger.warn("High CPU usage: %d%%", cpuUsage);
    }

    // Simulate occasional errors
    if (random(100) < 15)
    {
        const char *errors[] = {
            "Network timeout",
            "Sensor calibration drift",
            "Power fluctuation detected",
            "Communication checksum error"};
        multiLogger.err("%s", errors[random(4)]);
    }
}

/*
 * EXAMPLE OUTPUT (appears in BOTH Serial Monitor and on-screen):
 *
 * === Serial Monitor Output ===
 * [INFO] Multi-destination logging system started
 * [INFO] Active destinations: Serial + Screen
 * [INFO] Scanning for 5 devices...
 * [INFO] Device scan initiated
 * [INFO] Temperature Sensor: Signal 85%
 * [ERR ] Humidity Sensor: No response
 * [INFO] Motion Detector: Signal 92%
 * [INFO] Light Sensor: Signal 78%
 * [ERR ] Door Sensor: No response
 * [INFO] Scan complete: 3 online, 2 offline
 * [WARN] 2 devices need attention
 * [INFO] System initialization complete
 * ...
 *
 * === Screen Display ===
 * Multi-Log Demo
 * ===============
 * [00:01] [INFO] Multi-destination logging system started
 * [00:01] [INFO] Active destinations: Serial + Screen
 * [00:01] [INFO] Scanning for 5 devices...
 * [00:01] [INFO] Device scan initiated
 * [00:01] [INFO] Temperature Sensor: Signal 85%
 * [00:01] [ERR ] Humidity Sensor: No response
 * [00:01] [INFO] Motion Detector: Signal 92%
 * [00:01] [INFO] Light Sensor: Signal 78%
 * [00:01] [ERR ] Door Sensor: No response
 * [00:01] [INFO] Scan complete: 3 online, 2 offline
 * [00:01] [WARN] 2 devices need attention
 * [00:01] [INFO] System initialization complete
 * ...
 *
 * Benefits of Multi-Destination Logging:
 * - Serial: Detailed logging, copy/paste capability, permanent record
 * - Screen: Real-time visual feedback, color coding, standalone operation
 * - Synchronized: Identical messages in both destinations
 * - Flexible: Can disable either destination independently
 */
