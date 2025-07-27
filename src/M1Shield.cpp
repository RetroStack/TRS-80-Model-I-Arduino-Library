/**
 * @file M1Shield.cpp
 * @brief Implementation of the M1Shield hardware abstraction layer
 *
 * This file provides the complete implementation of the M1ShieldClass,
 * managing all hardware components of the TRS-80 Model I Arduino Shield
 * including display, input devices, and status indicators.
 */

#include "M1Shield.h"
#include <Adafruit_GFX.h>
#include <SPI.h>
#include <Arduino.h>
#include "Model1.h"

// Hardware timing constants
constexpr unsigned long DEBOUNCE_TIME = 250; // Button debounce time in milliseconds

// RGB LED pin assignments
constexpr uint8_t PIN_ACTIVE_LED = 41; // Activity indicator LED pin
constexpr uint8_t PIN_LED_BLUE = A9;   // Blue channel of RGB LED
constexpr uint8_t PIN_LED_GREEN = A10; // Green channel of RGB LED
constexpr uint8_t PIN_LED_RED = A11;   // Red channel of RGB LED

// Button pins
constexpr uint8_t PIN_MENU = 10;
constexpr uint8_t PIN_LEFT = 7;
constexpr uint8_t PIN_RIGHT = 6;
constexpr uint8_t PIN_DOWN = 5;
constexpr uint8_t PIN_UP = 4;

// Joystick pins
constexpr uint8_t PIN_JOYSTICK_BUTTON = 39;
constexpr uint8_t PIN_JOYSTICK_X = A12;
constexpr uint8_t PIN_JOYSTICK_Y = A13;

constexpr uint8_t JOYSTICK_CENTER_MIN = 100;
constexpr uint8_t JOYSTICK_CENTER_MAX = 155;

// Define global instance
M1ShieldClass M1Shield;

/**
 * Constructor: Initializes internal state.
 */
M1ShieldClass::M1ShieldClass() : 
#if defined(USE_ILI9488)
    // ILI9488 uses Arduino_GFX and requires special initialization in begin()
    _tft(nullptr, 320, 480), // Temporary construction - proper bus will be set in begin()
#elif defined(USE_ST7789) || defined(USE_ST7789_240x240)
    _tft(TFT_CS, TFT_DC, TFT_RST),
#elif defined(USE_ST7735)
    _tft(TFT_CS, TFT_DC, TFT_RST),
#elif defined(USE_ILI9341)
    _tft(TFT_CS, TFT_DC, TFT_RST),
#elif defined(USE_HX8357)
    _tft(TFT_CS, TFT_DC, TFT_RST),
#elif defined(USE_ILI9325)
    _tft(TFT_CS, TFT_DC, TFT_RST),
#else
    _tft(TFT_CS, TFT_DC, TFT_RST),
#endif
    _menuPressed(0),
    _upPressed(0),
    _downPressed(0),
    _leftPressed(0),
    _rightPressed(0),
    _joystickPressed(0),
    _screenWidth(0),
    _screenHeight(0),
    _screen(nullptr)
{
}

/**
 * Destructor: Cleans up resources.
 */
M1ShieldClass::~M1ShieldClass()
{
    // Close and delete the current screen if it exists
    if (_screen)
    {
        _screen->close();
        delete _screen;
        _screen = nullptr;
    }
}

/**
 * Initializes hardware: pins, display, LED, and input modes.
 */
