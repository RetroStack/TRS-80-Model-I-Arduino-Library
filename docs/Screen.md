# Screen Class

The `Screen` class provides the foundational interface for all user interface screens in the TRS-80 Model I library. It implements a standard lifecycle pattern with activation states, input handling, and rendering management for building complex multi-screen applications.

## Table of Contents

- [Constructor](#constructor)
- [Lifecycle Management](#lifecycle-management)
- [Input Handling](#input-handling)
- [Action Enumeration](#action-enumeration)
- [Screen Navigation](#screen-navigation)
- [Abstract Methods](#abstract-methods)
- [State Management](#state-management)
- [Implementation Pattern](#implementation-pattern)
- [Memory Management](#memory-management)
- [Notes](#notes)
- [Example](#example)

## Constructor

```cpp
Screen()
```

Creates a new `Screen` instance in inactive state. The screen must be activated using `open()` before it can process input or render content.

> **Note**: This is an abstract class - cannot be instantiated directly

## Lifecycle Management

### Screen States

Screens follow a defined lifecycle:

1. **Inactive**: Screen created but not active
2. **Active**: Screen opened and processing input/updates
3. **Closed**: Screen deactivated and cleaned up

### Lifecycle Methods

```cpp
virtual bool open()          // Activate screen and perform setup
virtual void close()         // Deactivate screen and cleanup
bool isActive()              // Check if screen is currently active
void refresh()               // Force complete screen redraw
```

**Lifecycle Example:**

```cpp
Screen* myScreen = new MyScreen();

// Screen starts inactive
assert(!myScreen->isActive());

// Activate the screen
if (myScreen->open()) {
    // Screen is now active and ready
    assert(myScreen->isActive());

    // Process the screen
    myScreen->loop();

    // Deactivate when done
    myScreen->close();
    assert(!myScreen->isActive());
}
```

## Input Handling

### Input Processing

```cpp
virtual Screen* actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY) = 0
```

**Pure virtual method** that handles all user input and determines navigation:

- **action**: Bit flags indicating which inputs are active
- **offsetX**: Horizontal offset for touch/pointer input (0-based)
- **offsetY**: Vertical offset for touch/pointer input (0-based)
- **Returns**: Screen pointer for navigation (nullptr = stay on current screen)

### Input Detection

Use bitwise operations to check for input combinations:

```cpp
Screen* MyScreen::actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY) {
    // Check for specific button
    if (action & BUTTON_MENU) {
        return &mainMenu;  // Navigate to main menu
    }

    // Check for any directional input
    if (action & (UP_ANY | DOWN_ANY | LEFT_ANY | RIGHT_ANY)) {
        handleMovement(action);
        return nullptr;  // Stay on current screen
    }

    // Check for multiple simultaneous inputs
    if (action & (BUTTON_MENU | BUTTON_UP)) {
        switchMode();
        return nullptr;
    }

    return nullptr;  // No action taken
}
```

## Action Enumeration

### Individual Actions

**Joystick Directions (1-128):**

- `JOYSTICK_LEFT` - Joystick moved left
- `JOYSTICK_RIGHT` - Joystick moved right
- `JOYSTICK_UP` - Joystick moved up
- `JOYSTICK_DOWN` - Joystick moved down
- `JOYSTICK_UP_LEFT` - Diagonal up-left
- `JOYSTICK_UP_RIGHT` - Diagonal up-right
- `JOYSTICK_DOWN_LEFT` - Diagonal down-left
- `JOYSTICK_DOWN_RIGHT` - Diagonal down-right

**Button Presses (256-8192):**

- `BUTTON_MENU` - Menu button pressed
- `BUTTON_LEFT` - Left directional button
- `BUTTON_RIGHT` - Right directional button
- `BUTTON_UP` - Up directional button
- `BUTTON_DOWN` - Down directional button
- `BUTTON_JOYSTICK` - Joystick center button

### Convenience Masks

**Input Groups:**

- `JOYSTICK_ANY` - Any joystick movement
- `BUTTON_ANY` - Any button press
- `BUTTON_DIRECTIONAL` - Any directional button (excludes menu/joystick)

**Directional Combinations:**

- `LEFT_ANY` - Any leftward input (joystick + button)
- `RIGHT_ANY` - Any rightward input (joystick + button)
- `UP_ANY` - Any upward input (joystick + button)
- `DOWN_ANY` - Any downward input (joystick + button)

### Input Examples

```cpp
// Check for any left movement
if (action & LEFT_ANY) {
    moveLeft();
}

// Check for specific button combination
if (action & (BUTTON_MENU | BUTTON_UP)) {
    openSettings();
}

// Check for any input at all
if (action != NONE) {
    lastInputTime = millis();
}

// Check for joystick vs button input
if (action & JOYSTICK_ANY) {
    handleAnalogInput();
} else if (action & BUTTON_ANY) {
    handleDigitalInput();
}
```

## Screen Navigation

### Navigation Return Values

The `actionTaken()` method return value controls screen transitions:

- **`nullptr`**: Stay on current screen
- **`Screen*`**: Navigate to specified screen
- **`this`**: Explicitly stay on current screen (same as nullptr)

### Navigation Patterns

```cpp
// Return to previous screen
if (action & BUTTON_MENU) {
    return previousScreen;
}

// Navigate to new screen
if (action & BUTTON_RIGHT) {
    return new SettingsScreen();
}

// Conditional navigation
if (action & BUTTON_LEFT) {
    if (hasUnsavedChanges()) {
        return new SaveConfirmScreen();
    } else {
        return previousScreen;
    }
}

// Stay on current screen
return nullptr;
```

## Abstract Methods

### Required Implementations

Derived classes must implement these pure virtual methods:

```cpp
virtual void _drawScreen() = 0      // Initial screen rendering
virtual void loop() = 0             // Main update loop
virtual Screen* actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY) = 0  // Input handling
```

### Method Responsibilities

- **`_drawScreen()`**: Handle initial setup and full screen rendering
- **`loop()`**: Process ongoing updates, animations, and time-based logic  
- **`actionTaken()`**: Process input and return navigation result

### Display Updates for OLED Compatibility

**IMPORTANT**: When implementing custom drawing operations in your screen classes, you must call `M1Shield.display()` after drawing operations to ensure OLED displays (SSD1306, SH1106) update properly.

The base `Screen` class automatically calls `M1Shield.display()` in:
- `open()`: After calling `_drawScreen()`
- `refresh()`: After calling `_drawScreen()`

**Your screen implementations should call `M1Shield.display()` after:**
- Drawing operations in `_drawScreen()`
- Custom drawing in `loop()` 
- Drawing operations in response to input in `actionTaken()`

```cpp
// Example: Custom screen with proper display updates
class CustomScreen : public Screen {
protected:
    void _drawScreen() override {
        Adafruit_GFX& gfx = M1Shield.getGFX();
        
        // Perform drawing operations
        gfx.fillScreen(0x0000);
        gfx.setTextColor(0xFFFF);
        gfx.setCursor(10, 10);
        gfx.print("Custom Screen");
        
        // Not needed here - base Screen::open() calls M1Shield.display()
        // But required if you call _drawScreen() manually
    }
    
public:
    void loop() override {
        // Check for updates that require redrawing
        if (dataChanged) {
            Adafruit_GFX& gfx = M1Shield.getGFX();
            gfx.fillRect(10, 50, 200, 20, 0x0000);
            gfx.setCursor(10, 50);
            gfx.print("Updated data");
            
            // REQUIRED: Push changes to OLED displays
            M1Shield.display();
            
            dataChanged = false;
        }
    }
    
    Screen* actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY) override {
        if (action & BUTTON_RIGHT) {
            // Update display in response to input
            Adafruit_GFX& gfx = M1Shield.getGFX();
            gfx.fillRect(10, 100, 100, 20, 0xFFE0);
            gfx.setTextColor(0x0000);
            gfx.setCursor(10, 100);
            gfx.print("Button pressed");
            
            // REQUIRED: Push changes to OLED displays
            M1Shield.display();
        }
        return nullptr;
    }
};
```

**Note**: TFT displays update immediately during drawing operations, so `M1Shield.display()` has no effect but is safe to call.

## State Management

### Activation State

```cpp
bool isActive()              // Check if screen is active
```

### Manual Control

```cpp
void refresh()               // Force complete redraw
```

**When to use refresh():**

- Display corruption detected
- External changes to display state
- Major screen state changes requiring full redraw
- Switching between screens with different layouts

## Implementation Pattern

### Basic Screen Implementation

```cpp
class GameScreen : public Screen {
private:
    int _score;
    int _lives;
    bool _gameOver;

protected:
    void _drawScreen() override {
        // Initial setup - called when screen opens
        M1Shield.clearScreen();
        drawGameBoard();
        drawUI();
    }

public:
    GameScreen() : _score(0), _lives(3), _gameOver(false) {
        // Constructor - screen is inactive
    }

    void loop() override {
        // Called every frame while active
        if (!_gameOver) {
            updateGame();
            updateAnimations();
            checkCollisions();
        }

        // Update display elements that change
        updateScore();
        updateLives();
    }

    Screen* actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY) override {
        if (_gameOver) {
            // Game over - any input returns to menu
            if (action != NONE) {
                return new MainMenuScreen();
            }
        } else {
            // Game active - handle player input
            if (action & LEFT_ANY) {
                movePlayerLeft();
            }
            if (action & RIGHT_ANY) {
                movePlayerRight();
            }
            if (action & BUTTON_JOYSTICK) {
                fireWeapon();
            }
            if (action & BUTTON_MENU) {
                return new PauseScreen();
            }
        }

        return nullptr;  // Stay on game screen
    }

private:
    void updateGame() {
        // Game logic updates
    }

    void drawGameBoard() {
        // Draw static game elements
    }

    void drawUI() {
        // Draw user interface elements
    }
};
```

### Screen with Multiple States

```cpp
class SettingsScreen : public Screen {
private:
    enum SettingsMode {
        DISPLAY_SETTINGS,
        AUDIO_SETTINGS,
        INPUT_SETTINGS
    } _currentMode;

    int _selectedOption;

protected:
    void _drawScreen() override {
        drawModeSelector();
        drawCurrentSettings();
    }

public:
    void loop() override {
        // Update blinking cursor or animations
        updateCursor();
    }

    Screen* actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY) override {
        if (action & BUTTON_MENU) {
            return previousScreen;  // Back to previous
        }

        if (action & LEFT_ANY) {
            switchToPreviousMode();
            refresh();  // Force redraw for mode change
        }

        if (action & RIGHT_ANY) {
            switchToNextMode();
            refresh();
        }

        if (action & UP_ANY) {
            _selectedOption--;
            if (_selectedOption < 0) _selectedOption = getMaxOptions() - 1;
        }

        if (action & DOWN_ANY) {
            _selectedOption++;
            if (_selectedOption >= getMaxOptions()) _selectedOption = 0;
        }

        return nullptr;
    }
};
```

## Memory Management

### Screen Lifecycle

- **Creation**: Screens typically created on demand
- **Management**: M1Shield automatically manages active screen
- **Cleanup**: Previous screens freed when navigating to new screens
- **Persistence**: Some screens may be kept in memory for quick access

### Copy Prevention

```cpp
Screen(const Screen&) = delete              // No copy constructor
Screen& operator=(const Screen&) = delete   // No copy assignment
```

Screens cannot be copied to prevent:

- Accidental state duplication
- Memory management issues
- Multiple active screen instances

## Notes

- **Single Active Screen**: Only one screen should be active at a time
- **Thread Safety**: Designed for single-threaded Arduino main loop usage
- **Memory Efficiency**: Screens freed automatically during navigation
- **Input Frequency**: `actionTaken()` called when input state changes
- **Loop Frequency**: `loop()` called every frame (typically 60Hz)
- **Rendering**: `_drawScreen()` called once when screen opens, `refresh()` for forced redraws

## Example

```cpp
#include <Screen.h>
#include <M1Shield.h>

class CounterScreen : public Screen {
private:
    int _counter;
    unsigned long _lastUpdate;

protected:
    void _drawScreen() override {
        // Initial screen setup
        Adafruit_GFX& gfx = M1Shield.getGFX();
        gfx.fillScreen(0x0000);
        gfx.setTextColor(0xFFFF);
        gfx.setTextSize(2);

        // Draw static elements
        gfx.setCursor(10, 10);
        gfx.print("Counter Demo");

        // Draw initial counter value
        drawCounter();
    }

public:
    CounterScreen() : _counter(0), _lastUpdate(0) {
        // Screen starts inactive
    }

    void loop() override {
        // Auto-increment every second
        if (millis() - _lastUpdate > 1000) {
            _counter++;
            drawCounter();
            _lastUpdate = millis();

            // Update LED color based on counter
            if (_counter % 2 == 0) {
                M1Shield.setLEDColor(COLOR_GREEN);
            } else {
                M1Shield.setLEDColor(COLOR_BLUE);
            }
        }
    }

    Screen* actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY) override {
        // Handle user input
        if (action & BUTTON_UP) {
            _counter++;
            drawCounter();
        }

        if (action & BUTTON_DOWN) {
            _counter--;
            drawCounter();
        }

        if (action & BUTTON_LEFT) {
            _counter = 0;
            drawCounter();
        }

        if (action & BUTTON_MENU) {
            // Exit to main menu
            return new MainMenuScreen();
        }

        // Stay on counter screen
        return nullptr;
    }

private:
    void drawCounter() {
        Adafruit_GFX& gfx = M1Shield.getGFX();

        // Clear counter area
        gfx.fillRect(10, 50, 200, 30, 0x0000);

        // Draw new counter value
        gfx.setCursor(10, 50);
        gfx.setTextColor(0xFFE0);
        gfx.setTextSize(3);
        gfx.print("Count: ");
        gfx.print(_counter);
    }
};

// Usage
void setup() {
    M1Shield.begin();

    CounterScreen* counter = new CounterScreen();
    M1Shield.setScreen(counter);
}

void loop() {
    M1Shield.processInput();
    M1Shield.updateScreen();
    M1Shield.renderScreen();
}
```
