#include <Arduino.h>
#include <Model1.h>
#include <Video.h>
#include <Cassette.h>
#include <SerialLogger.h>

// Video class instance for TRS-80 screen control
Video video;

// Cassette class needed for 32/64 character mode selection
Cassette cassette;

// Logger for debugging viewport operations
SerialLogger logger;

// Multiple viewport configurations for testing
ViewPort fullScreen = {0, 0, 64, 16};
ViewPort mainWindow = {5, 2, 54, 12};
ViewPort smallWindow = {20, 5, 24, 6};
ViewPort narrowColumn = {60, 0, 4, 16};
ViewPort statusBar = {0, 15, 64, 1};

void setup()
{
    // Initialize serial communication
    Serial.begin(115200);
    delay(1000);

    // Initialize the Model 1 hardware interface
    Model1.begin();

    // Set up logging
    video.setLogger(logger);

    // Take control of the TRS-80 bus
    Model1.activateTestSignal();

    // Configure video system
    cassette.set64CharacterMode();
    video.setLowerCaseMod(false);

    Serial.println(F("=== Comprehensive ViewPort Example ==="));
    Serial.println(F("Testing multiple viewport configurations"));
}

void loop()
{
    // Multiple viewport demonstrations
    demonstrateMultipleViewports();
    delay(4000);

    // Advanced viewport operations
    demonstrateAdvancedViewportOperations();
    delay(4000);

    // Viewport boundary analysis
    demonstrateViewportAnalysis();
    delay(4000);

    // Dynamic viewport switching
    demonstrateDynamicViewportSwitching();
    delay(4000);

    // Viewport memory operations
    demonstrateViewportMemoryOperations();
    delay(4000);

    // Edge case testing
    demonstrateViewportEdgeCases();
    delay(4000);
}

void demonstrateMultipleViewports()
{
    Serial.println(F("\\n--- Multiple Viewport Demonstration ---"));

    // Start with full screen viewport
    video.setViewPort(fullScreen);
    video.cls('.');

    // Create main content window
    video.setViewPort(mainWindow);
    video.cls(' ');
    drawBorder("MAIN WINDOW");

    // Create small overlay window
    video.setViewPort(smallWindow);
    video.cls(' ');
    drawBorder("SMALL");

    // Create status bar
    video.setViewPort(statusBar);
    video.cls('-');
    video.print(2, 0, "STATUS: MULTI-VIEWPORT DEMO ACTIVE");

    // Create narrow side column
    video.setViewPort(narrowColumn);
    video.cls(' ');
    video.print(0, 0, "SIDE");
    video.print(1, 1, "BAR");

    Serial.println(F("Multiple viewports created with different sizes"));
    delay(3000);
}

void demonstrateAdvancedViewportOperations()
{
    Serial.println(F("\\n--- Advanced ViewPort Operations ---"));

    // Work in main window
    video.setViewPort(mainWindow);
    video.cls();

    video.println("ADVANCED VIEWPORT FEATURES:");
    video.println();

    // Demonstrate coordinate system
    video.print("VIEWPORT SIZE: ");
    video.print(video.getWidth());
    video.print(" x ");
    video.println(video.getHeight());

    video.print("SCREEN POSITION: (");
    video.print(video.getStartX());
    video.print(",");
    video.print(video.getStartY());
    video.print(") to (");
    video.print(video.getEndX());
    video.print(",");
    video.print(video.getEndY());
    video.println(")");

    video.print("TOTAL CHARACTERS: ");
    video.println(video.getSize());
    video.println();

    // Test reading from viewport
    video.println("READING TEST:");
    video.print(5, 8, "READ THIS TEXT");
    char *readText = video.read(5, 8, 4, false);
    video.print(0, 9, "READ: '");
    if (readText) {
        video.print(readText);
        free(readText);
    } else {
        video.print("ERROR");
    }
    video.println("'");

    Serial.println(F("Advanced viewport operations completed"));
}

void demonstrateViewportAnalysis()
{
    Serial.println(F("\\n--- ViewPort Analysis ---"));

    video.setViewPort(mainWindow);
    video.cls();

    video.println("VIEWPORT COORDINATE ANALYSIS:");
    video.println();

    // Show how viewport coordinates translate to screen coordinates
    for (int vp_y = 0; vp_y < 5; vp_y++)
    {
        for (int vp_x = 0; vp_x < 10; vp_x += 5)
        {
            uint8_t screen_x = video.getAbsoluteX(vp_x);
            uint8_t screen_y = video.getAbsoluteY(vp_y);
            uint16_t address = video.getAddress(vp_x, vp_y);

            video.print("VP(");
            video.print(vp_x);
            video.print(",");
            video.print(vp_y);
            video.print(")=SCR(");
            video.print(screen_x);
            video.print(",");
            video.print(screen_y);
            video.print(")");

            if (vp_x == 0)
            {
                video.print(" @0x");
                video.print(address, HEX);
            }
            video.println();
        }
    }

    Serial.println(F("Viewport coordinate analysis completed"));
}

