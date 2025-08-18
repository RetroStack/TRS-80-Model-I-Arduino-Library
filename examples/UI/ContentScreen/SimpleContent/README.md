# Simple ContentScreen Example

A basic example demonstrating the fundamental features of ContentScreen in an easy-to-understand way.

## What This Example Does

This simple ContentScreen example demonstrates:

1. **Structured Layout**: Automatic header, footer, and content area management
2. **User Feedback**: Alert dialogs, confirmation dialogs, and notifications
3. **Progress Indication**: Dynamic progress bar updates
4. **Navigation**: Up/Down button navigation with visual feedback
5. **Content Drawing**: Custom content rendering in the content area
6. **Demo Mode**: Real-time content updates and animation

## What You'll Learn

- How to create a ContentScreen subclass
- Setting up header titles and footer hints
- Drawing custom content in the content area
- Using the user feedback systems (alert, confirm, notify)
- Handling button input and navigation
- Updating progress bars dynamically
- Managing screen state and real-time updates

## Expected Behavior

### Main Menu Mode

- **Header**: Shows "Simple Content" title with progress bar
- **Content Area**: List of 5 selectable options
- **Footer**: Shows "Up/Down: Navigate | Menu: Select"
- **Selection**: Current option highlighted with dark background
- **Progress**: Bar updates based on current selection (0-100%)

### Available Options

1. **Show Alert**: Displays an information alert dialog
2. **Show Confirm**: Shows a Yes/No confirmation dialog
3. **Show Notify**: Displays an auto-expiring notification
4. **Update Progress**: Manually updates the progress bar
5. **Toggle Demo**: Switches to/from real-time demo mode

### Demo Mode

- **Header**: Shows "Demo Mode" title
- **Content Area**: Real-time updating information
- **Footer**: Shows "Menu: Exit Demo"
- **Animation**: Moving yellow circle based on progress
- **Updates**: Time and progress values update every second

## Controls

- **Up Button**: Navigate to previous option
- **Down Button**: Navigate to next option
- **Menu Button**: Select current option / Exit demo mode
- **Other Buttons**: No action

## Hardware Requirements

- **M1Shield** with display
- **Arduino Mega 2560**
- Any **supported display** (ST7789, ILI9341, ST7735, etc.)
- **Working buttons** for navigation

## Code Structure

### Files

- `SimpleContent.ino` - Main Arduino sketch with setup/loop
- `SimpleContent.h` - Class definition and interface
- `SimpleContent.cpp` - Implementation of all functionality

### Key Methods

#### ContentScreen Overrides

```cpp
bool open()                           // Initialize screen
void _drawContent()                   // Draw content area
ActionTaken actionTaken(ActionTaken)  // Handle button input
void loop()                          // Real-time updates
```

#### Helper Methods

```cpp
void drawOptionsList()               // Draw main menu
void drawDemoContent()               // Draw demo mode content
void executeCurrentOption()         // Handle menu selection
void updateProgress()                // Update progress bar
```

## User Feedback Systems

### Alert Dialog

```cpp
alertF(F("This is an alert dialog!"), F("Information"));
```

- Blocks execution until dismissed
- Shows message with OK button
- Used for important information

### Confirmation Dialog

```cpp
ConfirmResult result = confirmF(F("Do you want to continue?"), F("Confirmation"));
if (result == CONFIRM_YES) {
    // User selected Yes
} else {
    // User selected No
}
```

- Blocks execution until choice made
- Returns YES or NO result
- Used for important decisions

### Notification

```cpp
notifyF(F("This notification will auto-expire"));
```

- Non-blocking, auto-expires
- Shows temporary status messages
- Used for feedback and status updates

## Layout Management

### Header Area

- Automatically positioned at top
- Contains title and progress bar
- Updates via `setTitleF()` and `setProgress()`

### Content Area

- Main drawing area between header and footer
- Access boundaries with `getContentAreaTop()` and `getContentAreaBottom()`
- Draw custom content in `_drawContent()`

### Footer Area

- Automatically positioned at bottom
- Shows button hints and navigation help
- Updates via `setFooterF()`

## Customization Ideas

1. **Change Options**: Modify the options array for different menu items
2. **Add Content**: Create new drawing functions for custom content
3. **Progress Logic**: Link progress to different values (time, completion, etc.)
4. **Animation**: Add more animated elements in demo mode
5. **Data Display**: Show sensor readings or system information
6. **Color Schemes**: Customize colors for different visual themes

## Next Steps

After understanding this example:

1. Modify the options and see how the menu changes
2. Add your own content drawing functions
3. Experiment with different progress update patterns
4. Explore the **ComprehensiveContent** example for advanced features
5. Build your own ContentScreen for your specific project

## Common Patterns

### Drawing in Content Area

```cpp
void _drawContent() {
    Adafruit_GFX &display = M1Shield.getGFX();
    int contentY = getContentAreaTop();

    display.setTextColor(M1Shield.convertColor(0xFFFF));
    display.setCursor(10, contentY + 10);
    display.print("Your content here");
}
```

### Handling Navigation

```cpp
ActionTaken actionTaken(ActionTaken action) {
    if (action & BUTTON_UP) {
        // Navigate up
        refresh(); // Update display
        return BUTTON_UP;
    }
    // Handle other buttons...
    return NONE;
}
```

### Real-time Updates

```cpp
void loop() {
    if (millis() - lastUpdate > 1000) {
        lastUpdate = millis();
        // Update content
        refresh(); // Redraw screen
    }
}
```
