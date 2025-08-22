/*
 * LoggerScreen.ino - Visual Logging with Rotational Buffer Example
 *
 * This example demonstrates LoggerScreen usage including the rotational
 * buffer feature for preserving log history. See README.md for complete documentation.
 *
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#include <M1Shield.h>
#include <LoggerScreen.h>

// For ST7789 240x320 displays (most common, landscape becomes 320x240)
#include <Display_ST7789_320x240.h>
Display_ST7789_320x240 displayProvider;

// For ST7789 240x240 square displays
// #include <Display_ST7789_240x240.h>
// Display_ST7789_240x240 displayProvider;

// For ST7789 320x170 wide displays (landscape)
// #include <Display_ST7789_320x170.h>
// Display_ST7789_320x170 displayProvider;

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

// ----------------------------------

// Create logger screen instance
LoggerScreen *logger;

// Variables for demonstrating rotational buffer
int messageCount = 0;
unsigned long lastLogTime = 0;
bool bufferDemoComplete = false;
bool screenActivated = false;

void setup()
{
    // Initialize serial for debugging
    Serial.begin(115200);
    Serial.println("=== LoggerScreen with Rotational Buffer Example ===");

    // Initialize M1Shield
    M1Shield.begin(displayProvider);

    Serial.println("M1Shield initialized");

    // Create logger screen with title
    logger = new LoggerScreen("Logger with Buffer");

    // Configure rotational buffer for 15 log entries
    logger->setLogBufferSize(15);
    Serial.println("Rotational buffer configured for 15 entries");

    // Configure logger options
    logger->setColorCodingEnabled(true); // Use color coding
    logger->setTimestampEnabled(true);   // Show timestamps
    logger->setTextSize(1);              // Small text for more lines

    // Pre-fill buffer with some log entries BEFORE activating screen
    // This demonstrates the buffer's ability to capture logs when screen is inactive
    Serial.println("Pre-filling buffer with historical logs...");
    logger->info("System initialization started");
    logger->debug("Loading configuration...");
    logger->info("Configuration loaded successfully");
    logger->warn("Low memory detected: 512 bytes free");
    logger->info("Network module initialized");
    logger->err("Failed to connect to WiFi network");
    logger->warn("Retrying network connection...");
    logger->info("Connected to network successfully");
    logger->debug("System startup complete");
    logger->info("Ready for user interaction");

    Serial.println("Buffer pre-filled with 10 historical entries");
    Serial.print("Buffer contains ");
    Serial.print(logger->getLogBufferCount());
    Serial.println(" entries");

    // Wait 2 seconds to simulate time passing
    delay(2000);

    // NOW activate the screen - this will replay all buffered entries
    Serial.println("Activating logger screen - buffer will be replayed");
    M1Shield.setScreen(logger);
    screenActivated = true;

    Serial.println("Logger screen activated with buffer replay");
    Serial.println("Press Menu button to close and reopen logger");
}

void loop()
{
    // Handle M1Shield processing
    M1Shield.loop();

    unsigned long currentTime = millis();

    // Continue logging new messages every 4 seconds after screen activation
    if (screenActivated && currentTime - lastLogTime >= 4000)
    {
        lastLogTime = currentTime;
        messageCount++;

        // Demonstrate different log levels with realistic messages
        switch (messageCount % 6)
        {
        case 1:
            logger->info("Runtime message #%d: System operating normally", messageCount);
            break;
        case 2:
            logger->debug("Debug info: Processing cycle %d complete", messageCount);
            break;
        case 3:
            logger->warn("Warning #%d: Temperature above normal range", messageCount);
            break;
        case 4:
            logger->info("User action: Button press detected");
            break;
        case 5:
            logger->err("Error #%d: Sensor timeout occurred", messageCount);
            break;
        case 0:
            logger->info("Status: All systems operational, uptime %ds", millis() / 1000);
            break;
        }

        // Show buffer status in serial
        Serial.print("Buffer status: ");
        Serial.print(logger->getLogBufferCount());
        Serial.print("/");
        Serial.print(logger->getLogBufferSize());
        Serial.println(" entries");

        // Reset counter after 30 messages to prevent overflow
        if (messageCount >= 30)
        {
            messageCount = 0;
            logger->info("Session complete - restarting message cycle");
        }
    }

    // Small delay for smooth operation
    delay(100);
}
