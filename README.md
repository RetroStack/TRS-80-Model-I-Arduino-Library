# TRS-80 Model I Arduino Library

**NOTE**: There are still a lot of changes that are happening within this library. Use this only for testing. We will make breaking changes. A stable version will be released shortly before TA 2025.

Arduino library for interfacing with TRS-80 Model I hardware via the 40-pin edge connector. Control memory, video, keyboard, and bus signals to create ROM dumpers, RAM testers, and custom peripherals.

**Hardware Required:** Arduino Mega 2560 (only board with sufficient GPIO pins)

![Overview](Images/Overview.png)

## Quick Start

### Basic Usage

```cpp
#include <M1Shield.h>
#include <Display_ST7789.h>  // Choose your display

Display_ST7789 displayProvider;

void setup() {
    M1Shield.begin(displayProvider);
}

void loop() {
    M1Shield.loop();
}
```

### Read & Write

```cpp
#include <Model1.h>

void setup() {
    Model1.begin();
    Model1.activateTestSignal();

    uint8_t data = Model1.readByte(0x0000);  // Read ROM
    Model1.writeByte(0x3C00, 0x41);          // Write to video RAM

    Model1.deactivateTestSignal();
}
```

### UI Framework with String Support

```cpp
#include <ContentScreen.h>

class MyScreen : public ContentScreen {
public:
    MyScreen() {
        // Three ways to set titles/text - choose what fits your needs:
        setTitle("Static Title");                    // Simple C-string
        setTitle(String("Score: ") + String(score)); // Dynamic Arduino String
        setTitleF(F("Memory Efficient"));            // Flash string (F-macro)
    }

protected:
    void _drawContent() override {
        // Draw dynamic content easily
        String playerName = "Alice";
        drawText(10, 10, String("Player: ") + playerName, WHITE);
        drawTextF(10, 30, F("Static text"), YELLOW);  // Saves RAM
    }
};
```

## Key Features

- **Complete TRS-80 Bus Control** - Read/write memory, control signals, interrupts
- **High-Level Classes** - Model1, Video, Keyboard, ROM, Cassette for easy access
- **Low-Level Access** - Model1LowLevel for advanced users requiring precise control
- **M1Shield Support** - Hardware abstraction for Arduino shield with display/input
- **UI Framework** - Screen management system for building interactive applications
- **Flexible String Support** - All text functions support C-strings, Arduino Strings, and F-strings
- **Multiple Display Support** - ST7789, ILI9341, ST7796, SSD1306, and more
- **Arduino Compatible** - Standard Print interface, familiar patterns

## Installation

1. Install via Arduino Library Manager or download from GitHub
2. Connect Arduino Mega 2560 to TRS-80 (M1Shield recommended, direct wiring supported)
3. Try examples: **M1Shield/HardwareTest** or **Model1/BasicAccess**

## Examples

**[Complete Examples Guide](examples/)** - Comprehensive examples with progressive learning paths

The examples folder contains over 40 examples organized by functionality:

- **[M1Shield Examples](examples/M1Shield/)** - Hardware platform and display examples
- **[Model1 Examples](examples/Model1/)** - TRS-80 hardware interface (ROM, RAM, Video, Keyboard, etc.)
- **[UI Examples](examples/UI/)** - User interface framework and screen management
- **[Logger Examples](examples/Logger/)** - Debugging and monitoring system

All examples follow a Simple/Comprehensive pattern for progressive learning, with complete documentation and integration guidance.

## Connection Options

**M1Shield (Recommended):** Clean, safe connection with built-in display and controls  
**Direct Wiring:** Connect via jumper wires using our [pin mapping guide](docs/PinMapping.md)

**WARNING:** **See [TEST Signal Fix](docs/TESTMod.md)** for hardware reliability improvement

## Documentation

**[Complete Documentation](docs/)** - Detailed API reference and guides

## Main TRS-80 Model 1 Repository

For additional resources related to the TRS-80 Model 1, be sure to check out the [central page for the TRS-80 Model 1](https://www.github.com/RetroStack/TRS-80-Model-I) on [RetroStack](https://www.github.com/RetroStack).

## Support this Project

RetroStack is passionate about exploring and preserving the legacy of older computer systems. My work involves creating detailed documentation and videos to share the knowledge. I am also dedicated to reviving these classic systems by reimplementing them and offering replacement parts at no cost. If you're keen on supporting this unique project, I invite you to visit my [Patreon page](https://www.patreon.com/RetroStack). Your support would be immensely valuable and greatly appreciated!

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
