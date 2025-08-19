# UI Examples

This directory contains comprehensive examples demonstrating all UI components available in the M1Shield Arduino Library. Each UI component provides different capabilities for creating interactive displays and user interfaces.

## UI Components Overview

### Screen (Base Class)

**Location:** `Screen/`
**Purpose:** Foundation for all UI screens
**Key Features:**

- Screen lifecycle management (open, close, active states)
- Basic input handling
- Display integration
- Memory management

**Examples:**

- **SimpleScreen** - Basic screen implementation
- **ComprehensiveScreen** - Advanced features with animations, input demos, and lifecycle management

**[View Screen Examples and Documentation →](Screen/README.md)**

### ContentScreen (Structured Layout)

**Location:** `ContentScreen/`
**Purpose:** Structured screen layout with header, content area, footer, and progress bar
**Key Features:**

- Automatic layout management
- Title and progress bar support
- User feedback system (notifications, alerts, confirmations)
- Button labels and footer management
- Content area clipping and positioning

**Examples:**

- **SimpleContent** - Basic content screen with text and user feedback
- **ComprehensiveContent** - Advanced layout with multiple modes, real-time data, and interactive elements

**[View ContentScreen Examples and Documentation →](ContentScreen/README.md)**

### MenuScreen (Navigation Interface)

**Location:** `MenuScreen/`
**Purpose:** Menu-based navigation system
**Key Features:**

- Dynamic menu item management
- Pagination for large menus
- Configuration value display
- Menu item enabling/disabling
- Hierarchical menu support

**Examples:**

- **SimpleMenu** - Basic menu implementation
- **ComprehensiveMenu** - Advanced menu with submenus, configuration, and complex navigation

**[View MenuScreen Examples and Documentation →](MenuScreen/README.md)**

### ConsoleScreen (Terminal Interface)

**Location:** `ConsoleScreen/`
**Purpose:** Terminal-like text output with advanced scrolling and paging
**Key Features:**

- Automatic scrolling and paging
- Multiple paging modes (auto-clear, wait-button, wait-timeout)
- Color-coded text output
- Auto-forward functionality
- One-time execution support
- Print interface compatibility

**Examples:**

- **SimpleConsole** - Basic terminal functionality
- **ComprehensiveConsole** - Advanced console with paging, auto-forward, and debugging features

**[View ConsoleScreen Examples and Documentation →](ConsoleScreen/README.md)**

### LoggerScreen (Specialized Display)

**Location:** `LoggerScreen/`
**Purpose:** Specialized screen for logging and monitoring applications
**Key Features:**

- Timestamp integration
- Log level color coding
- Automatic formatting
- Real-time log display

**Examples:**

- **LoggerScreen** - Demonstrates log display and real-time monitoring

**[View LoggerScreen Examples and Documentation →](LoggerScreen/README.md)**

## Quick Start

### 1. Choose Your UI Component

| Component         | Best For                                             | Complexity |
| ----------------- | ---------------------------------------------------- | ---------- |
| **Screen**        | Custom interfaces, games, specialized displays       | Low        |
| **ContentScreen** | Structured applications, dashboards, status displays | Medium     |
| **MenuScreen**    | Navigation, settings, hierarchical interfaces        | Medium     |
| **ConsoleScreen** | Logging, debugging, terminal-style interfaces        | Low-Medium |
| **LoggerScreen**  | System monitoring, data logging, diagnostics         | Low        |

### 2. Start with Simple Examples

Each component has a **Simple** example that demonstrates basic usage:

- `Screen/SimpleScreen/` - Basic screen implementation
- `ContentScreen/SimpleContent/` - Structured layout basics
- `MenuScreen/SimpleMenu/` - Basic menu navigation
- `ConsoleScreen/SimpleConsole/` - Terminal-style output
- `LoggerScreen/` - Logging interface

### 3. Explore Advanced Features

**Comprehensive** examples showcase all available features:

- `Screen/ComprehensiveScreen/` - Animations, lifecycle, input handling
- `ContentScreen/ComprehensiveContent/` - Real-time data, interactive elements
- `MenuScreen/ComprehensiveMenu/` - Submenus, configuration, complex navigation
- `ConsoleScreen/ComprehensiveConsole/` - All paging modes, auto-forward, performance monitoring

## Common Usage Patterns

### Basic Application Structure

```cpp
#include <Display_ST7789_320x240.h>
Display_ST7789_320x240 displayProvider;

#include <M1Shield.h>
#include "MyScreen.h"

MyScreen myScreen;

void setup() {
    M1Shield.begin(displayProvider);
    M1Shield.setScreen(&myScreen);
}

void loop() {
    M1Shield.loop();
}
```

