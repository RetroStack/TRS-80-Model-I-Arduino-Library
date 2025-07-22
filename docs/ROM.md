# ROM Class

The `ROM` class provides high-level utilities for interacting with TRS-80 Model I ROM memory regions.

## Table of Contents

- [Constructor](#constructor)
- [Methods](#methods)

  - [getROMStartAddress](#uint16_t-getromstartaddressuint8_t-rom)
  - [getROMLength](#uint16_t-getromlengthuint8_t-rom)
  - [getChecksum](#uint32_t-getchecksumuint8_t-rom)
  - [identifyROM](#const-__flashstringhelper-identifyrom)
  - [printROMContents](#void-printromcontentsuint8_t-rom)

- [Notes](#notes)
- [Example](#example)

## Constructor

```cpp
ROM()
```

## Setters

### `void setLogger(ILogger &logger)`

Sets the logger used for errors and warnings.

_This is often useful for debugging as it tells what went wrong._

## Methods

### `uint16_t getROMStartAddress(uint8_t rom)`

Returns the start address of the selected ROM.

### `uint16_t getROMLength(uint8_t rom)`

Returns the length in bytes of the selected ROM.

### `uint32_t getChecksum(uint8_t rom)`

Calculates a 16-bit checksum over the ROM data.

### `const __FlashStringHelper* identifyROM()`

Tries to identify the ROM based on known checksums. Returns a string with the ROM name or `nullptr` if unknown.

### `void printROMContents(uint8_t rom)`

Logs a human-readable hexdump of the ROM contents in hex and ASCII.

_You need to provide a logger for this function._

## Notes

- Valid ROM indices are 0–3.
- Always call `model1.activateTestSignal()` before using these methods.

## Example

```cpp
#include <Model1.h>
#include <ROM.h>

ROM rom;

void setup() {
  Serial.begin(115200);

  Model1.begin();

  // Take control of the bus
  Model1.activateTestSignal();

  // Get checksum of ROM A
  uint32_t checksum = rom.getChecksum(0);

  // Print checksum
  Serial.print("Checksum of ROM A: ");
  Serial.println(checksum, HEX);

  // Release control
  Model1.deactivateTestSignal();
}

void loop() {
}
```