void M1ShieldClass::begin()
{
    pinMode(PIN_ACTIVE_LED, OUTPUT);
    _inactive();

    pinMode(PIN_LED_RED, OUTPUT);
    pinMode(PIN_LED_GREEN, OUTPUT);
    pinMode(PIN_LED_BLUE, OUTPUT);
    setLEDColor(LEDColor::COLOR_OFF);

    pinMode(PIN_MENU, INPUT_PULLUP);
    pinMode(PIN_LEFT, INPUT_PULLUP);
    pinMode(PIN_RIGHT, INPUT_PULLUP);
    pinMode(PIN_DOWN, INPUT_PULLUP);
    pinMode(PIN_UP, INPUT_PULLUP);

    pinMode(PIN_JOYSTICK_BUTTON, INPUT_PULLUP);
    pinMode(PIN_JOYSTICK_X, INPUT);
    pinMode(PIN_JOYSTICK_Y, INPUT);

    // Initialize display with error handling
    // Manual Reset Sequence
    if (TFT_RST >= 0)
    {
        pinMode(TFT_RST, OUTPUT);
        digitalWrite(TFT_RST, LOW);
        delay(50);
        digitalWrite(TFT_RST, HIGH);
        delay(50);
    }

    // Initialize display based on the selected type
    _screenWidth = DISPLAY_WIDTH;
    _screenHeight = DISPLAY_HEIGHT;

#if defined(USE_ST7789)
    _tft.init(_screenWidth, _screenHeight, SPI_MODE0);
    _tft.enableTearing(true);
    _tft.setRotation(3);
#elif defined(USE_ST7789_240x240)
    _tft.init(_screenWidth, _screenHeight, SPI_MODE0);
    _tft.enableTearing(true);
    _tft.setRotation(3);
#elif defined(USE_ST7735)
    _tft.initR(INITR_BLACKTAB);
    _tft.setRotation(3);
#elif defined(USE_ILI9341)
    _tft.begin();
    _tft.setRotation(3);
#elif defined(USE_ILI9488)
    // ILI9488 requires special initialization with Arduino_GFX
    // The current architecture doesn't properly support Arduino_GFX initialization
    // This is a temporary fix - proper support requires architectural changes
    _tft.begin();
    _tft.setRotation(3);
#elif defined(USE_HX8357)
    _tft.begin();
    _tft.setRotation(3);
#elif defined(USE_ILI9325)
    _tft.begin();
    _tft.setRotation(3);
#else
    // Default to ST7789
    _tft.init(_screenWidth, _screenHeight, SPI_MODE0);
    _tft.enableTearing(true);
    _tft.setRotation(3);
#endif
}

/**
 * Returns true if the display has been successfully initialized.
 */
bool M1ShieldClass::isDisplayInitialized() const
{
    return (_screenWidth > 0 && _screenHeight > 0);
}

/**
 * Returns a reference to the display's Adafruit_GFX interface.
 * Allows drawing to the TFT display.
 */
Adafruit_GFX &M1ShieldClass::getGFX()
{
    return _tft;
}

/**
 * Returns the width of the screen. It is 0 when not initialized.
 */
uint16_t M1ShieldClass::getScreenWidth() const
{
    return _screenWidth;
}

/**
 * Returns the height of the screen. It is 0 when not initialized.
 */
uint16_t M1ShieldClass::getScreenHeight() const
{
    return _screenHeight;
}

/**
 * Sets the screen used to display items on the screen.
 *
 * NOTE: After assigning it to the shield, it completely owns the value and potentially
 *       frees it at some point. Do not depend on the value to exist beyond.
 */
bool M1ShieldClass::setScreen(Screen *screen)
{
    if (!screen)
        return false;

    // Free up the old screen to avoid memory leaks
    if (_screen)
    {
        _screen->close();
        delete _screen;
        _screen = nullptr;
    }

    if (!screen->open())
    {
        // Screen failed to open, clean up
        delete screen;
        _screen = nullptr;
        return false;
    }

    _screen = screen;
    return true;
}

/**
 * Turns on the active status LED.
 */
void M1ShieldClass::_active() const
{
    digitalWrite(PIN_ACTIVE_LED, LOW);
}

/**
 * Turns off the active status LED.
 */
void M1ShieldClass::_inactive() const
{
    digitalWrite(PIN_ACTIVE_LED, HIGH);
}

/**
 * Sets the RGB LED to the given red, green, blue state.
 */
void M1ShieldClass::setLEDColor(uint8_t r, uint8_t g, uint8_t b) const
{
    analogWrite(PIN_LED_RED, 255 - r);
    analogWrite(PIN_LED_GREEN, 255 - g);
    analogWrite(PIN_LED_BLUE, 255 - b);
}

/**
 * Sets the RGB LED to a predefined color.
 */
void M1ShieldClass::setLEDColor(LEDColor color, uint8_t intensity) const
{
    switch (color)
    {
    case LEDColor::COLOR_OFF:
        setLEDColor(0, 0, 0);
        break;
    case LEDColor::COLOR_RED:
        setLEDColor(255, 0, 0);
        break;
    case LEDColor::COLOR_GREEN:
        setLEDColor(0, 255, 0);
        break;
    case LEDColor::COLOR_BLUE:
        setLEDColor(0, 0, 255);
        break;
    case LEDColor::COLOR_YELLOW:
        setLEDColor(255, 255, 0);
        break;
    case LEDColor::COLOR_MAGENTA:
        setLEDColor(255, 0, 255);
        break;
    case LEDColor::COLOR_CYAN:
        setLEDColor(0, 255, 255);
        break;
    case LEDColor::COLOR_WHITE:
        setLEDColor(255, 255, 255);
        break;
    }
}

