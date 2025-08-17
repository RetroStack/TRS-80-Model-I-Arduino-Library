# MenuScreen Class

The `MenuScreen` class provides a complete framework for creating navigable menu interfaces with automatic pagination, visual selection feedback, and input handling. It extends `ContentScreen` to offer standardized menu functionality while allowing customization of menu items and actions.

**NEW: Comprehensive String Support** - Menu items now support three formats:

- **C-string arrays** (`const char**`) - For simple static menus
- **String arrays** (`String*`) - For dynamic menus built at runtime
- **F-string arrays** (`const __FlashStringHelper**`) - For memory-efficient static menus

Plus all inherited ContentScreen string functions (setTitle, notify, alert, confirm, drawText).

## Table of Contents

- [Constructor](#constructor)
- [Destructor](#destructor)
- [Menu Management](#menu-management)
- [Navigation System](#navigation-system)
- [Input Handling](#input-handling)
- [Pagination](#pagination)
- [Protected Methods](#protected-methods)
  - [Abstract Methods](#abstract-methods)
  - [Optional Override Methods](#optional-override-methods)
  - [Protected Utility Methods](#protected-utility-methods)
- [Visual Design](#visual-design)
- [Implementation Pattern](#implementation-pattern)
- [Memory Management](#memory-management)
- [Notes](#notes)
- [Example](#example)

## Constructor

- **`MenuScreen()`** - Creates a new MenuScreen instance

Creates a new `MenuScreen` instance with:

- No menu items initially
- First item selected by default
- Page 0 displayed
- Automatic pagination based on content area height

> **Note**: This is an abstract class - derived classes must implement `_getSelectedMenuItemScreen()`

## Destructor

- **`virtual ~MenuScreen()`** - Frees dynamically allocated menu items

Ensures proper cleanup of any dynamically allocated menu items to prevent memory leaks when menu screens are destroyed.

## Menu Management

MenuScreen supports three ways to set menu items for maximum flexibility:

- **`void setMenuItems(const char** menuItems, uint8_t menuItemCount)`\*\* - Set menu items from C-string array
- **`void setMenuItems(String* menuItems, uint8_t menuItemCount)`** - Set menu items from Arduino String array
- **`void setMenuItemsF(const \_\_FlashStringHelper** menuItems, uint8_t menuItemCount)`\*\* - Set menu items from F-string array
- **`void clearMenuItems()`** - Clear and free all dynamically allocated menu items

### Setting Menu Items

Choose the method that best fits your use case:

#### Static Menus (C-strings)

For simple, unchanging menu items:

```cpp
const char* items[] = {"New Game", "Load Game", "Settings", "Exit"};
setMenuItems(items, 4);
```

#### Dynamic Menus (Arduino Strings)

For menus with content built at runtime:

```cpp
String playerName = "Alice";
int score = 1250;

String items[] = {
    String("Continue as ") + playerName,
    String("High Score: ") + String(score),
    "New Game",
    "Settings"
};
setMenuItems(items, 4);
```

#### Memory-Efficient Static Menus (F-strings)

For static menus that save RAM:

```cpp
const __FlashStringHelper* items[] = {
    F("New Game"),
    F("Load Game"),
    F("Settings"),
    F("Exit")
};
setMenuItemsF(items, 4);
```

### Menu Item Management

All `setMenuItems()` methods:

- **Deep Copy**: Creates copies of all strings - original array can be freed
- **Dynamic Allocation**: Automatically manages memory for menu items
- **Auto-Reset**: Selection returns to first item when menu changes
- **Pagination Update**: Automatically recalculates pages when items change

**Example:**

```cpp
const char* items[] = {"New Game", "Load Game", "Settings", "Exit"};
setMenuItems(items, 4);
```

### Setting Menu Items from FlashString

The `setMenuItemsF()` method provides memory-efficient menu configuration using FlashString (F() macro):

- **Flash Storage**: Menu text stored in flash memory instead of RAM
- **Deep Copy**: Converts FlashStrings to regular strings with automatic allocation
- **Memory Efficient**: Significant RAM savings for static menu text
- **Same Behavior**: Identical functionality to `setMenuItems()` with better memory usage

**Example:**

```cpp
// Memory-efficient menu using FlashString array
static const __FlashStringHelper* flashItems[] = {
    F("New Game"),     // Stored in flash, not RAM
    F("Load Game"),
    F("Settings"),
    F("Exit")
};
setMenuItemsF(flashItems, 4);
```

**Memory Benefits:**

- **RAM Savings**: Menu text stored in abundant flash memory instead of scarce RAM
- **Static Text**: Ideal for fixed menu items that don't change at runtime
- **Automatic Conversion**: FlashStrings automatically converted and allocated as needed

## Navigation System

### Selection Control

Navigation is handled automatically by the base class:

- **Up Arrow**: Move to previous item (wraps to last item)
- **Down Arrow**: Move to next item (wraps to first item)
- **Page Up**: Jump to first item on previous page
- **Page Down**: Jump to first item on next page
- **Select Action**: Left, Right arrows, or Joystick button

### Selection Wrapping

- **Top Wrap**: Selecting up from first item goes to last item
- **Bottom Wrap**: Selecting down from last item goes to first item
- **Page Boundaries**: Selection automatically handles page transitions
- **Smooth Navigation**: Page changes maintain relative position when possible

## Input Handling

- **`virtual void loop()`** - Main loop processing for menu screen updates (override for custom processing)
- **`Screen* actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY) override`** - Handle user input actions and navigate accordingly

The `actionTaken()` method processes user input for menu navigation including:

- Up/Down: Navigate between menu items with wraparound
- Right/Left/Select: Activate selected menu item
- Menu: Return to previous screen
- Page navigation for multi-page menus

## Navigation System

### Selection Control

Navigation is handled automatically by the base class:

- **Up Arrow**: Move to previous item (wraps to last item)
- **Down Arrow**: Move to next item (wraps to first item)
- **Page Up**: Jump to first item on previous page
- **Page Down**: Jump to first item on next page
- **Select Action**: Left, Right arrows, or Joystick button

### Selection Wrapping

- **Top Wrap**: Selecting up from first item goes to last item
- **Bottom Wrap**: Selecting down from last item goes to first item
- **Page Boundaries**: Selection automatically handles page transitions
- **Smooth Navigation**: Page changes maintain relative position when possible

## Pagination

### Automatic Page Calculation

Pagination is handled automatically by the MenuScreen system:

- **Automatic**: Pages change automatically when selection moves beyond page boundaries
- **Efficient**: Only displays items for current page
- **Adaptive**: Page size adjusts to available content area height
- **Visual Indicators**: Page numbers shown when multiple pages exist

**Pagination Example:**

```
Page 1 of 3        Page 2 of 3        Page 3 of 3
-----------        -----------        -----------
> New Game         > Options          > Credits
  Load Game          Graphics           About
  Quick Start        Audio            > Exit
  Tutorial           Controls
```

## Protected Methods

### Abstract Methods

- **`virtual Screen* _getSelectedMenuItemScreen(int index) = 0`** - Abstract method to get the screen for a selected menu item

**Must be implemented** by derived classes to define what happens when a menu item is selected. Return nullptr to stay on current screen.

### Optional Override Methods

- **`virtual const char* _getMenuItemConfigValue(uint8_t index)`** - Get configuration value string for a menu item
- **`virtual const __FlashStringHelper* _getMenuItemConfigValueF(uint8_t index)`** - Get configuration value FlashString for a menu item
- **`virtual bool _isMenuItemEnabled(uint8_t index) const`** - Check if a menu item is enabled/selectable

### Protected Utility Methods

- **`void _drawContent()`** - Draw the menu content area with paginated item list
- **`void _setSelectedMenuItemIndex(uint8_t index)`** - Set the currently selected menu item by index
- **`uint8_t _getSelectedMenuItemIndex() const`** - Get the index of the currently selected menu item

### Menu Configuration

```cpp
void _setMenuItems(const char** items, uint8_t count)  // Set menu items
void _setTitle(const char* title)                      // Set menu title
void _setFooterButtons(const char** buttons, uint8_t count)  // Set footer buttons
```

### Configuration Values (Optional)

```cpp
virtual const char* _getMenuItemConfigValue(int index)  // Get config value for menu item
```

**Optional override** to display configuration values right-aligned on each menu row:

```cpp
class SettingsMenu : public MenuScreen {
private:
    bool _soundEnabled = true;
    uint8_t _difficulty = 2;
    char _timeoutBuffer[8];

protected:
    const char* _getMenuItemConfigValue(int index) override {
        switch(index) {
            case 0: return _soundEnabled ? "On" : "Off";        // Boolean setting
            case 1: return _difficulty == 0 ? "Easy" :
                          _difficulty == 1 ? "Normal" : "Hard"; // Enum setting
            case 2:
                snprintf(_timeoutBuffer, sizeof(_timeoutBuffer), "%ds", _timeout);
                return _timeoutBuffer;                          // Numeric value
            default: return nullptr;                           // No config value
        }
    }
};
```

### FlashString Configuration Values (Optional)

```cpp
virtual const __FlashStringHelper* _getMenuItemConfigValueF(int index)  // Get flash config value for menu item
```

**Optional override** to display memory-efficient configuration values using FlashString (F() macro). Takes precedence over `_getMenuItemConfigValue()` if both are implemented:

```cpp
class SettingsMenu : public MenuScreen {
private:
    bool _soundEnabled = true;
    uint8_t _displayMode = 0;
    bool _autoSave = true;

protected:
    const __FlashStringHelper* _getMenuItemConfigValueF(int index) override {
        switch(index) {
            case 0: return _soundEnabled ? F("On") : F("Off");           // Boolean setting with flash storage
            case 1: return _displayMode == 0 ? F("LCD") :                // Enum with flash strings
                          _displayMode == 1 ? F("OLED") : F("CRT");
            case 2: return _autoSave ? F("Enabled") : F("Disabled");     // Status with flash storage
            case 3: return F("Default");                                 // Static value in flash
            default: return nullptr;                                     // No config value
        }
    }
};
```

**FlashString Benefits:**

- **Memory Efficient**: Configuration text stored in flash memory instead of RAM
- **Static Values**: Perfect for fixed configuration options that don't change
- **Priority**: FlashString version takes precedence over regular string version
- **Automatic Fallback**: System falls back to `_getMenuItemConfigValue()` if FlashString version returns nullptr

## Visual Design

### Menu Appearance

- **Selection Highlight**: Currently selected item is visually highlighted
- **Alternating Rows**: Subtle background alternation for readability
- **Consistent Spacing**: Uniform spacing between menu items
- **Page Indicators**: Page numbers displayed when multiple pages exist
- **Scroll Hints**: Visual indicators when more items are available

### Color Scheme

- **Selected Item**: Highlighted background with contrasting text
- **Normal Items**: Standard text color on transparent background
- **Alternating Rows**: Subtle background color variation
- **Page Info**: Dimmed text for page indicators

## Implementation Pattern

### Standard Menu Implementation

```cpp
class SettingsMenu : public MenuScreen {
private:
    // Optional: Store menu-specific state
    bool _soundEnabled;
    uint8_t _difficulty;

public:
    SettingsMenu() {
        // Configure menu items
        const char* items[] = {
            "Display Settings",
            "Audio Settings",
            "Input Settings",
            "Game Difficulty",
            "Reset to Defaults",
            "Back to Main Menu"
        };
        setMenuItems(items, 6);

        // Configure layout
        _setTitle("Settings");

        const char* buttons[] = {"[M] Exit", "Select >"};`
        _setFooterButtons(buttons, 2);

        // Initialize state
        _soundEnabled = true;
        _difficulty = 2;
    }

protected:
    Screen* _getSelectedMenuItemScreen(int index) override {
        switch(index) {
            case 0: return new DisplaySettingsScreen();
            case 1: return new AudioSettingsScreen();
            case 2: return new InputSettingsScreen();
            case 3: return new DifficultySettingsScreen();
            case 4:
                resetToDefaults();
                return nullptr;  // Stay on menu
            case 5: return previousScreen;  // Back to main menu
        }
        return nullptr;
    }

    const char* _getMenuItemConfigValue(int index) override {
        switch(index) {
            case 1: return _soundEnabled ? "On" : "Off";        // Audio Settings
            case 3: return _difficulty == 0 ? "Easy" :
                          _difficulty == 1 ? "Normal" : "Hard"; // Game Difficulty
            default: return nullptr;                           // No config for other items
        }
    }

private:
    void resetToDefaults() {
        _soundEnabled = true;
        _difficulty = 2;
        // Update settings...
    }
};
```

### Enabled/Disabled Items (Optional)

**Optional override** to control which menu items are enabled or disabled:

```cpp
virtual bool _isMenuItemEnabled(int index) { return true; }
```

- **Default**: All items enabled (returns `true`)
- **Visual**: Disabled items are grayed out with reduced contrast
- **Navigation**: Selection automatically skips over disabled items
- **Auto-adjustment**: If current selection becomes disabled, moves to next enabled item

**Visual Appearance:**

- **Enabled Items**: Normal colors with selection highlighting
- **Disabled Items**: Grayed out colors, cannot be selected
- **Selection**: Only enabled items can be highlighted and selected

**Example - Conditional Menu Items:**

```cpp
class GameMenu : public MenuScreen {
private:
    bool _gameInProgress;
    bool _hasSaveFile;

public:
    GameMenu() {
        const char* items[] = {
            "New Game",
            "Continue Game",
            "Load Game",
            "Settings",
            "Exit"
        };
        _setMenuItems(items, 5);
        _setTitle("Main Menu");

        // Check game state
        _gameInProgress = checkGameState();
        _hasSaveFile = checkSaveFile();
    }

protected:
    Screen* _getSelectedMenuItemScreen(int index) override {
        switch(index) {
            case 0: return new NewGameScreen();
            case 1: return new ContinueGameScreen();
            case 2: return new LoadGameScreen();
            case 3: return new SettingsScreen();
            case 4: return nullptr; // Exit
        }
        return nullptr;
    }

    bool _isMenuItemEnabled(int index) override {
        switch(index) {
            case 0: return true;                    // New Game - always enabled
            case 1: return _gameInProgress;         // Continue - only if game active
            case 2: return _hasSaveFile;           // Load - only if save exists
            case 3: return true;                    // Settings - always enabled
            case 4: return true;                    // Exit - always enabled
            default: return true;
        }
    }

private:
    bool checkGameState() {
        // Check if game is currently in progress
        return false; // Implementation specific
    }

    bool checkSaveFile() {
        // Check if save file exists
        return true; // Implementation specific
    }
};
```

### Dynamic Menu Generation

```cpp
class FileMenu : public MenuScreen {
private:
    std::vector<String> _fileList;

public:
    FileMenu() {
        loadFileList();
        updateMenuItems();
        _setTitle("Select File");
    }

private:
    void loadFileList() {
        // Load files from storage
        _fileList.clear();
        _fileList.push_back("game1.save");
        _fileList.push_back("game2.save");
        _fileList.push_back("game3.save");
    }

    void updateMenuItems() {
        // Convert vector to const char* array
        const char** items = new const char*[_fileList.size()];
        for (size_t i = 0; i < _fileList.size(); i++) {
            items[i] = _fileList[i].c_str();
        }

        _setMenuItems(items, _fileList.size());
        delete[] items;  // MenuScreen made copies
    }

protected:
    Screen* _getSelectedMenuItemScreen(int index) override {
        if (index < _fileList.size()) {
            return new GameScreen(_fileList[index]);
        }
        return nullptr;
    }
};
```

## Memory Management

### Automatic Memory Handling

- **String Duplication**: All menu item strings are duplicated internally
- **Automatic Cleanup**: Memory freed when menu is destroyed or items change
- **Safe References**: Original string arrays can be freed after `_setMenuItems()`
- **Dynamic Sizing**: Memory allocated based on actual menu size

### Memory Considerations

- **Per-Item Overhead**: Each menu item requires strlen + 1 bytes
- **Page Calculation**: Minimal overhead for pagination calculations
- **Selection State**: Single byte for current selection index
- **Total Usage**: Approximately (total string length + item count + 16) bytes

## Notes

- **Abstract Class**: Cannot be instantiated directly - must be subclassed
- **Automatic Input**: Navigation input handled automatically by base class
- **Memory Efficient**: Only allocates memory for actual menu items
- **Display Adaptive**: Automatically adjusts to different screen sizes
- **Thread Safety**: Designed for single-threaded Arduino main loop usage
- **Consistent Behavior**: All MenuScreen instances share the same navigation model

## Example

```cpp
#include <MenuScreen.h>
#include "GameScreen.h"
#include <SettingsScreen.h>

class MainMenu : public MenuScreen {
private:
    GameScreen* _gameScreen;
    SettingsScreen* _settingsScreen;

public:
    MainMenu() {
        // Create child screens
        _gameScreen = new GameScreen();
        _settingsScreen = new SettingsScreen();

        // Configure menu
        const char* items[] = {
            "New Game",
            "Continue Game",
            "Settings",
            "High Scores",
            "About",
            "Exit"
        };
        _setMenuItems(items, 6);

        _setTitle("Main Menu");

        const char* buttons[] = {"Select", "Exit"};
        _setFooterButtons(buttons, 2);
    }

    ~MainMenu() {
        delete _gameScreen;
        delete _settingsScreen;
    }

protected:
    Screen* _getSelectedMenuItemScreen(int index) override {
        switch(index) {
            case 0:  // New Game
                _gameScreen->startNewGame();
                return _gameScreen;

            case 1:  // Continue Game
                if (_gameScreen->hasSavedGame()) {
                    _gameScreen->loadSavedGame();
                    return _gameScreen;
                } else {
                    // Show message: "No saved game"
                    return nullptr;  // Stay on menu
                }

            case 2:  // Settings
                return _settingsScreen;

            case 3:  // High Scores
                return new HighScoreScreen();

            case 4:  // About
                return new AboutScreen();

            case 5:  // Exit
                // Handle exit logic
                return nullptr;
        }
        return nullptr;
    }

public:
    // Override actionTaken to handle exit
    Screen* actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY) override {
        // Let parent handle navigation first
        Screen* result = MenuScreen::actionTaken(action, offsetX, offsetY);

        // Handle exit button
        if ((action & BUTTON_RIGHT) && _getSelectedMenuItemIndex() == 5) {
            // Exit application
            M1Shield.setLEDColor(COLOR_RED);
            Serial.println("Exiting application...");
            // Could return nullptr or shutdown screen
        }

        return result;
    }
};

// Usage
void setup() {
    M1Shield.begin(displayProvider);

    MainMenu* menu = new MainMenu();
    M1Shield.setScreen(menu);
}

void loop() {
    M1Shield.loop();
}
```

## FlashString Example

This example demonstrates memory-efficient menu creation using FlashString (F() macro):

```cpp
#include <MenuScreen.h>

class FlashStringMenu : public MenuScreen {
private:
    GameScreen* _gameScreen;
    SettingsScreen* _settingsScreen;

public:
    FlashStringMenu() {
        // Create child screens
        _gameScreen = new GameScreen();
        _settingsScreen = new SettingsScreen();

        // Configure menu using FlashString - saves significant RAM
        static const __FlashStringHelper* flashMenuItems[] = {
            F("New Game"),         // Stored in flash memory
            F("Continue Game"),    // Not consuming precious RAM
            F("Settings"),
            F("High Scores"),
            F("Credits"),
            F("Exit")
        };
        setMenuItemsF(flashMenuItems, 6);  // Memory-efficient menu setup

        // Title using FlashString
        setTitleF(F("Main Menu"));

        // Footer buttons using FlashString
        static const __FlashStringHelper* flashButtons[] = {
            F("Select"),
            F("Exit")
        };
        setButtonItemsF(flashButtons, 2);
    }

    ~FlashStringMenu() {
        delete _gameScreen;
        delete _settingsScreen;
    }

protected:
    Screen* _getSelectedMenuItemScreen(int index) override {
        switch(index) {
            case 0:  // New Game
                _gameScreen->startNewGame();
                return _gameScreen;

            case 1:  // Continue Game
                if (_gameScreen->hasSavedGame()) {
                    _gameScreen->loadSavedGame();
                    return _gameScreen;
                } else {
                    // Could show "No saved game" message
                    return nullptr;
                }

            case 2:  // Settings
                return _settingsScreen;

            case 3:  // High Scores
                return new HighScoreScreen();

            case 4:  // Credits
                return new CreditsScreen();

            case 5:  // Exit
                // Handle application exit
                return nullptr;
        }
        return nullptr;
    }

    // Dynamic menu updates using FlashString
    void updateMenuForGameState() {
        if (_gameScreen->hasSavedGame()) {
            // Show continue option
            static const __FlashStringHelper* gameMenuItems[] = {
                F("New Game"),
                F("Continue Game"),  // Available
                F("Settings"),
                F("Exit")
            };
            setMenuItemsF(gameMenuItems, 4);
        } else {
            // Hide continue option
            static const __FlashStringHelper* noGameItems[] = {
                F("New Game"),
                F("Settings"),
                F("Exit")
            };
            setMenuItemsF(noGameItems, 3);
        }
    }
};
```

**FlashString Benefits:**

- **RAM Conservation**: Menu text stored in flash memory instead of scarce RAM
- **Static Efficiency**: Perfect for fixed menu items that don't change
- **Easy Conversion**: Simple replacement of regular strings with F() macro
- **Same Functionality**: Identical behavior to regular string methods
