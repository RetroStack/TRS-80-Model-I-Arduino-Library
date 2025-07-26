# MenuScreen Class

The `MenuScreen` class provides a complete framework for creating navigable menu interfaces with automatic pagination, visual selection feedback, and input handling. It extends `ContentScreen` to offer standardized menu functionality while allowing customization of menu items and actions.

## Table of Contents

- [Constructor](#constructor)
- [Menu Management](#menu-management)
- [Navigation System](#navigation-system)
- [Pagination](#pagination)
- [Visual Design](#visual-design)
- [Protected Methods](#protected-methods)
- [Implementation Pattern](#implementation-pattern)
- [Memory Management](#memory-management)
- [Notes](#notes)
- [Example](#example)

## Constructor

```cpp
MenuScreen()
```

Creates a new `MenuScreen` instance with:

- No menu items initially
- First item selected by default
- Page 0 displayed
- Automatic pagination based on content area height

> **Note**: This is an abstract class - derived classes must implement `_getSelectedMenuItemScreen()`

## Menu Management

### Setting Menu Items

```cpp
void _setMenuItems(const char** menuItems, uint8_t menuItemCount)
```

Configures the menu with an array of menu item strings:

- **Deep Copy**: Creates copies of all strings - original array can be freed
- **Dynamic Allocation**: Automatically manages memory for menu items
- **Auto-Reset**: Selection returns to first item when menu changes
- **Pagination Update**: Automatically recalculates pages when items change

**Example:**

```cpp
const char* items[] = {"New Game", "Load Game", "Settings", "Exit"};
_setMenuItems(items, 4);
```

### Menu Item Access

```cpp
uint8_t getMenuItemCount()           // Get total number of menu items
uint8_t getSelectedIndex()           // Get currently selected item index (0-based)
const char* getSelectedItem()        // Get text of currently selected item
const char* getMenuItem(uint8_t index)  // Get text of specific menu item
```

## Navigation System

### Selection Control

Navigation is handled automatically by the base class:

- **Up Arrow**: Move to previous item (wraps to last item)
- **Down Arrow**: Move to next item (wraps to first item)
- **Page Up**: Jump to first item on previous page
- **Page Down**: Jump to first item on next page
- **Select Action**: Typically Menu button or Right arrow

### Selection Wrapping

- **Top Wrap**: Selecting up from first item goes to last item
- **Bottom Wrap**: Selecting down from last item goes to first item
- **Page Boundaries**: Selection automatically handles page transitions
- **Smooth Navigation**: Page changes maintain relative position when possible

## Pagination

### Automatic Page Calculation

```cpp
uint8_t _getItemsPerPage()    // Returns items that fit in content area
uint8_t getCurrentPage()      // Returns current page number (0-based)
uint8_t getTotalPages()       // Returns total number of pages
```

### Page Navigation

- **Automatic**: Pages change automatically when selection moves beyond page boundaries
- **Efficient**: Only displays items for current page
- **Adaptive**: Page size adjusts to available content area height
- **Visual Indicators**: Page numbers shown when multiple pages exist

**Pagination Example:**

```
Page 1 of 3        Page 2 of 3        Page 3 of 3
-----------        -----------        -----------
► New Game         ► Options          ► Credits
  Load Game          Graphics           About
  Quick Start        Audio            ► Exit
  Tutorial           Controls
```

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

## Protected Methods

### Pure Virtual Method

```cpp
virtual Screen* _getSelectedMenuItemScreen(int index) = 0
```

**Must be implemented** by derived classes to define what happens when a menu item is selected:

```cpp
class MainMenu : public MenuScreen {
protected:
    Screen* _getSelectedMenuItemScreen(int index) override {
        switch(index) {
            case 0: return new GameScreen();
            case 1: return new LoadScreen();
            case 2: return new SettingsScreen();
            case 3: return nullptr;  // Exit - handled by returning nullptr
        }
        return nullptr;
    }
};
```

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
        _setMenuItems(items, 6);

        // Configure layout
        _setTitle("Settings");

        const char* buttons[] = {"◄ Back", "Select ►"};
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
#include "MenuScreen.h"
#include "GameScreen.h"
#include "SettingsScreen.h"

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
        if ((action & BUTTON_RIGHT) && getSelectedIndex() == 5) {
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
    M1Shield.begin();

    MainMenu* menu = new MainMenu();
    M1Shield.setScreen(menu);
}

void loop() {
    M1Shield.processInput();
    M1Shield.updateScreen();
    M1Shield.renderScreen();
}
```
