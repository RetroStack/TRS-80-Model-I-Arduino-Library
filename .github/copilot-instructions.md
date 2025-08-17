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

Every class should have its own file (header and cpp)

### Naming Conventions

- **Files**: Use descriptive names like `SimpleScreen.h` not `ExampleScreen.h`
- **Classes**: Match filename (e.g., `SimpleScreen` class in `SimpleScreen.h`)
- **No special characters**: ASCII-only throughout codebase

### Example Structure

- Each example should have `.ino`, `.h`, `.cpp`, and `README.md` files
- README files should include usage examples and learning objectives
- Documentation should be moved from `.ino` files to README files for accessibility

## Documentation Requirements

### API Documentation

- All public methods must have comprehensive documentation

### Example Documentation

- README.md files should be primary documentation
- Include "What You'll Learn" and "What It Does" sections

## Testing and Validation

### Compilation Requirements

- Don't run compilation yourself. The user will do that.
