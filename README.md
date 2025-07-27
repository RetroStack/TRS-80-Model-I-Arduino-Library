# TRS-80 Model I – Arduino Library

This Arduino library enables interfacing a TRS-80 Model I mainboard directly with an Arduino. It allows reading and writing data, controlling bus signals, and developing tools such as ROM dumpers, RAM testers, or custom peripherals for the TRS-80.

**Note:** Currently, **only the Arduino Mega 2560** is supported. This board provides enough GPIO pins to connect to the 40-pin edge connector of the TRS-80 Model I. More boards may be added in the future if there is interest.

## Features

### Core TRS-80 Integration

- Direct connection to the TRS-80 Model I system bus via the edge connector.
- Control over the address and data bus, abstracted in an easy-to-use interface.
- **Model1 Class**
  - Enables reading and writing to all addresses, including video RAM, DRAM, ROM (read-only), keyboard, and any other address space you want to inspect or modify.
  - Provides support for DRAM refresh
  - Supports I/O access as well.
  - Additional support for interrupt, WAIT, and TEST signals.
  - Provides convenience functions to check address spaces.

### Arduino Ecosystem Compatibility

- **Print Interface Support** – Video and ConsoleScreen classes implement Arduino's Print interface for seamless integration with existing code
- **Standard Arduino Patterns** – Familiar setup()/loop() structure and Arduino data types throughout
- **Serial Monitor Integration** – All logging classes work with Arduino Serial Monitor
- **Library Manager Compatible** – Standard Arduino library structure for easy installation

### Hardware Abstraction Classes

- **Cassette** – Higher-level access for the cassette interface.
- **Keyboard** – Class to read keyboard state, including press and release events.
- **ROM** – Access to ROM content and checksum comparison.
- **Video** – Provides direct access to video reading/writing, including viewports with scrolling support.
- **ILogger** - Standard interface for all logging events.
- **SerialLogger** - Specific implementation of an ILogger for Serial logging.

### M1Shield Framework (Arduino Shield Support)

- **M1Shield Framework** – Complete hardware abstraction and UI framework for TRS-80 Model I Arduino Shield
  - **M1Shield** – Hardware abstraction layer for display, input (buttons/joystick), LED control, and screen management with multi-display support via DisplayProvider system.
  - **DisplayProvider System** – Flexible, type-safe display support with optimized providers for different controller types:
    - **Display_ST7789** – ST7789 240x320 displays (most common, landscape oriented)
    - **Display_ST7789_240x240** – Square ST7789 240x240 displays
    - **Display_ST7735** – Smaller ST7735 128x160 displays
    - **Display_ILI9341** – ILI9341 240x320 displays (alternative common type)
    - **Display_ST7796** – Large ST7796 320x480 displays (landscape oriented)
    - **Display_HX8357** – Large HX8357 320x480 displays
    - **Display_ILI9325** – Parallel interface ILI9325 240x320 displays

### UI Framework

- **Screen Framework** – User interface abstractions
  - **Screen** – Abstract base class for all UI screens with lifecycle management, input handling, and navigation system.
  - **ContentScreen** – Structured layout template with automatic header, content, footer, and progress bar regions for consistent UI design.
  - **MenuScreen** – Paginated menu framework with automatic navigation, selection handling, and memory management for building interactive menus.
  - **M1TerminalScreen** – TRS-80 Model I terminal emulation with 64×16 character grid, incremental rendering, and direct Model1 system integration.
  - **ConsoleScreen** – Scrollable terminal-like interface implementing Arduino's Print interface for debugging, logging, and continuous text output with automatic screen clearing.

## Quick Start

### For M1Shield Users (Recommended)

1. **Install this library** in Arduino IDE
2. **Connect TRS-80 Model I Arduino Shield** to your Arduino Mega 2560
3. **Select your display provider** by modifying the M1Shield example:
   ```cpp
   #include <Display_ST7789.h>    // Choose your display type
   #include <M1Shield.h>
   
   Display_ST7789 displayProvider;  // Create provider instance
   
   void setup() {
       M1Shield.begin(displayProvider);  // Initialize with provider
   }
   ```
4. **Try the M1Shield → HardwareTest example** to verify everything works
5. **Explore Screen examples** to learn the UI framework

### For Direct Connection Users

1. **Install this library** in Arduino IDE
2. **Wire Arduino Mega 2560** to TRS-80 edge connector (see [pin mapping docs](/docs))
3. **Try the Model1 example** to test basic bus access
4. **Use ROM or Video examples** to explore TRS-80 hardware

