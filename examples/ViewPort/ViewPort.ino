#include <Arduino.h>
#include <Model1.h>
#include <Video.h>

// Define an accessible variable for the Model 1 & video subsystem instance
Model1 *model1;
Video *video;

void setup()
{
    // Initialize a Model 1 instance to access it
    model1 = new Model1();

    // Be sure to setup the hardware pins
    model1->begin();

    // Defined a viewport that will be used as screen for the video subsystem
    // Only items within these coordinates will be modified
    // This includes scrolling and wrapping strings.
    ViewPort viewPort;
    viewPort.x = 20;
    viewPort.y = 5;
    viewPort.width = 40;
    viewPort.height = 5;

    // Create an instance of the video subsystem, supplying the Model 1 connection
    // and the viewport
    video = new Video(model1, viewPort);

    // Activate the test signal for the Arduino to take over the Model 1 system.
    // You need to activate it, otherwise the Z80 will continue to be in control.
    // The Arduino will also be blocked from accessing the bus without activation.
    model1->activateTestSignal();

    // Set to 64 character mode (in case it isn't)
    video->set64Mode();

    // Sets wether the Model 1 has a lower-case mod
    // Keeping it "false" (default) is the safest option,
    // but converts all lower-case characters to upper-case.
    video->setLowerCaseMod(false);
}

void loop()
{
    // Clear the screen using spaces
    video->cls();
    delay(2000); // Wait 2 seconds

    // Clear the screen using "@" characters
    video->cls('@');
    delay(2000); // Wait 2 seconds

    // Clear the screen using a string
    video->cls("Hello World! ");
    delay(2000); // Wait 2 seconds

    // Scroll 3 rows of the screen. Not supplying any number defaults to 1.
    video->scroll(3);
    delay(2000); // Wait 2 seconds

    // Clear for further test
    video->cls();

    // Print at current cursor location
    // This is on the upper left edge of the screen after a "cls()".
    video->print("UPPER LEFT");
    delay(2000); // Wait 2 seconds

    // Set cursor location to somewhere central
    // Column 20 and row 8. 64 columns and 16 rows is the maximum allowed.
    video->setXY(17, 2);

    // Print at current cursor location
    // With the "Ln" at the end of "print" you can add a new line after the text
    // The cursor will move to the beginning of the next line.
    video->printLn("MIDDLE");
    video->print("START ");
    delay(2000); // Wait 2 seconds

    // Read some string from the screen and write at current cursor position
    video->print(video->read(18, 2, 5));
    delay(2000); // Wait 2 seconds

    // Disable auto-scrolling when the cursor reaches the end of the viewport
    video->setAutoScroll(false);

    // You can also supply the coordinates (X and Y respectively) directly
    video->print("RIGHT BOTTOM", 28, 4);
    delay(2000); // Wait 2 seconds
}
