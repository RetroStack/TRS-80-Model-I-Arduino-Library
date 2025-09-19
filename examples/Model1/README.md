# Model1 Interface Examples

This directory contains examples demonstrating the Model1 class functionality for interfacing with the TRS-80 Model I computer.

## Available Examples

### Cassette

Examples for cassette tape interface functionality.

### Keyboard

Examples for keyboard interface and input handling.

### Model1

Core Model1 interface examples for basic memory and I/O operations.

### RAM

Examples for RAM testing and memory operations.

### ROM

Examples for ROM dumping and ROM-related operations.

### Video

Examples for video memory manipulation and display operations.

### ViewPort

Examples for viewport configuration and video region management.

## Key Features

The Model1 class provides comprehensive access to TRS-80 Model I hardware:

- **Memory Access**: Read/write memory at any address
- **I/O Operations**: Access I/O ports and system signals
- **Address Space Management**: ROM, RAM, video, and I/O region detection
- **Hardware Control**: Bus signals, interrupts, and timing
- **SD Card Export**: Memory dumping to binary files
- **Debugging Support**: Memory printing and state logging

## Hardware Requirements

All examples require:

- Arduino Mega 2560
- M1Shield (TRS-80 Model I interface shield)
- TRS-80 Model I computer
- SD card (for examples with file operations)

## Getting Started

1. Connect your Arduino Mega 2560 to the M1Shield
2. Connect the M1Shield to your TRS-80 Model I via the 40-pin edge connector
3. Insert an SD card into the shield (for file operations)
4. Load any example and upload to your Arduino
5. Open the Serial Monitor to see debug output

Each example directory contains detailed README files with specific usage instructions.
