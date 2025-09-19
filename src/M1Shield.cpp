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

// Hardware timing constants
constexpr unsigned long DEBOUNCE_TIME = 250; // Button debounce time in milliseconds

// RGB LED pin assignments
constexpr uint8_t PIN_ACTIVE_LED = 13; // Activity indicator LED pin
constexpr uint8_t PIN_LED_BLUE = 10;   // Blue channel of RGB LED
constexpr uint8_t PIN_LED_GREEN = 11;  // Green channel of RGB LED
constexpr uint8_t PIN_LED_RED = 12;    // Red channel of RGB LED

// Button pins
constexpr uint8_t PIN_MENU = 41;
constexpr uint8_t PIN_SELECT = 40;
constexpr uint8_t PIN_LEFT = A8;
constexpr uint8_t PIN_RIGHT = A9;
constexpr uint8_t PIN_DOWN = A10;
constexpr uint8_t PIN_UP = A11;

// Joystick pins
constexpr uint8_t PIN_JOYSTICK_BUTTON = 39;
constexpr uint8_t PIN_JOYSTICK_X = A12;
constexpr uint8_t PIN_JOYSTICK_Y = A13;

constexpr uint8_t JOYSTICK_CENTER_MIN = 100;
constexpr uint8_t JOYSTICK_CENTER_MAX = 155;

// Display pin definitions
constexpr int8_t PIN_TFT_CS = 9;   // Chip Select
constexpr int8_t PIN_TFT_DC = 8;   // Data/Command
constexpr int8_t PIN_TFT_RST = 38; // Reset pin (-1 if not used)

// Cassette interface pins
constexpr uint8_t PIN_CR1 = 43;       // Cassette Remote 1 (configurable input/output)
constexpr uint8_t PIN_CR2 = 42;       // Cassette Remote 2 (configurable input/output)
constexpr uint8_t PIN_CASS_IN = A14;  // Cassette input from the perspective of the Model 1
constexpr uint8_t PIN_CASS_OUT = A15; // Cassette output from the perspective of the Model 1

// Buzzer pin
constexpr uint8_t PIN_BUZZER = 4; // Buzzer pin

// SD Card pin
constexpr uint8_t PIN_SD_SELECT = 49; // SD card chip select pin

// Define global instance
M1ShieldClass M1Shield;

// Constructor - Initialize M1Shield with default values
M1ShieldClass::M1ShieldClass() : _screen(nullptr),
                                 _displayProvider(nullptr),
                                 _logger(nullptr),
                                 _menuPressed(0),
                                 _selectPressed(0),
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

// Destructor - Clean up screen and display resources
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

// Initialize M1Shield with display provider and configure pins
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
    pinMode(PIN_SELECT, INPUT_PULLUP);
    pinMode(PIN_LEFT, INPUT_PULLUP);
    pinMode(PIN_RIGHT, INPUT_PULLUP);
    pinMode(PIN_DOWN, INPUT_PULLUP);
    pinMode(PIN_UP, INPUT_PULLUP);

    pinMode(PIN_JOYSTICK_BUTTON, INPUT_PULLUP);
    pinMode(PIN_JOYSTICK_X, INPUT);
    pinMode(PIN_JOYSTICK_Y, INPUT);

    pinMode(PIN_BUZZER, OUTPUT);

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
            _logger->errF(F("M1Shield: Failed to initialize display provider"));
        }
        return false;
    }

    // Initialize display based on the selected type
    _screenWidth = provider.width();
    _screenHeight = provider.height();

    if (_logger)
    {
        _logger->infoF(F("M1Shield: Display initialized successfully (%dx%d)"), _screenWidth, _screenHeight);
    }

    return success;
}

// Enable joystick input processing
void M1ShieldClass::activateJoystick()
{
    _activeJoystick = true;
}

// Disable joystick input processing
void M1ShieldClass::deactivateJoystick()
{
    _activeJoystick = false;
}

// Check if display has been properly initialized
bool M1ShieldClass::isDisplayInitialized() const
{
    return (_displayProvider != nullptr && _screenWidth > 0 && _screenHeight > 0);
}

