# Keyboard Class

The `Keyboard` class provides high-level access to the TRS-80 Model I keyboard. It abstracts the 8x8 keyboard matrix mapped into memory and provides methods to check which keys are pressed, track changes, and retrieve key codes.

It uses the `Model1` object to read memory addresses corresponding to each keyboard row. The `KeyboardChangeIterator` helps enumerate which keys have changed since the last scan.

## Constructor

```cpp
Keyboard()
```

## Setters

### `void setLogger(ILogger &logger)`

Sets the logger used for errors and warnings.

_This is often useful for debugging as it tells what went wrong._

## Methods

### `bool isKeyPressed()`

Returns `true` if any key on the keyboard is currently pressed.

### `void update()`

Scans the keyboard and saves the state internally.

_Call this function once before starting to read state to make sure the last state is up to date._

### `KeyboardChangeIterator changes()`

Returns an iterator over keys whose state has changed since the last call to `change()`, `getFirstJustPressedKey()` or `update()`. Use this to detect presses and releases.

### `uint8_t getFirstJustPressedKey()`

Scans for the first key that has just been pressed since the last update and returns its key code. Returns 0 if none has changed.

_This recognizes only the first of all changes. If you want to get a full picture, use the `changes()` function._

## KeyboardChangeIterator

This iterator inspects the differences between two keyboard states.

### Methods

- `bool hasNext()` – Returns true if there is at least one more changed key.
- `void next()` – Moves to the next changed key.
- `bool wasJustPressed()` – Returns true if the key was previously released and is now pressed.
- `bool wasReleased()` – Returns true if the key was previously pressed and is now released.
- `uint8_t keyValue()` – Returns the key code from the lookup tables (ASCII or special code).
- `uint8_t row()` – Returns the keyboard matrix row.
- `uint8_t column()` – Returns the column.
- `bool isShiftPressed()` – Returns true if Shift was pressed during the scan.

## Special Key Codes

In addition to ASCII, some keys have unique codes:

- `KEY_ENTER`
- `KEY_CLEAR`
- `KEY_BREAK`
- `KEY_UP`, `KEY_DOWN`, `KEY_LEFT`, `KEY_RIGHT`
- `KEY_SPACE`
- `KEY_SHIFT`

## Notes

- Always call `model1.activateTestSignal()` before reading keyboard state.
- The lookup tables map matrix positions to characters or codes.
- Upper/lower case is determined by whether Shift is pressed.

## Example

```cpp
#include <Model1.h>
#include <Keyboard.h>

Model1* model1;
Keyboard* keyboard;

void setup() {
  Serial.begin(115200);

  model1 = new Model1();
  model1->begin();

  keyboard = new Keyboard(model1);

  model1->activateTestSignal();
}

void loop() {
  KeyboardChangeIterator it = keyboard.changes();
  while (it.hasNext()) {
    if (it.wasJustPressed()) {
      Serial.print("Key pressed: ");
      Serial.println(it.keyValue(), HEX);
    }
    it.next();
  }
}
```
