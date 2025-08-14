# ILogger Interface

The `ILogger` interface defines how logging works in the library. You can implement this yourself if you want to send logs to Serial, a file, or another output.
The class inherits the `Print` behavior, and therefore all `Print::print` and `Print::println` methods are available.

## Methods

Any class implementing `ILogger` **must** provide:

- `void info(const char* fmt, ...)`: For informational messages.
- `void warn(const char* fmt, ...)`: For warnings.
- `void err(const char* fmt, ...)`: For errors.

Each method takes a printf-style format string and additional arguments.

## Example Implementation

Here is an example logger that prints messages to a character LCD display (using the LiquidCrystal library):

```cpp
#include <LiquidCrystal.h>
#include <ILogger.h>

class LCDLogger : public ILogger {
public:
    LCDLogger(LiquidCrystal* lcd) : _lcd(lcd) {}

    void info(const char* fmt, ...) override {
        _lcd->clear();
        _lcd->print("INFO: ");
        char buf[32];
        va_list args;
        va_start(args, fmt);
        vsnprintf(buf, sizeof(buf), fmt, args);
        va_end(args);
        _lcd->setCursor(0, 1);
        _lcd->print(buf);
    }

    void warn(const char* fmt, ...) override {
        _lcd->clear();
        _lcd->print("WARN: ");
        char buf[32];
        va_list args;
        va_start(args, fmt);
        vsnprintf(buf, sizeof(buf), fmt, args);
        va_end(args);
        _lcd->setCursor(0, 1);
        _lcd->print(buf);
    }

    void err(const char* fmt, ...) override {
        _lcd->clear();
        _lcd->print("ERROR: ");
        char buf[32];
        va_list args;
        va_start(args, fmt);
        vsnprintf(buf, sizeof(buf), fmt, args);
        va_end(args);
        _lcd->setCursor(0, 1);
        _lcd->print(buf);
    }

    using ILogger::info;
    using ILogger::warn;
    using ILogger::err;

private:
    LiquidCrystal* _lcd;
};
```

Usage:

```cpp
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
lcd.begin(16, 2);

LCDLogger logger(&lcd);
Model1.setLogger(logger);
```

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
