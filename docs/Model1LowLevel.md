# Model1LowLevel Class Documentation

## Overview

The `Model1LowLevel` class provides direct, low-level access to all TRS-80 Model 1 hardware control signals and buses. This class bypasses the safety mechanisms of the main `Model1` class and should be used with extreme caution.

**⚠️ WARNING:** This class provides direct hardware access that can potentially damage your Arduino or connected hardware if used incorrectly. Only use if you fully understand the TRS-80 Model 1 hardware architecture.

## Features

- **Direct Pin Configuration**: Set pins as INPUT or OUTPUT
- **Pin Configuration Reading**: Check current INPUT/OUTPUT state
- **Signal Control**: Set pins HIGH or LOW
- **Signal Reading**: Read current pin states
- **Bus Control**: Complete address and data bus manipulation
- **High Performance**: Static inline functions with zero call overhead
- **Macro Integration**: Uses optimized port macros for maximum speed

## Function Categories

### 1. Configuration Functions

Set pin direction (INPUT/OUTPUT mode).

```cpp
// Configure individual pins
Model1LowLevel::configRAS(OUTPUT);  // Set RAS as output
Model1LowLevel::configCAS(INPUT);   // Set CAS as input

// Configure buses
Model1LowLevel::configAddressBus(0xFFFF);  // All address pins as output
Model1LowLevel::configDataBus(0xFF);       // All data pins as output
```

**Available Functions:**

- `configRAS(uint8_t outputMode)`
- `configCAS(uint8_t outputMode)`
- `configMUX(uint8_t outputMode)`
- `configRD(uint8_t outputMode)`
- `configWR(uint8_t outputMode)`
- `configIN(uint8_t outputMode)`
- `configOUT(uint8_t outputMode)`
- `configINT(uint8_t outputMode)`
- `configTEST(uint8_t outputMode)`
- `configWAIT(uint8_t outputMode)`
- `configSYS_RES(uint8_t outputMode)`
- `configINT_ACK(uint8_t outputMode)`
- `configAddressBus(uint16_t outputMode)`
- `configDataBus(uint8_t outputMode)`

### 2. Configuration Readers

Read current pin direction configuration.

```cpp
// Check if pin is configured as output
uint8_t rasConfig = Model1LowLevel::configReadRAS();
if (rasConfig == OUTPUT) {
    // RAS is configured as output
}

// Check bus configuration
uint16_t addrConfig = Model1LowLevel::configReadAddressBus();
uint8_t dataConfig = Model1LowLevel::configReadDataBus();
```

**Available Functions:**

- `configReadRAS()` → `uint8_t`
- `configReadCAS()` → `uint8_t`
- `configReadMUX()` → `uint8_t`
- `configReadRD()` → `uint8_t`
- `configReadWR()` → `uint8_t`
- `configReadIN()` → `uint8_t`
- `configReadOUT()` → `uint8_t`
- `configReadINT()` → `uint8_t`
- `configReadTEST()` → `uint8_t`
- `configReadWAIT()` → `uint8_t`
- `configReadSYS_RES()` → `uint8_t`
- `configReadINT_ACK()` → `uint8_t`
- `configReadAddressBus()` → `uint16_t`
- `configReadDataBus()` → `uint8_t`

### 3. Signal Setters

Set pin states (HIGH/LOW). Pin must be configured as OUTPUT first.

```cpp
// Configure and set individual signals
Model1LowLevel::configRAS(OUTPUT);
Model1LowLevel::setRAS(HIGH);  // Set RAS high

Model1LowLevel::configWR(OUTPUT);
Model1LowLevel::setWR(LOW);    // Set WR low

// Set bus values
Model1LowLevel::setAddressBus(0x1234);  // Set address to 0x1234
Model1LowLevel::setDataBus(0xFF);       // Set data to 0xFF
```

**Available Functions:**

- `setRAS(uint8_t value)`
- `setCAS(uint8_t value)`
- `setMUX(uint8_t value)`
- `setRD(uint8_t value)`
- `setWR(uint8_t value)`
- `setIN(uint8_t value)`
- `setOUT(uint8_t value)`
- `setINT(uint8_t value)`
- `setTEST(uint8_t value)`
- `setWAIT(uint8_t value)`
- `setSYS_RES(uint8_t value)`
- `setINT_ACK(uint8_t value)`
- `setAddressBus(uint16_t address)`
- `setDataBus(uint8_t data)`