/**
 * Internal debouncing helper for buttons.
 * Returns updated timestamp if still pressed, 0 if released.
 */
unsigned long M1ShieldClass::_getDebouncedState(int pin, unsigned long previousState) const
{
    if (digitalRead(pin) == LOW)
    {
        if (previousState == 0)
        {
            return millis(); // First detection
        }
        else if ((millis() - previousState) > DEBOUNCE_TIME)
        {
            return previousState; // Still pressed, stable
        }
        return previousState; // Still within debounce time
    }
    else
    {
        return 0; // Released
    }
}

// --- Button Input ---

/**
 * Returns true for the menu button as long as button is held down.
 */
bool M1ShieldClass::isMenuPressed() const
{
    return (digitalRead(PIN_MENU) == LOW);
}

/**
 * Returns true for the menu button only once per press.
 */
bool M1ShieldClass::wasMenuPressed()
{
    unsigned long newState = _getDebouncedState(PIN_MENU, _menuPressed);
    bool wasPressed = (_menuPressed == 0 && newState != 0);
    _menuPressed = newState;
    return wasPressed;
}

/**
 * Returns true for the left button as long as button is held down.
 */
bool M1ShieldClass::isLeftPressed() const
{
    return (digitalRead(PIN_LEFT) == LOW);
}

/**
 * Returns true for the left button only once per press.
 */
bool M1ShieldClass::wasLeftPressed()
{
    unsigned long newState = _getDebouncedState(PIN_LEFT, _leftPressed);
    bool wasPressed = (_leftPressed == 0 && newState != 0);
    _leftPressed = newState;
    return wasPressed;
}

/**
 * Returns true for the right button as long as button is held down.
 */
bool M1ShieldClass::isRightPressed() const
{
    return (digitalRead(PIN_RIGHT) == LOW);
}

/**
 * Returns true for the right button only once per press.
 */
bool M1ShieldClass::wasRightPressed()
{
    unsigned long newState = _getDebouncedState(PIN_RIGHT, _rightPressed);
    bool wasPressed = (_rightPressed == 0 && newState != 0);
    _rightPressed = newState;
    return wasPressed;
}

/**
 * Returns true for the up button as long as button is held down.
 */
bool M1ShieldClass::isUpPressed() const
{
    return (digitalRead(PIN_UP) == LOW);
}

/**
 * Returns true for the up button only once per press.
 */
bool M1ShieldClass::wasUpPressed()
{
    unsigned long newState = _getDebouncedState(PIN_UP, _upPressed);
    bool wasPressed = (_upPressed == 0 && newState != 0);
    _upPressed = newState;
    return wasPressed;
}

/**
 * Returns true for the down button as long as button is held down.
 */
bool M1ShieldClass::isDownPressed() const
{
    return (digitalRead(PIN_DOWN) == LOW);
}

/**
 * Returns true for the down button only once per press.
 */
bool M1ShieldClass::wasDownPressed()
{
    unsigned long newState = _getDebouncedState(PIN_DOWN, _downPressed);
    bool wasPressed = (_downPressed == 0 && newState != 0);
    _downPressed = newState;
    return wasPressed;
}

// --- Joystick Input ---

/**
 * Returns true for the joystick button as long as button is held down.
 */
bool M1ShieldClass::isJoystickPressed() const
{
    return (digitalRead(PIN_JOYSTICK_BUTTON) == LOW);
}

/**
 * Returns true for the joystick button only once per press.
 */
bool M1ShieldClass::wasJoystickPressed()
{
    unsigned long newState = _getDebouncedState(PIN_JOYSTICK_BUTTON, _joystickPressed);
    bool wasPressed = (_joystickPressed == 0 && newState != 0);
    _joystickPressed = newState;
    return wasPressed;
}

/**
 * Returns the current directional state of the joystick, including diagonals.
 */
JoystickDirection M1ShieldClass::getJoystickDirection() const
{
    uint8_t x = getJoystickX();
    uint8_t y = getJoystickY();

    // Diagonal directions
    if (x < JOYSTICK_CENTER_MIN && y < JOYSTICK_CENTER_MIN)
        return UP_LEFT;
    if (x > JOYSTICK_CENTER_MAX && y < JOYSTICK_CENTER_MIN)
        return UP_RIGHT;
    if (x < JOYSTICK_CENTER_MIN && y > JOYSTICK_CENTER_MAX)
        return DOWN_LEFT;
    if (x > JOYSTICK_CENTER_MAX && y > JOYSTICK_CENTER_MAX)
        return DOWN_RIGHT;

    // Cardinal directions
    if (x < JOYSTICK_CENTER_MIN)
        return LEFT;
    if (x > JOYSTICK_CENTER_MAX)
        return RIGHT;
    if (y < JOYSTICK_CENTER_MIN)
        return UP;
    if (y > JOYSTICK_CENTER_MAX)
        return DOWN;

    return CENTER;
}