// Get reference to the Adafruit_GFX display object
Adafruit_GFX &M1ShieldClass::getGFX()
{
    if (!_displayProvider)
    {
        if (_logger)
        {
            _logger->errF(F("M1Shield: Attempted to get GFX without initialized display provider"));
        }
        // This will likely cause a crash, but at least we log it
    }
    return _displayProvider->getGFX();
}

// Get display screen width in pixels
uint16_t M1ShieldClass::getScreenWidth() const
{
    return _screenWidth;
}

// Get display screen height in pixels
uint16_t M1ShieldClass::getScreenHeight() const
{
    return _screenHeight;
}

// Get reference to the display provider
DisplayProvider &M1ShieldClass::getDisplayProvider() const
{
    if (!_displayProvider)
    {
        if (_logger)
        {
            _logger->errF(F("M1Shield: Attempted to get display provider that is not initialized"));
        }
        // This will likely cause a crash, but at least we log it
    }
    return *_displayProvider;
}

// Set the logger for debugging and information output
void M1ShieldClass::setLogger(ILogger &logger)
{
    _logger = &logger;
}

// Get the current logger instance
ILogger *M1ShieldClass::getLogger() const
{
    return _logger;
}

// Update display with current frame buffer contents
bool M1ShieldClass::display()
{
    if (_displayProvider)
    {
        bool result = _displayProvider->display();
        if (!result && _logger)
        {
            _logger->warnF(F("M1Shield: Display update failed"));
        }
        return result;
    }

    if (_logger)
    {
        _logger->warnF(F("M1Shield: Attempted to update display without initialized display provider"));
    }
    return false;
}

// Convert color from RGB to display format
uint16_t M1ShieldClass::convertColor(uint16_t color)
{
    if (_displayProvider)
    {
        return _displayProvider->convertColor(color);
    }
    return color; // Return original color if no display provider
}

