# LoggerScreen

The `LoggerScreen` is a visual logging destination that extends `ConsoleScreen` with logging functionality compatible with the `ILogger` interface. It provides real-time, on-screen logging with formatted output, color coding, timestamps, and an optional rotational buffer for preserving log history - perfect for debugging and monitoring applications on the M1Shield display.

## Table of Contents

- [Features](#features)
- [Usage](#usage)
  - [Basic Logger Screen](#basic-logger-screen)
  - [Rotational Buffer Usage](#rotational-buffer-usage)
  - [Integration with CompositeLogger](#integration-with-compositelogger)
  - [Configuration Options](#configuration-options)
- [API Reference](#api-reference)
  - [Constructor](#constructor)
  - [Configuration Methods](#configuration-methods)
    - [setTimestampEnabled](#void-settimestampenabledbool-enabled)
    - [isTimestampEnabled](#bool-istimestampenabled-const)
    - [setColorCodingEnabled](#void-setcolorcodingenabledbool-enabled)
    - [isColorCodingEnabled](#bool-iscolorcodingenabled-const)
    - [resetTimestamp](#void-resettimestamp)
  - [Buffer Management](#buffer-management)
    - [setLogBufferSize](#void-setlogbuffersizeuint16_t-size)
    - [getLogBufferSize](#uint16_t-getlogbuffersize-const)
    - [clearLogBuffer](#void-clearlogbuffer)
    - [getLogBufferCount](#uint16_t-getlogbuffercount-const)
  - [ILogger Compatibility](#ilogger-compatibility)
    - [asLogger](#ilogger-aslogger)
  - [Logging Interface](#logging-interface)
    - [info](#void-infoconst-char-fmt-)
    - [warn](#void-warnconst-char-fmt-)
    - [err](#void-errconst-char-fmt-)
    - [debug](#void-debugconst-char-fmt-)
  - [Inherited from ConsoleScreen](#inherited-from-consolescreen)
- [Visual Output Format](#visual-output-format)
- [Use Cases](#use-cases)
- [Performance Considerations](#performance-considerations)
- [Integration with M1Shield](#integration-with-m1shield)
- [Thread Safety](#thread-safety)

## Features

- **Visual Logging**: Display log messages directly on the M1Shield screen
- **Rotational Buffer**: Optional circular buffer to preserve log history when screen is inactive
- **Automatic Replay**: Buffered entries are automatically replayed when screen opens
- **ILogger Compatibility**: Works with CompositeLogger through adapter pattern
- **Color-Coded Levels**: Different colors for INFO (white), WARN (yellow), ERR (red), and DEBUG (cyan)
- **Timestamps**: Optional relative timestamps showing elapsed time
- **Auto-Scrolling**: Automatic screen clearing when full for continuous logging
- **Print Interface**: Inherited from ConsoleScreen for direct text output
- **Real-Time Feedback**: Immediate visual feedback for debugging and monitoring

## Usage

### Basic Logger Screen

```cpp
#include <LoggerScreen.h>
#include <M1Shield.h>

LoggerScreen* logScreen = new LoggerScreen("System Monitor");

void setup() {
    M1Shield.begin(displayProvider);
    M1Shield.setScreen(logScreen);

    // Log some messages
    logScreen->info("System starting up...");
    logScreen->warn("Configuration using defaults");
    logScreen->err("Sensor not detected");
}

void loop() {
    // Your application code here
    logScreen->info("Loop iteration %d", loopCount++);
    delay(1000);
}
```

### Rotational Buffer Usage

The rotational buffer allows you to preserve log history even when the screen is inactive, perfect for capturing system startup logs or background activity.

```cpp
#include <LoggerScreen.h>

LoggerScreen* logger = new LoggerScreen("Buffered Logger");

void setup() {
    M1Shield.begin(displayProvider);

    // Configure buffer for 20 log entries
    logger->setLogBufferSize(20);

    // Pre-fill buffer with historical logs (before activating screen)
    logger->info("System initialization started");
    logger->debug("Loading configuration from EEPROM");
    logger->warn("Default configuration loaded");
    logger->info("Network interface initializing");
    logger->err("WiFi connection failed, retrying...");
    logger->info("Connected to WiFi successfully");
    logger->info("System ready for operation");

    // Later, when user opens logger screen...
    // All buffered entries will be replayed in chronological order
    M1Shield.setScreen(logger);

    // Buffer status
    Serial.print("Buffer has ");
    Serial.print(logger->getLogBufferCount());
    Serial.print("/");
    Serial.print(logger->getLogBufferSize());
    Serial.println(" entries");
}

void loop() {
    // Continue logging - entries go to both buffer and display
    logger->info("Runtime status: %d", millis());
    delay(5000);
}

// Buffer management
void clearHistory() {
    logger->clearLogBuffer();  // Clear all buffered entries
}

void disableBuffer() {
    logger->setLogBufferSize(0);  // Disable buffering
}
```

**Key Buffer Features:**

- **Circular Buffer**: Oldest entries are overwritten when buffer is full
- **Persistent Logging**: Entries are captured even when screen is inactive
- **Automatic Replay**: All buffered entries shown when screen opens
- **Color Preservation**: Replayed entries maintain original colors
- **Memory Efficient**: Buffer size is configurable (0 = disabled)

### Integration with CompositeLogger

```cpp
#include <CompositeLogger.h>
#include <SerialLogger.h>
#include <LoggerScreen.h>

SerialLogger serialLogger;
LoggerScreen* screenLogger = new LoggerScreen("Debug Log");
CompositeLogger multiLogger;

void setup() {
    Serial.begin(115200);
    M1Shield.begin(displayProvider);

    // Set up multi-destination logging
    multiLogger.addLogger(&serialLogger);           // Log to Serial
    multiLogger.addLogger(screenLogger->asLogger()); // Log to screen (use asLogger())

    M1Shield.setScreen(screenLogger);

    // Now logs appear in both Serial Monitor and on screen
    multiLogger.info("Multi-destination logging active");
    multiLogger.warn("System memory: %d bytes free", getFreeMemory());
}
```

### Configuration Options

```cpp
LoggerScreen* logger = new LoggerScreen("Custom Logger");

// Configure display options
logger->setTimestampEnabled(false);      // Disable timestamps
logger->setColorCodingEnabled(false);    // Use monochrome output
logger->setTextSize(2);                  // Larger text
logger->setConsoleBackground(M1Shield.convertColor(0x0010));    // Dark blue background

// Reset timestamp counter
logger->resetTimestamp();
```

## API Reference

### Constructor

```cpp
LoggerScreen(const char* title = "Logger")
```

Creates a new logger screen with the specified title.

### Configuration Methods

#### `void setTimestampEnabled(bool enabled)`

#### `bool isTimestampEnabled() const`

Control whether timestamps are displayed in log messages.

#### `void setColorCodingEnabled(bool enabled)`

#### `bool isColorCodingEnabled() const`

Control whether different log levels use different colors.

#### `void resetTimestamp()`

Reset the timestamp reference point to the current time.

### Buffer Management

#### `void setLogBufferSize(uint16_t size)`

Configure the size of the rotational log buffer. Set to 0 to disable buffering.

**Parameters:**

- `size` - Number of log entries to buffer (0 = disabled)

**Example:**

```cpp
logger->setLogBufferSize(50);  // Buffer 50 entries
logger->setLogBufferSize(0);   // Disable buffering
```

#### `uint16_t getLogBufferSize() const`

Get the current buffer size.

**Returns:** Current buffer size (0 if disabled)

#### `void clearLogBuffer()`

Clear all entries from the log buffer.

**Example:**

```cpp
logger->clearLogBuffer();  // Remove all buffered entries
```

#### `uint16_t getLogBufferCount() const`

Get the number of log entries currently stored in the buffer.

**Returns:** Number of entries in buffer (0 to buffer size)

**Example:**

```cpp
uint16_t used = logger->getLogBufferCount();
uint16_t total = logger->getLogBufferSize();
Serial.print("Buffer: ");
Serial.print(used);
Serial.print("/");
Serial.print(total);
Serial.println(" entries");
```

### ILogger Compatibility

#### `ILogger* asLogger()`

Get an ILogger adapter for this LoggerScreen. Use this method to add the LoggerScreen to a CompositeLogger:

```cpp
LoggerScreen* screen = new LoggerScreen("Debug");
CompositeLogger composite;
composite.addLogger(screen->asLogger());  // Use asLogger()
```

### Logging Interface

#### `void info(const char *fmt, ...)`

#### `void warn(const char *fmt, ...)`

#### `void err(const char *fmt, ...)`

#### `void debug(const char *fmt, ...)`

Standard logging methods with printf-style formatting. Messages are displayed with appropriate formatting and colors.

**Note:** The adapter returned by `asLogger()` also supports String and F() macro overloads through the ILogger interface:

```cpp
ILogger* adapter = screen->asLogger();
adapter->info("Temperature: %dC", temp);           // printf-style
adapter->warn(String("Status: ") + statusText);    // String support
adapter->err(F("System error"));                   // F() macro support
adapter->debug(F("Debug: state=%s"), currentState); // Debug with format
```

### Inherited from ConsoleScreen

The LoggerScreen inherits all ConsoleScreen functionality:

```cpp
// Print interface methods
size_t write(uint8_t ch)
size_t print(...)
size_t println(...)

// Console control
void cls()
void refresh()
void setTextColor(uint16_t foreground, uint16_t background = 0)
void setConsoleBackground(uint16_t color)
void setTextSize(uint8_t size)
void setTabSize(uint8_t size)
```

## Visual Output Format

### With Timestamps (Default)

```
[12:34] [INFO] System initialized
[12:35] [WARN] Low memory: 512 bytes
[12:36] [ERR ] Sensor timeout on pin 7
[12:37] [DBUG] Variable x=42, state=READY
```

### Without Timestamps

```
[INFO] System initialized
[WARN] Low memory: 512 bytes
[ERR ] Sensor timeout on pin 7
[DBUG] Variable x=42, state=READY
```

### Color Coding

- **INFO**: White text - normal system information
- **WARN**: Yellow text - warnings and non-critical issues
- **ERR**: Red text - errors and critical problems
- **DEBUG**: Cyan text - debug information and detailed tracing
- **Timestamps**: Light gray text for easy distinction

## Use Cases

### System Diagnostics

```cpp
LoggerScreen* diagLogger = new LoggerScreen("Diagnostics");

void runDiagnostics() {
    diagLogger->info("Starting system diagnostics...");

    if (testMemory()) {
        diagLogger->info("Memory test: PASSED");
    } else {
        diagLogger->err("Memory test: FAILED");
    }

    if (testSensors()) {
        diagLogger->info("Sensor test: PASSED");
    } else {
        diagLogger->warn("Sensor test: Some sensors offline");
    }

    diagLogger->info("Diagnostics complete");
}
```

### Real-Time Monitoring

```cpp
LoggerScreen* monitor = new LoggerScreen("System Monitor");

void loop() {
    int memFree = getFreeMemory();
    int tempC = readTemperature();

    if (memFree < 512) {
        monitor->warn("Low memory: %d bytes free", memFree);
    }

    if (tempC > 75) {
        monitor->err("High temperature: %dC", tempC);
    } else {
        monitor->info("Status OK - Temp: %dC, Mem: %d", tempC, memFree);
    }

    delay(5000);
}
```

### Debug Output

```cpp
LoggerScreen* debugLog = new LoggerScreen("Debug Output");

void debugFunction() {
    debugLog->info("Entering function: %s", __FUNCTION__);

    for (int i = 0; i < sensorCount; i++) {
        int value = readSensor(i);
        if (value < 0) {
            debugLog->err("Sensor %d read error: %d", i, value);
        } else {
            debugLog->info("Sensor %d: %d", i, value);
        }
    }

    debugLog->info("Function complete");
}
```

### Development vs Production Logging

```cpp
CompositeLogger logger;
SerialLogger serialLogger;

void setupLogging() {
    // Always log to serial for development
    logger.addLogger(&serialLogger);

    #ifdef DEBUG_BUILD
    // In debug builds, also show on screen
    LoggerScreen* screenLogger = new LoggerScreen("Debug Log");
    logger.addLogger(screenLogger->asLogger());  // Use asLogger()
    M1Shield.setScreen(screenLogger);
    #endif

    logger.info("Logging system configured");
}
```

## Performance Considerations

- **Memory Usage**: Uses string buffers for formatting (256 bytes for log lines)
- **Display Updates**: Each log message triggers a screen update
- **Color Changes**: Color coding requires additional text color changes
- **Scrolling**: Automatic screen clearing when full provides consistent performance

## Integration with M1Shield

The LoggerScreen works seamlessly with the M1Shield framework:

- **Navigation**: Standard M1Shield navigation (Menu button to exit)
- **Display Providers**: Compatible with all DisplayProvider types
- **Screen Management**: Full lifecycle management by M1Shield
- **Input Handling**: Inherits standard screen input handling

## Thread Safety

LoggerScreen is not inherently thread-safe. In multi-threaded environments, external synchronization is required when multiple threads log to the same LoggerScreen instance.
