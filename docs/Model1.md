# Model1 Class

The `Model1` class provides the main interface for interacting with the TRS-80 Model I system bus. It enables:

- Taking control of the bus with the `*TEST` signal.
- Reading and writing memory and I/O.
- Managing DRAM refresh.
- Triggering interrupts.
- Controlling the WAIT signal.
- Logging system state.

> ⚠️ **Important:** Only one `Model1` instance should be active at a time, as the class manages exclusive control.

## Table of Contents

- [Constructor](#constructor)
- [Initialization](#initialization)
- [Loggers](#loggers)
- [Memory Refresh](#memory-refresh)
- [Address Space Checks](#address-space-checks)
- [Memory Access](#memory-access)
- [I/O Access](#io-access)
- [System Updates](#system-updates)
- [System Signals](#system-signals)
- [Interrupt Control](#interrupt-control)
- [TEST Signal Control](#test-signal-control)
- [WAIT Signal Control](#wait-signal-control)
- [State Reporting](#state-reporting)
- [Memory Display](#memory-display)
- [Version Info](#version-info)
- [Example Workflow](#example-workflow)
- [Notes](#notes)

## Constructor

- **`Model1()`** - Creates a new Model1 instance

## Initialization

- **`void begin(int refreshTimer = -1)`** - Initializes hardware pins and optional memory refresh
- **`void end()`** - Deinitializes hardware (called automatically in destructor)

## Initialization

- **`void begin(int refreshTimer = -1)`** - Initializes hardware pins and optional memory refresh
- **`void end()`** - Deinitializes hardware (called automatically in destructor)

The `begin()` method initializes hardware pins and optional memory refresh:

- `refreshTimer`:
  - `-1`: No DRAM refresh.
  - `1`: Timer 1 refresh.
  - `2`: Timer 2 refresh.

If you setup a refresh timer here, make sure to provide the timer interrupt function:

```cpp
// For Timer 2 use TIMER2_COMPA_vect, otherwise TIMER1_COMPA_vect
ISR(TIMER2_COMPA_vect)
{
    // Trigger a refresh to happen
    Model1.nextUpdate();
}
```

You most likely never will need `end()`.

## Loggers

- **`void setLogger(ILogger &logger)`** - Sets the logger used for errors and warnings

This is often useful for debugging as it tells what went wrong.

## Memory Refresh

- **`void activateMemoryRefresh()`** - Enable DRAM memory refresh cycles
- **`void deactivateMemoryRefresh()`** - Disable DRAM memory refresh cycles

## Address Space Checks

Convenience methods for determining memory regions:

- **`bool isROMAddress(uint16_t address)`** - Check if address is in ROM space
- **`bool isUnusedAddress(uint16_t address)`** - Check if address is in unused space
- **`bool isMemoryMappedIOAddress(uint16_t address)`** - Check if address is memory-mapped I/O
- **`bool isKeyboardAddress(uint16_t address)`** - Check if address is keyboard I/O
- **`bool isVideoAddress(uint16_t address)`** - Check if address is video RAM
- **`bool isSystemAddress(uint16_t address)`** - Check if address is system space
- **`bool isLowerMemoryAddress(uint16_t address)`** - Check if address is in lower memory
- **`bool isHigherMemoryAddress(uint16_t address)`** - Check if address is in higher memory

Example:

```cpp
if (Model1.isVideoAddress(0x3C00)) {
  // Handle video RAM
}
```

## Memory Access

**Note:** These require the bus to be active [`activateTestSignal`](#test-signal-control).

### Read Memory

- **`uint8_t readMemory(uint16_t address)`** - Read single byte from memory
- **`uint8_t* readMemory(uint16_t address, uint16_t length)`** - Read block from memory (heap-allocated buffer)

_(Remember to `delete[]` the buffer returned by block read.)_

### Write Memory

- **`void writeMemory(uint16_t address, uint8_t data)`** - Write single byte to memory
- **`void writeMemory(uint16_t address, uint8_t* data, uint16_t length)`** - Write block to memory
- **`void writeMemory(uint16_t address, uint8_t* data, uint16_t length, uint16_t offset)`** - Write block with offset
- **`void copyMemory(uint16_t src_address, uint16_t dst_address, uint16_t length)`** - Copy memory between addresses
- **`void fillMemory(uint8_t fill_data, uint16_t address, uint16_t length)`** - Fill memory with a byte
- **`void fillMemory(uint8_t* fill_data, uint16_t length, uint16_t start_address, uint16_t end_address)`** - Fill memory repeating a byte array

## I/O Access

**Note:** These also require the bus to be [active](#test-signal-control).

- **`uint8_t readIO(uint8_t address)`** - Read from I/O port
- **`void writeIO(uint8_t address, uint8_t data)`** - Write to I/O port

## System Updates

- **`void nextUpdate()`** - Trigger the next system update cycle (used in refresh timer interrupts)

## System Signals

- **`bool readSystemResetSignal()`** - Returns true if the system reset signal (SYS_RES) is active
- **`bool readInterruptAcknowledgeSignal()`** - Returns true if the CPU has acknowledged an interrupt request

These functions can be used for monitoring the CPU state without taking over the bus.

## Interrupt Control

- **`bool triggerInterrupt(uint8_t interrupt, uint16_t timeout = 1000)`** - Trigger an interrupt vector
- **`void activateInterruptRequestSignal()`** - Manually activate the interrupt request signal
- **`void deactivateInterruptRequestSignal()`** - Manually deactivate the interrupt request signal

The `triggerInterrupt()` method returns `true` if acknowledged within timeout. The `interrupt` parameter is the identifier provided to the CPU for resolution on the interrupt vector.

_When using manual control, you have to manage the interrupt identifier yourself._

## TEST Signal Control

Controls whether the Arduino takes control of the bus.

- **`void activateTestSignal()`** - Take control of the bus and make it mutable
- **`void deactivateTestSignal()`** - Release control back to the CPU
- **`bool hasActiveTestSignal()`** - Check if TEST signal is currently active

## WAIT Signal Control

Controls whether the CPU is held in a wait state.

- **`void activateWaitSignal()`** - Put CPU in wait state
- **`void deactivateWaitSignal()`** - Release CPU from wait state

## State Reporting

- **`char* getState()`** - Get current state string (heap-allocated)
- **`void logState()`** - Log the current state using the configured logger

## Memory Display

- **`void printMemoryContents(uint16_t start, uint16_t length, PRINT_STYLE style = BOTH, bool relative = false, uint16_t bytesPerLine = 32)`** - Print memory contents to Serial
- **`void printMemoryContents(Print &output, uint16_t start, uint16_t length, PRINT_STYLE style = BOTH, bool relative = false, uint16_t bytesPerLine = 32)`** - Print memory contents to specified output stream

PRINT_STYLE options:

- `ASCII` - ASCII representation only
- `HEXADECIMAL` - Hexadecimal values only
- `BOTH` - Both hex and ASCII (default)

## Version Info

- **`uint8_t getVersionMajor()`** - Get major version number
- **`uint8_t getVersionMinor()`** - Get minor version number
- **`uint8_t getVersionRevision()`** - Get revision number
- **`char* getVersion()`** - Get version string (heap-allocated)

## Example Workflow

```cpp
#include <Model1.h>

void setup() {
  Serial.begin(115200);
  Model1.begin();

  // Take control of the bus
  Model1.activateTestSignal();

  // Read the first 256 bytes of ROM
  for (uint16_t i = 0x0000; i < 0x0100; i++) {
    uint8_t val = Model1.readMemory(i);
    Serial.print("0x");
    Serial.print(i, HEX);
    Serial.print(": ");
    Serial.println(val, HEX);
  }

  // Release control
  Model1.deactivateTestSignal();
}

void loop() {
}
```

## Notes

- Always call `activateTestSignal()` before memory or I/O operations.
- Remember to `delete[]` buffers returned by `readMemory()` block operations.
- Use proper timer interrupt handlers when enabling memory refresh.
