# CompositeLogger

The `CompositeLogger` is a logger implementation that forwards log events to multiple registered `ILogger` instances. This allows you to send log output to multiple destinations simultaneously, such as Serial output, file storage, network endpoints, or any other logger implementation.

## Table of Contents

- [Features](#features)
- [Constructor](#constructor)
- [Logger Management Methods](#logger-management-methods)
  - [addLogger](#bool-addloggerilogger-logger)
  - [removeLogger](#bool-removeloggerilogger-logger)
  - [clearLoggers](#void-clearloggers)
  - [getLoggerCount](#uint8_t-getloggercount-const)
  - [hasLogger](#bool-hasloggerilogger-logger-const)
- [Logging Methods](#logging-methods)
  - [info](#void-infoconst-char-fmt-)
  - [warn](#void-warnconst-char-fmt-)
  - [err](#void-errconst-char-fmt-)
  - [write](#size_t-writeuint8_t-ch--size_t-writeconst-uint8_t-buffer-size_t-size)
- [Configuration](#configuration)
- [Use Cases](#use-cases)
- [Performance Considerations](#performance-considerations)
- [Thread Safety](#thread-safety)
- [Examples](#examples)

## Features

- **Multiple Logger Support**: Register up to 8 different logger instances
- **Automatic Forwarding**: All log calls are automatically forwarded to all registered loggers
- **Dynamic Management**: Add and remove loggers at runtime
- **Memory Efficient**: Uses a simple array-based storage system
- **Type Safety**: Prevents duplicate registrations and null pointer issues
- **Full ILogger Interface**: Supports all standard logging methods (info, warn, err) and Print functionality

## Constructor

```cpp
CompositeLogger()
```

Creates an empty composite logger with no registered loggers.

## Examples

### Basic Setup

```cpp
#include <CompositeLogger.h>
#include <SerialLogger.h>

// Create individual loggers
SerialLogger serialLogger;
// Assume we have other loggers like FileLogger, NetworkLogger, etc.

// Create composite logger
CompositeLogger multiLogger;

void setup() {
    Serial.begin(115200);

    // Register multiple loggers
    multiLogger.addLogger(&serialLogger);
    // multiLogger.addLogger(&fileLogger);
    // multiLogger.addLogger(&networkLogger);

    // Now all log calls go to all registered loggers
    multiLogger.info("System initialized with %d loggers", multiLogger.getLoggerCount());
}
```

### Managing Loggers

```cpp
// Add a logger
if (multiLogger.addLogger(&newLogger)) {
    Serial.println("Logger added successfully");
} else {
    Serial.println("Failed to add logger (max capacity or duplicate)");
}

// Remove a specific logger
if (multiLogger.removeLogger(&oldLogger)) {
    Serial.println("Logger removed successfully");
} else {
    Serial.println("Logger not found");
}

// Check if a logger is registered
if (multiLogger.hasLogger(&someLogger)) {
    Serial.println("Logger is registered");
}

// Get current logger count
uint8_t count = multiLogger.getLoggerCount();
Serial.printf("Currently have %d loggers registered", count);

// Clear all loggers
multiLogger.clearLoggers();
```

### Logging with CompositeLogger

```cpp
// All these calls are forwarded to all registered loggers
multiLogger.info("Application started");
multiLogger.warn("Memory usage: %d%%", memoryUsage);
multiLogger.err("Critical error in module %s", moduleName);

// Print interface also works
multiLogger.print("Raw output: ");
multiLogger.println(sensorValue);
```

## Logger Management Methods

### `bool addLogger(ILogger* logger)`

- **Parameters**: `logger` - Pointer to ILogger instance to register
- **Returns**: `true` if successfully added, `false` if maximum capacity reached or logger already registered
- **Note**: Prevents duplicate registrations and null pointer additions

### `bool removeLogger(ILogger* logger)`

- **Parameters**: `logger` - Pointer to ILogger instance to remove
- **Returns**: `true` if logger was found and removed, `false` if not found

### `void clearLoggers()`

Removes all registered loggers from the composite logger.

### `uint8_t getLoggerCount() const`

- **Returns**: Number of currently registered loggers (0-8)

### `bool hasLogger(ILogger* logger) const`

- **Parameters**: `logger` - Pointer to ILogger instance to check
- **Returns**: `true` if logger is currently registered, `false` otherwise

## Logging Methods

### `void info(const char *fmt, ...)`

### `void warn(const char *fmt, ...)`

### `void err(const char *fmt, ...)`

Standard logging methods with printf-style formatting. All calls are forwarded to all registered loggers.

**Note**: The CompositeLogger also supports String and F() macro overloads through the inherited ILogger interface.

### `size_t write(uint8_t ch)` / `size_t write(const uint8_t *buffer, size_t size)`

Print interface methods that forward output to all registered loggers. Returns the total number of bytes written across all loggers.

## Configuration

### Maximum Logger Capacity

The default maximum number of loggers is 8, defined by the `MAX_LOGGERS` constant. This can be modified in the header file if needed:

```cpp
static const uint8_t MAX_LOGGERS = 8; // Adjust as needed
```

## Use Cases

### Development and Production Logging

```cpp
CompositeLogger logger;
SerialLogger serialLogger;
FileLogger fileLogger;

void setup() {
    // Always log to serial for development
    logger.addLogger(&serialLogger);

    #ifdef PRODUCTION
    // In production, also log to file
    logger.addLogger(&fileLogger);
    #endif

    logger.info("Logging system initialized");
}
```

### Multi-Destination Logging

```cpp
CompositeLogger logger;
SerialLogger serialLogger;
SDCardLogger sdLogger;
WiFiLogger wifiLogger;

void setupLogging() {
    logger.addLogger(&serialLogger);    // Local debugging
    logger.addLogger(&sdLogger);        // Persistent storage
    logger.addLogger(&wifiLogger);      // Remote monitoring

    logger.info("Multi-destination logging active");
}
```

### Conditional Logging

```cpp
CompositeLogger logger;
SerialLogger serialLogger;
DebugLogger debugLogger;

void enableDebugMode(bool enable) {
    if (enable && !logger.hasLogger(&debugLogger)) {
        logger.addLogger(&debugLogger);
        logger.info("Debug logging enabled");
    } else if (!enable && logger.hasLogger(&debugLogger)) {
        logger.removeLogger(&debugLogger);
        logger.info("Debug logging disabled");
    }
}
```

## Performance Considerations

- **Formatting Overhead**: Each log message is formatted once and then forwarded to all loggers
- **Write Operations**: The `write()` methods sum bytes written across all loggers
- **Memory Usage**: Uses a fixed array of 8 pointers (32-64 bytes depending on platform)
- **Error Handling**: If one logger fails, others continue to receive log messages

## Thread Safety

The `CompositeLogger` is not inherently thread-safe. If used in a multi-threaded environment, external synchronization is required to prevent race conditions during logger registration/removal operations.