void demonstrateDynamicViewportSwitching()
{
    Serial.println(F("\\n--- Dynamic ViewPort Switching ---"));

    // Animate viewport size changes
    for (int size = 10; size <= 50; size += 5)
    {
        ViewPort dynamicViewPort;
        dynamicViewPort.x = (64 - size) / 2;
        dynamicViewPort.y = (16 - size / 3) / 2;
        dynamicViewPort.width = size;
        dynamicViewPort.height = size / 3;

        video.setViewPort(dynamicViewPort);
        video.cls();

        video.print("SIZE: ");
        video.print(size);
        video.print(" x ");
        video.println(size / 3);

        video.print("DYNAMIC VIEWPORT");
        video.println();
        video.print("ANIMATION TEST");

        delay(500);
    }

    // Return to main window
    video.setViewPort(mainWindow);
    video.cls();
    video.println("DYNAMIC SWITCHING COMPLETE");

    Serial.println(F("Dynamic viewport switching completed"));
}

void demonstrateViewportMemoryOperations()
{
    Serial.println(F("\\n--- ViewPort Memory Operations ---"));

    video.setViewPort(smallWindow);
    video.cls();

    // Fill viewport with pattern
    video.cls("AB");
    delay(1000);

    // Read memory from current viewport area
    Model1.activateTestSignal();

    Serial.println(F("\\nViewPort Memory Contents:"));
    for (uint8_t y = 0; y < video.getHeight(); y++)
    {
        Serial.print(F("Row "));
        Serial.print(y);
        Serial.print(F(": "));

        for (uint8_t x = 0; x < video.getWidth(); x++)
        {
            uint16_t address = video.getAddress(x, y);
            uint8_t memByte = Model1.readMemory(address);
            char character = video.convertModel1CharacterToLocal(memByte);

            if (character >= 32 && character <= 126)
                Serial.print(character);
            else
                Serial.print('.');
        }
        Serial.println();
    }

    Model1.deactivateTestSignal();

    // Show memory address mapping
    video.cls();
    video.println("MEMORY MAPPING:");
    uint16_t startAddr = video.getAddress(0, 0);
    uint16_t endAddr = video.getAddress(video.getWidth() - 1, video.getHeight() - 1);

    video.print("START: 0x");
    video.println(startAddr, HEX);
    video.print("END: 0x");
    video.println(endAddr, HEX);

    Serial.println(F("Viewport memory operations completed"));
}

void demonstrateViewportEdgeCases()
{
    Serial.println(F("\\n--- ViewPort Edge Cases ---"));

    // Test minimum viewport
    ViewPort miniViewPort = {30, 8, 4, 2};
    video.setViewPort(miniViewPort);
    video.cls();
    video.print("MINI");
    video.print(0, 1, "4x2");
    delay(2000);

    // Test single character viewport
    ViewPort singleChar = {32, 8, 1, 1};
    video.setViewPort(singleChar);
    video.cls();
    video.print("X");
    delay(1000);

    // Test viewport at screen edges
    ViewPort rightEdge = {63, 0, 1, 16};
    video.setViewPort(rightEdge);
    video.cls('|');
    delay(1000);

    ViewPort bottomEdge = {0, 15, 64, 1};
    video.setViewPort(bottomEdge);
    video.cls('-');
    video.print(25, 0, "BOTTOM EDGE");
    delay(2000);

    // Test auto-scroll in small viewport
    video.setViewPort(miniViewPort);
    video.cls();
    video.setAutoScroll(true);

    for (int i = 1; i <= 5; i++)
    {
        video.print("L");
        video.println(i);
        delay(800);
    }

    Serial.println(F("Edge case testing completed"));

    // Reset to main window for next iteration
    video.setViewPort(mainWindow);
}

void drawBorder(const char *title)
{
    // Draw top border
    video.print(0, 0, "+");
    for (int x = 1; x < video.getWidth() - 1; x++)
    {
        video.print("-");
    }
    video.println("+");

    // Draw title in top border
    if (title && strlen(title) > 0)
    {
        uint8_t titleStart = (video.getWidth() - strlen(title) - 2) / 2;
        video.print(titleStart, 0, " ");
        video.print(title);
        video.print(" ");
    }

    // Draw side borders
    for (int y = 1; y < video.getHeight() - 1; y++)
    {
        video.print(0, y, "|");
        video.print(video.getWidth() - 1, y, "|");
    }

    // Draw bottom border
    video.print(0, video.getHeight() - 1, "+");
    for (int x = 1; x < video.getWidth() - 1; x++)
    {
        video.print("-");
    }
    video.print("+");
}
