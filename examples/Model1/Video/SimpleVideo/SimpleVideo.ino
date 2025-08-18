#include <Arduino.h>
#include <Model1.h>
#include <Video.h>
#include <Cassette.h>

// Video class instance for TRS-80 screen control
Video video;

// Cassette class needed for 32/64 character mode selection
Cassette cassette;

void setup()
{
    // Initialize the Model 1 hardware interface
    Model1.begin();

    // Take control of the TRS-80 bus (required for video operations)
    Model1.activateTestSignal();

    // Set to 64 character mode for maximum screen width
    cassette.set64CharacterMode();

    // Configure for systems without lowercase modification
    // Set to true if your TRS-80 has a lowercase mod installed
    video.setLowerCaseMod(false);
}

void loop()
{
    // Basic screen clearing demonstrations
    demonstrateScreenClearing();
    delay(2000);

    // Basic text output demonstrations
    demonstrateTextOutput();
    delay(2000);

    // Basic cursor positioning demonstrations
    demonstrateCursorPositioning();
    delay(2000);

    // Basic scrolling demonstration
    demonstrateScrolling();
    delay(2000);
}

void demonstrateScreenClearing()
{
    // Clear screen with spaces (default)
    video.cls();
    delay(1000);

    // Clear screen with a specific character
    video.cls('*');
    delay(1000);

    // Clear screen with a repeating string
    video.cls("HELLO ");
    delay(1000);
}

void demonstrateTextOutput()
{
    video.cls();

    // Print at current cursor location (top-left after cls)
    video.print("BASIC VIDEO OUTPUT");
    delay(1000);

    // Move to new line and print
    video.println();
    video.println("SECOND LINE");
    delay(1000);

    // Print at specific coordinates
    video.print(10, 5, "POSITIONED TEXT");
    delay(1000);
}

void demonstrateCursorPositioning()
{
    video.cls();

    // Get and display current cursor position
    video.print("CURSOR AT: ");
    video.print(video.getX());
    video.print(",");
    video.print(video.getY());
    delay(1000);

    // Move cursor to center of screen
    video.setXY(25, 8);
    video.print("CENTER");
    delay(1000);

    // Move cursor to bottom-right area
    video.setXY(50, 14);
    video.print("BOTTOM RIGHT");
    delay(1000);
}

void demonstrateScrolling()
{
    video.cls();

    // Fill screen with numbered lines
    for (int i = 1; i <= 20; i++)
    {
        video.print("LINE ");
        video.print(i);
        video.println();
    }
    delay(2000);

    // Demonstrate single-line scrolling
    video.scroll();
    delay(1000);

    // Demonstrate multi-line scrolling
    video.scroll(3);
    delay(2000);
}
