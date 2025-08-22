# ButtonScreen Class

The `ButtonScreen` class provides a horizontal button interface that displays button items in an expanded footer area instead of the traditional vertical layout. This class inherits from `ContentScreen` and implements the same interface patterns as `MenuScreen` but with horizontal navigation.

## Overview

- **Base Class**: `ContentScreen`
- **Navigation**: Horizontal (left/right) with scrolling view window
- **Display Area**: Expanded footer (2 rows of text)
- **Max Visible Items**: 4 items simultaneously
- **Scrolling**: One-by-one item shifting when moving beyond visible area
- **Button Items**: Reuses ContentScreen's button items system (since buttons aren't visible in footer mode)
- **Features**: Config values, enabled/disabled items, truncated text handling

## Key Features

### Horizontal Layout

- Button items are displayed horizontally in a 2-row expanded footer
- First row: Button item text (button-style labels)
- Second row: Configuration values (if provided)
- Maximum 4 items visible at once

### Intelligent Scrolling

- When moving beyond visible area, items shift one position
- Smooth horizontal scrolling maintains focus on selected item
- View window automatically adjusts to keep selection visible

### Intelligent Button Item Reuse

- Reuses ContentScreen's existing button items system instead of creating duplicate functionality
- Since footer displays the horizontal buttons, normal button items aren't visible anyway
- Efficient memory usage by avoiding duplicate storage systems

### Configuration Support

- Supports both string and FlashString configuration values
- Config values displayed below button items in second row
- Automatic text truncation for both button items and config values

## Constructor

```cpp
ButtonScreen();
```

Creates a horizontal button screen with:

- Default empty button items list
- Selection at index 0
- View window starting at index 0

## Public Methods

### Button Item Management (Inherited from ContentScreen)

```cpp
void setButtonItems(const char **buttonItems, uint8_t buttonItemCount);
void setButtonItemsF(const __FlashStringHelper **buttonItems, uint8_t buttonItemCount);
void clearButtonItems();
```

Set horizontal button items using ContentScreen's button items system. The ButtonScreen displays these horizontally in the expanded footer instead of as traditional footer buttons.

```cpp
void refreshButtons();
```

Refresh the horizontal menu display without full screen redraw.

### Navigation and Input

```cpp
Screen *actionTaken(ActionTaken action, int8_t offsetX, int8_t offsetY) override;
```

Handle user input:

- **LEFT/RIGHT**: Navigate between button items horizontally
- **UP/SELECT**: Activate selected button item (if enabled)
- **MENU**: Exit menu (return to previous screen)

### Lifecycle

```cpp
bool open() override;
```

Initialize the horizontal menu when screen opens, finding the first enabled item.

```cpp
void loop() override;
```

Main update loop that handles notification timeouts and other base functionality.

## Protected Methods (For Derived Classes)

### Abstract Methods (Must Implement)

```cpp
virtual Screen *_getSelectedMenuItemScreen(int index) = 0;
```

Return the screen to navigate to when button item is selected:

- `index >= 0`: Selected button item index
- `index == -1`: Exit/back navigation

### Optional Override Methods

```cpp
virtual const char *_getMenuItemConfigValue(uint8_t index);
virtual const __FlashStringHelper *_getMenuItemConfigValueF(uint8_t index);
virtual bool _isMenuItemEnabled(uint8_t index) const;
```

Provide configuration values and enable/disable functionality for button items (using same index as button items).

## Layout and Display

### Footer Dimensions

- **Normal Display**: 32 pixels height (2 rows × 16 pixels)
- **Small Display**: 20 pixels height (2 rows × 10 pixels)
- **Text Size**: Size 2 for normal, Size 1 for small displays

### Item Distribution

- Screen width divided evenly among max visible items (4)
- Items are centered within their allocated width
- Vertical borders separate items (normal display only)

### Text Handling

- Automatic truncation with "..." when text exceeds item width
- Both menu text and config values are truncated independently
- Centered alignment within each item's allocated space

## Color Scheme

- **Normal Items**: Black background, white text
- **Selected Items**: Yellow background, black text
- **Disabled Items**: Dark gray background, light gray text
- **Config Values**: Light gray text (unless selected)

## Usage Example

```cpp
class MyHorizontalButtons : public ButtonScreen {
public:
    MyHorizontalButtons() : ButtonScreen() {
        setTitleF(F("Horizontal Menu"));

        const char* items[] = {"Option 1", "Option 2", "Option 3", "Option 4", "Option 5"};
        setButtonItems(items, 5);
    }

protected:
    void _drawContent() override {
        auto& gfx = M1Shield.getGFX();

        uint16_t contentY = _getContentTop();
        gfx.setTextColor(M1Shield.convertColor(0xFFFF));
        gfx.setTextSize(1);

        gfx.setCursor(10, contentY + 10);
        gfx.print(F("Select an option below:"));
    }

    Screen* _getSelectedButtonItemScreen(int index) override {
        if (index == -1) {
            return nullptr; // Exit menu
        }

        // Handle specific button selections
        switch (index) {
            case 0: return new MyOption1Screen();
            case 1: return new MyOption2Screen();
            // ... etc
            default: return nullptr;
        }
    }

    const char* _getButtonItemConfigValue(uint8_t index) override {
        switch (index) {
            case 0: return "ON";
            case 1: return "OFF";
            case 2: return "AUTO";
            default: return nullptr;
        }
    }

    bool _isButtonItemEnabled(uint8_t index) const override {
        // Example: disable option 2
        return index != 1;
    }
};
```

## Navigation Behavior

### Horizontal Movement

1. **Right Arrow**: Move to next enabled item

   - If at rightmost visible item, scroll view right by one position
   - Wrap to beginning when reaching end of list

2. **Left Arrow**: Move to previous enabled item
   - If at leftmost visible item, scroll view left by one position
   - Wrap to end when reaching beginning of list

### View Window Management

The view window automatically adjusts to ensure the selected item remains visible:

- Moving past right edge shifts all items left
- Moving past left edge shifts all items right
- Window bounds are maintained within total item count

## Memory Management

- Button items are managed by ContentScreen's existing system
- No additional dynamic allocation for menu functionality
- Automatic cleanup through ContentScreen's destructor
- Efficient reuse of existing infrastructure

## Integration with ContentScreen

ButtonScreen efficiently reuses ContentScreen's button items system:

- Button items normally displayed in footer are repurposed for horizontal menu
- Since horizontal menu replaces footer entirely, no conflict occurs
- Leverages existing string management and memory allocation
- Maintains compatibility with all ContentScreen features (notifications, progress bar, etc.)
- Uses notification system for temporary messages
- Respects header/title functionality
- Maintains progress bar functionality
- Follows standard screen lifecycle (open/close/loop)

The horizontal menu replaces the footer area entirely, providing a compact but feature-rich navigation interface suitable for devices with limited vertical screen space.
