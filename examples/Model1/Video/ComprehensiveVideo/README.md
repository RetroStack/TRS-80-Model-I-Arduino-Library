# ComprehensiveVideo Example

This example demonstrates advanced video operations, memory manipulation, character conversion, and performance features of the Video class.

## What It Does

- **Advanced Screen Operations**: Complex clearing patterns and boundary testing
- **Character Conversion**: Demonstrates conversion between local and Model1 character sets
- **Memory Operations**: Reads and analyzes video memory contents
- **Viewport Information**: Shows coordinate system and address calculation
- **Performance Features**: Tests auto-scroll behavior and timing
- **Character Mode Switching**: Demonstrates 32/64 character mode changes

## What You'll Learn

- Advanced video memory manipulation techniques
- Character encoding differences between systems
- Video memory address calculation and mapping
- Performance optimization with auto-scroll settings
- Memory reading techniques for screen capture
- Character mode switching effects on display
- Logging integration for debugging video operations

## Hardware Requirements

- Arduino Mega 2560
- TRS-80 Model I with 40-pin edge connector interface
- Serial monitor for memory dump output
- Proper bus control via TEST signal

## Key Functions Demonstrated

- `video.convertLocalCharacterToModel1()` - Character encoding conversion
- `video.convertModel1CharacterToLocal()` - Character decoding conversion
- `video.getAddress()` - Memory address calculation
- `video.setAutoScroll()` - Performance control
- `Model1.readMemory()` - Direct video memory access
- `video.setLogger()` - Debug logging setup
- Memory dump analysis and formatting

## Serial Output

This example produces detailed output on the serial monitor including:

- Video memory hex dumps with ASCII representation
- Memory address mappings
- Character conversion test results
- Performance timing information

## Usage

1. Connect your Arduino to the TRS-80 Model I edge connector
2. Open the Serial Monitor at 115200 baud
3. Upload this sketch to your Arduino Mega 2560
4. Watch both the TRS-80 display and serial output
5. Observe advanced video operations and memory analysis

This example is ideal for developers who need deep control over video operations, memory analysis, or are building diagnostic tools for TRS-80 systems.
