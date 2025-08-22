# ButtonScreen Examples

These examples demonstrate the ButtonScreen class, which provides horizontal button navigation in the footer area.

## ButtonScreen Overview

The ButtonScreen class inherits from ContentScreen and provides horizontal navigation through button items. Unlike MenuScreen's vertical layout, ButtonScreen displays items horizontally in an expanded footer area.

## Key Features

- **Horizontal Navigation**: Left/right movement through button items
- **Footer Display**: Uses expanded footer (2 rows) for button layout
- **Button Item Reuse**: Efficiently reuses ContentScreen's button system
- **Scrolling Viewport**: Shows 4 items at once with intelligent scrolling
- **Config Values**: Optional second row for dynamic information

## Examples

### SimpleButtonScreen

Basic horizontal button navigation demonstrating fundamental ButtonScreen usage.

**What You'll Learn:**

- Creating a basic ButtonScreen
- Adding button items
- Handling selection events
- Basic navigation patterns

### ComprehensiveButtonScreen

Advanced ButtonScreen features with dynamic config values and state management.

**What You'll Learn:**

- Advanced configuration
- Dynamic config updates
- State-dependent button enabling
- Complex navigation patterns
- FlashString button items

## Hardware Requirements

- Arduino Mega 2560 (required for sufficient GPIO pins)
- M1Shield with compatible display
- TRS-80 Model I (optional for UI-only examples)

## Getting Started

1. Start with SimpleButtonScreen to understand basic concepts
2. Progress to ComprehensiveButtonScreen for advanced features
3. Use these examples as templates for your own horizontal navigation interfaces

The ButtonScreen class is ideal when you need horizontal navigation or want to display button options in the footer area while keeping the main content area available for other purposes.

## What It Does

- Creates a horizontal menu with 6 items: Audio, Video, System, Debug, Network, Reset
- Shows 4 items at a time with automatic horizontal scrolling
- Displays configuration values in a second row below each button item
- Demonstrates enabled/disabled button items (Reset option is disabled)
- Shows selection results in a console screen
- Uses ContentScreen's button items system for efficient memory usage

## What You'll Learn

- How to create a horizontal button display using `ButtonScreen`
- Using ContentScreen's `setButtonItems()` for horizontal button content
- Implementing horizontal navigation (left/right scrolling)
- Handling button item selection and screen transitions
- Controlling enabled/disabled button items
- Working with the expanded footer layout (2 rows of text)
- Efficient reuse of existing button items system

## Hardware Requirements

- Arduino Mega 2560
- M1Shield with compatible display

## Navigation

- **Left/Right Arrows**: Navigate horizontally between button items
- **Up Arrow or Joystick**: Select the highlighted button item
- **Menu Button**: Exit the menu

## Key Features Demonstrated

### Horizontal Layout

- Button items displayed side-by-side in footer area
- Automatic text truncation to fit item width
- Visual separation between items

### Scrolling View Window

- Shows 4 items at once
- Automatically scrolls when moving beyond visible area
- Smooth one-by-one item shifting

### Configuration Values

- Second row shows status/config information for each item
- Different value types: quality settings, version numbers, status indicators

### State Management

- Disabled items shown in grayed-out colors
- Selected item highlighted with yellow background
- Proper memory management for menu strings

## Display Layout

```
┌─────────────────────────────┐
│         Title Area          │
├─────────────────────────────┤
│                             │
│       Content Area          │
│        (Empty)              │
│                             │
├─────────────────────────────┤
│ Audio │ Video │System│Debug │ ← Button Items (Row 1)
│ HIGH  │1024x768│v1.3.0│ OFF │ ← Config Values (Row 2)
├─────────────────────────────┤
│      Progress Bar           │
└─────────────────────────────┘
```

## Code Structure

### MainHorizontalMenu Class

- Inherits from `ButtonScreen`
- Sets up 6 button items with configuration values
- Implements selection handling and item state management

### SelectionDisplay Class

- Simple console screen showing selected option
- Demonstrates screen transitions from horizontal menu

## Customization

You can easily modify this example to:

- Change the number and names of button items
- Modify configuration values and their display
- Add different screen types for each menu option
- Customize colors and layout (in the ButtonScreen class)
- Add more complex navigation patterns

## Expected Output

When you run this example:

1. A horizontal menu appears with Audio, Video, System, Debug visible
2. Navigation with left/right moves between items, scrolling to show Network
3. The Reset option appears disabled (grayed out)
4. Selecting an item shows a result screen with the selection details
5. Menu button returns to the main menu

This example provides a foundation for building horizontal navigation interfaces suitable for devices with limited vertical screen space.
