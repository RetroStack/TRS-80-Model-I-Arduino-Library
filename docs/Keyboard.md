# Keyboard Class

The `Keyboard` class provides high-level access to the TRS-80 Model I keyboard. It abstracts the 8x8 keyboard matrix mapped into memory and provides methods to check which keys are pressed, track changes, and retrieve key codes. It uses the `Model1` object to read memory addresses corresponding to each keyboard row.

## Table of Contents

- [Overview](#overview)
- [Constructor](#constructor)
- [Configuration Methods](#configuration-methods)
  - [setLogger](#void-setloggerilogger-logger)
- [Keyboard State Methods](#keyboard-state-methods)
  - [isKeyPressed](#bool-iskeypressed)
  - [update](#void-update)
  - [getFirstJustPressedKey](#uint8_t-getfirstjustpressedkey)
  - [changes](#keyboardchangeiterator-changes)
- [KeyboardChangeIterator Class](#keyboardchangeiterator-class)
  - [Constructor](#keyboardchangeiterator-constructor)
  - [Navigation Methods](#navigation-methods)
  - [State Query Methods](#state-query-methods)
  - [Key Information Methods](#key-information-methods)
- [Special Key Codes](#special-key-codes)
- [Notes](#notes)
- [Example](#example)

## Overview

The `Keyboard` class provides high-level access to the TRS-80 Model I keyboard. It abstracts the 8x8 keyboard matrix mapped into memory and provides methods to check which keys are pressed, track changes, and retrieve key codes. The associated `KeyboardChangeIterator` helps enumerate which keys have changed since the last scan.

## Constructor

```cpp
Keyboard()
```

Creates a new Keyboard instance. Initializes the keyboard state tracking arrays.

## Configuration Methods

### `void setLogger(ILogger &logger)`

Sets the logger used for errors and warnings.

**Parameters:**

- `logger`: Reference to an ILogger implementation

_This is often useful for debugging as it tells what went wrong._

## Keyboard State Methods

### `bool isKeyPressed()`

Returns `true` if any key on the keyboard is currently pressed.

**Returns:** true if any key is currently pressed, false otherwise

### `void update()`

Scans the keyboard and saves the current state internally for change detection.

_Call this function once before starting to read state to make sure the last state is up to date._

### `uint8_t getFirstJustPressedKey()`

Scans for the first key that has just been pressed since the last update and returns its key code.

**Returns:** Key code of the first pressed key, or 0 if no key was just pressed

_This recognizes only the first of all changes. If you want to get a full picture, use the `changes()` function._

### `KeyboardChangeIterator changes()`

Returns an iterator over keys whose state has changed since the last call to `update()`, `changes()`, or `getFirstJustPressedKey()`.

**Returns:** Iterator for enumerating key state changes

Use this to detect both presses and releases of all keys that have changed.

## KeyboardChangeIterator Class

The `KeyboardChangeIterator` inspects the differences between two keyboard states and allows iteration through all changed keys.

### KeyboardChangeIterator Constructor

```cpp
KeyboardChangeIterator(const uint8_t *previous, const uint8_t *current)
```

**Parameters:**

- `previous`: Previous keyboard state array (8 bytes)
- `current`: Current keyboard state array (8 bytes)

_Typically created via `Keyboard::changes()` rather than directly._

### Navigation Methods

#### `bool hasNext()`

Returns true if there is at least one more changed key to examine.

**Returns:** true if more changes are available

#### `void next()`

Moves the iterator to the next changed key.

_Must call `hasNext()` first to ensure there are more changes._

### State Query Methods

#### `bool wasJustPressed()`

Returns true if the current key was previously released and is now pressed.

**Returns:** true if key was just pressed

#### `bool wasReleased()`

Returns true if the current key was previously pressed and is now released.

**Returns:** true if key was just released

#### `bool wasPressed()`

Returns true if the key was pressed in the previous state.

**Returns:** true if previously pressed

#### `bool isPressed()`

Returns true if the key is pressed in the current state.

**Returns:** true if currently pressed

### Key Information Methods

#### `uint8_t keyValue()`

Returns the key code from the lookup tables (ASCII or special code).

**Returns:** ASCII character or special key code

#### `string keyName()`

Returns a human-readable string name for the key.

**Returns:** String representation of the key name

- Special keys return descriptive names: "ENTER", "SHIFT", "SPACE", "CLEAR", etc.
- ASCII characters return single-character strings: "A", "1", "!", etc.
- Unknown keys return hex notation: "0x7F", "0xA0", etc.

**Example:**
```cpp
KeyboardChangeIterator it = keyboard.changes();
while (it.hasNext()) {
    string name = it.keyName();  // e.g., "ENTER", "A", "SPACE"
    Serial.println("Key: " + name);
    it.next();
}
```

#### `uint8_t keyIndex()`

Returns the linear key index (0-63) in the 8x8 matrix.

**Returns:** Key index (0-63)

#### `uint8_t row()`

Returns the keyboard matrix row (0-7).

**Returns:** Row number (0-7)

#### `uint8_t column()`

Returns the keyboard matrix column (0-7).

**Returns:** Column number (0-7)

#### `bool isShiftPressed()`

Returns true if Shift was pressed during the current scan.

**Returns:** true if Shift key is pressed

_This affects character case for letter keys._

## Special Key Codes

In addition to standard ASCII characters, some keys have unique codes:

| Key         | Code | Hex Value | Description     |
| ----------- | ---- | --------- | --------------- |
| `KEY_SPACE` | 32   | 0x20      | Space character |
| `KEY_SHIFT` | 129  | 0x81      | Shift key       |
| `KEY_ENTER` | 176  | 0xB0      | Enter key       |
| `KEY_BREAK` | 177  | 0xB1      | Break key       |
| `KEY_CLEAR` | 178  | 0xB2      | Clear key       |
| `KEY_RIGHT` | 215  | 0xD7      | Right arrow     |
| `KEY_LEFT`  | 216  | 0xD8      | Left arrow      |
| `KEY_DOWN`  | 217  | 0xD9      | Down arrow      |
| `KEY_UP`    | 218  | 0xDA      | Up arrow        |

**Character Encoding:**

- Regular ASCII letters (a-z, A-Z) are returned as their ASCII values
- Numbers (0-9) and symbols follow standard ASCII encoding
- Case is determined by Shift key state via `isShiftPressed()`
- Special function keys use the codes above

## Notes

- Always call `model1.activateTestSignal()` before reading keyboard state.
- The lookup tables map matrix positions to characters or codes.
- Upper/lower case is determined by whether Shift is pressed.

## Example

```cpp
#include <Model1.h>
#include <Keyboard.h>

Keyboard keyboard;

void setup() {
  Serial.begin(115200);

  Model1.begin();
  Model1.activateTestSignal();
}

void loop() {
  // Update keyboard state
  keyboard.update();

  // Check if any key is pressed
  if (keyboard.isKeyPressed()) {
    Serial.println("A key is pressed");
  }

  // Get first pressed key
  uint8_t firstKey = keyboard.getFirstJustPressedKey();
  if (firstKey != 0) {
    Serial.print("First key pressed: ");
    Serial.println(firstKey, HEX);
  }

  // Iterate through all changed keys
  KeyboardChangeIterator it = keyboard.changes();
  while (it.hasNext()) {
    if (it.wasJustPressed()) {
      Serial.print("Key pressed: ");
      Serial.print(it.keyName());        // Human-readable name
      Serial.print(" (0x");
      Serial.print(it.keyValue(), HEX);
      Serial.print(") at row ");
      Serial.print(it.row());
      Serial.print(", col ");
      Serial.print(it.column());

      if (it.isShiftPressed()) {
        Serial.print(" (with Shift)");
      }
      Serial.println();
    }

    if (it.wasReleased()) {
      Serial.print("Key released: ");
      Serial.print(it.keyName());        // Human-readable name
      Serial.print(" (0x");
      Serial.print(it.keyValue(), HEX);
      Serial.println(")");
    }

    it.next();
  }

  delay(50);  // Small delay for debouncing
}
```
