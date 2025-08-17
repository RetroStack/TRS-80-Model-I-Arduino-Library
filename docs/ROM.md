# ROM Class

The `ROM` class provides high-level utilities for interacting with TRS-80 Model I ROM memory regions.

## Table of Contents

- [Overview](#overview)
- [Constructor](#constructor)
- [Configuration Methods](#configuration-methods)
  - [setLogger](#void-setloggerilogger-logger)
- [ROM Information Methods](#rom-information-methods)
  - [getROMStartAddress](#uint16_t-getromstartaddressuint8_t-rom)
  - [getROMLength](#uint16_t-getromlengthuint8_t-rom)
  - [getChecksum](#uint32_t-getchecksumuint8_t-rom)
  - [identifyROM](#const-__flashstringhelper-identifyrom)
- [Content Methods](#content-methods)
  - [printROMContents](#void-printromcontentsuint8_t-rom-print_style-style--both-bool-relative--true-uint16_t-bytesperline--32)
- [Constants](#constants)
- [Notes](#notes)
- [Example](#example)

## Overview

The ROM class enables interaction with the TRS-80 Model I's ROM memory regions (A, B, C, D). It provides methods to query ROM information, calculate checksums, identify known ROM versions, and display ROM contents in various formats.

## Constructor

```cpp
ROM()
```

Creates a new ROM instance. No parameters required.

## Configuration Methods

### `void setLogger(ILogger &logger)`

Sets the logger used for errors and warnings.

**Parameters:**

- `logger`: Reference to an ILogger implementation

_This is often useful for debugging as it tells what went wrong._

## ROM Information Methods

### `uint16_t getROMStartAddress(uint8_t rom)`

Returns the start address of the selected ROM.

**Parameters:**

- `rom`: ROM index (0-3 for ROMs A-D)

**Returns:** Start address of the ROM region

### `uint16_t getROMLength(uint8_t rom)`

Returns the length in bytes of the selected ROM.

**Parameters:**

- `rom`: ROM index (0-3 for ROMs A-D)

**Returns:** Length of the ROM in bytes

### `uint32_t getChecksum(uint8_t rom)`

Calculates a 16-bit checksum over the ROM data.

**Parameters:**

- `rom`: ROM index (0-3 for ROMs A-D)

**Returns:** 32-bit checksum value

### `const __FlashStringHelper* identifyROM()`

Tries to identify the ROM based on known checksums. Returns a string with the ROM name or `nullptr` if unknown.

**Returns:**

- ROM identification string if known
- `nullptr` if ROM is unrecognized

## Content Methods

### `void printROMContents(uint8_t rom, PRINT_STYLE style = BOTH, bool relative = true, uint16_t bytesPerLine = 32)`

Logs a human-readable hexdump of the ROM contents in hex and ASCII.

**Parameters:**

- `rom`: ROM index (0-3 for ROMs A-D)
- `style`: Display style (ASCII, HEXADECIMAL, or BOTH)
- `relative`: If true, shows relative addresses; if false, shows absolute addresses
- `bytesPerLine`: Number of bytes to display per line (default: 32)

_You need to provide a logger for this function._

## Constants

**PRINT_STYLE enum values:**

- `ASCII`: Display only ASCII representation
- `HEXADECIMAL`: Display only hexadecimal values
- `BOTH`: Display both hex and ASCII (default)

## Notes

- Valid ROM indices are 0-3 (corresponding to ROMs A, B, C, D)
- Always call `Model1.activateTestSignal()` before using these methods
- Remember to call `Model1.deactivateTestSignal()` when finished
- ROM identification is based on a database of known ROM checksums
- The checksum calculation covers the entire ROM content
- Content printing requires a logger to be set via `setLogger()`

## Example

```cpp
#include <Model1.h>
#include <ROM.h>
#include <SerialLogger.h>

ROM rom;
SerialLogger logger;

void setup() {
  Serial.begin(115200);

  // Set up logger
  logger.begin();
  rom.setLogger(logger);

  Model1.begin();

  // Take control of the bus
  Model1.activateTestSignal();

  // Get information about ROM A (index 0)
  uint16_t startAddr = rom.getROMStartAddress(0);
  uint16_t length = rom.getROMLength(0);
  uint32_t checksum = rom.getChecksum(0);

  // Print ROM information
  Serial.print("ROM A Start Address: 0x");
  Serial.println(startAddr, HEX);
  Serial.print("ROM A Length: ");
  Serial.println(length);
  Serial.print("ROM A Checksum: 0x");
  Serial.println(checksum, HEX);

  // Try to identify the ROM
  const __FlashStringHelper* romName = rom.identifyROM();
  if (romName) {
    Serial.print("ROM Identified as: ");
    Serial.println(romName);
  } else {
    Serial.println("ROM not recognized");
  }

  // Print ROM contents (first 256 bytes in both hex and ASCII)
  Serial.println("\nROM A Contents:");
  rom.printROMContents(0, BOTH, true, 16);

  // Release control
  Model1.deactivateTestSignal();
}

void loop() {
}
```