// Set and switch to a new screen, replacing the current one
bool M1ShieldClass::setScreen(Screen *screen)
{
    if (!screen)
    {
        if (_logger)
        {
            _logger->warnF(F("M1Shield: Attempted to set null screen"));
        }
        return false;
    }

    // Free up the old screen to avoid memory leaks
    if (_screen)
    {
        if (_logger)
        {
            // Log screen closing with title if available
            const char *title = _screen->getTitle();
            if (title)
            {
                _logger->infoF(F("M1Shield: Closing screen '%s'"), title);
            }
            else
            {
                _logger->infoF(F("M1Shield: Closing current screen"));
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
        const char *title = screen->getTitle();
        if (title)
        {
            _logger->infoF(F("M1Shield: Opening screen '%s'"), title);
        }
        else
        {
            _logger->infoF(F("M1Shield: Opening new screen"));
        }
    }

    if (!screen->open())
    {
        if (_logger)
        {
            _logger->errF(F("M1Shield: Failed to open new screen"));
        }
        // Screen failed to open, clean up
        delete screen;
        _screen = nullptr;
        return false;
    }

    _screen = screen;

    if (_logger)
    {
        _logger->infoF(F("M1Shield: Screen transition completed successfully"));
    }

    return true;
}

// Set active LED state
void M1ShieldClass::_active() const
{
    digitalWrite(PIN_ACTIVE_LED, LOW);
}

// Set inactive LED state
void M1ShieldClass::_inactive() const
{
    digitalWrite(PIN_ACTIVE_LED, HIGH);
}

// Set RGB LED color using individual channel values
void M1ShieldClass::setLEDColor(uint8_t r, uint8_t g, uint8_t b) const
{
    analogWrite(PIN_LED_RED, 255 - r);
    analogWrite(PIN_LED_GREEN, 255 - g);
    analogWrite(PIN_LED_BLUE, 255 - b);
}

// Set RGB LED color using predefined color constants
void M1ShieldClass::setLEDColor(LEDColor color, uint8_t intensity) const
{
    (void)intensity; // Parameter reserved for future use
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

// Get debounced button state with timing control
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

// Check if menu button is currently pressed
bool M1ShieldClass::isMenuPressed() const
{
    return (digitalRead(PIN_MENU) == LOW);
}

// Check if menu button was just pressed (debounced)
bool M1ShieldClass::wasMenuPressed()
{
    unsigned long newState = _getDebouncedState(PIN_MENU, _menuPressed);
    bool wasPressed = (_menuPressed == 0 && newState != 0);
    _menuPressed = newState;
    return wasPressed;
}

// Check if select button is currently pressed
bool M1ShieldClass::isSelectPressed() const
{
    return (digitalRead(PIN_SELECT) == LOW);
}

// Check if select button was just pressed (debounced)
bool M1ShieldClass::wasSelectPressed()
{
    unsigned long newState = _getDebouncedState(PIN_SELECT, _selectPressed);
    bool wasPressed = (_selectPressed == 0 && newState != 0);
    _selectPressed = newState;
    return wasPressed;
}

// Check if left button is currently pressed
bool M1ShieldClass::isLeftPressed() const
{
    return (digitalRead(PIN_LEFT) == LOW);
}

// Check if left button was just pressed (debounced)
bool M1ShieldClass::wasLeftPressed()
{
    unsigned long newState = _getDebouncedState(PIN_LEFT, _leftPressed);
    bool wasPressed = (_leftPressed == 0 && newState != 0);
    _leftPressed = newState;
    return wasPressed;
}

// Check if right button is currently pressed
bool M1ShieldClass::isRightPressed() const
{
    return (digitalRead(PIN_RIGHT) == LOW);
}

// Check if right button was just pressed (debounced)
bool M1ShieldClass::wasRightPressed()
{
    unsigned long newState = _getDebouncedState(PIN_RIGHT, _rightPressed);
    bool wasPressed = (_rightPressed == 0 && newState != 0);
    _rightPressed = newState;
    return wasPressed;
}

// Check if up button is currently pressed
bool M1ShieldClass::isUpPressed() const
{
    return (digitalRead(PIN_UP) == LOW);
}

// Check if up button was just pressed (debounced)
bool M1ShieldClass::wasUpPressed()
{
    unsigned long newState = _getDebouncedState(PIN_UP, _upPressed);
    bool wasPressed = (_upPressed == 0 && newState != 0);
    _upPressed = newState;
    return wasPressed;
}

// Check if down button is currently pressed
bool M1ShieldClass::isDownPressed() const
{
    return (digitalRead(PIN_DOWN) == LOW);
}

// Check if down button was just pressed (debounced)
bool M1ShieldClass::wasDownPressed()
{
    unsigned long newState = _getDebouncedState(PIN_DOWN, _downPressed);
    bool wasPressed = (_downPressed == 0 && newState != 0);
    _downPressed = newState;
    return wasPressed;
}

// --- Joystick Input ---

// Check if joystick button is currently pressed
bool M1ShieldClass::isJoystickPressed() const
{
    return (digitalRead(PIN_JOYSTICK_BUTTON) == LOW);
}

// Check if joystick button was just pressed (debounced)
bool M1ShieldClass::wasJoystickPressed()
{
    unsigned long newState = _getDebouncedState(PIN_JOYSTICK_BUTTON, _joystickPressed);
    bool wasPressed = (_joystickPressed == 0 && newState != 0);
    _joystickPressed = newState;
    return wasPressed;
}

// Get joystick direction based on analog position
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

// Check if joystick is in center position
bool M1ShieldClass::isJoystickCentered() const
{
    return getJoystickDirection() == CENTER;
}

// Get joystick X-axis position (0-255 scaled to 0-63)
uint8_t M1ShieldClass::getJoystickX() const
{
    return analogRead(PIN_JOYSTICK_X) >> 2;
}

// Get joystick Y-axis position (0-255 scaled to 0-63)
uint8_t M1ShieldClass::getJoystickY() const
{
    return analogRead(PIN_JOYSTICK_Y) >> 2;
}

// ========== Cassette Interface Implementation ==========

// Configure CR1 pin as input or output
void M1ShieldClass::setCR1Mode(bool isOutput) const
{
    pinMode(PIN_CR1, isOutput ? OUTPUT : INPUT);
}

// Configure CR2 pin as input or output
void M1ShieldClass::setCR2Mode(bool isOutput) const
{
    pinMode(PIN_CR2, isOutput ? OUTPUT : INPUT);
}

// Write digital value to CR1 pin
void M1ShieldClass::writeCR1(bool value) const
{
    digitalWrite(PIN_CR1, value ? HIGH : LOW);
}

// Write digital value to CR2 pin
void M1ShieldClass::writeCR2(bool value) const
{
    digitalWrite(PIN_CR2, value ? HIGH : LOW);
}

// Read digital value from CR1 pin
bool M1ShieldClass::readCR1() const
{
    return digitalRead(PIN_CR1) == HIGH;
}

// Read digital value from CR2 pin
bool M1ShieldClass::readCR2() const
{
    return digitalRead(PIN_CR2) == HIGH;
}

// Write analog value to cassette input pin
void M1ShieldClass::writeCassetteIn(uint8_t value) const
{
    analogWrite(PIN_CASS_IN, value);
}

// Read analog value from cassette output pin
uint16_t M1ShieldClass::readCassetteOut() const
{
    return analogRead(PIN_CASS_OUT);
}

// ========== SD Card Methods ==========

// Get SD card chip select pin number
uint8_t M1ShieldClass::getSDCardSelectPin() const
{
    return PIN_SD_SELECT;
}

// ========== Buzzer Methods ==========

// Activate buzzer sound
void M1ShieldClass::buzzerOn() const
{
    digitalWrite(PIN_BUZZER, HIGH);
}

// Deactivate buzzer sound
void M1ShieldClass::buzzerOff() const
{
    digitalWrite(PIN_BUZZER, LOW);
}

// Buzz for specified duration in milliseconds
void M1ShieldClass::buzz(unsigned int durationMs) const
{
    buzzerOn();
    delay(durationMs);
    buzzerOff();
}

// Main loop - Process input and update current screen
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
    int8_t offsetX = 0;
    int8_t offsetY = 0;
    bool joystickMoved = false;

    if (_activeJoystick)
    {
        // Get joystick coordinates
        uint8_t x = getJoystickX();
        uint8_t y = getJoystickY();

        // Calculate center point (around 127 for 0-255 range)
        int16_t centerX = 127;
        int16_t centerY = 127;

        // Convert to signed coordinates relative to center using proper arithmetic
        // Use int16_t for intermediate calculation to avoid overflow, then clamp to int8_t range
        int16_t tempX = (int16_t)x - centerX;
        int16_t tempY = (int16_t)y - centerY;

        // Clamp to int8_t range (-128 to +127)
        offsetX = (int8_t)constrain(tempX, -128, 127);
        offsetY = (int8_t)constrain(tempY, -128, 127);

        // Only consider it "moved" if beyond the deadzone
        if (x < JOYSTICK_CENTER_MIN || x > JOYSTICK_CENTER_MAX ||
            y < JOYSTICK_CENTER_MIN || y > JOYSTICK_CENTER_MAX)
        {
            joystickMoved = true;

            // Diagonal directions for Joystick
            if (x < JOYSTICK_CENTER_MIN && y < JOYSTICK_CENTER_MIN)
            {
                action = static_cast<ActionTaken>(action | JOYSTICK_UP_LEFT);
            }
            else if (x > JOYSTICK_CENTER_MAX && y < JOYSTICK_CENTER_MIN)
            {
                action = static_cast<ActionTaken>(action | JOYSTICK_UP_RIGHT);
            }
            else if (x < JOYSTICK_CENTER_MIN && y > JOYSTICK_CENTER_MAX)
            {
                action = static_cast<ActionTaken>(action | JOYSTICK_DOWN_LEFT);
            }
            else if (x > JOYSTICK_CENTER_MAX && y > JOYSTICK_CENTER_MAX)
            {
                action = static_cast<ActionTaken>(action | JOYSTICK_DOWN_RIGHT);
            }

            // Cardinal directions for Joystick
            else if (x < JOYSTICK_CENTER_MIN)
            {
                action = static_cast<ActionTaken>(action | JOYSTICK_LEFT);
            }
            else if (x > JOYSTICK_CENTER_MAX)
            {
                action = static_cast<ActionTaken>(action | JOYSTICK_RIGHT);
            }
            else if (y < JOYSTICK_CENTER_MIN)
            {
                action = static_cast<ActionTaken>(action | JOYSTICK_UP);
            }
            else if (y > JOYSTICK_CENTER_MAX)
            {
                action = static_cast<ActionTaken>(action | JOYSTICK_DOWN);
            }
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
    if (wasSelectPressed())
    {
        action = static_cast<ActionTaken>(action | BUTTON_SELECT);
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

    // Was any action taken or joystick moved beyond deadzone?
    if (action != ActionTaken::NONE || joystickMoved)
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
