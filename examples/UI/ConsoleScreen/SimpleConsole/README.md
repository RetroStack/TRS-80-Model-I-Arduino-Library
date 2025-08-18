# Simple Console Example

This example demonstrates basic ConsoleScreen functionality for terminal-style text output and scrolling.

## What You'll Learn

- How to create and use a basic ConsoleScreen
- How to output text with automatic scrolling
- How to use color-coded output for different message types
- How to integrate ConsoleScreen with M1Shield display system
- Basic console interaction patterns

## What It Does

This simple example provides a foundation for console-style applications:

1. **Basic Console Setup**: Creates a simple console with automatic scrolling
2. **Text Output**: Demonstrates various text output methods
3. **Color Support**: Shows different colors for different message types
4. **Scrolling**: Automatic scrolling when text exceeds screen space
5. **Display Integration**: Proper integration with M1Shield display system

## Key Features

### Simple Console Operations

- **Text Output**: Basic text writing to console
- **Automatic Scrolling**: Text scrolls when screen fills
- **Color Support**: Different colors for various message types
- **Clear Integration**: Simple M1Shield integration

### Display Management

- **Auto-sizing**: Console automatically fits display dimensions
- **Font Management**: Appropriate font sizing for readability
- **Background**: Clean console background and appearance
- **Screen Integration**: Proper M1Shield screen lifecycle

## Technical Implementation

### Console Setup

```cpp
#include <M1Shield.h>
#include "SimpleConsole.h"

SimpleConsole consoleScreen;

void setup() {
    M1Shield.begin(displayProvider);
    M1Shield.setScreen(&consoleScreen);
}
```

### Basic Output

- **Text Output**: Simple text writing methods
- **Color Coding**: Different colors for info, warnings, errors
- **Line Management**: Automatic line wrapping and scrolling
- **Display Updates**: Efficient screen updates

## Sample Output

```
Simple Console Demo
==================
Starting console operations...
[INFO] System initialized successfully
[WARN] Low memory warning
[ERR ] Connection failed - retrying
Status: All systems operational
Debug: Processing complete
Ready for user input...
```

## Hardware Requirements

- Arduino Mega 2560 (required for sufficient GPIO pins)
- M1Shield with compatible display
- Proper display provider selection for your hardware

## DisplayProvider Configuration

Select your display provider:

```cpp
// For ST7789 displays (most common)
#include <Display_ST7789_320x240.h>
Display_ST7789_320x240 displayProvider;

// For other display types, see M1Shield examples
```

## Usage Notes

- Perfect for simple text-based applications
- Automatic scrolling keeps recent text visible
- Color coding improves readability
- Foundation for more complex console applications
- Integrates well with logging systems

## Integration with Other Examples

This simple console example pairs well with:

- **ComprehensiveConsole**: Move here after mastering basics
- **Logger Examples**: Use console for visual logging output
- **M1Shield Examples**: Foundation for text-based interfaces

Simple console provides an easy way to create terminal-style interfaces on the M1Shield display.