### 4. Signal Readers

Read current pin states.

```cpp
// Read individual signals
uint8_t rasState = Model1LowLevel::readRAS();
if (rasState == HIGH) {
    // RAS is currently high
}

// Read bus values
uint16_t address = Model1LowLevel::readAddressBus();
uint8_t data = Model1LowLevel::readDataBus();
```

**Available Functions:**

- `readRAS()` → `uint8_t`
- `readCAS()` → `uint8_t`
- `readMUX()` → `uint8_t`
- `readRD()` → `uint8_t`
- `readWR()` → `uint8_t`
- `readIN()` → `uint8_t`
- `readOUT()` → `uint8_t`
- `readINT()` → `uint8_t`
- `readTEST()` → `uint8_t`
- `readWAIT()` → `uint8_t`
- `readSYS_RES()` → `uint8_t`
- `readINT_ACK()` → `uint8_t`
- `readAddressBus()` → `uint16_t`
- `readDataBus()` → `uint8_t`

## Usage Examples

### Basic Signal Control

```cpp
#include "Model1LowLevel.h"

void setup() {
    // Configure RAS as output and set it high
    Model1LowLevel::configRAS(OUTPUT);
    Model1LowLevel::setRAS(HIGH);

    // Configure WR as input to read external signal
    Model1LowLevel::configWR(INPUT);

    // Read the current state
    uint8_t wrState = Model1LowLevel::readWR();
    if (wrState == HIGH) {
        Serial.println("WR signal is high");
    }
}
```

### Memory Address/Data Bus Control

```cpp
void writeToMemory(uint16_t address, uint8_t data) {
    // Configure buses as outputs
    Model1LowLevel::configAddressBus(0xFFFF);  // All address pins output
    Model1LowLevel::configDataBus(0xFF);       // All data pins output

    // Set address and data
    Model1LowLevel::setAddressBus(address);
    Model1LowLevel::setDataBus(data);

    // Control write signal
    Model1LowLevel::configWR(OUTPUT);
    Model1LowLevel::setWR(LOW);   // Assert write
    delayMicroseconds(1);         // Hold time
    Model1LowLevel::setWR(HIGH);  // Deassert write
}

uint8_t readFromMemory(uint16_t address) {
    // Set address
    Model1LowLevel::configAddressBus(0xFFFF);
    Model1LowLevel::setAddressBus(address);

    // Configure data bus as input
    Model1LowLevel::configDataBus(0x00);

    // Control read signal
    Model1LowLevel::configRD(OUTPUT);
    Model1LowLevel::setRD(LOW);   // Assert read
    delayMicroseconds(1);         // Access time

    // Read data
    uint8_t data = Model1LowLevel::readDataBus();

    Model1LowLevel::setRD(HIGH);  // Deassert read

    return data;
}
```

### Configuration Checking

```cpp
void printPinConfigurations() {
    Serial.println("Pin Configuration Status:");

    // Check individual pin configurations
    Serial.print("RAS: ");
    Serial.println(Model1LowLevel::configReadRAS() == OUTPUT ? "OUTPUT" : "INPUT");

    Serial.print("CAS: ");
    Serial.println(Model1LowLevel::configReadCAS() == OUTPUT ? "OUTPUT" : "INPUT");

    Serial.print("WR: ");
    Serial.println(Model1LowLevel::configReadWR() == OUTPUT ? "OUTPUT" : "INPUT");

    // Check bus configurations
    uint16_t addrConfig = Model1LowLevel::configReadAddressBus();
    Serial.print("Address Bus Config: 0x");
    Serial.println(addrConfig, HEX);

    uint8_t dataConfig = Model1LowLevel::configReadDataBus();
    Serial.print("Data Bus Config: 0x");
    Serial.println(dataConfig, HEX);
}
```

## Access Through Direct Include

You can access the Model1LowLevel functionality by including it directly:

```cpp
#include <Model1LowLevel.h>

void setup() {
    // Access low-level functions directly
    Model1LowLevel::configRAS(OUTPUT);
    Model1LowLevel::setRAS(HIGH);

    uint8_t rasState = Model1LowLevel::readRAS();
}
```

## Performance Notes

- All functions are `static inline` for maximum performance
- Zero function call overhead when optimized
- Direct port manipulation through macros
- Suitable for time-critical applications
