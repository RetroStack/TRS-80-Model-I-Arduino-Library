# TRS-80 Model I Arduino Library Documentation

This comprehensive documentation covers the TRS-80 Model I Arduino Library, which enables modern microcontrollers to interface with and control vintage TRS-80 Model I computers through their 40-pin edge connector.

![Overview](../Images/Overview.png)

## Overview

The TRS-80 Model I Arduino Library transforms your Arduino Mega 2560 into a powerful peripheral controller for the TRS-80 Model I. By connecting to the computer's edge connector, the Arduino can:

- **Read and write memory** including RAM, ROM, and video memory
- **Monitor and control** all bus signals and hardware states
- **Emulate peripherals** like cassette drives and external devices
- **Provide modern interfaces** with displays, buttons, and logging
- **Debug and analyze** the TRS-80's operation in real-time

This library is ideal for:

- **Vintage computer enthusiasts** wanting to extend TRS-80 capabilities
- **Hardware developers** creating modern peripherals for classic systems
- **Researchers and educators** studying 1970s computer architecture
- **Preservation projects** backing up and analyzing vintage software

## Hardware Requirements

- **Arduino Mega 2560** (required for sufficient GPIO pins)
- **TRS-80 Model I** (any revision)
- **Connection method**:
  - **Recommended**: TRS-80 Model I Arduino Shield (M1Shield) for safe, reliable connections
  - **Advanced**: Direct wiring using our comprehensive pin mapping guide

## Getting Started

1. **Choose your connection method** - M1Shield (recommended) or direct wiring
2. **Review safety guidelines** in our hardware documentation
3. **Start with basic examples** using the Model1 and Video classes
4. **Explore advanced features** with logging, screen management, and low-level control

## Library Architecture

The library is organized into several key areas:

## Library Architecture

The library is organized into several key areas:

### Core TRS-80 Interface

- [**Model1**](Model1.md) — High-level interface for memory access, bus control, and system management. Start here for most applications.
- [**Model1LowLevel**](Model1LowLevel.md) — Direct hardware control for advanced users requiring precise signal timing (⚠️ Expert level).
- [**Cassette**](Cassette.md) — Cassette tape interface emulation and video mode control for authentic TRS-80 operation.
- [**Keyboard**](Keyboard.md) — Matrix keyboard reading with change detection and key mapping.
- [**Video**](Video.md) — Video memory manipulation, text display, and character encoding with viewport support.
- [**ROM**](ROM.md) — ROM analysis tools including reading, checksumming, and automatic identification of known ROM versions.

### Hardware Integration

- [**Pin Mapping**](PinMapping.md) — Complete wiring guide for direct Arduino-to-TRS-80 connections with safety procedures and testing.
- [**TEST Signal Modification**](TESTMod.md) — Essential hardware fix for TRS-80 bus timing issues that can cause crashes during external access.

### Development & Debugging

- [**ILogger**](ILogger.md) — Unified logging interface supporting multiple output formats and destinations.
- [**SerialLogger**](SerialLogger.md) — Serial port logging with formatted output and mute/unmute control.
- [**CompositeLogger**](CompositeLogger.md) — Multi-destination logging for simultaneous output to serial, display, and file systems.

### M1Shield Support

![M1Shield](../Images/M1Shield.png)

The M1Shield provides a safe, reliable connection method with integrated display and user interface:

- [**M1Shield**](M1Shield.md) — Main shield interface for display, input controls, LED indicators, screen management, and cassette interface (⚠️ Advanced).
- [**DisplayProvider**](DisplayProvider.md) — Adaptive display system supporting multiple controller types (TFT: ST7789, ST7735, ILI9341, ST7796, HX8357, ILI9325; OLED: SSD1306, SH1106).

### User Interface Framework

A complete screen management system for building interactive applications:

- [**Screen**](Screen.md) — Abstract base class providing lifecycle management, input handling, and navigation for all UI screens.
- [**ContentScreen**](ContentScreen.md) — Structured layout template with header, content area, footer, and progress indicators for organized displays.
- [**ConsoleScreen**](ConsoleScreen.md) — Terminal-style scrolling text interface ideal for debugging, logging, and command-line applications.
- [**LoggerScreen**](LoggerScreen.md) — Visual logging destination with color-coded messages, timestamps, and ILogger compatibility.
- [**MenuScreen**](MenuScreen.md) — Intelligent menu system with automatic pagination, navigation, and selection handling.

## Quick Start Guide

### Basic Memory Access

```cpp
#include <Model1.h>

void setup() {
  Model1.begin();
  Model1.activateTestSignal();

  uint8_t data = Model1.readByte(0x0000);  // Read ROM
  Model1.writeByte(0x3C00, 0x41);          // Write 'A' to video memory

  Model1.deactivateTestSignal();
}
```

### Using the M1Shield

```cpp
#include <M1Shield.h>
#include <MenuScreen.h>
#include <Display_ST7789.h>  // Or your preferred display

Display_ST7789 displayProvider;
MenuScreen menuScreen;

void setup() {
  M1Shield.begin(displayProvider);
  M1Shield.setScreen(&menuScreen);
}

void loop() {
  M1Shield.loop();  // Handles input, screen updates, and rendering
}
```

## Safety & Best Practices

- **Always power on the TRS-80 first** and let it boot completely
- **Use the M1Shield** for reliable connections and built-in protection
- **Follow the pin mapping guide carefully** if direct wiring
- **Review the TEST signal modification** for stable operation
- **Start with high-level classes** (Model1, Video) before using low-level access

## Documentation Structure

Each class documentation includes:

- **Complete API reference** with all methods and parameters
- **Usage examples** demonstrating common patterns
- **Implementation notes** for understanding behavior
- **Safety considerations** and best practices
- **Cross-references** to related classes and concepts

The documentation is designed to support users from beginner to expert level, with clear progression paths and appropriate warnings for advanced features.

## Community & Support

This library represents years of research into TRS-80 Model I hardware and careful reverse engineering of the original system. It enables modern development techniques while preserving the authentic feel and operation of this historic computer system.