### Screen Navigation

```cpp
Screen* MyScreen::actionTaken(ActionTaken action, uint8_t x, uint8_t y) {
    if (action & BUTTON_MENU) {
        return new NextScreen();  // Navigate to different screen
    }
    return nullptr;  // Stay on current screen
}
```

### Memory Efficiency

```cpp
// Use F-strings for static text to save RAM
setTitleF(F("My Application"));
println(F("Static message"));

// Use regular strings for dynamic content
String status = "Value: " + String(sensorReading);
println(status);
```

## Display Support

All examples support multiple display types. Uncomment the appropriate display in each example:

```cpp
// Most common - ST7789 320x240 landscape
#include <Display_ST7789_320x240.h>
Display_ST7789_320x240 displayProvider;

// Square displays
// #include <Display_ST7789_240x240.h>
// Display_ST7789_240x240 displayProvider;

// Wide displays
// #include <Display_ST7789_320x170.h>
// Display_ST7789_320x170 displayProvider;

// Smaller displays
// #include <Display_ST7735.h>
// Display_ST7735 displayProvider;

// Large displays
// #include <Display_ST7796.h>
// Display_ST7796 displayProvider;

// OLED displays
// #include <Display_SSD1306.h>
// Display_SSD1306 displayProvider;
```

## Hardware Requirements

- **Arduino-compatible board** (ESP32 recommended for performance)
- **M1Shield hardware** with display and buttons
- **Supported display** (see display options above)
- **Optional:** Joystick for enhanced navigation

## Input Controls

All UI components support standard M1Shield controls:

| Button         | Action         | Description                           |
| -------------- | -------------- | ------------------------------------- |
| **MENU**       | Select/Confirm | Primary action button                 |
| **UP/DOWN**    | Navigate       | Vertical navigation                   |
| **LEFT/RIGHT** | Navigate/Back  | Horizontal navigation or back action  |
| **Joystick**   | Enhanced Nav   | 8-directional navigation (if enabled) |

## API Integration

### String Functions

All UI components support three string formats:

```cpp
// C-strings (simple)
setTitle("My Title");

// Arduino Strings (dynamic)
setTitle(String("Score: ") + String(score));

// F-strings (memory efficient)
setTitleF(F("My Title"));
```

### Color Management

```cpp
// Use M1Shield color conversion for consistency
uint16_t white = M1Shield.convertColor(0xFFFF);
uint16_t red = M1Shield.convertColor(0xF800);
setTextColor(white, black);
```

### Memory Management

```cpp
// UI components handle memory automatically
// No need to manually free titles, menu items, etc.
// Just set new values and old memory is freed
```

## Development Tips

### 1. Start Simple

- Begin with Simple examples to understand basic concepts
- Gradually explore Comprehensive examples for advanced features

### 2. Use Appropriate Component

- **Screen:** For custom interfaces requiring full control
- **ContentScreen:** For structured applications with standard layout
- **MenuScreen:** For navigation and settings interfaces
- **ConsoleScreen:** For logging and terminal-style applications
- **LoggerScreen:** For specialized logging needs

### 3. Follow Patterns

- Study existing examples for best practices
- Use F-strings for static text
- Implement proper button handling
- Handle screen lifecycle correctly

### 4. Test on Target Hardware

- Different displays have different capabilities
- Test navigation and readability on actual hardware
- Consider performance on your target Arduino board

## Example Applications

### System Monitor

Use **ConsoleScreen** or **LoggerScreen** for real-time system monitoring with color-coded status messages.

### Settings Interface

Use **MenuScreen** for navigating configuration options with submenus and value displays.

### Dashboard

Use **ContentScreen** for structured data display with real-time updates and user feedback.

### Game Interface

Use **Screen** for custom game interfaces with animations and complex input handling.

### Data Logger

Use **LoggerScreen** or **ConsoleScreen** for continuous data logging with timestamps and automatic scrolling.

## API Documentation

For complete API documentation, see:

- [Screen API](../../../docs/Screen.md)
- [ContentScreen API](../../../docs/ContentScreen.md)
- [MenuScreen API](../../../docs/MenuScreen.md)
- [ConsoleScreen API](../../../docs/ConsoleScreen.md)
- [LoggerScreen API](../../../docs/LoggerScreen.md)
- [M1Shield API](../../../docs/M1Shield.md)

## Support

For questions, issues, or contributions:

- Check the [main README](../../../README.md) for general information
- Review the [API documentation](../../../docs/) for detailed reference
- Examine the example code for practical implementation patterns

Each UI component directory contains its own README.md with specific details and usage examples.
