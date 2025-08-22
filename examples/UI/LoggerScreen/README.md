# LoggerScreen with Rotational Buffer Example

An example demonstrating LoggerScreen's visual logging capabilities including the rotational buffer feature for preserving log history.

## What This Example Does

1. **Creates Visual Logger**: Sets up LoggerScreen with rotational buffer configuration
2. **Pre-fills Buffer**: Demonstrates logging before screen activation (historical logs)
3. **Buffer Replay**: Shows how buffered entries are replayed when screen opens
4. **Continuous Logging**: Real-time logging continues after buffer replay
5. **Multiple Log Levels**: Demonstrates info, warn, error, and debug messages
6. **Color Coding**: Different colors for different log levels with timestamps

## What You'll Learn

- How to create and configure a LoggerScreen with rotational buffer
- Using different log levels (info, warn, error, debug)
- Setting up rotational buffer to preserve log history
- Pre-filling buffer with historical data
- Buffer replay behavior when opening the logger
- Integrating persistent logging into your projects

## Expected Behavior

The example follows this sequence:

1. **System starts** and creates logger with 15-entry buffer
2. **Pre-fills buffer** with 10 historical log entries (while screen inactive)
3. **Activates screen** - all buffered entries are replayed in chronological order
4. **Continues logging** new real-time messages every 4 seconds
5. **Preserves history** - closing and reopening shows recent log history

The screen displays a scrolling log with:

- **White text** for INFO messages
- **Yellow text** for WARN messages
- **Red text** for ERROR messages
- **Cyan text** for DEBUG messages
- **Timestamps** showing relative time
- **Automatic scrolling** when screen fills
- **Historical context** from buffer replay

## Controls

- **Menu Button**: Close logger screen (press again to see buffer replay)
- **Other Buttons**: No action (focus on logging display)

## Key Features Demonstrated

### Rotational Buffer

```cpp
// Configure buffer for 15 entries
logger->setLogBufferSize(15);

// Check buffer status
uint16_t count = logger->getLogBufferCount();
uint16_t size = logger->getLogBufferSize();
```

### Pre-filling Buffer

```cpp
// Log entries before screen activation
logger->info("System initialization started");
logger->warn("Low memory detected");
logger->err("Failed to connect to WiFi");
// ... more entries

// Activate screen - buffer replays automatically
M1Shield.setScreen(logger);
```

### Buffer Management

```cpp
// Clear buffer if needed
logger->clearLogBuffer();

// Disable buffer (set to 0)
logger->setLogBufferSize(0);
```

## Hardware Requirements

- **M1Shield** with display
- **Arduino Mega 2560**
- Any **supported display** (ST7789, ILI9341, etc.)

## Display Support

This example supports multiple display types. Uncomment the appropriate lines in the code:

### Color TFT Displays

- **ST7789 320x240** (most common, landscape) - Default
- **ST7789 240x240** (square displays)
- **ST7789 320x170** (wide displays)
- **ST7735** (smaller 128x160 displays)
- **ILI9341** (240x320 displays)

### How to Change Display

1. Comment out the current display include and provider
2. Uncomment your display type
3. Upload the updated code

## Code Structure

```cpp
// Display setup
#include <Display_ST7789_320x240.h>
Display_ST7789_320x240 displayProvider;

// Core libraries
#include <M1Shield.h>
#include <LoggerScreen.h>

// Simple variables
LoggerScreen *logger;
int messageCount = 0;
unsigned long lastLogTime = 0;
```

## Example Output

```
Basic Logger
================
[INFO] Basic Logger starting...
[INFO] LoggerScreen initialized
[INFO] Ready for basic logging demo
[INFO] Message #1: Basic info log
[INFO] Message #2: Basic info log
[INFO] Message #3: Basic info log
[WARN] Warning #1: This is a warning message
[INFO] Message #4: Basic info log
[INFO] Message #5: Basic info log
[ERR ] Error #1: This is an error message
[INFO] Message #6: Basic info log
...
```

## Logging Pattern

- **Every 3 seconds**: Basic info message
- **Every 9 seconds** (3rd message): Warning message
- **Every 15 seconds** (5th message): Error message
- **After 20 messages**: Counter resets and cycle repeats

## Key Functions Used

### LoggerScreen Methods

- `logger->info(format, ...)` - Log info message in white
- `logger->warn(format, ...)` - Log warning message in yellow
- `logger->err(format, ...)` - Log error message in red

### Configuration Methods

- `setColorCodingEnabled(true)` - Enable color coding
- `setTextSize(1)` - Set text size for more lines

### M1Shield Integration

- `M1Shield.begin(displayProvider)` - Initialize hardware
- `M1Shield.setScreen(logger)` - Set logger as active screen
- `M1Shield.loop()` - Handle input and display updates

## Customization Ideas

1. **Change Timing**: Modify the 3000ms delay for different log frequency
2. **Add More Log Types**: Create different message patterns
3. **Custom Messages**: Replace the basic messages with your application data
4. **Sensor Integration**: Log real sensor readings instead of demo messages
5. **File Logging**: Combine with SerialLogger for dual output

## Next Steps

After understanding this basic example:

1. Explore the **CompositeLogger** example for multiple logging destinations
2. Learn about **SerialLogger** for file-based logging
3. Integrate logging into your own M1Shield projects
4. Experiment with different display configurations
