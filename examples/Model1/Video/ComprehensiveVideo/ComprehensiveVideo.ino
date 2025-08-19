#include <Arduino.h>
#include <Model1.h>
#include <Video.h>
#include <Cassette.h>
#include <SerialLogger.h>

// Video class instance for TRS-80 screen control
Video video;

// Cassette class needed for 32/64 character mode selection
Cassette cassette;

// Logger for debugging and information output
SerialLogger logger;

// Buffer for video memory operations
uint8_t videoMemoryBuffer[1024];

void setup()
{
    // Initialize serial communication
    Serial.begin(115200);
    delay(1000);

    // Initialize the Model 1 hardware interface
    Model1.begin();

    // Set up logging for video operations
    video.setLogger(logger);

    // Take control of the TRS-80 bus
    Model1.activateTestSignal();

    // Configure video system
    cassette.set64CharacterMode();
    video.setLowerCaseMod(false);
    video.setAutoScroll(true);

    Serial.println(F("=== Comprehensive Video Example ==="));
    Serial.println(F("Demonstrating advanced video operations"));
}

void loop()
{
    // Advanced screen operations
    demonstrateAdvancedScreenOperations();
    delay(3000);

    // Character conversion demonstrations
    demonstrateCharacterConversion();
    delay(3000);

    // Memory reading and analysis
    demonstrateMemoryOperations();
    delay(3000);

    // Viewport information
    demonstrateViewportInformation();
    delay(3000);

    // Performance and auto-scroll testing
    demonstratePerformanceFeatures();
    delay(3000);

    // Character mode switching
    demonstrateCharacterModes();
    delay(3000);
}

void demonstrateAdvancedScreenOperations()
{
    Serial.println(F("--- Advanced Screen Operations ---"));

    video.cls();

    // Test different clear patterns
    video.cls("*-*-");
    Serial.println(F("Cleared with pattern: *-*-"));
    delay(1500);

    // Advanced positioning with bounds checking
    video.cls();
    video.print(20, 0, "TESTING BOUNDARIES");

    // Test edge positions
    video.print(0, 0, "TOP-LEFT ");
    video.print(55, 0, "TOP-RIGHT");
    video.print(0, 15, "BOTTOM-LEFT");
    video.print(51, 15, "BOTTOM-RIGHT");

    // Test center positioning
    uint8_t centerX = (video.getWidth() - 6) / 2; // 6 = length of "CENTER"
    uint8_t centerY = video.getHeight() / 2;
    video.print(centerX, centerY, "CENTER");

    Serial.println(F("Positioned text at screen boundaries and center"));
    delay(2000);
}

void demonstrateCharacterConversion()
{
    Serial.println(F("--- Character Conversion ---"));

    video.cls();
    video.println("CHARACTER CONVERSION TEST:");
    video.println();

    // Test character conversion both ways
    const char testChars[] = "ABCabc123!@#";

    video.print("ORIGINAL: ");
    video.println(testChars);

    video.print("TO MODEL1: ");
    for (int i = 0; testChars[i] != '\0'; i++)
    {
        char converted = video.convertLocalCharacterToModel1(testChars[i]);
        video.print(converted, true); // Print raw to avoid double conversion
    }
    video.println();

    video.print("BACK TO LOCAL: ");
    for (int i = 0; testChars[i] != '\0'; i++)
    {
        char toModel1 = video.convertLocalCharacterToModel1(testChars[i]);
        char backToLocal = video.convertModel1CharacterToLocal(toModel1);
        video.print(backToLocal);
    }

    Serial.println(F("Character conversion test completed"));
    delay(3000);
}

