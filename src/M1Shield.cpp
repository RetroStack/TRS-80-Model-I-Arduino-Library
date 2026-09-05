/*
 * M1Shield.cpp - Hardware abstraction layer for Arduino shield
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#include "M1Shield.h"
#include <Adafruit_GFX.h>
#include <SPI.h>
#include <SD.h>
#include <Arduino.h>
#include "Model1.h"

// Hardware timing constants
constexpr unsigned long DEBOUNCE_TIME = 250; // Button debounce time in milliseconds

constexpr unsigned long JOYSTICK_REPEAT_DELAY = 400; // Hold time before a direction repeats
constexpr unsigned long JOYSTICK_REPEAT_RATE = 120;  // Interval between repeats after that

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

constexpr uint8_t PIN_BUZZER = 4; // Buzzer

// SD Card pin
constexpr uint8_t PIN_SD_SELECT = 49; // SD card chip select pin

// Define global instance
M1ShieldClass M1Shield;

// Constructor - Initialize M1Shield with default values
M1ShieldClass::M1ShieldClass() : _screen(nullptr),
                                 _displayProvider(nullptr),
                                 _logger(nullptr),
                                 _displayTarget(nullptr),
                                 _menuPressed(0),
                                 _selectPressed(0),
                                 _upPressed(0),
                                 _downPressed(0),
                                 _leftPressed(0),
                                 _rightPressed(0),
                                 _joystickPressed(0),
                                 _joystickDirection(NONE),
                                 _joystickRepeatTime(0),
                                 _joystickRepeatDelay(0),
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

    // Clean up display render target
    if (_displayTarget)
    {
        _renderManager.removeRenderTarget(_displayTarget);
        delete _displayTarget;
        _displayTarget = nullptr;
    }

    // Clean up display instance if it exists through the provider
    if (_displayProvider)
    {
        _displayProvider->destroy();
    }
}

// Initialize the shield hardware that does not depend on a display.
//
// This no-argument overload is the library's standard initialization entry
// point. It is always safe to call, and shared setup added here in future
// releases reaches existing sketches without changing their call sites -
// which is why it exists even when its body is trivial.
//
// It deliberately does not touch the display; use begin(DisplayProvider &)
// for that. begin(DisplayProvider &) calls this first.
bool M1ShieldClass::begin()
{
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

    return true;
}

// Initialize M1Shield with display provider and configure pins
bool M1ShieldClass::begin(DisplayProvider &provider)
{
    // Shared, display-independent initialization
    begin();

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

    _displayProvider = &provider;

    // Adafruit_GFX wraps text at the screen edge by default, so anything wider
    // than the panel spilled onto the row below instead of being clipped. Every
    // region here manages its own truncation, so wrapping is never wanted.
    provider.getGFX().setTextWrap(false);

    // Initialize display based on the selected type
    _screenWidth = provider.getScreenWidth();
    _screenHeight = provider.getScreenHeight();

    // Create and add display render target
    if (_displayTarget)
    {
        _renderManager.removeRenderTarget(_displayTarget);
        delete _displayTarget;
    }
    
    // Index 0, not the tail: removing and re-appending demoted the panel below
    // any target registered in between, and target 0 is the layout authority --
    // the whole UI would reflow to the secondary panel's geometry.
    _displayTarget = new DisplayRenderTarget(provider);
    if (_displayTarget && _renderManager.insertRenderTarget(_displayTarget, 0))
    {
        if (_logger)
        {
            _logger->infoF(F("M1Shield: Display render target created and added"));
        }
    }
    else
    {
        if (_logger)
        {
            _logger->warnF(F("M1Shield: Failed to create or add display render target"));
        }
        if (_displayTarget)
        {
            delete _displayTarget;
            _displayTarget = nullptr;
        }
    }

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
    // Answered from _displayProvider while getGFX() answered from the render
    // manager, so the two could disagree once a target was registered or its
    // provider swapped. Both now ask the manager.
    RenderTarget *active = _renderManager.getActiveTarget();
    if (active != nullptr)
    {
        return active->getScreenWidth() > 0 && active->getScreenHeight() > 0;
    }

    return (_displayProvider != nullptr && _screenWidth > 0 && _screenHeight > 0);
}

// Get reference to the Adafruit_GFX display object.
// Drawing goes through the primary render target; the provider is only used
// before begin() has registered one.
Adafruit_GFX &M1ShieldClass::getGFX()
{
    RenderTarget *target = _renderManager.getActiveTarget();
    if (target)
    {
        return target->getGFX();
    }

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
    RenderTarget *target = _renderManager.getActiveTarget();
    return target ? target->getScreenWidth() : _screenWidth;
}

// Get display screen height in pixels
uint16_t M1ShieldClass::getScreenHeight() const
{
    RenderTarget *target = _renderManager.getActiveTarget();
    return target ? target->getScreenHeight() : _screenHeight;
}

// Get reference to the display provider
DisplayProvider &M1ShieldClass::getDisplayProvider() const
{
    // Every sibling accessor resolves through the active render target; this
    // one answered from _displayProvider unconditionally, so under a render
    // pass on a second panel it returned the primary's provider while
    // getScreenWidth() beside it returned the secondary's width.
    RenderTarget *active = _renderManager.getActiveTarget();
    if (active != nullptr)
    {
        DisplayProvider *provider = active->getProvider();
        if (provider != nullptr)
        {
            return *provider;
        }
    }

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

// Update display with current frame buffer contents.
//
// Inside a render pass this pushes only the target currently being drawn -
// pushing the others would transfer targets that have not been drawn yet this
// frame, and would touch a second device's bus while the current one holds an
// open SPI transaction. Outside a pass it pushes every enabled target, which
// is the documented behaviour and is byte-identical to the single-target case.
bool M1ShieldClass::display()
{
    RenderTarget *pass = _renderManager.getPassTarget();
    if (pass)
    {
        return pass->display();
    }

    if (_renderManager.getRenderTargetCount() > 0)
    {
        bool result = _renderManager.displayAll();
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
    RenderTarget *target = _renderManager.getActiveTarget();
    if (target)
    {
        return target->convertColor(color);
    }
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

// Get reference to render manager
// Initialize and register an additional display panel
bool M1ShieldClass::addDisplay(DisplayRenderTarget &target, int8_t cs, int8_t dc, int8_t rst)
{
    // Sharing the primary's reset line would reset the primary panel: the
    // Adafruit driver toggles reset unconditionally while initializing, and
    // nothing re-initializes the primary afterwards, so it would go blank.
    if (rst >= 0 && rst == PIN_TFT_RST)
    {
        if (_logger)
        {
            _logger->errF(F("M1Shield: Additional display cannot share the primary reset pin"));
        }
        return false;
    }

    if (!target.begin(cs, dc, rst))
    {
        if (_logger)
        {
            _logger->errF(F("M1Shield: Failed to initialize additional display"));
        }
        return false;
    }

    if (!_renderManager.addRenderTarget(&target))
    {
        if (_logger)
        {
            _logger->errF(F("M1Shield: No room to register additional render target"));
        }
        return false;
    }

    if (_logger)
    {
        _logger->infoF(F("M1Shield: Added render target '%s'"), target.getName());
    }

    return true;
}

RenderManager &M1ShieldClass::getRenderManager()
{
    return _renderManager;
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
    // The header declares this with `intensity = 255` and says nothing about it
    // being inert, so a caller passing 64 got full brightness and no
    // diagnostic. Scale the channels instead of discarding it.
    const uint8_t full = intensity;
    switch (color)
    {
    case LEDColor::COLOR_OFF:
        setLEDColor(0, 0, 0);
        break;
    case LEDColor::COLOR_RED:
        setLEDColor(full, 0, 0);
        break;
    case LEDColor::COLOR_GREEN:
        setLEDColor(0, full, 0);
        break;
    case LEDColor::COLOR_BLUE:
        setLEDColor(0, 0, full);
        break;
    case LEDColor::COLOR_YELLOW:
        setLEDColor(full, full, 0);
        break;
    case LEDColor::COLOR_MAGENTA:
        setLEDColor(full, 0, full);
        break;
    case LEDColor::COLOR_CYAN:
        setLEDColor(0, full, full);
        break;
    case LEDColor::COLOR_WHITE:
        setLEDColor(full, full, full);
        break;
    }
}

// Edge-detect one button, advancing its stored state
bool M1ShieldClass::_wasPressed(int pin, unsigned long &state)
{
    unsigned long newState = _getDebouncedState(pin, state);
    bool pressed = (state == 0 && newState != 0);
    state = newState;
    return pressed;
}

// Get debounced button state with timing control
unsigned long M1ShieldClass::_getDebouncedState(int pin, unsigned long previousState) const
{
    if (digitalRead(pin) == LOW)
    {
        // First edge of a press is timestamped; later polls keep that stamp, so
        // wasPressed() reports a held button exactly once.
        return (previousState == 0) ? millis() : previousState;
    }

    // Released. Contact bounce reads HIGH for a few milliseconds in the middle
    // of a single physical press, and clearing the state on the first such read
    // let the next LOW look like a fresh press -- so one press was reported
    // several times. Hold it until the debounce window has actually elapsed.
    if (previousState != 0 && (millis() - previousState) < DEBOUNCE_TIME)
    {
        return previousState;
    }

    return 0;
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
    return _wasPressed(PIN_MENU, _menuPressed);
}

// Check if select button is currently pressed
bool M1ShieldClass::isSelectPressed() const
{
    return (digitalRead(PIN_SELECT) == LOW);
}

// Check if select button was just pressed (debounced)
bool M1ShieldClass::wasSelectPressed()
{
    return _wasPressed(PIN_SELECT, _selectPressed);
}

// Check if left button is currently pressed
bool M1ShieldClass::isLeftPressed() const
{
    return (digitalRead(PIN_LEFT) == LOW);
}

// Check if left button was just pressed (debounced)
bool M1ShieldClass::wasLeftPressed()
{
    return _wasPressed(PIN_LEFT, _leftPressed);
}

// Check if right button is currently pressed
bool M1ShieldClass::isRightPressed() const
{
    return (digitalRead(PIN_RIGHT) == LOW);
}

// Check if right button was just pressed (debounced)
bool M1ShieldClass::wasRightPressed()
{
    return _wasPressed(PIN_RIGHT, _rightPressed);
}

// Check if up button is currently pressed
bool M1ShieldClass::isUpPressed() const
{
    return (digitalRead(PIN_UP) == LOW);
}

// Check if up button was just pressed (debounced)
bool M1ShieldClass::wasUpPressed()
{
    return _wasPressed(PIN_UP, _upPressed);
}

// Check if down button is currently pressed
bool M1ShieldClass::isDownPressed() const
{
    return (digitalRead(PIN_DOWN) == LOW);
}

// Check if down button was just pressed (debounced)
bool M1ShieldClass::wasDownPressed()
{
    return _wasPressed(PIN_DOWN, _downPressed);
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
    return _wasPressed(PIN_JOYSTICK_BUTTON, _joystickPressed);
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

// Check if SD card is inserted and can be initialized
bool M1ShieldClass::isSDCardInserted() const
{
    // Try to initialize the SD card with the configured chip select pin
    bool cardDetected = SD.begin(PIN_SD_SELECT);

    if (cardDetected && _logger)
    {
        _logger->infoF(F("M1Shield: SD card detected and initialized successfully"));
    }
    else if (!cardDetected && _logger)
    {
        _logger->warnF(F("M1Shield: SD card not detected or initialization failed"));
    }

    return cardDetected;
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
    ActionTaken direction = NONE;

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
                direction = JOYSTICK_UP_LEFT;
            }
            else if (x > JOYSTICK_CENTER_MAX && y < JOYSTICK_CENTER_MIN)
            {
                direction = JOYSTICK_UP_RIGHT;
            }
            else if (x < JOYSTICK_CENTER_MIN && y > JOYSTICK_CENTER_MAX)
            {
                direction = JOYSTICK_DOWN_LEFT;
            }
            else if (x > JOYSTICK_CENTER_MAX && y > JOYSTICK_CENTER_MAX)
            {
                direction = JOYSTICK_DOWN_RIGHT;
            }

            // Cardinal directions for Joystick
            else if (x < JOYSTICK_CENTER_MIN)
            {
                direction = JOYSTICK_LEFT;
            }
            else if (x > JOYSTICK_CENTER_MAX)
            {
                direction = JOYSTICK_RIGHT;
            }
            else if (y < JOYSTICK_CENTER_MIN)
            {
                direction = JOYSTICK_UP;
            }
            else if (y > JOYSTICK_CENTER_MAX)
            {
                direction = JOYSTICK_DOWN;
            }
        }

        // The buttons run through _getDebouncedState(), which reports a held
        // button once. The stick had no such gate: its direction was rebuilt
        // from the current reading on every loop(), so holding it off-centre
        // re-sent the direction hundreds of times a second -- and since LEFT is
        // folded into the select mask, that re-triggered the selected item
        // continuously. Fire on entering a direction, then auto-repeat.
        unsigned long now = millis();
        if (direction != _joystickDirection)
        {
            _joystickDirection = direction;
            _joystickRepeatTime = now;
            _joystickRepeatDelay = JOYSTICK_REPEAT_DELAY;

            if (direction != NONE)
            {
                action = static_cast<ActionTaken>(action | direction);
            }
        }
        else if (direction != NONE && (now - _joystickRepeatTime) >= _joystickRepeatDelay)
        {
            _joystickRepeatTime = now;
            _joystickRepeatDelay = JOYSTICK_REPEAT_RATE;
            action = static_cast<ActionTaken>(action | direction);
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

    // Execute a loop within the screen in case it needs it.
    // setScreen() above may have failed to open the new screen and left
    // _screen as nullptr, so re-check before dispatching.
    if (_screen)
    {
        _screen->loop();
    }
}
