# ConsoleScreen Examples

This directory contains examples demonstrating the powerful ConsoleScreen functionality of the M1Shield library. ConsoleScreen provides terminal-like text output with advanced features like automatic scrolling, paging modes, color support, and interactive controls.

## Examples Overview

### SimpleConsole

**Location:** `SimpleConsole/`
**Purpose:** Basic ConsoleScreen demonstration
**Features:**

- Text output with colors
- Automatic scrolling
- Simple user interaction
- Real-time status updates
- Basic console controls

**Best for:** Learning console basics, simple logging applications

### ComprehensiveConsole

**Location:** `ComprehensiveConsole/`
**Purpose:** Advanced ConsoleScreen demonstration showcasing ALL features
**Features:**

- Multiple demonstration modes
- All paging modes (auto-clear, wait-button, wait-timeout, wait-both)
- Auto-forward functionality
- One-time execution demonstration
- Performance monitoring
- Color palette showcase
- Interactive controls with joystick support
- Data streaming simulation
- System log simulation

**Best for:** Understanding all ConsoleScreen capabilities, advanced applications

## ConsoleScreen Key Features

### Text Output & Formatting

- **Colors:** Full color support with easy color management
- **Text Sizes:** Multiple text size multipliers
- **Tabs:** Configurable tab stops for alignment
- **Print Interface:** Standard Arduino Print interface compatibility

### Automatic Scrolling

- **Auto-Clear:** Automatically clears screen when bottom is reached
- **Memory Efficient:** No buffer overflow, continuous operation
- **Smooth Operation:** Seamless text flow

### Paging Modes

- **PAGING_AUTO_CLEAR:** Automatic clearing (default)
- **PAGING_WAIT_BUTTON:** Wait for button press before clearing
- **PAGING_WAIT_TIMEOUT:** Wait for timeout before clearing
- **PAGING_WAIT_BOTH:** Wait for either button or timeout

### Advanced Features

- **Auto-Forward:** Automatic progression with configurable delay
- **One-Time Execution:** Special method called once after opening
- **Interactive Controls:** Button and joystick integration
- **Background Colors:** Customizable console background

## Quick Start

1. **Choose your display** in the .ino file
2. **Upload** the example to your Arduino
3. **Interact** using the M1Shield buttons:
   - **MENU:** Clear screen or continue paging
   - **UP/DOWN:** Navigate modes (Comprehensive example)
   - **LEFT/RIGHT:** Special functions
   - **Joystick:** Enhanced navigation (if enabled)

## API Usage Examples

### Basic Setup

```cpp
#include <ConsoleScreen.h>

class MyConsole : public ConsoleScreen {
public:
    MyConsole() {
        setTitle("My Console");
        setTextColor(0xFFFF);  // White text
        setConsoleBackground(0x0000);  // Black background
        cls();  // Clear screen
    }
};
```

### Text Output with Colors

```cpp
void loop() {
    setTextColor(0x07E0);  // Green
    print("INFO: ");
    setTextColor(0xFFFF);  // White
    println("System running normally");
}
```

### Paging Control

```cpp
void setup() {
    setPagingMode(PAGING_WAIT_BUTTON);
    setPagingTimeout(5000);  // 5 second timeout
}

Screen* actionTaken(ActionTaken action, uint8_t x, uint8_t y) {
    if (action & BUTTON_MENU && isWaitingForPaging()) {
        continuePaging();
    }
    return nullptr;
}
```

### Auto-Forward Feature

```cpp
void setup() {
    setAutoForward(true, 3000);  // 3 second delay
}
```

### One-Time Execution

```cpp
void _executeOnce() override {
    // Called 1 second after console opens
    println("Initialization complete!");
}
```

## Color Reference

Common colors for console output:

- **0xFFFF:** White (general text)
- **0x07E0:** Green (success, info)
- **0xFFE0:** Yellow (warnings)
- **0xF800:** Red (errors)
- **0x001F:** Blue (debug)
- **0x07FF:** Cyan (system messages)
- **0xF81F:** Magenta (user actions)
- **0x8410:** Gray (timestamps, metadata)

## Use Cases

### System Monitoring

Perfect for displaying real-time system status, sensor readings, and log messages with color-coded severity levels.

### Debug Output

Excellent for debugging applications with automatic scrolling and timestamp support.

### Interactive Terminals

Great for command-line style interfaces with user input handling.

### Data Logging

Ideal for continuous data streams with automatic screen management.

### Status Dashboards

Perfect for displaying ongoing system information with automatic updates.

## Tips & Best Practices

1. **Use F-strings** for static text to save RAM: `println(F("Static text"))`
2. **Choose appropriate paging mode** for your use case
3. **Use consistent color coding** for different message types
4. **Implement proper button handling** for interactive applications
5. **Consider auto-forward** for unattended operation
6. **Use timestamps** for log-style applications
7. **Test on your target display size** for optimal layout

## Hardware Requirements

- Compatible Arduino board (ESP32 recommended)
- M1Shield hardware
- Supported display (ST7789, ILI9341, etc.)
- Optional: Joystick for enhanced navigation

## See Also

- [ContentScreen Examples](../ContentScreen/) - Structured layout screens
- [MenuScreen Examples](../MenuScreen/) - Menu navigation screens
- [LoggerScreen Examples](../LoggerScreen/) - Specialized logging screens
- [API Documentation](../../../../docs/) - Complete API reference
