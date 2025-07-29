# ConsoleScreen Class

The `ConsoleScreen` class provides a scrollable terminal-like interface for text output on the M1Shield Arduino library. It extends the `ContentScreen` class and implements Arduino's `Print` interface to create a simple console that automatically clears when full, making it ideal for logging, debugging, and continuous text output applications.

**Print Interface Implementation**: ConsoleScreen fully implements Arduino's `Print` interface, providing all standard Arduino print functionality including automatic support for various data types, formatting options, and base conversions. This ensures full compatibility with the Arduino ecosystem.

## Table of Contents

1. [Overview](#overview)
2. [Class Hierarchy](#class-hierarchy)
3. [Core Features](#core-features)
4. [Constructor](#constructor)
5. [Text Output Methods](#text-output-methods)
6. [Color and Formatting](#color-and-formatting)
7. [Screen Management](#screen-management)
8. [One-Time Execution](#one-time-execution)
9. [Examples](#examples)
10. [Technical Details](#technical-details)
11. [Best Practices](#best-practices)

## Overview

`ConsoleScreen` creates a simple scrollable terminal interface that displays text output sequentially. When the text reaches the bottom of the screen, the entire console is automatically cleared and text continues from the top (wrap-around behavior). This makes it memory efficient and ideal for continuous logging or debugging output.

**Key characteristics:**

- Inherits from `ContentScreen` for structured layout
- Sequential text output only (no coordinate positioning)
- Automatic screen clearing when content reaches bottom
- Memory efficient - doesn't store written content
- Color support for text foreground and background
- Tab and newline character support
- One-time execution capability for initialization
- **Full Arduino Print Interface**: Inherits all standard Arduino print methods

### Arduino Print Interface Support

- Inherits from `Print` class for full Arduino ecosystem compatibility
- Automatic support for all data types: int, long, float, double, char, String
- Base conversion support: print(value, HEX), print(value, BIN), print(value, OCT)
- Precision control for floating point: print(3.14159, 2) → "3.14"
- Can be used anywhere a Print stream is expected
- Compatible with stream operators and printf-style libraries

## Class Hierarchy

```
Screen
    ↓
ContentScreen
    ↓
ConsoleScreen (implements Print interface)
```

**Print Interface**: ConsoleScreen inherits from Arduino's `Print` class, providing full compatibility with Arduino's print ecosystem and all standard print methods automatically.

## Core Features

### Automatic Screen Clearing

- Text automatically wraps to top when reaching screen bottom
- Entire console clears and starts fresh (memory efficient)
- No coordinate positioning - sequential output only
- Content is not stored in memory once rendered

### Simple Text Output

- `print()` and `println()` methods for text output
- Support for String, char\*, int, and float data types
- Newline character (\n) support
- Tab character support with configurable tab stops

### Color Support

- Foreground and background color settings
- Color applies to subsequently printed text
- Standard ST77XX color constants supported

### Memory Efficiency

- No internal text buffer storage
- Auto-clear behavior prevents memory buildup
- Ideal for continuous logging applications

## Constructor

```cpp
ConsoleScreen();
```

Creates a new console screen instance with default settings:

- Inherits content area management from ContentScreen
- Initializes text positioning at top-left of content area
- Sets default colors (white text on black background)
- Configures standard tab size (4 characters)

## Text Output Methods

### Print Methods (No Newline)

```cpp
void print(const String& text);
void print(const char* text);
void print(int value);
void print(float value, int decimals = 2);
```

**Usage:**

```cpp
ConsoleScreen console;

console.print("Temperature: ");
console.print(25.6, 1);
console.print("°C");
// Output: "Temperature: 25.6°C"
```

### Println Methods (With Newline)

```cpp
void println(const String& text);
void println(const char* text);
void println(int value);
void println(float value, int decimals = 2);
void println();  // Print newline only
```

**Usage:**

```cpp
console.println("=== SYSTEM STATUS ===");
console.println("CPU Temp: 45°C");
console.println("Memory: 75% used");
console.println();  // Blank line
```

### Arduino Print Interface Methods

Since ConsoleScreen implements the Print interface, all standard Arduino print methods are automatically available:

```cpp
console.print(42);                    // Integer: "42"
console.print(3.14159, 2);           // Float with 2 decimals: "3.14"
console.print(255, HEX);             // Hexadecimal: "FF"
console.print(255, BIN);             // Binary: "11111111"
console.print('A');                  // Single character: "A"
console.print("Hello");              // C-string: "Hello"
console.print(String("World"));      // String object: "World"

// All data types work with println() too
console.println(millis());           // Current time in milliseconds
console.println(analogRead(A0));     // Sensor reading
console.println(42.0f);              // Float value

// Can be used as a Print stream
void logMessage(Print& output, const char* msg) {
    output.print("[LOG] ");
    output.println(msg);
}
logMessage(console, "System ready");  // Works seamlessly
```

## Color and Formatting

### Text Color Settings

```cpp
void setTextColor(uint16_t color);
void setTextColor(uint16_t foreground, uint16_t background);
void setConsoleBgColor(uint16_t color);
```

**Example:**

```cpp
// Set text color only
console.setTextColor(M1Shield.convertColor(0x07E0));
console.println("SUCCESS: Operation completed");

// Set both foreground and background
console.setTextColor(M1Shield.convertColor(0xFFFF), M1Shield.convertColor(0xF800));
console.println("ERROR: Critical failure");

// Set console background color
console.setConsoleBgColor(M1Shield.convertColor(0x0000));
```

### Available Colors

```cpp
0x0000    0xFFFF    0xF800      0x07E0
0x001F     0x07FF     0xF81F  0xFFE0
```

### Tab Configuration

```cpp
void setTabSize(uint8_t size);
uint8_t getTabSize();
```

**Usage:**

```cpp
console.setTabSize(8);        // Set 8-character tab stops
console.print("Name:\tValue\n");
console.print("Temp:\t25.6°C\n");
```

## Screen Management

### Clear Operations

```cpp
void cls();           // Clear screen and reset cursor to top
void refresh();       // Redraw screen (clears and resets)
```

**Usage:**

```cpp
console.cls();        // Clear all text and start fresh
console.refresh();    // Same as cls() - redraw screen
```

### Screen State

```cpp
bool hasWrapped();    // Check if screen has wrapped around
```

The console automatically wraps when text reaches the bottom, clearing the entire screen and continuing from the top.

## One-Time Execution

The `ConsoleScreen` supports one-time execution for initialization tasks that should only run once after the screen is opened.

### Protected Method Override

```cpp
protected:
    virtual void _executeOnce();
```

This method is automatically called once, approximately 1 second after the screen is first opened. Override it in your derived class for initialization tasks.

**Example:**

```cpp
class StatusConsole : public ConsoleScreen {
protected:
    void _executeOnce() override {
        cls();
        setTextColor(M1Shield.convertColor(0x07FF));
        println("=== STATUS CONSOLE ===");
        setTextColor(M1Shield.convertColor(0xFFFF));
        println("System initialized");
        println("Ready for operation!");
    }

public:
    StatusConsole() {
        _setTitle("System Status");
        // _executeOnce() will be called automatically after ~1 second
    }
};
```

## Examples

### Basic Console Usage with Print Interface

```cpp
ConsoleScreen console;

void setup() {
    // Console will automatically initialize
}

void loop() {
    static unsigned long lastUpdate = 0;

    if (millis() - lastUpdate > 2000) {  // Every 2 seconds
        lastUpdate = millis();

        // Using Print interface methods automatically
        console.print("Uptime: ");
        console.print(millis() / 1000);    // Print automatically handles unsigned long
        console.println(" seconds");

        console.print("Free RAM: ");
        console.println(freeMemory());      // Print automatically handles int

        // Show sensor data with formatting
        float temperature = 23.456;
        console.print("Temp: ");
        console.print(temperature, 1);      // 1 decimal place: "23.5"
        console.println("°C");

        // Show hex values
        console.print("Status: 0x");
        console.println(getStatusRegister(), HEX);  // Print in hexadecimal
    }

    console.loop();  // Required for screen updates
}

// Function that accepts any Print stream - console works seamlessly
void logToAnyStream(Print& stream, const char* message) {
    stream.print("[");
    stream.print(millis());
    stream.print("] ");
    stream.println(message);
}

void somewhereInCode() {
    logToAnyStream(console, "This works!");  // ConsoleScreen is a Print stream
    logToAnyStream(Serial, "This also works!");  // Same function works with Serial
}
```

### Debug Console with Colors

```cpp
class DebugConsole : public ConsoleScreen {
private:
    unsigned long _messageCount = 0;

protected:
    void _executeOnce() override {
        cls();
        setTextColor(M1Shield.convertColor(0x07E0), M1Shield.convertColor(0x0000));
        println("=== DEBUG CONSOLE ===");
        setTextColor(M1Shield.convertColor(0xFFFF), M1Shield.convertColor(0x0000));
        println("Logging system ready");
        println("------------------------");
    }

public:
    DebugConsole() {
        _setTitle("Debug Output");
        setConsoleBgColor(M1Shield.convertColor(0x0000));
    }

    void logInfo(const char* message) {
        setTextColor(M1Shield.convertColor(0x07FF));
        print("[INFO] ");
        setTextColor(M1Shield.convertColor(0xFFFF));
        println(message);
        _messageCount++;
    }

    void logWarning(const char* message) {
        setTextColor(0xFFE0);
        print("[WARN] ");
        setTextColor(0xFFFF);
        println(message);
        _messageCount++;
    }

    void logError(const char* message) {
        setTextColor(0xF800);
        print("[ERROR] ");
        setTextColor(0xFFFF);
        println(message);
        _messageCount++;
    }

    void showStats() {
        println();
        setTextColor(0xF81F);
        print("Messages logged: ");
        println(_messageCount);
        setTextColor(0xFFFF);
    }
};
```

### Serial Monitor Style Console

```cpp
class SerialConsole : public ConsoleScreen {
protected:
    void _executeOnce() override {
        cls();
        setTextColor(0xFFFF, 0x001F);
        println(" SERIAL MONITOR ");
        setTextColor(0xFFFF, 0x0000);
        println();
    }

public:
    SerialConsole() {
        _setTitle("Serial Monitor");
        setTabSize(4);
    }

    void loop() override {
        // Check for serial data
        if (Serial.available()) {
            String data = Serial.readString();
            data.trim();

            if (data.length() > 0) {
                print(">> ");
                println(data);
            }
        }

        ConsoleScreen::loop();
    }

    void echo(const char* text) {
        print("<< ");
        println(text);
    }
};
```

### Multi-Line Output Example

```cpp
class SystemStatus : public ConsoleScreen {
protected:
    void _executeOnce() override {
        cls();
        displaySystemInfo();
    }

public:
    void displaySystemInfo() {
        cls();

        // Header
        setTextColor(0x07E0);
        println("================================");
        println("       SYSTEM STATUS");
        println("================================");
        setTextColor(0xFFFF);
        println();

        // System info
        print("Arduino:\t");
        println("Nano");

        print("Clock:\t\t");
        println("16 MHz");

        print("RAM:\t\t");
        print(freeMemory());
        println(" bytes free");

        print("Uptime:\t\t");
        print(millis() / 1000);
        println(" seconds");

        println();

        // Status indicators
        setTextColor(0x07E0);
        println("✓ System operational");
        println("✓ Sensors active");
        setTextColor(0xFFE0);
        println("⚠ Low memory warning");
        setTextColor(0xFFFF);
    }
};
```

## Technical Details

### Print Interface Implementation

- Implements Arduino's `Print` class through `write(uint8_t)` method
- All Print methods automatically use the `write()` implementation
- Optimized `write(buffer, size)` method for bulk character output
- Full compatibility with Arduino ecosystem and third-party libraries
- Can be passed to any function expecting a `Print&` parameter

### Memory Management

- No internal text buffer - text is rendered directly to display
- Automatic screen clearing prevents memory accumulation
- Minimal RAM footprint regardless of output volume

### Text Positioning

- Uses pixel-based positioning internally
- Character width and height calculated based on text size
- Automatic line wrapping at content area boundaries

### Auto-Clear Behavior

- Triggers when next line would exceed content area bottom
- Clears entire console and resets cursor to top-left
- No partial scrolling - complete refresh for memory efficiency

### Performance Characteristics

- Very low memory usage
- Fast rendering with direct display writes
- No string storage or manipulation overhead

## Best Practices

### Print Interface Usage

- Leverage inherited Print methods for automatic data type handling
- Use base conversion for debugging: `console.print(value, HEX)`
- Take advantage of precision control: `console.print(3.14159, 2)`
- Pass console to functions expecting Print streams for code reuse
- Use `write()` method directly for single characters when performance matters

### Efficient Output

- Use `print()` for building lines, `println()` to complete them
- Minimize color changes for better performance
- Clear screen periodically in long-running applications

### Memory Management

- Let auto-clear handle memory - don't manually clear unless needed
- Avoid storing console content in variables
- Use for streaming output, not persistent display

### Color Usage

- Set colors before text output
- Use consistent color schemes for message types
- Reset to default colors after special formatting

### One-Time Initialization

- Use `_executeOnce()` for setup that should happen only once
- Keep initialization fast to avoid blocking
- Set title and initial state in constructor

The `ConsoleScreen` class provides a simple, efficient solution for scrolling text output with automatic memory management and full Arduino Print interface compatibility. This makes it ideal for debugging interfaces, logging systems, and any application requiring continuous text display while maintaining seamless integration with the Arduino ecosystem.
