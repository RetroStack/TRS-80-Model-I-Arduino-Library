# Video Class

The `Video` class provides comprehensive control over the TRS-80 Model I video RAM and display behavior. It handles text output, viewport clipping, cursor movement, scrolling, and character encoding (including support for systems with or without lowercase modifications).
The class inherits the `Print` behavior, and therefore all `Print::print` and `Print::println` methods are available.

## Constructor

- `Video()`: Creates an instance using the full 64×16 screen.

## Setters

### `void setLogger(ILogger &logger)`

Sets the logger used for errors and warnings.

_This is often useful for debugging as it tells what went wrong._

## Methods

### Clearing the Screen

- `cls()`: Clears the viewport by filling it with space characters.
- `cls(char character)`: Fills the viewport with the given character.
- `cls(char* characters)`: Repeats the given string to fill the viewport.

### Printing Text

All the methods that `Print::print` and `Print::println` provide are available in additon to:

- `print(const char character, bool raw)`: Prints a single character in raw format (as given) or automatically translates it with `convertLocalCharacterToModel1`.
- `print(const char* str, uint8_t x, uint8_t y)`: Prints string at specified coordinates.
- `print(const char* str, uint16_t length, uint8_t x, uint8_t y)`: Prints string at specified coordinates.

_By default, all methods use `convertLocalCharacterToModel1` automatically._

There is also the following method that changes the print behavior:

- `setAutoScroll(bool enabled)`: Enables or disables automatic scrolling when reaching the bottom.

### Reading Video Memory

- `read(uint8_t x, uint8_t y, uint16_t length, bool raw)`: Reads characters from video RAM starting at `(x, y)`.

_Make sure to release the buffer._

### Scrolling

- `scroll(uint8_t rows=1)`: Scrolls content up by the specified number of rows, clearing the newly exposed rows.

### Cursor Position

- `setX(uint8_t x)`: Sets cursor column.
- `setY(uint8_t y)`: Sets cursor row.
- `setXY(uint8_t x, uint8_t y)`: Sets both cursor coordinates.
- `getX()`: Returns cursor X.
- `getY()`: Returns cursor Y.

### Viewport Information

- `getStartX()`: Returns absolute starting X.
- `getStartY()`: Returns absolute starting Y.
- `getEndX()`: Returns absolute end X.
- `getEndY()`: Returns absolute end Y.
- `getWidth()`: Returns viewport width.
- `getHeight()`: Returns viewport height.
- `getSize()`: Returns the total number of characters in viewport.

### Character Conversion

- `convertLocalCharacterToModel1(char c)`: Converts ASCII to TRS-80 encoding.
- `convertModel1CharacterToLocal(char c)`: Converts TRS-80 encoding to ASCII.
- `setLowerCaseMod(bool enabled)`: Enables lowercase support.

_Without lowercase mod enabled, all lowercase characters will be converted to uppercase._

## Behavior Details

- If printing moves past the right edge, the cursor wraps to the next line.
- If the cursor moves past the bottom edge, `autoScroll` determines whether scrolling or wrapping occurs (starting from the top again).
- All writes are relative to the viewport.
- Lowercase conversion affects whether letters appear as uppercase.

## Notes

- Always call `activateTestSignal()` on the `Model1` instance before using these methods.
- `ViewPort` coordinates and sizes must be within the 64×16 bounds.

## Example

```cpp
#include <Model1.h>
#include <Video.h>

Video video;

void setup() {
  Serial.begin(115200);

  Model1.begin();
  Model1.activateTestSignal();
}

void loop() {
    video.cls();

    video.printLn("Hello World!");
    video.print("Second line");

    video.setXY(20, 12);
    video.print("Somewhere in the middle");

    video.scroll();
    video.print("Scrolled up.");

    delay(2000);
}
```
