# M1Shield Examples

This folder contains examples demonstrating M1Shield hardware functionality and capabilities.

## Overview

The M1Shield is a comprehensive Arduino shield that provides hardware interface capabilities for the TRS-80 Model I, including display, input controls, LED feedback, and user interface framework. These examples show how to use all M1Shield features effectively.

## M1Shield Hardware Components

The M1Shield includes:

- **Display Interface**: Support for various LCD/OLED displays via DisplayProvider system
- **Input Controls**: 5 buttons (Menu, Up, Down, Left, Right) plus analog joystick
- **LED Feedback**: RGB LED for status and visual feedback
- **TRS-80 Interface**: 40-pin edge connector for TRS-80 Model I connection
- **UI Framework**: Built-in screen management and user interface capabilities

## Examples

### [SimpleM1Shield](SimpleM1Shield/README.md)

**Basic M1Shield hardware testing**

Learn fundamental M1Shield concepts including:

- Display initialization with DisplayProvider system
- Button and joystick input detection
- LED color control and feedback
- Basic graphics and text display
- Serial Monitor debugging

### [ComprehensiveM1Shield](ComprehensiveM1Shield/README.md)

**Advanced hardware analysis and diagnostics**

Explore advanced M1Shield capabilities including:

- Comprehensive hardware testing and validation
- Performance monitoring and benchmarking
- Advanced input analysis with statistics
- Multiple display modes and effects
- Real-time diagnostics and health monitoring
- Professional debugging and optimization

## Key Concepts

### DisplayProvider System

The M1Shield uses a flexible DisplayProvider system:

- **Multiple Display Support**: ST7789, ST7735, ILI9341, SSD1306, and more
- **Automatic Configuration**: Each provider optimizes settings for its display
- **Easy Selection**: Simply include the correct provider and declare it
- **Plug-and-Play**: Minimal configuration required

### Input System

Comprehensive input handling:

- **Digital Buttons**: 5 tactile buttons with debouncing
- **Analog Joystick**: X/Y movement plus center button
- **Direction Detection**: Built-in directional interpretation
- **State Tracking**: Easy press/release detection

### Visual Feedback

Multiple feedback mechanisms:

- **RGB LED**: Programmable color feedback
- **Display Graphics**: Full graphics capabilities via Adafruit_GFX
- **Text Display**: Multi-size text rendering
- **Status Indicators**: Visual system status display

## Display Provider Selection

Choose the correct display provider for your hardware:

```cpp
// Common displays (uncomment one):

// ST7789 240x320 displays (landscape: 320x240)
#include <Display_ST7789_320x240.h>
Display_ST7789_320x240 displayProvider;

// ST7789 240x240 square displays
#include <Display_ST7789_240x240.h>
Display_ST7789_240x240 displayProvider;

// ST7735 128x160 displays
#include <Display_ST7735.h>
Display_ST7735 displayProvider;

// Monochrome OLED 128x64 displays
#include <Display_SSD1306.h>
Display_SSD1306 displayProvider;
```

## Important Considerations

### Hardware Requirements

- **Arduino Mega 2560**: Required for sufficient GPIO pins
- **Compatible Display**: Must match selected DisplayProvider
- **Proper Connections**: Ensure all connections are secure
- **Adequate Power**: Sufficient power supply for display and logic

### Display Selection

- **Controller Identification**: Check display controller chip marking
- **Resolution Matching**: Ensure provider matches display resolution
- **Interface Type**: SPI displays are most commonly supported
- **Testing**: Start with most common provider for your display size

### Performance Optimization

- **Provider Selection**: Use provider optimized for your specific display
- **Graphics Efficiency**: Minimize unnecessary display updates
- **Input Polling**: Balance responsiveness with processing load
- **Memory Management**: Monitor memory usage in complex applications

## Common Use Cases

- **Hardware Testing**: Verify M1Shield functionality and connections
- **Input Interface**: Create custom input handling for applications
- **Display Applications**: Build graphical user interfaces
- **System Diagnostics**: Monitor and analyze system performance
- **Prototyping**: Rapid development of TRS-80 interface applications
- **Educational Projects**: Learn embedded systems and hardware interface

## Getting Started

1. **Start with SimpleM1Shield**: Learn basic hardware operation
2. **Select correct display**: Identify and configure your display provider
3. **Test all inputs**: Verify buttons and joystick operation
4. **Explore feedback**: Understand LED and display capabilities
5. **Advanced features**: Move to ComprehensiveM1Shield for detailed analysis

## Integration with Other Examples

M1Shield examples integrate well with:

- **UI Framework Examples**: Use M1Shield as foundation for user interfaces
- **TRS-80 Hardware Examples**: Combine M1Shield with TRS-80 functionality
- **Custom Applications**: Build specialized tools using M1Shield capabilities

The M1Shield provides a complete hardware platform for TRS-80 Model I interface development and general embedded applications.
