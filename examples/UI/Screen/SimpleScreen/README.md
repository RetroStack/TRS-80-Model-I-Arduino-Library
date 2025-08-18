# Simple Screen Example

The easiest possible example to get you started with screens. Perfect if you're new to Arduino or the M1Shield!

## What You'll Learn

- How to create a basic interactive screen from the Screen base class
- How to handle button presses with the ActionTaken enum
- How to change LED colors programmatically
- How to display dynamic text content on the screen
- How to implement the core Screen methods: `_drawScreen()`, `loop()`, and `actionTaken()`

## What It Does

- Shows a simple screen with instructions and status information
- Counts how many times you press any button
- Up/Down buttons (or joystick) cycle through 8 different LED colors
- Menu button resets the button press counter to zero
- Demonstrates real-time display updates via `refresh()`

## Controls

- **Up Button** (or Joystick Up): Next LED color (0-7, wraps around)
- **Down Button** (or Joystick Down): Previous LED color (7-0, wraps around)
- **Menu Button**: Reset button press counter to 0
- **Any button press**: Increases the total button press counter

## What You Need

- M1Shield with working display (any supported display type)
- Arduino Mega 2560

## How to Use

1. Choose your display type in `SimpleScreen.ino` (uncomment the correct include)
2. Upload `SimpleScreen.ino` to your Arduino
3. Watch the screen - you should see "Simple Screen" title and instructions
4. Press the Up button - LED should change color!
5. Press any button - the counter should increase
6. Press Menu - counter resets to 0

## Troubleshooting

- **Blank screen?** Make sure you've uncommented the correct display include in the .ino file
- **Wrong colors?** Some displays may have different color formats - this is normal
- **No response?** Check your wiring matches the M1Shield pin configuration

## Understanding the Code

This example demonstrates direct use of the Screen base class:

- **Manual control**: You implement all display logic yourself
- **Full flexibility**: Complete control over what appears where
- **Foundation concepts**: The patterns used here apply to all UI classes

This is different from higher-level classes:

- **ContentScreen**: Adds automatic header/footer layout
- **MenuScreen**: Adds navigation menu functionality
- **ConsoleScreen**: Adds terminal-like text interface

## Code Structure

```cpp
class SimpleScreen : public Screen {
private:
    int _buttonPressCount;  // Track user interactions
    int _ledColor;          // Current LED color (0-7)

protected:
    void _drawScreen() override;  // Initial/full screen rendering

public:
    void loop() override;         // Called continuously (LED updates)
    Screen* actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY) override;
};
```

## Next Steps

After mastering this simple example:

1. Try the **ComprehensiveScreen** example to see all Screen capabilities
2. Move to **ContentScreen** examples for structured layouts
3. Explore **MenuScreen** examples for navigation systems
4. Build your own custom screens using these patterns
