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
- [Address Space Checks](#address-space-checks)
- [Memory Access](#memory-access)
- [I/O Access](#io-access)
- [Interrupt Control](#interrupt-control)
- [TEST Signal Control](#test-signal-control)
- [WAIT Signal Control](#wait-signal-control)
- [State Reporting](#state-reporting)
- [Version Info](#version-info)
- [Example Workflow](#example-workflow)

## Constructor

```cpp
Model1()
```

Creates a new `Model1` instance.

## Initialization

### `begin(int refreshTimer = -1)`

Initializes hardware pins and optional memory refresh:

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
    model1->refreshNextMemoryRow();
}
```

### `end()`

Deinitializes hardware. Called automatically in the destructor.

You most likely never will need this.

## Loggers

### `void setLogger(ILogger &logger)`

Sets the logger used for errors and warnings.

_This is often useful for debugging as it tells what went wrong._

## Address Space Checks

Convenience methods for determining memory regions:

```cpp
bool isROMAddress(uint16_t address)
bool isUnusedAddress(uint16_t address)
bool isMemoryMappedIOAddress(uint16_t address)
bool isKeyboardAddress(uint16_t address)
bool isVideoAddress(uint16_t address)
bool isSystemAddress(uint16_t address)
bool isLowerMemoryAddress(uint16_t address)
bool isHigherMemoryAddress(uint16_t address)
```

Example:

```cpp
if (model1.isVideoAddress(0x3C00)) {
  // Handle video RAM
}
```

## Memory Access

**Note:** These require the bus to be active [`activateTestSignal`]](#test-signal-control).

### Read Memory

- Single byte:

  ```cpp
  uint8_t readMemory(uint16_t address)
  ```

- Block read (heap-allocated buffer):

  ```cpp
  uint8_t* readMemory(uint16_t address, uint16_t length)
  ```

  _(Remember to `delete[]` the buffer.)_

### Write Memory

- Single byte:

  ```cpp
  void writeMemory(uint16_t address, uint8_t data)
  ```

- Block write:

  ```cpp
  void writeMemory(uint16_t address, uint8_t* data, uint16_t length)
  ```

- Block write with offset:

  ```cpp
  void writeMemory(uint16_t address, uint8_t* data, uint16_t length, uint16_t offset)
  ```

- Copy memory between addresses:

  ```cpp
  void copyMemory(uint16_t src_address, uint16_t dst_address, uint16_t length)
  ```

- Fill memory with a byte:

  ```cpp
  void fillMemory(uint8_t fill_data, uint16_t address, uint16_t length)
  ```

- Fill memory repeating a byte array:

  ```cpp
  void fillMemory(uint8_t* fill_data, uint16_t length, uint16_t start_address, uint16_t end_address)
  ```

## I/O Access

**Note:** These also require the bus to be [active](#test-signal-control).

- Read from I/O:

  ```cpp
  uint8_t readIO(uint8_t address)
  ```

- Write to I/O:

  ```cpp
  void writeIO(uint8_t address, uint8_t data)
  ```

## System Signals

- `bool readSystemResetSignal()`

  Returns `true` if the system reset signal (`SYS_RES`) is active.

- `bool readInterruptAcknowledgeSignal()`

  Returns `true` if the CPU has acknowledged an interrupt request (see below).

These functions can be used for monitoring the CPU state without taking over the bus.

## Interrupt Control

- Trigger an interrupt vector:

  ```cpp
  bool triggerInterrupt(uint8_t interrupt, uint16_t timeout = 1000)
  ```

  Returns `true` if acknowledged within timeout.

  `interrupt` is the identifier provided to the CPU for resolution on the interrupt vector.

- Manually control the interrupt request signal:

  ```cpp
  void activateInterruptRequestSignal()
  void deactivateInterruptRequestSignal()
  ```

  _You have to manage the interrupt identitier yourself._

## TEST Signal Control

Controls whether the Arduino takes control of the bus.

- Activate TEST:

  ```cpp
  void activateTestSignal()
  ```

  Takes control and makes the bus mutable.

- Deactivate TEST:

  ```cpp
  void deactivateTestSignal()
  ```

  Releases control back to the CPU.

## WAIT Signal Control

Controls whether the CPU is held in a wait state.

- Activate WAIT:

  ```cpp
  void activateWaitSignal()
  ```

- Deactivate WAIT:

  ```cpp
  void deactivateWaitSignal()
  ```

## State Reporting

- Get current state string (heap-allocated):

  ```cpp
  char* getState()
  ```

- Log the state:

  ```cpp
  void logState()
  ```

## Version Info

- Retrieve version numbers:

  ```cpp
  uint8_t getVersionMajor()
  uint8_t getVersionMinor()
  uint8_t getVersionRevision()
  ```

- Retrieve version string:

  ```cpp
  char* getVersion()
  ```

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
