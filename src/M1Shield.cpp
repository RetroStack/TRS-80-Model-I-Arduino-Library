/*
 * M1Shield.cpp - Hardware abstraction layer for Arduino shield
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#include "M1Shield.h"
#include <Adafruit_GFX.h>
#include <SPI.h>
#include <Arduino.h>
#include "Model1.h"
#include "ContentScreen.h"

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

// Display pin definitions
constexpr int8_t PIN_TFT_CS = 9;   // Chip Select
constexpr int8_t PIN_TFT_DC = 8;   // Data/Command
constexpr int8_t PIN_TFT_RST = -1; // Reset pin (-1 if not used)

// Cassette interface pins
constexpr uint8_t PIN_CR1 = 2;        // Cassette Remote 1 (configurable input/output)
constexpr uint8_t PIN_CR2 = 3;        // Cassette Remote 2 (configurable input/output)
constexpr uint8_t PIN_CASS_IN = A14;  // Cassette input from the perspective of the Model 1
constexpr uint8_t PIN_CASS_OUT = A15; // Cassette output from the perspective of the Model 1

// Define global instance
M1ShieldClass M1Shield;

// Constructor
M1ShieldClass::M1ShieldClass() : _screen(nullptr),
                                 _displayProvider(nullptr),
                                 _logger(nullptr),
                                 _menuPressed(0),
                                 _upPressed(0),
                                 _downPressed(0),
                                 _leftPressed(0),
                                 _rightPressed(0),
                                 _joystickPressed(0),
                                 _screenWidth(0),
                                 _screenHeight(0),
                                 _activeJoystick(false)
{
}

M1ShieldClass::~M1ShieldClass()
{
    // Close and delete the current screen if it exists
    if (_screen)
    {
        _screen->close();
        delete _screen;
        _screen = nullptr;
    }

    // Clean up display instance if it exists through the provider
    if (_displayProvider)
    {
        _displayProvider->destroy();
    }
}

bool M1ShieldClass::begin(DisplayProvider &provider)
{
    _displayProvider = &provider;

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
    if (PIN_TFT_RST >= 0)
    {
        pinMode(PIN_TFT_RST, OUTPUT);
        digitalWrite(PIN_TFT_RST, LOW);
        delay(50);
        digitalWrite(PIN_TFT_RST, HIGH);
        delay(50);
    }

    bool success = provider.create(PIN_TFT_CS, PIN_TFT_DC, PIN_TFT_RST);

    if (!success)
    {
        if (_logger)
        {
            _logger->err(F("M1Shield: Failed to initialize display provider"));
        }
        return false;
    }

    // Initialize display based on the selected type
    _screenWidth = provider.width();
    _screenHeight = provider.height();

    if (_logger)
    {
        _logger->info(F("M1Shield: Display initialized successfully (%dx%d)"), _screenWidth, _screenHeight);
    }

    return success;
}

void M1ShieldClass::activateJoystick()
{
    _activeJoystick = true;
}

void M1ShieldClass::deactivateJoystick()
{
    _activeJoystick = false;
}

bool M1ShieldClass::isDisplayInitialized() const
{
    return (_displayProvider != nullptr && _screenWidth > 0 && _screenHeight > 0);
}

Adafruit_GFX &M1ShieldClass::getGFX()
{
    if (!_displayProvider)
    {
        if (_logger)
        {
            _logger->err(F("M1Shield: Attempted to get GFX without initialized display provider"));
        }
        // This will likely cause a crash, but at least we log it
    }
    return _displayProvider->getGFX();
}

uint16_t M1ShieldClass::getScreenWidth() const
{
    return _screenWidth;
}

uint16_t M1ShieldClass::getScreenHeight() const
{
    return _screenHeight;
}

DisplayProvider &M1ShieldClass::getDisplayProvider() const
{
    if (!_displayProvider)
    {
        if (_logger)
        {
            _logger->err(F("M1Shield: Attempted to get display provider that is not initialized"));
        }
        // This will likely cause a crash, but at least we log it
    }
    return *_displayProvider;
}

void M1ShieldClass::setLogger(ILogger &logger)
{
    _logger = &logger;
}

ILogger *M1ShieldClass::getLogger() const
{
    return _logger;
}

bool M1ShieldClass::display()
{
    if (_displayProvider)
    {
        bool result = _displayProvider->display();
        if (!result && _logger)
        {
            _logger->warn(F("M1Shield: Display update failed"));
        }
        return result;
    }

    if (_logger)
    {
        _logger->warn(F("M1Shield: Attempted to update display without initialized display provider"));
    }
    return false;
}

uint16_t M1ShieldClass::convertColor(uint16_t color)
{
    if (_displayProvider)
    {
        return _displayProvider->convertColor(color);
    }
    return color; // Return original color if no display provider
}

bool M1ShieldClass::setScreen(Screen *screen)
{
    if (!screen)
    {
        if (_logger)
        {
            _logger->warn(F("M1Shield: Attempted to set null screen"));
        }
        return false;
    }

    // Free up the old screen to avoid memory leaks
    if (_screen)
    {
        if (_logger)
        {
            // Try to get title context if this is a ContentScreen
            ContentScreen *contentScreen = dynamic_cast<ContentScreen *>(_screen);
            if (contentScreen && contentScreen->getTitle())
            {
                _logger->info(F("M1Shield: Closing screen '%s'"), contentScreen->getTitle());
            }
            else
            {
                _logger->info(F("M1Shield: Closing current screen"));
            }
        }

        _screen->close();
        delete _screen;
        _screen = nullptr;
    }

    // Automatically propagate logger from M1Shield to screen if screen doesn't have one
    if (_logger && !screen->getLogger())
    {
        screen->setLogger(*_logger);
    }

    // Log new screen opening with context if available
    if (_logger)
    {
        ContentScreen *contentScreen = dynamic_cast<ContentScreen *>(screen);
        if (contentScreen && contentScreen->getTitle())
        {
            _logger->info(F("M1Shield: Opening screen '%s'"), contentScreen->getTitle());
        }
        else
        {
            _logger->info(F("M1Shield: Opening new screen"));
        }
    }

    if (!screen->open())
    {
        if (_logger)
        {
            _logger->err(F("M1Shield: Failed to open new screen"));
        }
        // Screen failed to open, clean up
        delete screen;
        _screen = nullptr;
        return false;
    }

    _screen = screen;

    if (_logger)
    {
        _logger->info(F("M1Shield: Screen transition completed successfully"));
    }

    return true;
}

void M1ShieldClass::_active() const
{
    digitalWrite(PIN_ACTIVE_LED, LOW);
}

void M1ShieldClass::_inactive() const
{
    digitalWrite(PIN_ACTIVE_LED, HIGH);
}

void M1ShieldClass::setLEDColor(uint8_t r, uint8_t g, uint8_t b) const
{
    analogWrite(PIN_LED_RED, 255 - r);
    analogWrite(PIN_LED_GREEN, 255 - g);
    analogWrite(PIN_LED_BLUE, 255 - b);
}

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

bool M1ShieldClass::isMenuPressed() const
{
    return (digitalRead(PIN_MENU) == LOW);
}

bool M1ShieldClass::wasMenuPressed()
{
    unsigned long newState = _getDebouncedState(PIN_MENU, _menuPressed);
    bool wasPressed = (_menuPressed == 0 && newState != 0);
    _menuPressed = newState;
    return wasPressed;
}

bool M1ShieldClass::isLeftPressed() const
{
    return (digitalRead(PIN_LEFT) == LOW);
}

bool M1ShieldClass::wasLeftPressed()
{
    unsigned long newState = _getDebouncedState(PIN_LEFT, _leftPressed);
    bool wasPressed = (_leftPressed == 0 && newState != 0);
    _leftPressed = newState;
    return wasPressed;
}

bool M1ShieldClass::isRightPressed() const
{
    return (digitalRead(PIN_RIGHT) == LOW);
}

bool M1ShieldClass::wasRightPressed()
{
    unsigned long newState = _getDebouncedState(PIN_RIGHT, _rightPressed);
    bool wasPressed = (_rightPressed == 0 && newState != 0);
    _rightPressed = newState;
    return wasPressed;
}

bool M1ShieldClass::isUpPressed() const
{
    return (digitalRead(PIN_UP) == LOW);
}

bool M1ShieldClass::wasUpPressed()
{
    unsigned long newState = _getDebouncedState(PIN_UP, _upPressed);
    bool wasPressed = (_upPressed == 0 && newState != 0);
    _upPressed = newState;
    return wasPressed;
}

bool M1ShieldClass::isDownPressed() const
{
    return (digitalRead(PIN_DOWN) == LOW);
}

bool M1ShieldClass::wasDownPressed()
{
    unsigned long newState = _getDebouncedState(PIN_DOWN, _downPressed);
    bool wasPressed = (_downPressed == 0 && newState != 0);
    _downPressed = newState;
    return wasPressed;
}

// --- Joystick Input ---

bool M1ShieldClass::isJoystickPressed() const
{
    return (digitalRead(PIN_JOYSTICK_BUTTON) == LOW);
}

bool M1ShieldClass::wasJoystickPressed()
{
    unsigned long newState = _getDebouncedState(PIN_JOYSTICK_BUTTON, _joystickPressed);
    bool wasPressed = (_joystickPressed == 0 && newState != 0);
    _joystickPressed = newState;
    return wasPressed;
}

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

bool M1ShieldClass::isJoystickCentered() const
{
    return getJoystickDirection() == CENTER;
}

uint8_t M1ShieldClass::getJoystickX() const
{
    return analogRead(PIN_JOYSTICK_X) >> 2;
}

uint8_t M1ShieldClass::getJoystickY() const
{
    return analogRead(PIN_JOYSTICK_Y) >> 2;
}

// ========== Cassette Interface Implementation ==========

void M1ShieldClass::setCR1Mode(bool isOutput) const
{
    pinMode(PIN_CR1, isOutput ? OUTPUT : INPUT);
}

void M1ShieldClass::setCR2Mode(bool isOutput) const
{
    pinMode(PIN_CR2, isOutput ? OUTPUT : INPUT);
}

void M1ShieldClass::writeCR1(bool value) const
{
    digitalWrite(PIN_CR1, value ? HIGH : LOW);
}

void M1ShieldClass::writeCR2(bool value) const
{
    digitalWrite(PIN_CR2, value ? HIGH : LOW);
}

bool M1ShieldClass::readCR1() const
{
    return digitalRead(PIN_CR1) == HIGH;
}

bool M1ShieldClass::readCR2() const
{
    return digitalRead(PIN_CR2) == HIGH;
}

void M1ShieldClass::writeCassetteIn(uint8_t value) const
{
    analogWrite(PIN_CASS_IN, value);
}

uint16_t M1ShieldClass::readCassetteOut() const
{
    return analogRead(PIN_CASS_OUT);
}

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

    if (_activeJoystick)
    {
        // Get joystick coordinates
        uint8_t x = getJoystickX();
        uint8_t y = getJoystickY();

        // Diagonal directions for Joystick
        if (x < JOYSTICK_CENTER_MIN && y < JOYSTICK_CENTER_MIN)
        {
            offsetX = JOYSTICK_CENTER_MIN - x;
            offsetY = JOYSTICK_CENTER_MIN - y;
            action = static_cast<ActionTaken>(action | JOYSTICK_UP_LEFT);
        }
        else if (x > JOYSTICK_CENTER_MAX && y < JOYSTICK_CENTER_MIN)
        {
            offsetX = x - JOYSTICK_CENTER_MIN;
            offsetY = JOYSTICK_CENTER_MIN - y;
            action = static_cast<ActionTaken>(action | JOYSTICK_UP_RIGHT);
        }
        else if (x < JOYSTICK_CENTER_MIN && y > JOYSTICK_CENTER_MAX)
        {
            offsetX = JOYSTICK_CENTER_MIN - x;
            offsetY = y - JOYSTICK_CENTER_MIN;
            action = static_cast<ActionTaken>(action | JOYSTICK_DOWN_LEFT);
        }
        else if (x > JOYSTICK_CENTER_MAX && y > JOYSTICK_CENTER_MAX)
        {
            offsetX = x - JOYSTICK_CENTER_MIN;
            offsetY = y - JOYSTICK_CENTER_MIN;
            action = static_cast<ActionTaken>(action | JOYSTICK_DOWN_RIGHT);
        }

        // Cardinal directions for Joystick
        else if (x < JOYSTICK_CENTER_MIN)
        {
            offsetX = JOYSTICK_CENTER_MIN - x;
            action = static_cast<ActionTaken>(action | JOYSTICK_LEFT);
        }
        else if (x > JOYSTICK_CENTER_MAX)
        {
            offsetX = x - JOYSTICK_CENTER_MIN;
            action = static_cast<ActionTaken>(action | JOYSTICK_RIGHT);
        }
        else if (y < JOYSTICK_CENTER_MIN)
        {
            offsetY = JOYSTICK_CENTER_MIN - y;
            action = static_cast<ActionTaken>(action | JOYSTICK_UP);
        }
        else if (y > JOYSTICK_CENTER_MAX)
        {
            offsetY = y - JOYSTICK_CENTER_MIN;
            action = static_cast<ActionTaken>(action | JOYSTICK_DOWN);
        }

        if (wasJoystickPressed())
        {
            action = static_cast<ActionTaken>(action | BUTTON_JOYSTICK);
        }
    }

    // Get button states
    if (wasMenuPressed())
    {
        action = static_cast<ActionTaken>(action | BUTTON_MENU);
    }
    if (wasLeftPressed())
    {
        action = static_cast<ActionTaken>(action | BUTTON_LEFT);
    }
    if (wasRightPressed())
    {
        action = static_cast<ActionTaken>(action | BUTTON_RIGHT);
    }
    if (wasUpPressed())
    {
        action = static_cast<ActionTaken>(action | BUTTON_UP);
    }
    if (wasDownPressed())
    {
        action = static_cast<ActionTaken>(action | BUTTON_DOWN);
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
