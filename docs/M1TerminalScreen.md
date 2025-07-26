# M1TerminalScreen Class

The `M1TerminalScreen` class provides a complete TRS-80 Model I terminal emulation with authentic 64×16 character grid display. It features efficient incremental rendering, dual font support, hardware integration with Model1 systems, and scrollable viewport navigation for displaying the full 64-column terminal on smaller displays.

## Table of Contents

- [Constructor](#constructor)
- [Display Architecture](#display-architecture)
- [Model1 Integration](#model1-integration)
- [Navigation Controls](#navigation-controls)
- [Font Management](#font-management)
- [Performance Features](#performance-features)
- [Public Methods](#public-methods)
- [Buffer Management](#buffer-management)
- [Memory Layout](#memory-layout)
- [Notes](#notes)
- [Example](#example)

## Constructor

```cpp
M1TerminalScreen()
```

Creates a new M1TerminalScreen instance with:

- 64×16 character grid (1024 characters total)
- Dual buffer system for efficient change detection
- Default font set (Font A)
- Viewport positioned at top-left (0,0)
- Demo content loaded initially

## Display Architecture

### Character Grid Layout

```
Full 64×16 Grid:           Scrollable Viewport:
┌─────────────────────┐    ┌──────────────────────┐
│ 64 columns          │    │ Viewport (adaptive)  │
│ 16 rows             │ →  │ ← → H. Scrollable    │
│ 1024 total chars    │    │ ↑ ↓ V. Scrollable    │
│ TRS-80 Video RAM    │    │ Size: Content Area   │
└─────────────────────┘    └──────────────────────┘
```

### Viewport Behavior

The terminal automatically adapts the viewport size to the available content area:

- **Small displays**: Viewport shows subset of 64×16 grid with scrolling
- **Large displays**: Viewport may show the entire grid
- **Horizontal scrolling**: Arrow keys move viewport left/right in 5-character steps
- **Vertical scrolling**: Arrow keys move viewport up/down line by line
- **Dynamic bounds**: Scrolling limits automatically calculated based on content area

## Model1 Integration

### Automatic Hardware Detection

When connected to a TRS-80 Model I system:

```cpp
bool hasActiveTestSignal()  // Returns true if Model1 hardware is active
```

### Video Memory Synchronization

- **Live Mode**: Reads video memory from Model1 (0x3C00-0x3FFF) when test signal is active
- **Demo Mode**: Shows demonstration content when hardware is not available
- **Real-time Updates**: Automatically syncs with actual TRS-80 display state
- **Change Detection**: Only updates characters that have changed in video memory

### Hardware Integration Example

```cpp
void setup() {
    Model1.begin();
    Model1.activateTestSignal();  // Take control of TRS-80 bus

    M1TerminalScreen* terminal = new M1TerminalScreen();
    M1Shield.setScreen(terminal);

    // Terminal will now display live TRS-80 video memory
}
```

## Navigation Controls

### Horizontal Scrolling

- **Left Arrow**: Scroll viewport left (up to 5 characters)
- **Right Arrow**: Scroll viewport right (up to 5 characters)
- **Automatic Bounds**: Scrolling stops at grid edges
- **Smooth Movement**: Changes applied over multiple frames

### Vertical Scrolling

- **Up Arrow**: Scroll viewport up one line
- **Down Arrow**: Scroll viewport down one line
- **Line-by-Line**: Precise vertical navigation
- **Boundary Limits**: Scrolling constrained to grid dimensions

### Font Switching

- **Menu + Up**: Cycle through available font sets
- **Font A/B Support**: Switch between TRS-80 character sets
- **Live Updates**: Font changes applied immediately to all characters

### Navigation Example

```cpp
Screen* MyTerminal::actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY) {
    if (action & BUTTON_LEFT) {
        // Scrolls left automatically
    }
    if (action & BUTTON_RIGHT) {
        // Scrolls right automatically
    }
    if (action & BUTTON_UP) {
        // Scrolls up automatically
    }
    if (action & BUTTON_DOWN) {
        // Scrolls down automatically
    }
    if (action & (BUTTON_MENU | BUTTON_UP)) {
        // Switches to next font
    }
    return nullptr;  // Stay on terminal screen
}
```

## Font Management

### Font Selection

```cpp
void _setFont(uint8_t fontIndex)  // Switch between available fonts
uint8_t _getFont()                // Get current font index
```

### Supported Character Sets

- **Font A**: Standard TRS-80 character set with block graphics
- **Font B**: Alternative character set with different graphics
- **Graphics Support**: Full support for TRS-80 block and line graphics
- **ASCII Compatibility**: Standard ASCII characters render correctly

## Performance Features

### Incremental Rendering

- **Change Detection**: Only updates characters that have actually changed
- **Frame Distribution**: Updates distributed across multiple frames
- **Pixel-Level Optimization**: Only changed pixels within characters are redrawn
- **Adaptive Performance**: More changes processed per frame when needed

### Buffer Management

- **Dual Buffer System**:
  - `_bufferedVidMem[1024]`: Current character data
  - `_writtenVidMem[1024]`: Previously rendered characters
- **Memory Efficient**: Uses differential updates to minimize memory usage
- **Change Tracking**: Compares buffers to identify exactly what needs updating

### Performance Example

```cpp
void loop() {
    // Terminal automatically:
    // 1. Checks for Model1 video memory changes
    // 2. Updates only changed characters
    // 3. Distributes rendering across frames
    // 4. Maintains smooth 60fps performance
}
```

## Public Methods

### Content Loading

```cpp
void loadDemoContent()        // Load demonstration text pattern
void syncWithModel1()         // Synchronize with Model1 video memory
void clearDisplay()           // Clear all characters to spaces
```

### Display Control

```cpp
void forceRedraw()           // Force complete screen redraw
void setUpdateRate(uint8_t rate)  // Control rendering speed (characters per frame)
```

### Status Queries

```cpp
bool hasActiveTestSignal()   // Check if Model1 hardware is connected
bool isContentLoaded()       // Check if display has content
uint16_t getCharacterCount() // Get number of non-space characters
```

## Buffer Management

### Memory Layout

```
Video Memory Layout (1024 bytes):
Row 0:  [0-63]     - Characters 0-63 (top row)
Row 1:  [64-127]   - Characters 64-127
Row 2:  [128-191]  - Characters 128-191
...
Row 15: [960-1023] - Characters 960-1023 (bottom row)

Index Calculation: index = (y * 64) + x
```

### Buffer Operations

- **Read Access**: `uint8_t character = _bufferedVidMem[index]`
- **Write Access**: Automatically handled by Model1 synchronization
- **Change Detection**: Compares `_bufferedVidMem` with `_writtenVidMem`
- **Initialization**: Buffers initialized with spaces (0x20)

## Memory Layout

### Character Positioning

```cpp
// Convert coordinates to buffer index
uint16_t index = (y * 64) + x;

// Convert buffer index to coordinates
uint8_t x = index % 64;
uint8_t y = index / 64;
```

### Viewport Mapping

```cpp
// Terminal coordinates (0-63, 0-15) to screen coordinates
int16_t screenX = (terminalX - _horizontalScrollOffset) * CHAR_WIDTH;
int16_t screenY = (terminalY - _verticalScrollOffset) * CHAR_HEIGHT;
```

## Notes

- **Model1 Dependency**: Requires Model1 library for hardware integration
- **Memory Usage**: Uses 2KB for character buffers plus additional space for pixel data
- **Performance**: Optimized for real-time display updates with minimal CPU overhead
- **Display Compatibility**: Adapts to different M1Shield display types automatically
- **Thread Safety**: Designed for single-threaded Arduino main loop usage
- **Content Persistence**: Display content persists across font changes and navigation

## Example

```cpp
#include "M1TerminalScreen.h"
#include "Model1.h"
#include "M1Shield.h"

M1TerminalScreen* terminal;

void setup() {
    Serial.begin(115200);

    // Initialize hardware
    M1Shield.begin();
    Model1.begin();

    // Create terminal screen
    terminal = new M1TerminalScreen();

    // Check if Model1 hardware is available
    if (terminal->hasActiveTestSignal()) {
        Serial.println("Connected to TRS-80 Model I");
        Model1.activateTestSignal();
        terminal->syncWithModel1();
    } else {
        Serial.println("No TRS-80 hardware detected, using demo content");
        terminal->loadDemoContent();
    }

    // Activate terminal screen
    M1Shield.setScreen(terminal);

    Serial.println("Terminal ready!");
    Serial.println("Controls:");
    Serial.println("- Arrow keys: Navigate viewport");
    Serial.println("- Menu + Up: Switch fonts");
}

void loop() {
    // M1Shield automatically handles:
    // - Input processing and navigation
    // - Screen updates and rendering
    // - Model1 synchronization (if connected)
    M1Shield.processInput();
    M1Shield.updateScreen();
    M1Shield.renderScreen();

    // Optional: Monitor for new content
    static unsigned long lastCheck = 0;
    if (millis() - lastCheck > 1000) {
        if (terminal->hasActiveTestSignal()) {
            uint16_t charCount = terminal->getCharacterCount();
            Serial.print("Characters displayed: ");
            Serial.println(charCount);
        }
        lastCheck = millis();
    }
}
```
