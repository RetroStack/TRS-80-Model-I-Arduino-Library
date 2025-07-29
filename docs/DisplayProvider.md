# DisplayProvider System

The DisplayProvider system provides a flexible, type-safe way to support multiple display types in the M1Shield library. Each display controller has its own optimized provider class with proper initialization, rotation, and configuration.

## Table of Contents

- [Overview](#overview)
- [Available Providers](#available-providers)
- [Basic Usage](#basic-usage)
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

| Provider Class           | Controller | Physical Size | Rotated Size | Library Used     | Notes                  |
| ------------------------ | ---------- | ------------- | ------------ | ---------------- | ---------------------- |
| `Display_ST7789`         | ST7789     | 240x320       | 320x240      | Adafruit_ST7789  | Most common, landscape |
| `Display_ST7789_240x240` | ST7789     | 240x240       | 240x240      | Adafruit_ST7789  | Square displays        |
| `Display_ST7789_320x170` | ST7789     | 170x320       | 320x170      | Adafruit_ST7789  | Wide landscape format  |
| `Display_ST7789_320x240` | ST7789     | 240x320       | 320x240      | Adafruit_ST7789  | Alternative landscape  |
| `Display_ST7735`         | ST7735     | 128x160       | 160x128      | Adafruit_ST7735  | Smaller displays       |
| `Display_ILI9341`        | ILI9341    | 240x320       | 320x240      | Adafruit_ILI9341 | Alternative common     |
| `Display_ST7796`         | ST7796     | 320x480       | 480x320      | Adafruit_ST7796S | Large, landscape       |
| `Display_HX8357`         | HX8357     | 320x480       | 480x320      | Adafruit_HX8357  | Large, portrait        |
| `Display_ILI9325`        | ILI9325    | 240x320       | 320x240      | Adafruit_TFTLCD  | Parallel interface     |
| `Display_SSD1306`        | SSD1306    | 128x64        | 128x64       | Adafruit_SSD1306 | Monochrome OLED        |
| `Display_SH1106`         | SH1106     | 128x64        | 128x64       | Adafruit_SH110X  | Monochrome OLED        |

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

## Provider Interface

All display providers implement the `DisplayProvider` interface:

```cpp
class DisplayProvider {
public:
    virtual Adafruit_GFX *create(int8_t cs, int8_t dc, int8_t rst) = 0;
    virtual void destroy() = 0;
    virtual const char *name() const = 0;
    virtual uint16_t width() const = 0;
    virtual uint16_t height() const = 0;
};
```

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

    Adafruit_GFX *create(int8_t cs, int8_t dc, int8_t rst) override {
        if (_display) {
            delete _display;
        }
        _display = new Adafruit_NEWTYPE(cs, dc, rst);
        _display->begin();                    // Initialize display
        _display->setRotation(1);            // Set optimal rotation
        _display->invertDisplay(false);      // Configure as needed
        return _display;
    }

    void destroy() override {
        if (_display) {
            delete _display;
            _display = nullptr;
        }
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
           gfx.fillScreen(0xF800);  // Red screen
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
