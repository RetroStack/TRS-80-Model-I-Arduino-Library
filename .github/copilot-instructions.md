# GitHub Copilot Instructions for TRS-80 Model I Arduino Library

## Project Overview

This Arduino library provides hardware interface capabilities for the TRS-80 Model I computer via the 40-pin edge connector. The library enables control of memory, video, keyboard, and bus signals for creating ROM dumpers, RAM testers, and other utilities.

**Hardware Requirement:** Arduino Mega 2560 (only board with sufficient GPIO pins)

## Key Library Components

### Core Hardware Classes

- **Model1** - High-level TRS-80 interface, which should be the main interface for most uses
- **Model1LowLevel** - Advanced low-level bus control which can be risky to use when the user doesn't understand the hardware
- **M1Shield** - Hardware abstraction layer for Arduino shield, giving basic access to buttons, LEDs, and other pins not specific to the Model 1. It also provides the access point for the UI framework, enabling adding screens to it one at a time, and relaying keypresses.
- **Video, Keyboard, ROM, Cassette** - Specialized hardware interfaces, all of which use the Model1 class to access the Model 1. These are just high-level abstractions.

### UI Framework Classes

- **Screen** - Abstract base class for all UI screens. This enables full control of the UI with still being compatible with the UI framework
- **ContentScreen** - Structured layout template with header/content/footer/progressbar. This is useful if you want some structure, but still want to define custom views within the content area
- **MenuScreen** - Complete menu framework with pagination and navigation, simplifying menu implementation (based on ContentScreen class)
- **ConsoleScreen** - A screen based on the ContentScreen class that functions like a computer terminal or console. You can simply type one text line at a time, simplifying implementation if oyu simply want to write something to the screen.

## String Function Standards

**CRITICAL: All text-related functions support three string formats:**

1. **C-strings** (`const char*`) - For simple static text
2. **Arduino Strings** (`String`) - For dynamic content built at runtime
3. **F-strings** (`F()` macro) - For memory-efficient static text stored in Flash. Functions which accept the F marco should have the format <functionName>F, like "alert" and "alertF"

### String Function Patterns

All string functions follow this consistent API pattern:

```cpp
// Example: Title functions
void setTitle(const char *title);              // C-string version
void setTitle(String title);                   // Arduino String version
void setTitleF(const __FlashStringHelper *title); // F-string version
```

**Applied to:**

- `setTitle()` / `setTitleF()`
- `drawText()` / `drawTextF()`
- `notify()` / `notifyF()`
- `alert()` / `alertF()`
- `confirm()` / `confirmF()`
- `setMenuItems()` / `setMenuItemsF()`

### Implementation Rules

1. **No mixed parameter types** - Don't create functions like `confirmF(F(), "Cancel", "OK")`
2. **Simple pass-by-value** - Use `String title` not `const String &title`
3. **Consistent naming** - F-string versions always end with `F`
4. **Memory efficient delegation** - String versions call `.c_str()` and delegate to C-string version

## Code Standards

### File Organization

- **src/** - Core library implementation
- **examples/** - Demonstration code organized by functionality
- **docs/** - Comprehensive API documentation

Every class should have its own file (header and cpp).

Keep the "Code-Structure.md" file in the root directory up-to-date. When adding new functions, classes, files, directories, always keep it up to date. This is very important. This should be used by you as well to have a cached index of all functions and definitions and what they are used for.

Any class or method name change should be added to the keywords.txt in the project root.
Any changes to classnames (including adding/removing) require an update in the library.properties file in the root.

### Naming Conventions

- **Files**: Use descriptive names like `SimpleScreen.h` not `ExampleScreen.h`
- **Classes**: Match filename (e.g., `SimpleScreen` class in `SimpleScreen.h`)
- **Header Guards**: Use uppercase with underscores (e.g., `#ifndef COMPREHENSIVE_SCREEN_H` for `ComprehensiveScreen.h`). Single characters should not be separated (e.g., `ILOGGER_H` not `I_LOGGER_H`). Always include matching comment: `#endif /* HEADER_NAME_H */`
- **No special characters**: ASCII-only throughout codebase

### Example Structure

- Each example should have `.ino`, `.h`, `.cpp`, and `README.md` files
- README files should include usage examples and learning objectives
- Documentation should be moved from `.ino` files to README files for accessibility
- Examples for the UI which use a displayProvider should already provide commented-out headers and variables as shown in the simple screen example

### Screen Object Creation Rules

**CRITICAL: Never create Screen objects as global variables**

- **Rule**: Always create Screen objects dynamically using `new`
- **Reason**: Global object constructors run before some important code.

```cpp
// CORRECT
M1Shield.setScreen(new SimpleMenu());