### Display Provider Selection Guide

Choose the display provider that matches your hardware:

```cpp
// Most common displays (240x320, landscape orientation)
#include <Display_ST7789.h>      // ST7789 controller
#include <Display_ILI9341.h>     // ILI9341 controller

// Square displays
#include <Display_ST7789_240x240.h>  // 240x240 square

// Large displays (320x480)
#include <Display_ST7796.h>      // ST7796 controller (landscape)
#include <Display_HX8357.h>      // HX8357 controller (portrait)

// Smaller displays
#include <Display_ST7735.h>      // ST7735 128x160

// Parallel interface
#include <Display_ILI9325.h>     // ILI9325 parallel
```

## Documentation

You can find detailed documentation of all features [here](/docs).

## Hardware Requirements

### Arduino Board

- **Arduino Mega 2560** (required - provides sufficient GPIO pins for all TRS-80 signals)

### Connection Methods

**Option 1: TRS-80 Model I Arduino Shield (Recommended)**

- Clean, reliable connection with built-in display and controls
- DisplayProvider system supports multiple display types: ST7789, ST7789_240x240, ST7735, ILI9341, ST7796, HX8357, ILI9325
- Each display provider includes optimized initialization, rotation, and color settings
- Integrated buttons, joystick, and LED for user interface
- Type-safe display selection at compile time
- Reduces wiring complexity and connection errors

**Option 2: Direct Wiring**

- Arduino Mega 2560 connected via jumper wires to TRS-80 40-pin edge connector
- Requires careful wiring to avoid shorts (see [pin mapping docs](/docs/PinMapping.md))
- More complex but allows custom configurations

### TRS-80 Arduino Shield (Recommended)

A dedicated shield is available to make the connection cleaner and more reliable. It is maintained in a separate repository:

[TRS-80 Model I Arduino Shield](LINK_TO_SHIELD_DOCS)

_(Link will be updated once the repository is public.)_

## Installation

1. Clone or download this repository.
2. Copy the library folder into your Arduino libraries directory.
3. Open the Arduino IDE.
4. Go to **File → Examples → TRS-80 Model I** and choose an example:
   - **Model1, Video, ROM, Keyboard, Cassette** examples for direct TRS-80 system access
   - **M1Shield** examples for shield-based applications with display and input
   - **Screen, ContentScreen, ConsoleScreen, MenuScreen** examples for building custom user interfaces
5. Compile and upload to test it.

**Recommended Learning Path:**

1. **Start with HardwareTest** (M1Shield/HardwareTest.ino) - Verify your setup works
2. **Try Basic Classes** (Model1, ROM, Video examples) - Learn TRS-80 hardware access
3. **Explore UI Framework** (Screen → ContentScreen → MenuScreen → ConsoleScreen) - Build user interfaces
4. **Create Your Project** - Combine classes for custom TRS-80 tools and applications

**⚠️ WARNING:**  
When connecting to hardware, you often need to connect everything, turn the Model I on, and ensure it is in a good state **before** powering the Arduino. Many scripts assert the `TEST*` signal to take over execution. This is safe if the Model I has fully booted but can corrupt memory or cause instability if asserted immediately on startup.

**⚠️ IMPORTANT - TRS-80 Hardware Flaw:**  
The TRS-80 Model I contains a design flaw in the TEST signal implementation that can cause intermittent memory corruption or crashes when external devices take control of the bus. See the [TEST Signal Modification Guide](/docs/TESTMod.md) for details on this issue and an optional hardware fix that improves reliability.

## Main TRS-80 Model 1 Repository

For additional resources related to the TRS-80 Model 1, be sure to check out the [central page for the TRS-80 Model 1](https://www.github.com/RetroStack/TRS-80-Model-I) on [RetroStack](https://www.github.com/RetroStack).

## Support this Project

RetroStack is passionate about exploring and preserving the legacy of older computer systems. My work involves creating detailed documentation and videos to share the knowledge. I am also dedicated to reviving these classic systems by reimplementing them and offering replacement parts at no cost. If you're keen on supporting this unique project, I invite you to visit my [Patreon page](https://www.patreon.com/RetroStack). Your support would be immensely valuable and greatly appreciated!

## License

This project is licensed under the MIT License – see the [LICENSE](LICENSE) file for details.
