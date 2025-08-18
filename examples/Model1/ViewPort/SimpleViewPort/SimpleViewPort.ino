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

    // Take control of the TRS-80 bus
    Model1.activateTestSignal();

    // Set to 64 character mode
    cassette.set64CharacterMode();

    // Configure video system
    video.setLowerCaseMod(false);

    // Define a viewport - a rectangular area that constrains all video operations
    ViewPort viewPort;
    viewPort.x = 10;      // Start 10 characters from left edge
    viewPort.y = 3;       // Start 3 rows from top
    viewPort.width = 44;  // 44 characters wide
    viewPort.height = 10; // 10 rows tall

    // Apply the viewport to video operations
    video.setViewPort(viewPort);
}

void loop()
{
    // Show the viewport boundaries
    demonstrateViewportBoundaries();
    delay(3000);

    // Basic operations within viewport
    demonstrateBasicViewportOperations();
    delay(3000);

    // Show clipping behavior
    demonstrateViewportClipping();
    delay(3000);

    // Show scrolling within viewport
    demonstrateViewportScrolling();
    delay(3000);
}

void demonstrateViewportBoundaries()
{
    // Clear entire screen first (outside viewport)
    ViewPort fullScreen;
    fullScreen.x = 0;
    fullScreen.y = 0;
    fullScreen.width = 64;
    fullScreen.height = 16;
    video.setViewPort(fullScreen);
    video.cls('.');

    // Reset to our working viewport
    ViewPort workingViewPort;
    workingViewPort.x = 10;
    workingViewPort.y = 3;
    workingViewPort.width = 44;
    workingViewPort.height = 10;
    video.setViewPort(workingViewPort);

    // Clear viewport with different character to show boundaries
    video.cls(' ');

    // Draw border around viewport content area
    video.print(0, 0, "+-----------------------------------------+");
    for (int row = 1; row < 9; row++)
    {
        video.print(0, row, "|");
        video.print(43, row, "|");
    }
    video.print(0, 9, "+-----------------------------------------+");

    // Label the viewport
    video.print(15, 4, "VIEWPORT AREA");
    video.print(18, 5, "44x10 CHARS");
}

void demonstrateBasicViewportOperations()
{
    video.cls();

    // Show cursor starts at viewport's top-left (0,0 relative to viewport)
    video.print("CURSOR AT VIEWPORT ORIGIN");
    video.println();
    video.println();

    // Basic text output within viewport
    video.println("THIS TEXT STAYS WITHIN");
    video.println("THE DEFINED VIEWPORT");
    video.println();

    // Position text within viewport
    video.print(5, 6, "POSITIONED TEXT");
    video.print(25, 7, "MORE TEXT");

    // Show viewport coordinates vs screen coordinates
    video.print(0, 8, "VIEWPORT COORDS");
    video.print(0, 9, "SCREEN: (");
    video.print(video.getStartX());
    video.print(",");
    video.print(video.getStartY());
    video.print(")");
}

void demonstrateViewportClipping()
{
    video.cls();

    video.println("CLIPPING DEMONSTRATION:");
    video.println();

    // Try to print text that would exceed viewport width
    video.println("THIS IS A VERY LONG LINE THAT WILL BE CLIPPED WHEN IT EXCEEDS THE VIEWPORT WIDTH");
    video.println();

    // Try to position text near/past viewport edges
    video.print(40, 5, "EDGE TEXT"); // Near right edge
    video.print(45, 6, "CLIPPED");   // Past right edge - should be clipped

    // Show that coordinates are relative to viewport
    video.print(0, 8, "LEFT EDGE");
    video.print(38, 9, "RIGHT"); // Near right edge of viewport
}

void demonstrateViewportScrolling()
{
    video.cls();

    video.println("SCROLLING IN VIEWPORT:");
    video.println();

    // Fill viewport with numbered lines to demonstrate scrolling
    for (int i = 1; i <= 15; i++)
    {
        video.print("LINE ");
        video.print(i);
        video.print(" - VIEWPORT SCROLL TEST");
        video.println();
        delay(300);
    }

    delay(1000);

    // Manual scrolling demonstration
    video.println();
    video.println("MANUAL SCROLL:");
    delay(1000);

    video.scroll(2); // Scroll 2 lines
    video.println("SCROLLED 2 LINES");
    delay(1000);

    video.scroll(); // Scroll 1 line (default)
    video.println("SCROLLED 1 MORE LINE");
}
