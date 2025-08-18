# Screen Examples

This folder contains examples for the **Screen** class - the foundation of the M1Shield UI framework.

## Available Examples

### SimpleScreen/

**Perfect for getting started!** A basic Screen example showing:

- Button handling (Menu, Up, Down, Left, Right, Joystick)
- LED color control through button presses
- Button press counting and display
- Text display and screen updates
- Basic Screen patterns for beginner projects

**Files:**

- `SimpleScreen.ino` - Main Arduino sketch
- `SimpleScreen.h/.cpp` - Basic screen implementation

### ComprehensiveScreen/

**Complete Screen demonstration** showing ALL capabilities:

- **Input handling demo** - All button combinations and joystick directions
- **Lifecycle management** - Screen entry/exit, initialization, cleanup
- **Animation system** - Progress indicators, transitions, dynamic content
- Advanced patterns for professional applications

**Files:**

- `ComprehensiveScreen.ino` - Main Arduino sketch
- `ComprehensiveScreen.h/.cpp` - Main demo controller
- `InputDemoScreen.h/.cpp` - Complete input demonstration
- `LifecycleDemoScreen.h/.cpp` - Screen lifecycle management
- `AnimationDemoScreen.h/.cpp` - Animation and dynamic content

## Learning Path

1. **Start with SimpleScreen** - Learn basic concepts
2. **Move to ComprehensiveScreen** - See all advanced features
3. **Study ContentScreen** - Add headers, footers, and user feedback
4. **Explore MenuScreen** - Build navigation menus

## Screen Features

The Screen class is the foundation providing:

- **Input handling** - All button types and joystick directions via ActionTaken enum
- **Display management** - Manual control of all screen rendering and updates
- **Lifecycle control** - open(), close(), and active state management
- **Animation support** - Frame-based loop() method for real-time updates
- **Size adaptation** - isSmallDisplay() for responsive layouts

## Class Hierarchy

```
Screen (base class) <- You are here
+-- ContentScreen (adds header/footer/progress)
    +-- MenuScreen (adds menu navigation)
    +-- ConsoleScreen (adds terminal-like interface)
```

Screen is the foundation that all other UI classes build upon, providing complete manual control over display and input handling.
