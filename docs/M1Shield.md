# M1Shield Class

The `M1Shield` class provides a comprehensive hardware abstraction layer for the TRS-80 Model I Arduino Shield. It manages multi-display support through the DisplayProvider system, input handling (buttons and joystick), LED control, and screen management with a unified API that simplifies shield hardware interaction.

## Table of Contents

- [Constructor](#constructor)
- [Initialization](#initialization)
- [Display Provider System](#display-provider-system)
- [Display Management](#display-management)
  - [Color Conversion System](#color-conversion-system)
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

Creates a new `M1Shield` instance. The shield is not initialized until `begin()` is called with a DisplayProvider.

## Initialization

### `void begin(DisplayProvider &provider)`

Initializes all M1Shield hardware components using the specified display provider:

- Display subsystem (via DisplayProvider architecture)
- Button input pins with internal pull-ups
- Analog joystick pins
- RGB LED control pins
- Hardware abstraction layer setup

```cpp
#include <Display_ST7789.h>
#include <M1Shield.h>

void setup() {
    Display_ST7789 displayProvider;
    M1Shield.begin(displayProvider);
    // Hardware is now ready for use
}
```

## Display Provider System

The M1Shield uses a DisplayProvider architecture for flexible display support. Each display type has an optimized provider class:

### Available Display Providers

- **`Display_ST7789`** - ST7789 240x320 displays (landscape: 320x240)
- **`Display_ST7789_240x240`** - Square ST7789 240x240 displays
- **`Display_ST7735`** - Smaller ST7735 128x160 displays
- **`Display_ILI9341`** - ILI9341 240x320 displays (landscape: 320x240)
- **`Display_ST7796`** - Large ST7796 320x480 displays (landscape: 480x320)
- **`Display_HX8357`** - Large HX8357 320x480 displays
- **`Display_ILI9325`** - Parallel ILI9325 240x320 displays

### Display Provider Benefits

- **Automatic Configuration**: Each provider handles optimal initialization, rotation, and color settings
- **Type Safety**: Compile-time verification of display compatibility
- **Performance**: Display-specific optimizations
- **Extensibility**: Easy to add new display types

### Example Display Provider Usage

```cpp
// Select your display provider
#include <Display_ST7789.h>
Display_ST7789 displayProvider;

void setup() {
    // Initialize with provider
    M1Shield.begin(displayProvider);

    // Get display information
    Serial.print("Display: ");
    Serial.println(displayProvider.name());        // "ST7789 240x320"
    Serial.print("Size: ");
    Serial.print(displayProvider.width());         // 320 (after rotation)
    Serial.print("x");
    Serial.println(displayProvider.height());      // 240 (after rotation)
}
```

## Display Management

### Display Access

- `Adafruit_GFX& getGFX()`: Returns reference to graphics library for direct drawing
- `DisplayProvider& getDisplayProvider()`: Returns reference to the display provider instance
- `bool display()`: Updates the display (pushes framebuffer for OLED, validates for TFT)
- `uint16_t getScreenWidth()`: Returns display width in pixels
- `uint16_t getScreenHeight()`: Returns display height in pixels
- `bool isDisplayInitialized()`: Returns true if display initialization was successful

### Display Updates and OLED Compatibility

The `display()` method is **REQUIRED** for OLED displays (SSD1306, SH1106) and optional for TFT displays:

- **OLED Displays**: Use a framebuffer architecture where drawing operations update RAM, and `display()` transfers the framebuffer to the physical screen
- **TFT Displays**: Drawing operations update the screen immediately, so `display()` simply returns `true`

**When display() is called automatically:**
- Screen opening (`Screen::open()`)
- Screen refreshing (`Screen::refresh()`)
- After drawing operations in built-in screen classes
- After text output in console/terminal/logger screens
- After menu navigation changes

**When you must call display() manually:**
- Custom drawing operations outside of screen classes
- Direct GFX operations in your application code

```cpp
// Example: Manual display update required
Adafruit_GFX& gfx = M1Shield.getGFX();

// Draw operations only update OLED framebuffer
gfx.fillRect(10, 10, 100, 50, M1Shield.convertColor(0xFFFF));
gfx.setTextColor(M1Shield.convertColor(0x0000));
gfx.setCursor(15, 25);
gfx.print("Hello OLED");

// REQUIRED: Push framebuffer to OLED display
M1Shield.display();
```

### Color Conversion System

The M1Shield provides automatic color conversion to ensure compatibility across different display types and color depths:

- `uint16_t convertColor(uint16_t color)`: Converts 16-bit RGB565 color values to the optimal format for the current display

**Why use convertColor():**
- **Display Compatibility**: Automatically handles different color depths (16-bit TFT vs 1-bit OLED)
- **Monochrome Support**: Converts RGB colors to appropriate monochrome values for OLED displays
- **Future Compatibility**: Ensures your code works with different display providers
- **Performance**: Optimized conversions per display type

**Color Conversion Examples:**

```cpp
Adafruit_GFX& gfx = M1Shield.getGFX();

// Always use convertColor() for cross-display compatibility
gfx.fillScreen(M1Shield.convertColor(0x0000));        // Black background
gfx.setTextColor(M1Shield.convertColor(0xFFFF));      // White text
gfx.fillRect(10, 10, 50, 30, M1Shield.convertColor(0xF800));  // Red rectangle
gfx.fillRect(70, 10, 50, 30, M1Shield.convertColor(0x07E0));  // Green rectangle
gfx.fillRect(130, 10, 50, 30, M1Shield.convertColor(0x001F)); // Blue rectangle

// Color conversion works with any RGB565 color
gfx.setTextColor(M1Shield.convertColor(0xFFE0));      // Yellow text
gfx.fillCircle(50, 100, 20, M1Shield.convertColor(0x07FF));   // Cyan circle
```

**Common RGB565 Color Values:**
- `0x0000` - Black
- `0xFFFF` - White  
- `0xF800` - Red
- `0x07E0` - Green
- `0x001F` - Blue
- `0xFFE0` - Yellow
- `0xF81F` - Magenta
- `0x07FF` - Cyan

### Display Types

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

Analog joystick with direction detection, activation control, and raw value access:

```cpp
void activateJoystick()                     // Enable joystick for screen actions
void deactivateJoystick()                   // Disable joystick for screen actions
JoystickDirection getJoystickDirection()    // Get current direction
bool isJoystickCentered()                   // Check if joystick is centered
uint8_t getJoystickX()                      // Raw X value (0-255, 128 = center)
uint8_t getJoystickY()                      // Raw Y value (0-255, 128 = center)
bool isJoystickPressed()                    // Check if joystick button is currently pressed
bool wasJoystickPressed()                   // Check if joystick button was just pressed (debounced)
```

**Joystick Activation:**

By default, joystick input is **disabled** for screen actions. You can still read joystick values and button states, but they won't be sent to screens via `actionTaken()`. To enable joystick for screen navigation:

```cpp
void setup() {
    M1Shield.begin(displayProvider);

    // Enable joystick for screen actions (optional)
    M1Shield.activateJoystick();

    M1Shield.setScreen(new MyScreen());
}
```

When activated, joystick movements and button presses are sent to the current screen as `ActionTaken` flags.

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

void setup() {
    M1Shield.begin(displayProvider);

    // Optional: Enable joystick for screen actions
    // M1Shield.activateJoystick();

    M1Shield.setScreen(new MyScreen());
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
#include <Display_ST7789.h>
#include <M1Shield.h>

Display_ST7789 displayProvider;
MenuScreen* mainMenu = new MenuScreen();
GameScreen* game = new GameScreen();

void setup() {
    M1Shield.begin(displayProvider);

    // Optional: Enable joystick input for screen actions
    // M1Shield.activateJoystick();

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
Display_ST7789 displayProvider;

void setup() {
    M1Shield.begin(displayProvider);

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

### DisplayProvider Architecture

The M1Shield uses a modern DisplayProvider system for flexible display support. Instead of compile-time defines, you select display providers at runtime:

```cpp
// Example: Using ST7789 display
#include <Display_ST7789.h>
#include "M1Shield.h"

Display_ST7789 displayProvider;

void setup() {
    M1Shield.begin(displayProvider);
}
```

### Supported Display Types

| Provider Class           | Display Controller | Resolution        | Notes                           |
| ------------------------ | ------------------ | ----------------- | ------------------------------- |
| `Display_ST7789`         | ST7789             | 240x320 → 320x240 | Most common, landscape oriented |
| `Display_ST7789_240x240` | ST7789             | 240x240           | Square displays                 |
| `Display_ST7735`         | ST7735             | 128x160           | Smaller displays                |
| `Display_ILI9341`        | ILI9341            | 240x320 → 320x240 | Alternative common display      |
| `Display_ST7796`         | ST7796             | 320x480 → 480x320 | Large displays, landscape       |
| `Display_HX8357`         | HX8357             | 320x480           | Large displays, portrait        |
| `Display_ILI9325`        | ILI9325            | 240x320 → 320x240 | Parallel interface              |

### Migration from Legacy System

**Old way (deprecated):**

```cpp
#define USE_ST7789  // Compile-time selection
#include <M1Shield.h>
M1Shield.begin();
```

**New way (recommended):**

```cpp
#include <Display_ST7789.h>  // Runtime selection
#include <M1Shield.h>
Display_ST7789 displayProvider;
M1Shield.begin(displayProvider);
```

### Display Information Access

Get display information from the provider:

```cpp
#include <Display_ST7789.h>
#include <M1Shield.h>

Display_ST7789 displayProvider;
M1Shield.begin(displayProvider);

// Access display info
const char* name = displayProvider.name();      // "ST7789 240x320"
uint16_t width = displayProvider.width();       // 320 (after rotation)
uint16_t height = displayProvider.height();     // 240 (after rotation)

// Also available from M1Shield
uint16_t w = M1Shield.getScreenWidth();         // Same as provider.width()
uint16_t h = M1Shield.getScreenHeight();        // Same as provider.height()
```

## Notes

- **Single Instance**: Use the global `M1Shield` instance, don't create additional instances
- **Initialization Required**: Always call `begin()` before using any shield functions
- **Input Polling**: Button and joystick state is read when methods are called
- **Memory Management**: Screen transitions automatically handle memory cleanup
- **Thread Safety**: Designed for single-threaded Arduino main loop usage
- **Display Compatibility**: Automatically adapts to different display types and resolutions

## Example

```cpp
#include <Display_ST7789.h>  // Select your display provider
#include <M1Shield.h>

// Create display provider instance
Display_ST7789 displayProvider;

void setup() {
    Serial.begin(115200);

    // Initialize M1Shield hardware with display provider
    M1Shield.begin(displayProvider);

    // Optional: Enable joystick input for screen actions
    // M1Shield.activateJoystick();

    // Verify initialization
    if (M1Shield.isDisplayInitialized()) {
        Serial.print("Display ready: ");
        Serial.println(displayProvider.name());
        Serial.print("Resolution: ");
        Serial.print(displayProvider.width());
        Serial.print("x");
        Serial.println(displayProvider.height());

        // Draw welcome message
        Adafruit_GFX& gfx = M1Shield.getGFX();
        gfx.fillScreen(M1Shield.convertColor(0x0000));
        gfx.setTextColor(M1Shield.convertColor(0xFFFF));
        gfx.setTextSize(2);
        gfx.setCursor(10, 10);
        gfx.print("M1Shield Ready!");

        // Show display provider info on screen
        gfx.setTextSize(1);
        gfx.setCursor(10, 40);
        gfx.print("Provider: ");
        gfx.print(displayProvider.name());

        M1Shield.setLEDColor(COLOR_GREEN);
    } else {
        Serial.println("Display initialization failed!");
        M1Shield.setLEDColor(COLOR_RED);
    }
}

// Alternative way to access display provider later in your code
void displayInfo() {
    if (M1Shield.isDisplayInitialized()) {
        // Access display provider through M1Shield
        DisplayProvider& provider = M1Shield.getDisplayProvider();

        Serial.print("Current display: ");
        Serial.println(provider.name());
        Serial.print("Dimensions: ");
        Serial.print(provider.width());
        Serial.print("x");
        Serial.println(provider.height());

        // You can also access the graphics context through the provider
        Adafruit_GFX& gfx = provider.getGFX();
        gfx.fillScreen(M1Shield.convertColor(0x0000));
        gfx.setTextColor(M1Shield.convertColor(0xFFFF));
        gfx.setCursor(10, 10);
        gfx.print("Display: ");
        gfx.println(provider.name());

        // Update the display (important for OLED displays)
        if (provider.display()) {
            Serial.println("Display updated successfully");
        }
        // Alternative: M1Shield.display();
    }
}void loop() {
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