// WRONG - Global object creation
SimpleMenu mainMenu;  // Constructor runs before any other code
...
M1Shield.setScreen(mainMenu);
```

### Serial Communication Standards

**CRITICAL: Use consistent serial baud rate across all examples**

- **Rule**: Use `Serial.begin(115200);` for all examples where serial logging is needed unless there is a specific technical need to change this
- **Reason**: Provides consistent user experience and faster communication for debugging
- **Exceptions**: Only change baud rate if:
  - Hardware limitations require a different rate
  - Specific example demonstrates baud rate configuration
  - Compatibility with external devices requires different rate

```cpp
// CORRECT - Standard baud rate
Serial.begin(115200);

// ACCEPTABLE - With documentation for specific need
Serial.begin(9600);  // Using 9600 for compatibility with older terminal software
```

### Parent Constructor and Method Calling Rules

**CRITICAL: Always call parent constructors and virtual methods properly**

- **Rule**: All derived screen classes must explicitly call parent constructors in initialization lists
- **Rule**: When overriding virtual methods (`open()`, `close()`, `loop()`), always call parent implementation first
- **Reason**: Ensures proper initialization, cleanup, and functionality inheritance

```cpp
// CORRECT - Constructor calls
class MyMenu : public MenuScreen {
public:
    MyMenu() : MenuScreen() { /* custom init */ }
};

class MyContent : public ContentScreen {
public:
    MyContent() : ContentScreen() { /* custom init */ }
};

class MyScreen : public Screen {
public:
    MyScreen() : Screen() { /* custom init */ }
};

// CORRECT - Virtual method overrides
bool MyScreen::open() override {
    if (!Screen::open()) return false;  // Call parent first
    // Add custom logic here
    return true;
}

void MyScreen::close() override {
    // Custom cleanup here
    Screen::close();  // Call parent last for close()
}

void MyScreen::loop() override {
    Screen::loop();   // Call parent first for loop()
    // Add custom logic here
}

// WRONG - Missing parent calls
class BadMenu : public MenuScreen {
public:
    BadMenu() { /* Missing : MenuScreen() */ }
};
```

### ConsoleScreen Constructor Rules

**CRITICAL: Never perform display operations in ConsoleScreen constructors**

- **Rule**: ConsoleScreen constructors should only set configuration (title, colors, text size, etc.) - never call display methods like `print()`, `println()`, `cls()`, etc.
- **Rule**: Use the `_executeOnce()` override method for initial display operations
- **Reason**: Display operations in constructor happen before screen is properly initialized and active

```cpp
// CORRECT - ConsoleScreen constructor
class MyConsole : public ConsoleScreen {
public:
    MyConsole() : ConsoleScreen() {
        setTitleF(F("My Console"));
        setTextColor(M1Shield.convertColor(0xFFFF));
        setTextSize(1);
        // NO display operations here
    }

protected:
    void _executeOnce() override {
        // Display operations go here - called 1 second after screen opens
        cls();
        println(F("Welcome message"));
    }
};

// WRONG - Display operations in constructor
class BadConsole : public ConsoleScreen {
public:
    BadConsole() : ConsoleScreen() {
        cls();                    // BAD - display operation in constructor
        println(F("Welcome"));   // BAD - display operation in constructor
    }
};
```

### Parent Loop Method Calling Rules

**CRITICAL: Always call parent loop() method in derived screen classes**

- **Rule**: When overriding `loop()` method in ContentScreen or MenuScreen derivatives, always call parent `loop()` first
- **Reason**: Parent classes handle essential functionality like notification timeouts, lifecycle management, etc.

```cpp
// CORRECT - ContentScreen derivative
void MyContentScreen::loop() {
    ContentScreen::loop();  // Handle notification timeouts
    // Add custom loop logic here
}

// CORRECT - MenuScreen derivative
void MyMenuScreen::loop() {
    MenuScreen::loop();     // Calls ContentScreen::loop() internally
    // Add custom loop logic here
}

// WRONG - Missing parent call
void MyMenuScreen::loop() {
    // Custom logic only - notifications will never timeout!
}
```

## Documentation Requirements

### Source Code Documentation

- **Header files (.h)**: One-line comments for public methods only
- **CPP files (.cpp)**: Essential inline comments only for complex logic
- **Comprehensive documentation**: Located in corresponding docs/ folder files
- **No extensive multi-line comments** in source code files
- **No @brief, @param, @return** tags in source code

### API Documentation

- All comprehensive method documentation must be in docs/ folder
- Use docs/ClassName.md format for each class
- Include complete API reference, usage examples, and implementation notes
- Cross-reference related classes and concepts

### Example Documentation

- README.md files should be primary documentation
- Include "What You'll Learn" and "What It Does" sections

## Testing and Validation

### Compilation Requirements

- Don't run compilation yourself. The user will do that.
