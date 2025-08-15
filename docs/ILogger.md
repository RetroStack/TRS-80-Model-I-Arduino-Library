# ILogger Interface

## Overview

The `ILogger` interface provides a standardized logging system for the TRS-80 Model 1 Arduino Library. It extends Arduino's `Print` class and offers multiple ways to log messages with different severity levels.

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Method Signatures](#method-signatures)
  - [Core Interface (Pure Virtual)](#core-interface-pure-virtual)
  - [String Convenience Methods](#string-convenience-methods)
  - [F() Macro Support (Flash Strings)](#f-macro-support-flash-strings)
  - [Print Interface](#print-interface)
- [Usage Examples](#usage-examples)
  - [Basic Logging](#basic-logging)
  - [Printf-style Formatting](#printf-style-formatting)
  - [String Objects](#string-objects)
  - [F() Macro (Flash Strings)](#f-macro-flash-strings)
  - [Mixed Usage](#mixed-usage)
- [Memory Efficiency](#memory-efficiency)
- [Implementation Notes](#implementation-notes)
  - [For Library Users](#for-library-users)
  - [For Logger Implementers](#for-logger-implementers)
- [Available Implementations](#available-implementations)
- [Built-in Logger Implementations](#built-in-logger-implementations)
  - [SerialLogger](#seriallogger)
  - [CompositeLogger](#compositelogger)
- [See Also](#see-also)

## Features

- **Multiple Log Levels**: `info()`, `warn()`, and `err()` methods for different message types
- **Printf-style Formatting**: Support for format strings with variable arguments
- **String Object Support**: Direct logging of Arduino `String` objects
- **F() Macro Support**: Memory-efficient logging of flash strings using the `F()` macro
- **Print Interface**: Inherits from `Print` for direct character/buffer output

## Method Signatures

### Core Interface (Pure Virtual)

```cpp
virtual void info(const char *fmt, ...) = 0;
virtual void warn(const char *fmt, ...) = 0;
virtual void err(const char *fmt, ...) = 0;
```

### String Convenience Methods

```cpp
void info(const String &msg);    // Log String object as info
void warn(const String &msg);    // Log String object as warning
void err(const String &msg);     // Log String object as error
```

### F() Macro Support (Flash Strings)

```cpp
void infoF(const __FlashStringHelper *fmt, ...);    // Log F() format string as info
void warnF(const __FlashStringHelper *fmt, ...);    // Log F() format string as warning
void errF(const __FlashStringHelper *fmt, ...);     // Log F() format string as error
```

### Print Interface

```cpp
virtual size_t write(uint8_t ch) = 0;
virtual size_t write(const uint8_t *buffer, size_t size) = 0;
```

## Usage Examples

### Basic Logging

```cpp
logger->info("System initialized");
logger->warn("Low memory warning");
logger->err("Connection failed");
```

### Printf-style Formatting

```cpp
int value = 42;
const char* status = "OK";
logger->info("Sensor reading: %d, Status: %s", value, status);
logger->warn("Temperature: %d°C (threshold: %d°C)", temp, threshold);
logger->err("Error code: 0x%02X", errorCode);
```

### String Objects

```cpp
String message = "Dynamic message: " + String(counter);
logger->info(message);

String errorMsg = "Failed to connect to " + serverName;
logger->err(errorMsg);
```

### F() Macro (Flash Strings)

```cpp
// Simple messages using FlashString
logger->infoF(F("This string is stored in flash memory"));
logger->warnF(F("Low battery warning"));
logger->errF(F("Critical system error"));

// Format strings with arguments (FlashString format in flash, values from RAM)
int temperature = 25;
int humidity = 60;
logger->infoF(F("Temperature: %d°C, Humidity: %d%%"), temperature, humidity);
logger->warnF(F("Battery level: %d%% remaining"), batteryLevel);
logger->errF(F("Sensor %d failed with error code: 0x%02X"), sensorId, errorCode);
```

### Mixed Usage

```cpp
int temperature = 25;
String sensor = "DHT22";

// All of these work seamlessly:
logger->info("Temperature sensor initialized");              // const char*
logger->infoF(F("Reading from sensor..."));                  // F() macro (simple)
logger->info(String("Sensor: ") + sensor);                   // String object
logger->info("Current temperature: %d°C", temperature);      // Printf formatting
logger->infoF(F("Sensor %s reading: %d°C"), sensor.c_str(), temperature); // F() with formatting
```

## Memory Efficiency

- **const char\* methods**: No memory overhead, strings in program memory
- **F() macro methods (`infoF`, `warnF`, `errF`)**: Format strings stored in flash memory, saves RAM significantly
- **String object methods**: Use heap memory, convenient but less efficient

**Best Practices for Memory Efficiency:**

1. Use `infoF(F("format"), ...)` for format strings - stores format in flash
2. Use `info("literal")` for simple const char\* literals
3. Use `info(stringObj)` sparingly for dynamic String objects
4. Avoid repeated String concatenation in logging calls

For memory-constrained applications, prefer `const char*` literals and `F()` macro methods over `String` objects.

## Implementation Notes

### For Library Users

Simply use any of the supported method signatures. The interface automatically handles the conversion and routing to the underlying logger implementation.

### For Logger Implementers

Implement only the three pure virtual methods:

```cpp
class MyLogger : public ILogger {
public:
    void info(const char *fmt, ...) override;
    void warn(const char *fmt, ...) override;
    void err(const char *fmt, ...) override;

    // Add using declarations to expose inherited overloads
    using ILogger::info;
    using ILogger::warn;
    using ILogger::err;

    size_t write(uint8_t ch) override;
    size_t write(const uint8_t *buffer, size_t size) override;
};
```

The `using` declarations are necessary to prevent method hiding when you declare the `const char*` versions in your derived class.

## Available Implementations

- **SerialLogger**: Logs to Arduino Serial interface
- **CompositeLogger**: Broadcasts to multiple loggers
- **LoggerScreen**: Displays logs on screen with visual formatting

## See Also

- [SerialLogger](SerialLogger.md) - Serial port logging implementation
- [CompositeLogger](CompositeLogger.md) - Multi-logger broadcasting
- [LoggerScreen](LoggerScreen.md) - On-screen log display

## Built-in Logger Implementations

The library provides several ready-to-use logger implementations:

### SerialLogger

Outputs log messages to the Arduino Serial interface with formatted prefixes.

```cpp
#include <SerialLogger.h>

SerialLogger logger;
logger.info("System initialized");  // Output: [INFO] System initialized
logger.warn("Low memory");          // Output: [WARN] Low memory
logger.err("Connection failed");    // Output: [ERR ] Connection failed
```

### CompositeLogger

Forwards log messages to multiple registered loggers simultaneously, enabling multi-destination logging.

```cpp
#include <CompositeLogger.h>
#include <SerialLogger.h>

SerialLogger serialLogger;
// FileLogger fileLogger;  // Hypothetical file logger

CompositeLogger multiLogger;
multiLogger.addLogger(&serialLogger);
// multiLogger.addLogger(&fileLogger);

// Now logs go to both Serial and file
multiLogger.info("Message sent to all registered loggers");
```

The CompositeLogger is particularly useful for:

- **Development vs Production**: Log to Serial during development, add file/network logging for production
- **Redundancy**: Ensure critical logs reach multiple destinations
- **Debugging**: Temporarily add debug loggers without changing existing code
- **Performance Monitoring**: Send logs to both local storage and remote monitoring systems

See [CompositeLogger.md](CompositeLogger.md) for detailed usage examples and API reference.
