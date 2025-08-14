# ConsoleScreen Class

The `ConsoleScreen` class provides a scrollable terminal-like interface for text output on the M1Shield Arduino library. It extends the `ContentScreen` class and implements Arduino's `Print` interface to create a simple console that automatically clears when full, making it ideal for logging, debugging, and continuous text output applications.

**Print Interface Implementation**: ConsoleScreen fully implements Arduino's `Print` interface, providing all standard Arduino print functionality including automatic support for various data types, formatting options, and base conversions. This ensures full compatibility with the Arduino ecosystem.

## Table of Contents

- [Overview](#overview)
- [Class Hierarchy](#class-hierarchy)
- [Core Features](#core-features)
- [Constructor](#constructor)
- [Text Output Methods](#text-output-methods)
- [Write Interface Methods](#write-interface-methods)
- [Color and Formatting](#color-and-formatting)
  - [setTextColor](#text-color-settings)
  - [setConsoleBackground](#text-color-settings)
  - [setTextSize](#text-color-settings)
  - [setTabSize](#tab-configuration)
- [Screen Management](#screen-management)
  - [cls](#clear-operations)
  - [refresh](#clear-operations)
- [Auto-Paging System](#auto-paging-system)
  - [setPagingMode](#paging-modes)
  - [setPagingTimeout](#paging-modes)
  - [getPagingMode](#paging-modes)
  - [getPagingTimeout](#paging-modes)
  - [isWaitingForPaging](#paging-modes)
  - [continuePaging](#paging-modes)
- [One-Time Execution](#one-time-execution)
- [Examples](#examples)
- [Technical Details](#technical-details)
- [Best Practices](#best-practices)

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

### Auto-Paging System

- **Configurable Behavior**: Four distinct paging modes for different use cases
- **User Control**: Optional pause-and-wait functionality when screen fills
- **Timeout Support**: Configurable automatic continuation after specified delay
- **Button Integration**: M1Shield right button support for manual continuation
- **Visual Feedback**: Clear progress indicators and user prompts
- **Non-Blocking**: Paging waits don't interrupt other application logic

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

### Write Interface Methods

Since ConsoleScreen implements the Print interface, it also provides the core write methods:

```cpp
size_t write(uint8_t c);                                    // Write single character
size_t write(const uint8_t *buffer, size_t size);         // Write buffer of characters
```

**Usage:**

```cpp
// Write single character
console.write('A');                    // Outputs: "A"
console.write('\n');                   // Outputs newline

// Write buffer
uint8_t data[] = {72, 101, 108, 108, 111};  // "Hello"
console.write(data, 5);                     // Outputs: "Hello"

// These are used internally by all print() methods
console.print("Hello");  // Uses write() internally
```

**Note:** These are low-level methods typically used internally by the Print interface. For normal text output, use the print() and println() methods instead.

## Color and Formatting

### Text Color Settings

```cpp
void setTextColor(uint16_t color);
void setTextColor(uint16_t foreground, uint16_t background);
void setConsoleBackground(uint16_t color);
void setTextSize(uint8_t size);
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
console.setConsoleBackground(M1Shield.convertColor(0x0000));

// Set text size (1=smallest, 2=medium, 3=large, etc.)
console.setTextSize(2);
console.println("Large text");
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

## Auto-Paging System

The `ConsoleScreen` includes a sophisticated auto-paging system that provides user control over how the console behaves when it reaches the bottom of the screen. Instead of immediately clearing and wrapping to the top, you can configure the console to pause and wait for user interaction.

### Paging Modes

The auto-paging system supports four distinct modes via the `ConsolePagingMode` enum:

```cpp
enum ConsolePagingMode {
    PAGING_AUTO_CLEAR,    // Default: immediately clear and continue (original behavior)
    PAGING_WAIT_TIMEOUT,  // Wait for timeout, then continue
    PAGING_WAIT_BUTTON,   // Wait for M1Shield right button press to continue
    PAGING_WAIT_BOTH      // Wait for either timeout OR button press
};
```

### Configuration Methods

```cpp
void setPagingMode(ConsolePagingMode mode);
ConsolePagingMode getPagingMode();
void setPagingTimeout(unsigned long timeoutMs);
unsigned long getPagingTimeout();
```

### Status and Control Methods

```cpp
bool isWaitingForPaging();    // Check if console is currently paused waiting
void continuePaging();        // Programmatically continue from paused state
```

### Paging Mode Details

#### PAGING_AUTO_CLEAR (Default)

- **Behavior**: Immediately clears screen when full and continues from top
- **Use Case**: Continuous logging where interruption is not desired
- **Visual**: No pause, seamless operation
- **User Interaction**: None required

#### PAGING_WAIT_TIMEOUT

- **Behavior**: Shows message "--- Press RIGHT or wait 5s to continue ---", waits for timeout
- **Use Case**: Automatic progression with user read time
- **Default Timeout**: 5000ms (5 seconds), configurable
- **Visual**: Progress indicator showing remaining time
- **User Interaction**: Optional - can press right button to skip wait

#### PAGING_WAIT_BUTTON

- **Behavior**: Shows message "--- Press RIGHT button to continue ---", waits indefinitely
- **Use Case**: Manual control, debugging, step-through operation
- **Timeout**: None - waits indefinitely
- **Visual**: Static message with blinking cursor
- **User Interaction**: Required - must press M1Shield right button

#### PAGING_WAIT_BOTH

- **Behavior**: Shows message "--- Press RIGHT or wait 5s to continue ---", waits for either
- **Use Case**: Flexible operation allowing both manual and automatic progression
- **Default Timeout**: 5000ms (5 seconds), configurable
- **Visual**: Progress indicator with button option
- **User Interaction**: Optional - either button press or timeout

### Usage Examples

#### Basic Timeout Paging

```cpp
ConsoleScreen console;

void setup() {
    console.setPagingMode(PAGING_WAIT_TIMEOUT);
    console.setPagingTimeout(3000);  // 3 second wait
}

void loop() {
    static int counter = 0;

    console.print("Message ");
    console.println(counter++);

    delay(100);  // Generate messages quickly to trigger paging
    console.loop();  // Essential for paging system
}
```

#### Manual Button Control

```cpp
ConsoleScreen console;

void setup() {
    console.setPagingMode(PAGING_WAIT_BUTTON);
    // No timeout - waits for button indefinitely
}

void loop() {
    if (someCondition()) {
        console.println("Debug message that requires user attention");
        // Console will pause when full, waiting for right button
    }

    console.loop();  // Essential for button handling
}
```

#### Flexible Both Mode

```cpp
ConsoleScreen console;

void setup() {
    console.setPagingMode(PAGING_WAIT_BOTH);
    console.setPagingTimeout(7000);  // 7 second auto-continue
}

void loop() {
    static unsigned long lastMessage = 0;

    if (millis() - lastMessage > 500) {
        lastMessage = millis();

        console.print("Timestamp: ");
        console.println(millis());

        // Users can either wait 7 seconds or press button to continue
    }

    console.loop();
}
```

#### Programmatic Control

```cpp
ConsoleScreen console;

void setup() {
    console.setPagingMode(PAGING_WAIT_BUTTON);
}

void loop() {
    // Fill up the console
    for (int i = 0; i < 100; i++) {
        console.print("Line ");
        console.println(i);

        console.loop();  // Allow paging to trigger

        // Check if we're waiting and handle programmatically
        if (console.isWaitingForPaging()) {
            delay(2000);  // Simulate some processing
            console.continuePaging();  // Continue programmatically
            break;  // Exit loop since paging happened
        }
    }
}
```

### Visual Feedback System

The paging system provides clear visual feedback to users:

#### Timeout Mode Messages

- **Initial**: "--- Press RIGHT or wait 5s to continue ---"
- **Progress**: "--- Press RIGHT or wait 3s to continue ---" (countdown)
- **Completion**: Message clears, screen clears, normal operation resumes

#### Button Mode Messages

- **Message**: "--- Press RIGHT button to continue ---"
- **Visual Cue**: Blinking text or cursor to indicate waiting state
- **Completion**: Message clears on button press, screen clears

#### Color Coding

- **Paging Messages**: Displayed in distinguishable color (typically cyan/yellow)
- **Normal Text**: Resumes with previously set text colors
- **Preservation**: Text colors are preserved across paging operations

### Integration with M1Shield

The paging system is fully integrated with the M1Shield button system:

```cpp
// In your main loop, M1Shield.loop() handles button detection
void loop() {
    M1Shield.loop();     // Handles button state changes
    console.loop();      // Handles paging based on button state

    // Your application code here
}
```

**Button Behavior:**

- **Right Button**: Continues paging when in wait state
- **Button Debouncing**: Handled automatically by M1Shield
- **State Management**: Paging system tracks button press timing
- **Non-Interference**: Button only responds during paging wait states

### Advanced Configuration

#### Dynamic Mode Switching

```cpp
void switchToDebugMode() {
    console.setPagingMode(PAGING_WAIT_BUTTON);  // Manual step-through
    console.println("Entering debug mode - manual paging enabled");
}

void switchToProductionMode() {
    console.setPagingMode(PAGING_AUTO_CLEAR);   // Continuous operation
    console.println("Production mode - auto-clear enabled");
}
```

#### Conditional Paging

```cpp
void conditionalPaging() {
    if (debugMode) {
        console.setPagingMode(PAGING_WAIT_BUTTON);
    } else if (slowMode) {
        console.setPagingMode(PAGING_WAIT_TIMEOUT);
        console.setPagingTimeout(2000);
    } else {
        console.setPagingMode(PAGING_AUTO_CLEAR);
    }
}
```

### Performance Considerations

#### Memory Usage

- **Minimal Overhead**: Paging adds ~50 bytes of RAM usage
- **No Content Storage**: Still no buffering of console content
- **Efficient State**: Simple state machine for paging logic

#### Timing Considerations

- **Non-Blocking**: Paging wait states don't block other code execution
- **Loop Dependency**: Requires regular calls to `console.loop()` for proper operation
- **Timeout Accuracy**: Uses millis() for timeout tracking (1ms resolution)

#### Button Responsiveness

- **Immediate Response**: Button presses detected on next `console.loop()` call
- **Debouncing**: Handled by M1Shield system automatically
- **State Cleanup**: Proper state management prevents missed button presses

### Best Practices for Paging

#### Essential Loop Call

```cpp
void loop() {
    // ALWAYS include these for proper paging operation
    M1Shield.loop();     // Button detection
    console.loop();      // Paging state management

    // Your application code
}
```

#### Mode Selection Guidelines

- **PAGING_AUTO_CLEAR**: Fast logging, production systems, continuous monitoring
- **PAGING_WAIT_TIMEOUT**: User-friendly logging, casual debugging, demo applications
- **PAGING_WAIT_BUTTON**: Interactive debugging, step-through operation, educational demos
- **PAGING_WAIT_BOTH**: Flexible systems, user preference accommodation, mixed-use scenarios

#### Timeout Configuration

- **Fast Systems**: 2-3 seconds for quick progression
- **Reading Applications**: 5-7 seconds for comfortable reading time
- **Debug Applications**: 10+ seconds for detailed analysis
- **Demo Applications**: 3-5 seconds for presentation pacing

#### Error Handling

```cpp
// Check for valid timeout values
if (timeoutValue < 1000) {
    console.setPagingTimeout(1000);  // Minimum 1 second
}

// Verify paging state before critical operations
if (!console.isWaitingForPaging()) {
    console.println("Critical system message");
}
```

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
        setConsoleBackground(M1Shield.convertColor(0x0000));
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

### Auto-Paging Console Example

```cpp
class PagingConsole : public ConsoleScreen {
private:
    unsigned long lastMessageTime = 0;
    int messageCounter = 0;

protected:
    void _executeOnce() override {
        cls();
        setTextColor(0x07FF);
        println("=== PAGING DEMO CONSOLE ===");
        setTextColor(0xFFFF);
        println("Messages will auto-generate...");
        println("Console will pause when full");
        println("Press RIGHT button or wait for timeout");
        println("------------------------");
    }

public:
    PagingConsole() {
        _setTitle("Paging Demo");

        // Configure paging for demo
        setPagingMode(PAGING_WAIT_BOTH);    // Button OR timeout
        setPagingTimeout(4000);             // 4 second auto-continue
    }

    void loop() override {
        // Generate messages periodically
        if (millis() - lastMessageTime > 800) {
            lastMessageTime = millis();

            // Different types of messages for variety
            if (messageCounter % 4 == 0) {
                setTextColor(0x07E0);
                print("[INFO] ");
            } else if (messageCounter % 4 == 1) {
                setTextColor(0xFFE0);
                print("[WARN] ");
            } else if (messageCounter % 4 == 2) {
                setTextColor(0xF800);
                print("[ERROR] ");
            } else {
                setTextColor(0x07FF);
                print("[DEBUG] ");
            }

            setTextColor(0xFFFF);
            print("Message #");
            print(messageCounter);
            print(" at ");
            print(millis() / 1000);
            println("s");

            messageCounter++;
        }

        ConsoleScreen::loop();  // Essential for paging system
    }

    // Method to demonstrate dynamic paging mode changes
    void changePagingMode(ConsolePagingMode newMode) {
        setPagingMode(newMode);

        setTextColor(0xF81F);
        print(">>> Paging mode changed to: ");

        switch (newMode) {
            case PAGING_AUTO_CLEAR:
                println("AUTO_CLEAR");
                break;
            case PAGING_WAIT_TIMEOUT:
                println("WAIT_TIMEOUT");
                break;
            case PAGING_WAIT_BUTTON:
                println("WAIT_BUTTON");
                break;
            case PAGING_WAIT_BOTH:
                println("WAIT_BOTH");
                break;
        }
        setTextColor(0xFFFF);
    }
};

// Usage in main program
PagingConsole console;

void setup() {
    // Console initializes automatically with paging enabled
}

void loop() {
    M1Shield.loop();    // Essential for button detection
    console.loop();     // Essential for paging system and message generation

    // Example: Change paging mode based on some condition
    static unsigned long lastModeChange = 0;
    if (millis() - lastModeChange > 20000) {  // Change mode every 20 seconds
        static int modeIndex = 0;
        ConsolePagingMode modes[] = {
            PAGING_WAIT_BOTH,
            PAGING_WAIT_TIMEOUT,
            PAGING_WAIT_BUTTON,
            PAGING_AUTO_CLEAR
        };

        console.changePagingMode(modes[modeIndex]);
        modeIndex = (modeIndex + 1) % 4;
        lastModeChange = millis();
    }
}
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

### Auto-Paging System

- **Always call `M1Shield.loop()` and `console.loop()`** in your main loop for proper paging operation
- Choose appropriate paging mode for your use case:
  - `PAGING_AUTO_CLEAR`: Production systems, continuous monitoring
  - `PAGING_WAIT_TIMEOUT`: User-friendly interfaces, demos
  - `PAGING_WAIT_BUTTON`: Interactive debugging, step-through operation
  - `PAGING_WAIT_BOTH`: Flexible systems accommodating user preferences
- Configure timeouts appropriately (2-3s for fast systems, 5-7s for reading, 10+s for debugging)
- Check `isWaitingForPaging()` before critical operations if needed
- Use `continuePaging()` for programmatic control when appropriate

### Efficient Output

- Use `print()` for building lines, `println()` to complete them
- Minimize color changes for better performance
- Clear screen periodically in long-running applications

### Memory Management

- Let auto-clear handle memory - don't manually clear unless needed
- Avoid storing console content in variables
- Use for streaming output, not persistent display
- Paging adds minimal memory overhead (~50 bytes)

### Color Usage

- Set colors before text output
- Use consistent color schemes for message types
- Reset to default colors after special formatting

### One-Time Initialization

- Use `_executeOnce()` for setup that should happen only once
- Keep initialization fast to avoid blocking
- Set title and initial state in constructor

The `ConsoleScreen` class provides a simple, efficient solution for scrolling text output with automatic memory management, comprehensive auto-paging capabilities, and full Arduino Print interface compatibility. The configurable paging system allows for flexible user interaction patterns, from continuous automatic operation to interactive step-through debugging, making it ideal for debugging interfaces, logging systems, demonstration applications, and any application requiring continuous text display while maintaining seamless integration with the Arduino ecosystem.
