# Comprehensive Screen Example

This comprehensive example demonstrates **ALL Screen features!** Perfect for learning everything the Screen base class can do.

## What You'll Learn

- How to create custom screens from the base Screen class
- Complete ActionTaken input handling capabilities
- Screen lifecycle management (open, close, active states)
- Display size adaptation (small vs large displays)
- Screen navigation and transitions between multiple screens
- Advanced input handling (joystick + buttons + combinations)
- Performance optimization and real-time updates
- Professional screen architecture patterns

## What is Screen?

Screen is the base class for all UI elements in the M1Shield library:

- Provides fundamental display and input management with full manual control
- Handles screen lifecycle (active/inactive states) via open()/close()
- Processes input through ActionTaken enum with actionTaken() method
- Supports buttons, joystick directions, and analog offsets
- Automatic display size detection via isSmallDisplay()
- Foundation for ContentScreen, MenuScreen, and ConsoleScreen

## Screen Hierarchy

```
Screen <- Base class - you control everything manually
+-- ContentScreen <- Adds header/footer/progress bar automatically
    +-- MenuScreen <- Adds menu navigation on top of ContentScreen
    +-- ConsoleScreen <- Adds terminal interface on top of ContentScreen
```

## What This Example Demonstrates

### 1. Complete Input Handling (InputDemoScreen)

- All button types: `BUTTON_MENU`, `BUTTON_LEFT`, `BUTTON_RIGHT`, `BUTTON_UP`, `BUTTON_DOWN`, `BUTTON_JOYSTICK`
- All joystick directions including diagonals: `JOYSTICK_UP_LEFT`, `JOYSTICK_DOWN_RIGHT`, etc.
- Input combination detection using bit flags: `if (action & BUTTON_UP)`
- Analog joystick offsets: `offsetX` and `offsetY` parameters
- Real-time input state visualization

### 2. Animation & Performance (AnimationDemoScreen)

- Frame-based animations using `loop()` method
- Real-time performance monitoring and FPS tracking
- Dynamic content updates and progress indicators
- Efficient rendering techniques

## Demo Screens Architecture

### ComprehensiveScreen (Main Controller)

Acts as a master controller that cycles between two specialized demo screens:

```cpp
class ComprehensiveScreen : public Screen {
private:
    InputDemoScreen *inputDemo;
    AnimationDemoScreen *animationDemo;
    int currentDemo;
};
```

### InputDemoScreen

Focused demonstration of input handling:

```cpp
class InputDemoScreen : public Screen {
private:
    struct InputState {
        bool menuPressed, leftPressed, rightPressed;
        int joystickX, joystickY;
        String lastAction;
    } inputState;
};
```

### AnimationDemoScreen

Advanced animation and dynamic content:

```cpp
class AnimationDemoScreen : public Screen {
private:
    unsigned long animationStartTime;
    float animationProgress;
public:
    void loop() override;    // Real-time updates
};
```

## How to Use

1. Choose your display type in `ComprehensiveScreen.ino`
2. Upload to your Arduino Mega 2560
3. Use MENU button to cycle through the two demo screens:
   - **Input Demo**: Press all buttons and move joystick to see real-time input detection
   - **Animation Demo**: See smooth animations and performance monitoring
4. Study the source code to understand implementation patterns

## Controls

- **MENU Button**: Navigate between demo screens (Input → Animation → repeat)
- **ALL OTHER INPUTS**: Depend on current demo screen - each demonstrates different input handling

## Professional Patterns Demonstrated

### Memory Management

```cpp
// Proper dynamic allocation and cleanup
ComprehensiveScreen::ComprehensiveScreen() {
    inputDemo = new InputDemoScreen();
    animationDemo = new AnimationDemoScreen();
}

ComprehensiveScreen::~ComprehensiveScreen() {
    delete inputDemo;
    delete animationDemo;
}
```

### Input Processing with ActionTaken Enum

```cpp
Screen* actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY) override {
    // Handle specific buttons using bit flags
    if (action & BUTTON_MENU) {
        switchToNextDemo();
        return nullptr;  // Stay on current screen
    }

    // Handle joystick movements
    if (action & JOYSTICK_UP) {
        // Up joystick movement detected
    }

    // Handle multiple simultaneous inputs
    if (action & (BUTTON_UP | JOYSTICK_UP)) {
        // Either up button OR joystick up
    }

    // Use analog offsets for precise positioning
    if (action & JOYSTICK_ANY) {
        handleJoystickMovement(offsetX, offsetY);
    }

    // Delegate to current demo screen for specialized handling
    return getCurrentDemoScreen()->actionTaken(action, offsetX, offsetY);
}
```

### Lifecycle Management

```cpp
bool open() override {
    if (!Screen::open()) return false;  // Call parent first

    activationTime = millis();
    screenActivations++;

    // Initialize current demo screen
    getCurrentDemoScreen()->open();

    return true;
}

void close() override {
    unsigned long sessionDuration = millis() - activationTime;

    // Cleanup current demo screen
    getCurrentDemoScreen()->close();

    Screen::close();  // Call parent last
}
```

### Display Adaptation

```cpp
void _drawScreen() override {
    if (isSmallDisplay()) {
        // Compact layout for OLED displays (128x64)
        display.setTextSize(1);
        drawCompactHeader();
        drawCompactContent();
    } else {
        // Full layout for TFT displays (320x240+)
        display.setTextSize(2);
        drawFullHeader();
        drawDetailedContent();
    }
}
```

## Next Steps

After understanding this comprehensive example:

1. Study how **ContentScreen** builds on these Screen foundations
2. Learn how **MenuScreen** adds navigation to ContentScreen
3. Explore **ConsoleScreen** for terminal-like interfaces
4. Apply these patterns to build your own custom screens
