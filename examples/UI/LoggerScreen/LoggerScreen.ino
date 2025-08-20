/*
 * LoggerScreen.ino - Basic Visual Logging Example
 *
 * This example demonstrates basic LoggerScreen usage for visual logging
 * on the M1Shield display. See README.md for complete documentation.
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

// Simple variables for basic logging demo
int messageCount = 0;
unsigned long lastLogTime = 0;

void setup()
{
    // Initialize serial for debugging
    Serial.begin(115200);
    Serial.println("=== Basic LoggerScreen Example ===");

    // Initialize M1Shield
    M1Shield.begin(displayProvider);

    Serial.println("M1Shield initialized");

    // Create logger screen with title
    logger = new LoggerScreen("Basic Logger");

    // Configure basic logger options
    logger->setColorCodingEnabled(true); // Use color coding
    logger->setTextSize(1);              // Small text for more lines

    // Set the logger screen as active
    M1Shield.setScreen(logger);

    // Initial welcome messages
    logger->info("Basic Logger starting...");
    logger->info("LoggerScreen initialized");
    logger->info("Ready for basic logging demo");

    Serial.println("Logger screen activated");
    Serial.println("Press Menu button to exit");
}

void loop()
{
    // Handle M1Shield processing
    M1Shield.loop();

    unsigned long currentTime = millis();

    // Log a simple message every 3 seconds
    if (currentTime - lastLogTime >= 3000)
    {
        lastLogTime = currentTime;
        messageCount++;

        // Demonstrate different log levels
        logger->info("Message #%d: Basic info log", messageCount);

        // Occasionally show warning and error messages
        if (messageCount % 3 == 0)
        {
            logger->warn("Warning #%d: This is a warning message", messageCount / 3);
        }

        if (messageCount % 5 == 0)
        {
            logger->err("Error #%d: This is an error message", messageCount / 5);
        }

        if (messageCount % 2 == 0)
        {
            logger->debug("This is a debug message.");
        }

        // Reset counter after 20 messages to prevent overflow
        if (messageCount >= 20)
        {
            messageCount = 0;
            logger->info("Message counter reset - starting over");
        }
    }

    // Small delay for smooth operation
    delay(100);
}
