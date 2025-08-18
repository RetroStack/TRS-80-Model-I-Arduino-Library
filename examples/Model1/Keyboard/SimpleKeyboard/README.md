# SimpleKeyboard Example

This example demonstrates basic keyboard input reading from the TRS-80 Model I keyboard.

## What It Does

- **Simple Key Detection**: Shows the easiest way to detect key presses
- **Iterator Method**: Demonstrates advanced change detection
- **Key Interpretation**: Provides helpful information about pressed keys
- **Special Key Recognition**: Identifies common TRS-80 special keys

## What You'll Learn

- Two different methods for reading keyboard input
- Understanding TRS-80 key codes and character mapping
- Special key identification (ENTER, BREAK, arrows, etc.)
- Proper keyboard scanning techniques
- Serial output formatting for key data

## Hardware Requirements

- Arduino Mega 2560
- TRS-80 Model I with 40-pin edge connector interface
- Serial monitor for key press display
- Proper bus control via TEST signal

## Key Functions Demonstrated

- `keyboard.getFirstJustPressedKey()` - Simple key detection
- `keyboard.changes()` - Advanced change iterator
- `KeyboardChangeIterator` usage for detailed change tracking
- Key code interpretation and categorization
- Character vs. control key differentiation

## Special Keys Supported

- **ENTER** (0x0D) - Return/Enter key
- **BACKSPACE** (0x08) - Backspace key
- **SPACE** (0x20) - Space bar
- **BREAK** (0x1B) - Break key
- **Arrow Keys** (0x1C-0x1F) - Directional navigation
- **TAB** (0x09) - Tab key
- **CLEAR** (0x0C) - Clear key

## Serial Output

For each key press, displays:

- Character representation (if printable)
- Hexadecimal key code
- Key type identification
- Helpful descriptions for special keys

## Usage

1. Connect your Arduino to the TRS-80 Model I edge connector
2. Open the Serial Monitor at 9600 baud
3. Upload this sketch to your Arduino Mega 2560
4. Press keys on the TRS-80 keyboard
5. Watch the serial output for detailed key information

This example is perfect for understanding TRS-80 keyboard interfacing and serves as a foundation for applications requiring user input.
