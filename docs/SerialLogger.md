# SerialLogger Class

The `SerialLogger` class implements the `ILogger` interface and sends formatted log messages to the Serial output.

## Methods

- `void info(const char* fmt, ...)`: Prints an informational message with `[INFO]` prefix.
- `void warn(const char* fmt, ...)`: Prints a warning message with `[WARN]` prefix.
- `void err(const char* fmt, ...)`: Prints an error message with `[ERR ]` prefix.
- `void mute()`: Disables all logging.
- `void unmute()`: Enables logging again.

## Notes

When muted, all log methods return immediately and do not print.

Internally, `SerialLogger` uses `vsnprintf()` to format messages safely.

## Example

```cpp
SerialLogger logger;
logger.info("System initialized.");
logger.warn("Low memory: %d bytes", freeMemory);
logger.err("Failed to start device");

logger.mute();
logger.info("This will not be printed.");
logger.unmute();
logger.info("Logging resumed.");
```
