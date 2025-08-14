# ContentScreen Class

The `ContentScreen` class provides a structured layout template for screens with standardized header, content area, footer, and progress bar regions. It extends the basic `Screen` interface with automatic layout management, allowing developers to focus on content-specific functionality while maintaining consistent visual structure.

## Table of Contents

- [Constructor](#constructor)
- [Layout Structure](#layout-structure)
- [Protected Methods](#protected-methods)
  - [\_drawContent](#void-_drawcontent)
  - [Layout Getters](#layout-getters)
- [Public Methods](#public-methods)
  - [Title Management](#title-management)
    - [setTitle](#void-settitleconst-char-title)
    - [clearTitle](#void-cleartitle)
    - [getTitle](#const-char-gettitle-const)
  - [Progress Control](#progress-control)
    - [setProgressValue](#void-setprogressvalueint-value)
    - [getProgressValue](#uint8_t-getprogressvalue-const)
  - [Button Management](#button-management)
    - [setButtonItems](#void-setbuttonitemsconst-char-buttonitems-uint8_t-buttonitemcount)
    - [clearButtonItems](#void-clearbuttonitems)
  - [Content Area Utilities](#content-area-utilities)
    - [clearContentArea](#void-clearcontentarea)
    - [drawText](#void-drawtextuint16_t-x-uint16_t-y-const-char-text-uint16_t-color-uint8_t-size--1)
- [Layout Regions](#layout-regions)
- [Implementation Pattern](#implementation-pattern)
- [Examples](#examples)

## Constructor

```cpp
ContentScreen()
```

Creates a new `ContentScreen` instance with default layout settings:

- No title text
- No footer button labels
- Progress bar hidden (value 0)
- Content area maximized within available space

## Layout Structure

ContentScreen organizes the display into four distinct regions:

```
┌─────────────────────────────┐
│ Header (Title)              │
├─────────────────────────────┤
│                             │
│                             │
│     Content Area            │
│   (implemented by           │
│    derived classes)         │
│                             │
│                             │
├─────────────────────────────┤
│ Footer (Buttons/Status)     │
├─────────────────────────────┤
│ Progress Bar                │
└─────────────────────────────┘
```

## Protected Methods

### `void _drawContent()`

**Pure virtual method** - Derived classes must implement this to render their specific content within the designated content area bounds.

```cpp
class MyContentScreen : public ContentScreen {
protected:
    void _drawContent() override {
        uint16_t x = _getContentLeft();
        uint16_t y = _getContentTop();
        uint16_t w = _getContentWidth();
        uint16_t h = _getContentHeight();

        // Your content rendering here
        drawText(x, y, "Custom content");
    }
};
```

### Layout Getters

Access content area boundaries for positioning:

- `uint16_t _getContentLeft()`: Returns X coordinate of content area left edge
- `uint16_t _getContentTop()`: Returns Y coordinate of content area top edge
- `uint16_t _getContentWidth()`: Returns content area width in pixels
- `uint16_t _getContentHeight()`: Returns content area height in pixels

## Public Methods

### Title Management

Control the header area display:

#### `void setTitle(const char* title)`

Sets the title text displayed in the header region. The title is dynamically allocated and automatically freed when changed or when the screen is destroyed.

**Parameters:**

- `title`: Null-terminated string for title (pass nullptr or empty string to clear)

**Example:**

```cpp
setTitle("System Configuration");
setTitle(nullptr);  // Clear title
```

#### `void clearTitle()`

Clears the current title and frees any dynamically allocated title memory.

#### `const char* getTitle() const`

Returns the current title string, or nullptr if no title is set.

### Progress Control

Control progress bar visibility and value:

#### `void setProgressValue(int value)`

Sets progress bar value (0-100):

- `0`: Progress bar hidden
- `1-100`: Progress bar visible with percentage fill

**Example:**

```cpp
setProgressValue(0);   // Hide progress bar
setProgressValue(50);  // Show 50% progress
setProgressValue(100); // Show complete
```

#### `uint8_t getProgressValue() const`

Returns the current progress bar value (0-100).

### Button Management

Manage button labels in the footer:

#### `void setButtonItems(const char** buttonItems, uint8_t buttonItemCount)`

Sets button labels displayed in the footer region with dynamic memory allocation.

**Parameters:**

- `buttonItems`: Array of string pointers for button labels
- `buttonItemCount`: Number of strings in the array

**Example:**

```cpp
const char* buttons[] = {"◄ Back", "Select ►"};
setButtonItems(buttons, 2);

// Clear all buttons
setButtonItems(nullptr, 0);
```

#### `void clearButtonItems()`

Clears all button labels and frees allocated memory.

### Content Area Utilities

Utilities for content rendering:

#### `void clearContentArea()`

Clears the content area to background color while preserving header, footer, and progress bar areas.

#### `void drawText(uint16_t x, uint16_t y, const char* text, uint16_t color, uint8_t size = 1)`

Draws text within the content area with clipping.

**Parameters:**

- `x`: X position relative to content area
- `y`: Y position relative to content area
- `text`: Text to display
- `color`: Text color
- `size`: Text size multiplier (default: 1)

## Layout Regions

### Header Region

- **Purpose**: Title display and status indicators
- **Height**: Automatically calculated based on text size
- **Content**: Title text, system status, current mode

### Content Region

- **Purpose**: Main application area (implemented by subclasses)
- **Size**: Remaining space after header, footer, and progress bar
- **Content**: Custom rendering by derived classes

### Footer Region

- **Purpose**: Button labels and help text
- **Height**: Automatically calculated based on button labels
- **Content**: Action prompts, navigation hints, status text

### Progress Region

- **Purpose**: Progress indicator for long operations
- **Height**: Fixed height when visible, hidden when value is 0
- **Content**: Horizontal progress bar with percentage fill

## Implementation Pattern

Derived classes follow this standard pattern:

```cpp
class MyContentScreen : public ContentScreen {
private:
    int _currentValue;

protected:
    void _drawContent() override {
        // Get content area bounds
        uint16_t x = _getContentLeft();
        uint16_t y = _getContentTop();
        uint16_t w = _getContentWidth();
        uint16_t h = _getContentHeight();

        // Render content within bounds
        drawText(x + 10, y + 10, "Current Value:");
        drawText(x + 10, y + 30, String(_currentValue).c_str());
    }

public:
    MyContentScreen() : ContentScreen() {
        setTitle("My Screen");

        const char* buttons[] = {"◄ Back", "Select ►"};
        setButtonItems(buttons, 2);

        setProgressValue(0);  // Hide progress bar
        _currentValue = 42;
    }

    void loop() override {
        // Update content as needed
        _currentValue++;
        if (_currentValue > 100) _currentValue = 0;
    }

    Screen* actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY) override {
        if (action & BUTTON_MENU) {
            return nullptr;  // Handle back navigation
        }
        return nullptr;  // Stay on current screen
    }
};
```

## Notes

- **Automatic Layout**: All layout calculations are handled automatically
- **Memory Management**: Title and button labels are dynamically managed
- **Change Detection**: Only modified regions are redrawn for efficiency
- **Consistent Styling**: All ContentScreen implementations share visual consistency
- **Responsive Design**: Layout adapts to different display sizes automatically

## Example

```cpp
#include <ContentScreen.h>

class StatusScreen : public ContentScreen {
private:
    uint8_t _batteryLevel;
    bool _wifiConnected;

protected:
    void _drawContent() override {
        uint16_t x = _getContentLeft();
        uint16_t y = _getContentTop();

        // Draw status information
        drawText(x + 10, y + 10, "System Status");

        String battery = "Battery: " + String(_batteryLevel) + "%";
        drawText(x + 10, y + 30, battery.c_str());

        String wifi = "WiFi: " + String(_wifiConnected ? "Connected" : "Disconnected");
        drawText(x + 10, y + 50, wifi.c_str());
    }

public:
    StatusScreen() : ContentScreen() {
        setTitle("Device Status");

        const char* buttons[] = {"◄ Back", "Refresh"};
        setButtonItems(buttons, 2);

        _batteryLevel = 85;
        _wifiConnected = true;
    }

    void loop() override {
        // Update status periodically
        static unsigned long lastUpdate = 0;
        if (millis() - lastUpdate > 5000) {
            _batteryLevel = random(0, 101);
            _wifiConnected = random(0, 2);
            lastUpdate = millis();
        }
    }

    Screen* actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY) override {
        if (action & BUTTON_MENU) {
            return previousScreen;  // Navigate back
        }
        if (action & BUTTON_RIGHT) {
            // Refresh status
            _batteryLevel = random(0, 101);
            _wifiConnected = random(0, 2);
        }
        return nullptr;
    }
};
```
