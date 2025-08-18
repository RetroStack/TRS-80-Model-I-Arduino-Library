# Simple MenuScreen Example

The easiest way to get started with MenuScreen. Perfect if you're new to menus or just want a basic example!

**NEW: Demonstrates String Support** - This example shows how to use both static and dynamic string content in menus.

## What You'll Learn

- How to create a simple menu with items
- How to navigate menus (Up/Down/Select)
- How to handle menu selections
- How MenuScreen builds on ContentScreen
- **NEW: How to use different string types (C-strings, Arduino Strings, F-strings)**

## What It Does

- Shows a simple menu with 5 LED control options
- Highlights the current selection
- Changes LED color when you select items
- Shows notifications with different string types
- Demonstrates dynamic menu content

## String Features Demonstrated

This example shows three ways to work with text:

```cpp
// 1. Simple static text (C-strings)
setTitle("LED Control Menu");

// 2. Dynamic text (Arduino Strings)
String status = "Current: " + currentColorName;
notify(status);

// 3. Memory-efficient static text (F-strings)
notifyF(F("LED color changed!"));
```

## Controls

- **Up/Down**: Navigate menu items
- **Menu/Right**: Select current item
- **Left**: Back (if implemented)

## What You Need

- M1Shield with working display

## How to Use

1. Upload `SimpleMenu.ino` and watch the menu appear
2. Use Up/Down to navigate between menu items
3. Press Menu to select items
4. Watch the LED change colors based on your selection!

## Menu Items

The example includes these menu options:

- **Red LED**: Sets LED to red color
- **Green LED**: Sets LED to green color
- **Blue LED**: Sets LED to blue color
- **Yellow LED**: Sets LED to yellow color
- **Purple LED**: Sets LED to purple color

## Understanding MenuScreen

MenuScreen is built on top of ContentScreen and provides:

- **Automatic navigation**: Up/Down buttons handled automatically
- **Item highlighting**: Current selection is visually indicated
- **Pagination**: Long menus automatically scroll
- **Selection handling**: Easy override for item selection logic

## Code Structure

- **SimpleMenu.ino**: Main Arduino sketch with display setup
- **SimpleMenu.h/.cpp**: The menu implementation class

The example shows the basic pattern for creating menus:

1. Extend MenuScreen class
2. Override `getItems()` to define menu items
3. Override `onSelect()` to handle selections
4. Optionally override display and input methods

## Next Steps

After mastering this simple menu:

1. Try the **ComprehensiveMenu** example to see all MenuScreen features
2. Learn about configuration values, disabled items, and user feedback
3. Explore multi-level menu hierarchies and submenus
