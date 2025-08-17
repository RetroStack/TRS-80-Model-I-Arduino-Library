# Video Class

The `Video` class provides comprehensive control over the TRS-80 Model I video RAM and display behavior. It handles text output, viewport clipping, cursor movement, scrolling, and character encoding (including support for systems with or without lowercase modifications). The class inherits from `Print`, making all `Print::print` and `Print::println` methods available.

## Table of Contents

- [Overview](#overview)
- [Constructor](#constructor)
- [Configuration Methods](#configuration-methods)
  - [setLogger](#void-setloggerilogger-logger)
  - [setViewPort](#void-setviewportviewport-viewport)
  - [setAutoScroll](#void-setautoscrollbool-autoscroll)
  - [setLowerCaseMod](#void-setlowercasemodbool-haslowercasemod)
- [Screen Clearing Methods](#screen-clearing-methods)
  - [cls](#void-cls)
  - [cls (character)](#void-clschar-character)
  - [cls (string)](#void-clschar-characters)
  - [cls (string with length)](#void-clschar-characters-uint16_t-length)
- [Cursor Management](#cursor-management)
  - [getX](#uint8_t-getx)
  - [setX](#void-setxuint8_t-x)
  - [getY](#uint8_t-gety)
  - [setY](#void-setyuint8_t-y)
  - [setXY](#void-setxyuint8_t-x-uint8_t-y)
- [Viewport Information](#viewport-information)
  - [getStartX](#uint8_t-getstartx)
  - [getStartY](#uint8_t-getstarty)
  - [getEndX](#uint8_t-getendx)
  - [getEndY](#uint8_t-getendy)
  - [getWidth](#uint8_t-getwidth)
  - [getHeight](#uint8_t-getheight)
  - [getSize](#uint16_t-getsize)
- [Address Calculation](#address-calculation)
  - [getRowAddress](#uint16_t-getrowaddressuint8_t-y)
  - [getColumnAddress](#uint16_t-getcolumnaddressuint16_t-rowaddress-uint8_t-x)
  - [getAddress](#uint16_t-getaddressuint8_t-x-uint8_t-y)
  - [getAbsoluteX](#uint8_t-getabsolutexuint8_t-x)
  - [getAbsoluteY](#uint8_t-getabsoluteyuint8_t-y)
- [Text Output Methods](#text-output-methods)
  - [print (character with raw flag)](#void-printconst-char-character-bool-raw)
  - [print (positioned string)](#void-printuint8_t-x-uint8_t-y-const-char-str)
  - [print (positioned string with length)](#void-printuint8_t-x-uint8_t-y-const-char-str-uint16_t-length)
  - [write methods](#write-methods)
- [Reading Methods](#reading-methods)
  - [read](#char-readuint8_t-x-uint8_t-y-uint16_t-length-bool-raw)
- [Scrolling Methods](#scrolling-methods)
  - [scroll](#void-scroll)
  - [scroll (rows)](#void-scrolluint8_t-rows)
- [Character Conversion](#character-conversion)
  - [convertLocalCharacterToModel1](#char-convertlocalcharactertomodel1char-character)
  - [convertModel1CharacterToLocal](#char-convertmodel1charactertolocalchar-character)
- [Inherited Print Methods](#inherited-print-methods)
- [Behavior Details](#behavior-details)
- [Notes](#notes)
- [Example](#example)

## Overview

The `Video` class provides comprehensive control over the TRS-80 Model I video RAM and display behavior. It handles text output, viewport clipping, cursor movement, scrolling, and character encoding (including support for systems with or without lowercase modifications). The class inherits from `Print`, making all `Print::print` and `Print::println` methods available.

## Constructor

```cpp
Video()
```

Creates a Video instance using the full 64x16 screen viewport. The cursor starts at position (0,0), auto-scroll is enabled, and lowercase modification is disabled by default.

## Configuration Methods

### `void setLogger(ILogger &logger)`

Sets the logger used for errors and warnings.

**Parameters:**

- `logger`: Reference to an ILogger implementation

_This is often useful for debugging as it tells what went wrong._

### `void setViewPort(ViewPort viewPort)`

Sets the active viewport for all video operations.

**Parameters:**

- `viewPort`: ViewPort structure defining the active screen region

### `void setAutoScroll(bool autoScroll)`

Enables or disables automatic scrolling when the cursor reaches the bottom of the viewport.

**Parameters:**

- `autoScroll`: true to enable auto-scrolling, false to wrap to top

### `void setLowerCaseMod(bool hasLowerCaseMod)`

Enables or disables lowercase character support.

**Parameters:**

- `hasLowerCaseMod`: true if the system has lowercase modification, false for uppercase-only

_Without lowercase mod enabled, all lowercase characters will be converted to uppercase._

## Screen Clearing Methods

### `void cls()`

Clears the viewport by filling it with space characters.

### `void cls(char character)`

Fills the viewport with the specified character.

**Parameters:**

- `character`: Character to fill the viewport with

### `void cls(char* characters)`

Repeats the given string to fill the viewport.

**Parameters:**

- `characters`: String to repeat across the viewport

### `void cls(char* characters, uint16_t length)`

Repeats the given string (with specified length) to fill the viewport.

**Parameters:**

- `characters`: String to repeat across the viewport
- `length`: Length of the string to use

## Cursor Management

### `uint8_t getX()`

Returns the current cursor column position (relative to viewport).

**Returns:** Current X coordinate (0-based)

### `void setX(uint8_t x)`

Sets the cursor column position.

**Parameters:**

- `x`: Column position (0-based, relative to viewport)

### `uint8_t getY()`

Returns the current cursor row position (relative to viewport).

**Returns:** Current Y coordinate (0-based)

### `void setY(uint8_t y)`

Sets the cursor row position.

**Parameters:**

- `y`: Row position (0-based, relative to viewport)

### `void setXY(uint8_t x, uint8_t y)`

Sets both cursor coordinates simultaneously.

**Parameters:**

- `x`: Column position (0-based, relative to viewport)
- `y`: Row position (0-based, relative to viewport)

## Viewport Information

### `uint8_t getStartX()`

Returns the absolute starting X coordinate of the viewport.

**Returns:** Absolute starting column

### `uint8_t getStartY()`

Returns the absolute starting Y coordinate of the viewport.

**Returns:** Absolute starting row

### `uint8_t getEndX()`

Returns the absolute ending X coordinate of the viewport.

**Returns:** Absolute ending column

### `uint8_t getEndY()`

Returns the absolute ending Y coordinate of the viewport.

**Returns:** Absolute ending row

### `uint8_t getWidth()`

Returns the viewport width in characters.

**Returns:** Width in characters

### `uint8_t getHeight()`

Returns the viewport height in characters.

**Returns:** Height in characters

### `uint16_t getSize()`

Returns the total number of characters in the viewport.

**Returns:** Total character capacity (width x height)

## Address Calculation

### `uint16_t getRowAddress(uint8_t y)`

Calculates the memory address for a specific row.

**Parameters:**

- `y`: Row position (0-based)

**Returns:** Memory address of the row start

### `uint16_t getColumnAddress(uint16_t rowAddress, uint8_t x)`

Calculates the memory address for a specific column within a row.

**Parameters:**

- `rowAddress`: Starting address of the row
- `x`: Column position (0-based)

**Returns:** Memory address of the specific position

### `uint16_t getAddress(uint8_t x, uint8_t y)`

Calculates the memory address for specific coordinates.

**Parameters:**

- `x`: Column position (0-based)
- `y`: Row position (0-based)

**Returns:** Memory address of the position

### `uint8_t getAbsoluteX(uint8_t x)`

Converts viewport-relative X coordinate to absolute screen coordinate.

**Parameters:**

- `x`: Relative X coordinate

**Returns:** Absolute X coordinate

### `uint8_t getAbsoluteY(uint8_t y)`

Converts viewport-relative Y coordinate to absolute screen coordinate.

**Parameters:**

- `y`: Relative Y coordinate

**Returns:** Absolute Y coordinate

## Text Output Methods

### `void print(const char character, bool raw)`

Prints a single character with optional raw output.

**Parameters:**

- `character`: Character to print
- `raw`: If true, prints as-is; if false, applies character conversion

### `void print(uint8_t x, uint8_t y, const char* str)`

Prints a string at the specified coordinates.

**Parameters:**

- `x`: Column position (0-based, relative to viewport)
- `y`: Row position (0-based, relative to viewport)
- `str`: String to print

### `void print(uint8_t x, uint8_t y, const char* str, uint16_t length)`

Prints a string with specified length at the specified coordinates.

**Parameters:**

- `x`: Column position (0-based, relative to viewport)
- `y`: Row position (0-based, relative to viewport)
- `str`: String to print
- `length`: Number of characters to print

### Write Methods

```cpp
size_t write(uint8_t ch) override
size_t write(const uint8_t *buffer, size_t size) override
```

Implements the Print interface for standard Arduino print functionality.

## Reading Methods

### `char* read(uint8_t x, uint8_t y, uint16_t length, bool raw)`

Reads characters from video RAM starting at the specified position.

**Parameters:**

- `x`: Starting column (0-based, relative to viewport)
- `y`: Starting row (0-based, relative to viewport)
- `length`: Number of characters to read
- `raw`: If true, returns raw values; if false, applies character conversion

**Returns:** Pointer to allocated character buffer

_Make sure to release the buffer when finished._

## Scrolling Methods

### `void scroll()`

Scrolls the viewport content up by one row, clearing the bottom row.

### `void scroll(uint8_t rows)`

Scrolls the viewport content up by the specified number of rows.

**Parameters:**

- `rows`: Number of rows to scroll (default: 1)

## Character Conversion

### `char convertLocalCharacterToModel1(char character)`

Converts ASCII character to TRS-80 character encoding.

**Parameters:**

- `character`: ASCII character to convert

**Returns:** TRS-80 encoded character

### `char convertModel1CharacterToLocal(char character)`

Converts TRS-80 character encoding to ASCII.

**Parameters:**

- `character`: TRS-80 character to convert

**Returns:** ASCII character

## Inherited Print Methods

All standard Arduino Print methods are available:

```cpp
print(value)          // Various data types
println(value)        // With newline
printf(format, ...)   // Formatted output (if supported)
```

_By default, all Print methods use `convertLocalCharacterToModel1` automatically._

## Behavior Details

- If printing moves past the right edge, the cursor wraps to the next line.
- If the cursor moves past the bottom edge, `autoScroll` determines whether scrolling or wrapping occurs (starting from the top again).
- All writes are relative to the viewport.
- Lowercase conversion affects whether letters appear as uppercase.

## Notes

- Always call `activateTestSignal()` on the `Model1` instance before using these methods.
- `ViewPort` coordinates and sizes must be within the 64x16 bounds.

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

    video.println("Hello World!");
    video.print("Second line");

    video.setXY(20, 12);
    video.print("Somewhere in the middle");

    video.scroll();
    video.print("Scrolled up.");

    delay(2000);
}
```
