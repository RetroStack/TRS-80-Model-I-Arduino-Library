# DisplayProvider System

The DisplayProvider system provides a flexible, type-safe way to support multiple display types in the M1Shield library. Each display controller has its own optimized provider class with proper initialization, rotation, and configuration.

## Table of Contents

- [Overview](#overview)
- [Available Providers](#available-providers)
- [Basic Usage](#basic-usage)
- [SH1106 Dual Communication Support](#sh1106-dual-communication-support)
- [Provider Interface](#provider-interface)
- [Migration Guide](#migration-guide)
- [Adding New Displays](#adding-new-displays)
- [Troubleshooting](#troubleshooting)

## Overview

The DisplayProvider architecture uses a compile-time provider selection. This provides:

- **Type Safety**: Compile-time verification of display compatibility
- **Flexibility**: Easy switching between display types
- **Optimization**: Display-specific settings and initialization
- **Extensibility**: Simple to add new display types
- **Information Access**: Runtime access to display capabilities

## Available Providers

| Provider Class           | Controller | Physical Size | Rotated Size | Library Used     | Notes                 |
| ------------------------ | ---------- | ------------- | ------------ | ---------------- | --------------------- |
| `Display_ST7789_240x240` | ST7789     | 240x240       | 240x240      | Adafruit_ST7789  | Square displays       |
| `Display_ST7789_320x170` | ST7789     | 170x320       | 320x170      | Adafruit_ST7789  | Wide landscape format |
| `Display_ST7789_320x240` | ST7789     | 240x320       | 320x240      | Adafruit_ST7789  | Alternative landscape |
| `Display_ST7735`         | ST7735     | 128x160       | 160x128      | Adafruit_ST7735  | Smaller displays      |
| `Display_ILI9341`        | ILI9341    | 240x320       | 320x240      | Adafruit_ILI9341 | Alternative common    |
| `Display_ST7796`         | ST7796     | 320x480       | 480x320      | Adafruit_ST7796S | Large, landscape      |
| `Display_HX8357`         | HX8357     | 320x480       | 480x320      | Adafruit_HX8357  | Large, portrait       |
| `Display_ILI9325`        | ILI9325    | 240x320       | 320x240      | Adafruit_TFTLCD  | Parallel interface    |
| `Display_SSD1306`        | SSD1306    | 128x64        | 128x64       | Adafruit_SSD1306 | Monochrome OLED       |
| `Display_SH1106`         | SH1106     | 128x64        | 128x64       | Adafruit_SH110X  | Monochrome OLED, SPI/I2C |

## Basic Usage

### 1. Include the Provider

```cpp
#include <Display_ST7789.h>  // Choose your display type
#include <M1Shield.h>
```

### 2. Create Provider Instance

```cpp
#include <Display_ST7789.h>
#include <M1Shield.h>

Display_ST7789 displayProvider;  // Must match your hardware
```

### 3. Initialize M1Shield

```cpp
#include <Display_ST7789.h>
#include <M1Shield.h>

void setup() {
    M1Shield.begin(displayProvider);

    if (M1Shield.isDisplayInitialized()) {
        Serial.println("Display ready!");
    }
}
```

### 4. Access Display Information

```cpp
// Get display details from provider
const char* name = displayProvider.name();      // "ST7789 240x320"
uint16_t width = displayProvider.width();       // 320 (after rotation)
uint16_t height = displayProvider.height();     // 240 (after rotation)

// Or from M1Shield (same values)
uint16_t w = M1Shield.getScreenWidth();
uint16_t h = M1Shield.getScreenHeight();
```

### 5. Drawing and Display Updates

```cpp
void loop() {
    // Get graphics context for drawing
    Adafruit_GFX& gfx = displayProvider.getGFX();
    // Alternative: Adafruit_GFX& gfx = M1Shield.getGFX();

    // Draw something
    gfx.fillScreen(M1Shield.convertColor(0x0000));
    gfx.setTextColor(M1Shield.convertColor(0xFFFF));
    gfx.setCursor(10, 10);
    gfx.println("Hello World!");

    // Update display (REQUIRED for OLED displays)
    if (displayProvider.display()) {
        Serial.println("Display updated successfully");
    }
    // Alternative: M1Shield.display();

    delay(1000);
}
```

## OLED Display Requirements

**IMPORTANT**: OLED displays (SSD1306, SH1106) use a framebuffer architecture that requires explicit display updates to push changes from memory to the physical screen.

### Why Display Updates Are Required

- **TFT Displays**: Changes appear immediately when drawing operations are performed
- **OLED Displays**: Drawing operations only update a RAM framebuffer; `display()` must be called to transfer the framebuffer to the physical screen

### When to Call display()

The library automatically calls `M1Shield.display()` in the following scenarios:
- When screens open (`Screen::open()`)
- When screens refresh (`Screen::refresh()`) 
- After individual drawing operations in screen implementations
- After text output in console/terminal screens
- After menu navigation changes

### Manual Display Updates

If you're performing custom drawing operations outside of the built-in screen classes, you must call `display()` after your drawing:

```cpp
// Custom drawing example
Adafruit_GFX& gfx = M1Shield.getGFX();

// Perform drawing operations
gfx.fillRect(10, 10, 50, 30, M1Shield.convertColor(0xFFFF));
gfx.setTextColor(M1Shield.convertColor(0x0000));
gfx.setCursor(15, 20);
gfx.print("Custom");

// REQUIRED: Push changes to OLED displays
M1Shield.display();
```

### Performance Considerations

- **Batch Operations**: Group multiple drawing operations before calling `display()` for better performance
- **TFT Displays**: Calling `display()` has no effect (returns `true` immediately)
- **OLED Displays**: Each `display()` call transfers the entire framebuffer over I2C/SPI

```cpp
// Good: Batched drawing with single display update
gfx.fillRect(0, 0, 100, 50, M1Shield.convertColor(0x0000));
gfx.setTextColor(M1Shield.convertColor(0xFFFF));
gfx.setCursor(10, 10);
gfx.print("Line 1");
gfx.setCursor(10, 25);
gfx.print("Line 2");
M1Shield.display(); // Single update for all changes

// Avoid: Multiple display calls for performance
gfx.fillRect(0, 0, 100, 50, M1Shield.convertColor(0x0000));
M1Shield.display(); // Unnecessary for this operation
gfx.setTextColor(M1Shield.convertColor(0xFFFF));
M1Shield.display(); // Unnecessary for this operation
gfx.print("Text");
M1Shield.display(); // Only this one is needed
```

### 6. Advanced Provider Access

```cpp
void displayStatus() {
    // Access provider through M1Shield (useful when provider instance not available)
    DisplayProvider& provider = M1Shield.getDisplayProvider();

    Serial.print("Display: ");
    Serial.println(provider.name());
    Serial.print("Size: ");
    Serial.print(provider.width());
    Serial.print("x");
    Serial.println(provider.height());

    // Direct graphics access
    Adafruit_GFX& gfx = provider.getGFX();
    gfx.setTextSize(1);
    gfx.setCursor(0, 0);
    gfx.print(provider.name());

    // Update display
    provider.display();
}
```

## SH1106 Dual Communication Support

The `Display_SH1106` provider supports both SPI and I2C communication protocols. You specify the desired protocol when creating the provider instance.

### SPI Mode

```cpp
#include <Display_SH1106.h>
#include <M1Shield.h>

// Create SH1106 provider in SPI mode
Display_SH1106 displayProvider(true);  // true = SPI mode

void setup() {
    M1Shield.begin(displayProvider);
    
    // Display name will show "SH1106 (SPI)"
    Serial.println(displayProvider.name());
}
```

**SPI Wiring:**
- VCC → 3.3V/5V
- GND → GND  
- CLK → SPI Clock (SCK)
- MOSI → SPI Data (MOSI)
- CS → Digital pin (specified in M1Shield.begin())
- DC → Digital pin (specified in M1Shield.begin())
- RST → Digital pin (optional, can be -1)

### I2C Mode

```cpp
#include <Display_SH1106.h>
#include <M1Shield.h>

// Create SH1106 provider in I2C mode (default)
Display_SH1106 displayProvider;        // Default: I2C mode
// Display_SH1106 displayProvider(false);  // Explicit I2C mode

void setup() {
    M1Shield.begin(displayProvider);
    
    // Display name will show "SH1106 (I2C)"
    Serial.println(displayProvider.name());
}
```

**I2C Wiring:**
- VCC → 3.3V/5V
- GND → GND
- SDA → I2C Data (SDA)
- SCL → I2C Clock (SCL)
- RST → Digital pin (optional, can be -1)

### Communication Protocol Benefits

**SPI Advantages:**
- Faster data transfer rates
- More predictable timing
- Can use dedicated pins (doesn't interfere with I2C devices)

**I2C Advantages:**
- Uses fewer pins (only 2 data lines)
- Can share bus with other I2C devices
- Standard address (0x3C) - no pin configuration needed

## Provider Interface

All display providers implement the `DisplayProvider` interface:

```cpp
class DisplayProvider {
public:
    virtual bool create(int8_t cs, int8_t dc, int8_t rst) = 0;
    virtual void destroy() = 0;
    virtual Adafruit_GFX &getGFX() = 0;
    virtual bool display() = 0;
    virtual const char *name() const = 0;
    virtual uint16_t width() const = 0;
    virtual uint16_t height() const = 0;
};
```

### Method Details

- **`create(cs, dc, rst)`**: Initialize the display hardware

  - Returns `true` if successful, `false` on failure
  - Handles display-specific configuration and rotation
  - Previous versions returned `Adafruit_GFX*` pointer

- **`destroy()`**: Clean up display resources

  - Safe to call multiple times
  - Automatically called by destructor

- **`getGFX()`**: Get reference to Adafruit_GFX for drawing

  - Returns reference to the underlying graphics context
  - Use for all drawing operations

- **`display()`**: Update the display (NEW)

  - For OLED displays: Pushes framebuffer to screen
  - For TFT displays: Returns `true` (updates immediately)
  - Returns `true` on success, `false` on failure

- **`name()`**: Get human-readable display name

  - Returns string like "ST7789 240x320"
  - Useful for debugging and user interfaces

- **`width()` / `height()`**: Get display dimensions
  - Returns dimensions after rotation is applied
  - Matches what you'll use for drawing coordinates

### Methods

- **`create()`**: Creates and initializes the display instance, stores it internally
- **`destroy()`**: Cleans up the internally stored display resources
- **`name()`**: Returns human-readable display name
- **`width()`**: Returns effective width (after rotation)
- **`height()`**: Returns effective height (after rotation)

## Adding New Displays

To add support for a new display controller:

### 1. Create Provider Class

```cpp
// Display_NEWTYPE.h
#include <Adafruit_NEWTYPE.h>
#include <DisplayProvider.h>

class Display_NEWTYPE : public DisplayProvider {
private:
    Adafruit_NEWTYPE *_display;

public:
    Display_NEWTYPE() : _display(nullptr) {}

    bool create(int8_t cs, int8_t dc, int8_t rst) override {
        if (_display) {
            delete _display;
        }
        _display = new Adafruit_NEWTYPE(cs, dc, rst);
        _display->begin();                    // Initialize display
        _display->setRotation(1);            // Set optimal rotation
        _display->invertDisplay(false);      // Configure as needed
        return _display != nullptr;          // Return success status
    }

    void destroy() override {
        if (_display) {
            delete _display;
            _display = nullptr;
        }
    }

    Adafruit_GFX &getGFX() override {
        return *_display;
    }

    bool display() override {
        // For OLED displays, call _display->display()
        // For TFT displays, return (_display != nullptr)
        if (_display) {
            _display->display();  // Only if display has this method
            return true;
        }
        return false;
    }

    const char *name() const override {
        return "NEWTYPE 240x320";
    }

    uint16_t width() const override {
        return 320;  // After rotation
    }

    uint16_t height() const override {
        return 240;  // After rotation
    }

    ~Display_NEWTYPE() override {
        destroy();
    }
};
```

### 2. Add to Keywords (optional)

> Only for library additions!

Update `keywords.txt`:

```
Display_NEWTYPE    KEYWORD1
```

### 3. Add to Library (optional)

> Only for library additions!

Update `library.properties`:

```
includes=...,Display_NEWTYPE.h,...
```

## Troubleshooting

### Display Not Working

1. **Check Provider Selection**

   ```cpp
   // Make sure you're using the correct provider for your hardware
   Display_ST7789 provider;  // Must match your actual display
   ```

2. **Verify Wiring**

> This is the default for the Shield.

- CS (Chip Select): Pin 9
- DC (Data/Command): Pin 8
- RST (Reset): Pin -1 (not used)
- Power and SPI connections

3. **Test with Simple Code**

   ```cpp
   #include <Display_ST7789.h>
   #include <M1Shield.h>

   Display_ST7789 displayProvider;

   void setup() {
       Serial.begin(115200);
       M1Shield.begin(displayProvider);

       if (M1Shield.isDisplayInitialized()) {
           Serial.println("Display OK!");
           Adafruit_GFX& gfx = M1Shield.getGFX();
           gfx.fillScreen(M1Shield.convertColor(0xF800));  // Red screen
       } else {
           Serial.println("Display failed!");
       }
   }

   void loop() {}
   ```

### Compilation Errors

1. **"Cannot declare variable to be of abstract type"**

   - Don't use `DisplayProvider` directly
   - Use concrete classes like `Display_ST7789`

2. **"Cannot open source file"**

   - Make sure you included the correct provider header
   - Check that the library is properly installed

### Performance Issues

- Each provider is optimized for its display type
- ST7789 and ILI9341 providers use optimized SPI settings
- Large displays (ST7796, HX8357) may be slower due to size
- Use appropriate text sizes and avoid frequent full-screen updates
