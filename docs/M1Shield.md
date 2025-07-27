# M1Shield Class

The `M1Shield` class provides a comprehensive hardware abstraction layer for the TRS-80 Model I Arduino Shield. It manages multi-display support, input handling (buttons and joystick), LED control, and screen management with a unified API that simplifies shield hardware interaction.

## Table of Contents

- [Constructor](#constructor)
- [Initialization](#initialization)
- [Display Management](#display-management)
- [Input Handling](#input-handling)
  - [Button Input](#button-input)
  - [Joystick Input](#joystick-input)
- [LED Control](#led-control)
- [Screen Management](#screen-management)
- [Hardware Status](#hardware-status)
- [Display Support](#display-support)
- [Notes](#notes)
- [Example](#example)

## Constructor

```cpp
M1Shield()
```

Creates a new `M1Shield` instance. The shield is not initialized until `begin()` is called.

## Initialization

### `void begin()`

Initializes all M1Shield hardware components:

- Display subsystem (ST7789, ST7735, or ILI9341)
- Button input pins with internal pull-ups
- Analog joystick pins
- RGB LED control pins
- Hardware abstraction layer setup

```cpp
void setup() {
    M1Shield.begin();
    // Hardware is now ready for use
}
```

## Display Management

### Display Access

- `Adafruit_GFX& getGFX()`: Returns reference to graphics library for direct drawing
- `uint16_t getScreenWidth()`: Returns display width in pixels
- `uint16_t getScreenHeight()`: Returns display height in pixels
- `bool isDisplayInitialized()`: Returns true if display initialization was successful

### Display Types

The library automatically detects and supports multiple display types:

| Display | Resolution | Define                 |
| ------- | ---------- | ---------------------- |
| ST7789  | 320×240    | `USE_ST7789` (default) |
| ST7735  | 128×160    | `USE_ST7735`           |
| ILI9341 | 240×320    | `USE_ILI9341`          |

```cpp
// Select display type before including M1Shield.h
#define USE_ST7735  // For 128×160 displays
#include "M1Shield.h"
```

## Input Handling

### Button Input

Six buttons are supported: Menu, Up, Down, Left, Right, and Joystick button.

**Current State Methods (continuous reading):**

```cpp
bool isMenuPressed()       // Menu/center button
bool isUpPressed()         // Up directional button
bool isDownPressed()       // Down directional button
bool isLeftPressed()       // Left directional button
bool isRightPressed()      // Right directional button
bool isJoystickPressed()   // Joystick center button (push down)
```

**Event Detection Methods (one-shot, consuming):**

```cpp
bool wasMenuPressed()      // Menu button was pressed since last call
bool wasUpPressed()        // Up button was pressed since last call
bool wasDownPressed()      // Down button was pressed since last call
bool wasLeftPressed()      // Left button was pressed since last call
bool wasRightPressed()     // Right button was pressed since last call
bool wasJoystickPressed()  // Joystick button was pressed since last call
```

**Example Usage:**

```cpp
void loop() {
    // Use was* methods for one-time actions (menus, toggles)
    if (M1Shield.wasMenuPressed()) {
        Serial.println("Menu button was pressed - toggle menu");
    }

    // Use is* methods for continuous actions (movement, games)
    if (M1Shield.isUpPressed() || M1Shield.isDownPressed()) {
        Serial.println("Directional button held - continuous movement");
    }
}
```

### Joystick Input

Analog joystick with direction detection and raw value access:

```cpp
JoystickDirection getJoystickDirection()    // Get current direction
bool isJoystickCentered()                   // Check if joystick is centered
uint8_t getJoystickX()                      // Raw X value (0-255, 128 = center)
uint8_t getJoystickY()                      // Raw Y value (0-255, 128 = center)
```

**JoystickDirection Enum:**

- `CENTER` - Joystick is centered
- `UP` - Joystick moved up
- `DOWN` - Joystick moved down
- `LEFT` - Joystick moved left
- `RIGHT` - Joystick moved right
- `UP_LEFT` - Diagonal up-left
- `UP_RIGHT` - Diagonal up-right
- `DOWN_LEFT` - Diagonal down-left
- `DOWN_RIGHT` - Diagonal down-right

**Example Usage:**

```cpp
void loop() {
    JoystickDirection dir = M1Shield.getJoystickDirection();

    if (dir != CENTER) {
        Serial.print("Joystick moved: ");

        // Convert direction to string manually
        switch (dir) {
            case UP: Serial.print("UP"); break;
            case DOWN: Serial.print("DOWN"); break;
            case LEFT: Serial.print("LEFT"); break;
            case RIGHT: Serial.print("RIGHT"); break;
            case UP_LEFT: Serial.print("UP_LEFT"); break;
            case UP_RIGHT: Serial.print("UP_RIGHT"); break;
            case DOWN_LEFT: Serial.print("DOWN_LEFT"); break;
            case DOWN_RIGHT: Serial.print("DOWN_RIGHT"); break;
            default: Serial.print("UNKNOWN"); break;
        }

        Serial.print(" (X: ");
        Serial.print(M1Shield.getJoystickX());
        Serial.print(", Y: ");
        Serial.print(M1Shield.getJoystickY());
        Serial.println(")");
    }
}
```

## LED Control

RGB LED with predefined color constants:

```cpp
void setLEDColor(LEDColor color, uint8_t intensity = 255)
void setLEDColor(uint8_t r, uint8_t g, uint8_t b)
```

**LEDColor Constants:**

- `COLOR_OFF` - LED turned off
- `COLOR_RED` - Red color (errors, alerts)
- `COLOR_GREEN` - Green color (success, ready)
- `COLOR_BLUE` - Blue color (information, processing)
- `COLOR_YELLOW` - Yellow color (warnings)
- `COLOR_MAGENTA` - Magenta color (special states)
- `COLOR_CYAN` - Cyan color (active operations)
- `COLOR_WHITE` - White color (maximum brightness)

**Example Usage:**

```cpp
void updateLEDStatus() {
    if (M1Shield.isJoystickPressed()) {
        M1Shield.setLEDColor(COLOR_WHITE);
    } else if (M1Shield.isMenuPressed()) {
        M1Shield.setLEDColor(COLOR_CYAN);
    } else if (M1Shield.isLeftPressed() || M1Shield.isRightPressed()) {
        M1Shield.setLEDColor(COLOR_BLUE);
    } else if (!M1Shield.isJoystickCentered()) {
        M1Shield.setLEDColor(COLOR_MAGENTA);
    } else {
        M1Shield.setLEDColor(COLOR_OFF);
    }
}
```

## Screen Management

Advanced screen management system for complex applications:

```cpp
void setScreen(Screen* screen)              // Set active screen
Screen* getCurrentScreen()                  // Get current screen pointer
void processInput()                         // Process input and handle screen transitions
void updateScreen()                         // Update current screen (calls loop())
void renderScreen()                         // Render current screen if needed
```

**Screen Lifecycle:**

1. Create screen instance
2. Call `setScreen()` to activate
3. M1Shield automatically calls screen's `open()`, `loop()`, and `actionTaken()` methods
4. Screen transitions handled automatically based on `actionTaken()` return values

**Example Usage:**

```cpp
#include "MenuScreen.h"
#include "GameScreen.h"

MenuScreen* mainMenu = new MenuScreen();
GameScreen* game = new GameScreen();

void setup() {
    M1Shield.begin();
    M1Shield.setScreen(mainMenu);  // Start with main menu
}

void loop() {
    M1Shield.loop();    // Handle input, screen updates, and transitions
}
```

## Hardware Status

Query hardware initialization status:

```cpp
bool isDisplayInitialized()     // Display ready for use
```

**Example Usage:**

```cpp
void setup() {
    M1Shield.begin();

    if (M1Shield.isDisplayInitialized()) {
        Serial.println("M1Shield ready!");
        M1Shield.setLEDColor(COLOR_GREEN);
    } else {
        Serial.println("M1Shield initialization failed!");
        M1Shield.setLEDColor(COLOR_RED);
    }
}
```

## Display Support

### Multi-Display Compatibility

The M1Shield library automatically detects and configures the correct display driver:

```cpp
// Display configuration examples:

// ST7789 (320×240) - Default, most common
#include "M1Shield.h"

// ST7735 (128×160) - Smaller TFT
#define USE_ST7735
#include "M1Shield.h"

// ILI9341 (240×320) - Alternative TFT
#define USE_ILI9341
#include "M1Shield.h"
```

### Display Constants

Available at compile time after display type selection:

- `DISPLAY_WIDTH` - Width in pixels
- `DISPLAY_HEIGHT` - Height in pixels
- `DISPLAY_NAME` - Human-readable display type name

## Notes

- **Single Instance**: Use the global `M1Shield` instance, don't create additional instances
- **Initialization Required**: Always call `begin()` before using any shield functions
- **Input Polling**: Button and joystick state is read when methods are called
- **Memory Management**: Screen transitions automatically handle memory cleanup
- **Thread Safety**: Designed for single-threaded Arduino main loop usage
- **Display Compatibility**: Automatically adapts to different display types and resolutions

## Example

```cpp
#include "M1Shield.h"

void setup() {
    Serial.begin(115200);

    // Initialize M1Shield hardware
    M1Shield.begin();

    // Verify initialization
    if (M1Shield.isDisplayInitialized()) {
        Serial.println("Display ready!");

        // Draw welcome message
        Adafruit_GFX& gfx = M1Shield.getGFX();
        gfx.fillScreen(ST77XX_BLACK);
        gfx.setTextColor(ST77XX_WHITE);
        gfx.setTextSize(2);
        gfx.setCursor(10, 10);
        gfx.print("M1Shield Ready!");

        M1Shield.setLEDColor(COLOR_GREEN);
    } else {
        Serial.println("Display initialization failed!");
        M1Shield.setLEDColor(COLOR_RED);
    }
}

void loop() {
    // Test all inputs with LED feedback
    if (M1Shield.isJoystickPressed()) {
        M1Shield.setLEDColor(COLOR_WHITE);
        Serial.println("Joystick button pressed");
    } else if (M1Shield.isMenuPressed()) {
        M1Shield.setLEDColor(COLOR_CYAN);
        Serial.println("Menu button pressed");
    } else if (M1Shield.isLeftPressed() || M1Shield.isRightPressed() ||
               M1Shield.isUpPressed() || M1Shield.isDownPressed()) {
        M1Shield.setLEDColor(COLOR_BLUE);
        Serial.println("Directional button pressed");
    } else if (!M1Shield.isJoystickCentered()) {
        M1Shield.setLEDColor(COLOR_MAGENTA);

        JoystickDirection dir = M1Shield.getJoystickDirection();
        Serial.print("Joystick moved: ");

        // Convert direction to string manually
        switch (dir) {
            case UP: Serial.print("UP"); break;
            case DOWN: Serial.print("DOWN"); break;
            case LEFT: Serial.print("LEFT"); break;
            case RIGHT: Serial.print("RIGHT"); break;
            case UP_LEFT: Serial.print("UP_LEFT"); break;
            case UP_RIGHT: Serial.print("UP_RIGHT"); break;
            case DOWN_LEFT: Serial.print("DOWN_LEFT"); break;
            case DOWN_RIGHT: Serial.print("DOWN_RIGHT"); break;
            default: Serial.print("UNKNOWN"); break;
        }
        Serial.println();
    } else {
        M1Shield.setLEDColor(COLOR_OFF);
    }

    delay(50);  // Small delay to prevent overwhelming serial output
}
```
