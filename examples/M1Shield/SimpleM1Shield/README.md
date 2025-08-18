# Simple M1Shield Example

This example demonstrates basic M1Shield hardware testing and interaction.

## What You'll Learn

- How to initialize M1Shield with the DisplayProvider system
- How to select the correct display type for your hardware
- How to read button and joystick input
- How to control the LED with different colors
- How to draw basic graphics on the display
- How to use Serial Monitor for debugging

## What It Does

This simple example provides a straightforward introduction to M1Shield:

1. **Display Initialization**: Sets up the display using the DisplayProvider system
2. **Input Monitoring**: Reads all buttons (Menu, Up, Down, Left, Right) and joystick
3. **LED Feedback**: Provides visual feedback with different LED colors
4. **Serial Output**: Shows clear input detection messages
5. **Basic Graphics**: Displays simple test pattern on screen

## Key Features

### Simple Input Detection

- **Button Monitoring**: Detects press/release for all 5 buttons
- **Joystick Reading**: Tracks movement and button press
- **Clear Feedback**: Easy-to-understand Serial Monitor output

### LED Color Feedback

- **Menu Button**: LED turns CYAN
- **Arrow Buttons**: LED turns BLUE
- **Joystick Movement**: LED turns GREEN
- **Joystick Press**: LED turns WHITE
- **No Input**: LED turns OFF

### Display Provider System

- **Easy Selection**: Simple #include and declaration
- **Automatic Setup**: Provider handles all display configuration
- **Multiple Support**: Works with various display types
- **Plug-and-Play**: Just select your display and run

## Display Provider Selection

Select your display by uncommenting the correct provider:

```cpp
// For ST7789 240x320 displays (most common)
#include <Display_ST7789_320x240.h>
Display_ST7789_320x240 displayProvider;

// For smaller ST7735 displays
// #include <Display_ST7735.h>
// Display_ST7735 displayProvider;

// For monochrome OLED displays
// #include <Display_SSD1306.h>
// Display_SSD1306 displayProvider;
```

## Sample Output

```
=== Simple M1Shield Test ===
Learning M1Shield basics...

Display: ST7789_320x240 (320x240)
Initializing M1Shield...
M1Shield ready!
Press buttons and move joystick...
=====================================
Menu: PRESSED
Menu: RELEASED
Up: PRESSED
Joystick: UP
Joystick: CENTER
```

## Hardware Requirements

- Arduino Mega 2560 (required for sufficient GPIO pins)
- M1Shield with compatible display
- Proper display provider selection for your hardware

## Usage Notes

- Start with this example to learn M1Shield basics
- Use Serial Monitor (115200 baud) to see input feedback
- LED provides immediate visual feedback for all inputs
- Safe for all hardware - no risk of damage

## Integration with Other Examples

This simple M1Shield example pairs well with:

- **ComprehensiveM1Shield**: Move here after mastering basics
- **UI Examples**: Foundation for user interface development
- **Hardware Projects**: Base for custom M1Shield applications

Perfect starting point for learning M1Shield hardware interaction!
