# ILogger Interface

The `ILogger` interface defines how logging works in the library. You can implement this yourself if you want to send logs to Serial, a file, or another output.

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
#include "ILogger.h"

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

private:
    LiquidCrystal* _lcd;
};
```

Usage:

```cpp
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
lcd.begin(16, 2);

LCDLogger logger(&lcd);
Model1 model1(&logger);
```
