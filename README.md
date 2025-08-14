# TRS-80 Model I Arduino Library

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

## Key Features

- **Complete TRS-80 Bus Control** - Read/write memory, control signals, interrupts
- **High-Level Classes** - Model1, Video, Keyboard, ROM, Cassette for easy access
- **Low-Level Access** - Model1LowLevel for advanced users requiring precise control
- **M1Shield Support** - Hardware abstraction for Arduino shield with display/input
- **UI Framework** - Screen management system for building interactive applications
- **Multiple Display Support** - ST7789, ILI9341, ST7796, SSD1306, and more
- **Arduino Compatible** - Standard Print interface, familiar patterns

## Installation

1. Install via Arduino Library Manager or download from GitHub
2. Connect Arduino Mega 2560 to TRS-80 (M1Shield recommended, direct wiring supported)
3. Try examples: **M1Shield/HardwareTest** or **Model1/BasicAccess**

## Connection Options

**M1Shield (Recommended):** Clean, safe connection with built-in display and controls  
**Direct Wiring:** Connect via jumper wires using our [pin mapping guide](docs/PinMapping.md)

⚠️ **See [TEST Signal Fix](docs/TESTMod.md)** for hardware reliability improvement

## Documentation

**[Complete Documentation](docs/)** - Detailed API reference and guides

## Main TRS-80 Model 1 Repository

For additional resources related to the TRS-80 Model 1, be sure to check out the [central page for the TRS-80 Model 1](https://www.github.com/RetroStack/TRS-80-Model-I) on [RetroStack](https://www.github.com/RetroStack).

## Support this Project

RetroStack is passionate about exploring and preserving the legacy of older computer systems. My work involves creating detailed documentation and videos to share the knowledge. I am also dedicated to reviving these classic systems by reimplementing them and offering replacement parts at no cost. If you're keen on supporting this unique project, I invite you to visit my [Patreon page](https://www.patreon.com/RetroStack). Your support would be immensely valuable and greatly appreciated!

## License

This project is licensed under the MIT License – see the [LICENSE](LICENSE) file for details.
