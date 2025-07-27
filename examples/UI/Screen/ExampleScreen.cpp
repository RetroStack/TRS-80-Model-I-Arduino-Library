/**
 * Simple Example Screen - Implementation
 *
 * This shows how to create a basic interactive screen with the M1Shield.
 * Great starting point for beginners!
 */

#include "ExampleScreen.h"
#include "M1Shield.h"

// Constructor - runs once when screen is created
ExampleScreen::ExampleScreen()
{
    // Initialize our variables
    _buttonPressCount = 0;
    _ledColor = 0;
}

// This method draws everything you see on the screen
void ExampleScreen::_drawScreen()
{
    // Get access to the display
    Adafruit_GFX &display = M1Shield.getGFX();

    // Clear the screen (make it black)
    display.fillScreen(ST77XX_BLACK);

    // Set text color to white and size
    display.setTextColor(ST77XX_WHITE);
    display.setTextSize(2);

    // Show a title at the top
    display.setCursor(10, 10);
    display.print("Simple Screen");

    // Show some instructions
    display.setTextSize(1);
    display.setCursor(10, 40);
    display.print("Press any button!");

    // Show how many times buttons were pressed
    display.setCursor(10, 60);
    display.print("Button presses: ");
    display.print(_buttonPressCount);

    // Show current LED color
    display.setCursor(10, 80);
    display.print("LED Color: ");
    display.print(_ledColor);

    // Add some helpful text
    display.setCursor(10, 100);
    display.print("Up/Down: Change LED");
    display.setCursor(10, 110);
    display.print("Menu: Reset counter");
}

// This method runs continuously while the screen is active
void ExampleScreen::loop()
{
    // Set the LED to our current color
    M1Shield.setLEDColor(static_cast<LEDColor>(_ledColor));
}

// This method is called whenever the user presses a button or moves the joystick
Screen *ExampleScreen::actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY)
{
    // This method gets called only when something was pressed

    // Count this button press
    _buttonPressCount++;

    // Handle specific buttons
    if (action & (JOYSTICK_UP | BUTTON_UP))
    {
        // Up button: next LED color
        _ledColor++;
        if (_ledColor > 7)
            _ledColor = 0; // Wrap around (colors 0-7)
    }
    else if (action & (JOYSTICK_DOWN | BUTTON_DOWN))
    {
        // Down button: previous LED color
        _ledColor--;
        if (_ledColor < 0)
            _ledColor = 7; // Wrap around
    }
    else if (action & BUTTON_MENU)
    {
        // Menu button: reset the counter
        _buttonPressCount = 0;
    }

    // Tell the system to redraw the screen since something changed
    refresh();

    // Return nullptr to stay on this screen
    // (You could return a different screen to switch to it)
    return nullptr;
}