void demonstrateMemoryOperations()
{
    Serial.println(F("--- Memory Operations ---"));

    video.cls();
    video.println("MEMORY DUMP EXAMPLE");
    video.println("CHECK SERIAL OUTPUT");

    // Quickly read video memory
    for (uint16_t i = 0; i < 1024; i++)
    {
        videoMemoryBuffer[i] = Model1.readMemory(0x3C00 + i);
    }

    // Analyze and display memory contents
    Serial.println(F("=== VIDEO MEMORY DUMP ==="));
    Serial.println(F("Address  | Hex Data                    | ASCII"));
    Serial.println(F("---------|-----------------------------|---------"));

    for (uint16_t row = 0; row < 16; row++)
    {
        Serial.print(F("Row "));
        if (row < 10)
            Serial.print('0');
        Serial.print(row);
        Serial.print(F("   | "));

        // Print hex values for one row (64 characters)
        for (uint8_t col = 0; col < 64; col += 8)
        {
            for (uint8_t i = 0; i < 8 && (col + i) < 64; i++)
            {
                uint16_t addr = (col + i) + (row * 64);
                if (videoMemoryBuffer[addr] < 16)
                    Serial.print('0');
                Serial.print(videoMemoryBuffer[addr], HEX);
                Serial.print(' ');
            }
            Serial.print(F("| "));
        }

        // Print ASCII representation
        Serial.print(F("| "));
        for (uint8_t col = 0; col < 64; col++)
        {
            uint16_t addr = col + (row * 64);
            char ch = video.convertModel1CharacterToLocal(videoMemoryBuffer[addr]);
            if (ch >= 32 && ch <= 126)
                Serial.print(ch);
            else
                Serial.print('.');
        }
        Serial.println();
    }

    delay(2000);
}

void demonstrateViewportInformation()
{
    Serial.println(F("--- Viewport Information ---"));

    video.cls();
    video.println("VIEWPORT INFO:");

    // Display viewport dimensions
    video.print("SIZE: ");
    video.print(video.getWidth());
    video.print("X");
    video.print(video.getHeight());
    video.println();

    video.print("START: (");
    video.print(video.getStartX());
    video.print(",");
    video.print(video.getStartY());
    video.println(")");

    video.print("END: (");
    video.print(video.getEndX());
    video.print(",");
    video.print(video.getEndY());
    video.println(")");

    video.print("TOTAL CHARS: ");
    video.println(video.getSize());

    // Demonstrate address calculation
    video.println();
    video.println("ADDRESS CALCULATION:");
    uint8_t testX = 10, testY = 5;
    uint16_t address = video.getAddress(testX, testY);
    video.print("ADDR(");
    video.print(testX);
    video.print(",");
    video.print(testY);
    video.print(") = 0x");
    video.println(address, HEX);

    Serial.println(F("Viewport information displayed"));
    delay(3000);
}

void demonstratePerformanceFeatures()
{
    Serial.println(F("--- Performance Features ---"));

    video.cls();
    video.println("PERFORMANCE TEST");

    // Test auto-scroll behavior
    video.println("TESTING AUTO-SCROLL...");
    video.setAutoScroll(true);

    // Fill screen to trigger scrolling
    for (int i = 1; i <= 25; i++)
    {
        video.print("LINE ");
        video.print(i);
        video.print(" - AUTO SCROLL TEST");
        video.println();
        delay(200);
    }

    delay(1000);

    // Test without auto-scroll
    video.cls();
    video.println("TESTING WITHOUT AUTO-SCROLL...");
    video.setAutoScroll(false);

    // This should wrap to top when reaching bottom
    for (int i = 1; i <= 20; i++)
    {
        video.print("LINE ");
        video.print(i);
        video.print(" - NO AUTO SCROLL");
        video.println();
        delay(100);
    }

    // Re-enable auto-scroll for normal operation
    video.setAutoScroll(true);

    Serial.println(F("Performance test completed"));
    delay(2000);
}

void demonstrateCharacterModes()
{
    Serial.println(F("--- Character Mode Switching ---"));

    video.cls();
    video.println("CHARACTER MODE TEST");
    video.println();

    // Test 64-character mode
    video.println("64-CHAR MODE:");
    // These characters will automatically be mapped as best as possible
    video.println("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()_+-=[]{}|;:,.<>?");
    delay(2000);

    // Switch to 32-character mode
    cassette.set32CharacterMode();
    delay(500);

    video.cls();
    video.println("32-CHAR MODE:");
    video.println("ABCDEFGHIJKLMNOPQRSTUVWXYZ012345");
    video.println("NOTICE THE DOUBLING EFFECT");
    delay(3000);

    // Switch back to 64-character mode
    cassette.set64CharacterMode();
    delay(500);

    video.cls();
    video.println("BACK TO 64-CHAR MODE");
    video.println("FULL WIDTH RESTORED");

    Serial.println(F("Character mode switching completed"));
    delay(2000);
}