/**
 * Returns true if the joystick is centered (or close to centered).
 */
bool M1ShieldClass::isJoystickCentered() const
{
    return getJoystickDirection() == CENTER;
}

/**
 * Returns the current X-axis value of the joystick (0–255).
 */
uint8_t M1ShieldClass::getJoystickX() const
{
    return analogRead(PIN_JOYSTICK_X) >> 2;
}

/**
 * Returns the current Y-axis value of the joystick (0–255).
 */
uint8_t M1ShieldClass::getJoystickY() const
{
    return analogRead(PIN_JOYSTICK_Y) >> 2;
}

/**
 * Executes another loop and calls the loop on attached screen
 */
void M1ShieldClass::loop()
{
    // Keep track of TEST* signal and show state
    if (Model1.hasActiveTestSignal())
    {
        _active();
    }
    else
    {
        _inactive();
    }

    // If there is no screen attached, don't do more and ignore the rest
    if (!_screen)
        return;

    // Figure out what action has been taken and send to screen
    ActionTaken action = NONE;
    uint8_t offsetX = 0;
    uint8_t offsetY = 0;

    // Get joystick coordinates
    uint8_t x = getJoystickX();
    uint8_t y = getJoystickY();

    // Diagonal directions for Joystick
    if (x < JOYSTICK_CENTER_MIN && y < JOYSTICK_CENTER_MIN)
    {
        offsetX = JOYSTICK_CENTER_MIN - x;
        offsetY = JOYSTICK_CENTER_MIN - y;
        action |= JOYSTICK_UP_LEFT;
    }
    else if (x > JOYSTICK_CENTER_MAX && y < JOYSTICK_CENTER_MIN)
    {
        offsetX = x - JOYSTICK_CENTER_MIN;
        offsetY = JOYSTICK_CENTER_MIN - y;
        action |= JOYSTICK_UP_RIGHT;
    }
    else if (x < JOYSTICK_CENTER_MIN && y > JOYSTICK_CENTER_MAX)
    {
        offsetX = JOYSTICK_CENTER_MIN - x;
        offsetY = y - JOYSTICK_CENTER_MIN;
        action |= JOYSTICK_DOWN_LEFT;
    }
    else if (x > JOYSTICK_CENTER_MAX && y > JOYSTICK_CENTER_MAX)
    {
        offsetX = x - JOYSTICK_CENTER_MIN;
        offsetY = y - JOYSTICK_CENTER_MIN;
        action |= JOYSTICK_DOWN_RIGHT;
    }

    // Cardinal directions for Joystick
    else if (x < JOYSTICK_CENTER_MIN)
    {
        offsetX = JOYSTICK_CENTER_MIN - x;
        action |= JOYSTICK_LEFT;
    }
    else if (x > JOYSTICK_CENTER_MAX)
    {
        offsetX = x - JOYSTICK_CENTER_MIN;
        action |= JOYSTICK_RIGHT;
    }
    else if (y < JOYSTICK_CENTER_MIN)
    {
        offsetY = JOYSTICK_CENTER_MIN - y;
        action |= JOYSTICK_UP;
    }
    else if (y > JOYSTICK_CENTER_MAX)
    {
        offsetY = y - JOYSTICK_CENTER_MIN;
        action |= JOYSTICK_DOWN;
    }

    // Get button states
    if (wasMenuPressed())
    {
        action |= BUTTON_MENU;
    }
    if (wasLeftPressed())
    {
        action |= BUTTON_LEFT;
    }
    if (wasRightPressed())
    {
        action |= BUTTON_RIGHT;
    }
    if (wasUpPressed())
    {
        action |= BUTTON_UP;
    }
    if (wasDownPressed())
    {
        action |= BUTTON_DOWN;
    }
    if (wasJoystickPressed())
    {
        action |= BUTTON_JOYSTICK;
    }

    // Was any action taken?
    if (action != ActionTaken::NONE)
    {
        Screen *newScreen = _screen->actionTaken(action, offsetX, offsetY);

        // Was a new screen returned?
        if (newScreen != nullptr)
        {
            setScreen(newScreen);
        }
    }

    // Execute a loop within the screen in case it needs it
    _screen->loop();
}
