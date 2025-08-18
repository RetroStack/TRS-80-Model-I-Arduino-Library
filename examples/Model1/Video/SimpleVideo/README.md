# SimpleVideo Example

This example demonstrates basic video operations on the TRS-80 Model I using the Video class without viewport constraints.

## What It Does

- **Screen Clearing**: Shows different ways to clear the screen (spaces, characters, patterns)
- **Text Output**: Demonstrates basic text printing at cursor and specific positions
- **Cursor Management**: Shows how to position and track the cursor
- **Scrolling**: Demonstrates single and multi-line scrolling

## What You'll Learn

- How to initialize the Video class for full-screen operations
- Basic text output methods (`print`, `println`)
- Cursor positioning with `setXY`, `getX`, `getY`
- Screen clearing techniques with `cls()`
- Simple scrolling with `scroll()`
- Integration with Cassette class for character mode control

## Hardware Requirements

- Arduino Mega 2560
- TRS-80 Model I with 40-pin edge connector interface
- Proper bus control via TEST signal

## Key Functions Demonstrated

- `video.cls()` - Clear screen with different patterns
- `video.print()` - Print text at cursor or specific coordinates
- `video.setXY()` - Position cursor
- `video.scroll()` - Scroll screen content
- `cassette.set64CharacterMode()` - Set display width

## Usage

1. Connect your Arduino to the TRS-80 Model I edge connector
2. Upload this sketch to your Arduino Mega 2560
3. Watch the display cycle through various text operations
4. Observe how text wraps and scrolls naturally

This example provides a foundation for understanding basic video operations before moving to more advanced features like viewports and memory manipulation.
