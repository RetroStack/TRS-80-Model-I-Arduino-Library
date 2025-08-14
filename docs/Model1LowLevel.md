# Model1LowLevel Class

## Overview

The `Model1LowLevel` class provides direct, low-level access to all TRS-80 Model 1 hardware control signals and buses. This class bypasses the safety mechanisms of the main `Model1` class and should be used with extreme caution.

**⚠️ WARNING:** This class provides direct hardware access that can potentially damage your Arduino or connected hardware if used incorrectly. Only use if you fully understand the TRS-80 Model 1 hardware architecture.

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Configuration Functions](#configuration-functions)
- [Configuration Readers](#configuration-readers)
- [Signal Writers](#signal-writers)
- [Signal Readers](#signal-readers)
- [Bus Control Functions](#bus-control-functions)
- [Usage Examples](#usage-examples)
- [Performance Notes](#performance-notes)

## Features

- **Direct Pin Configuration**: Set pins as INPUT or OUTPUT
- **Pin Configuration Reading**: Check current INPUT/OUTPUT state
- **Signal Control**: Set pins HIGH or LOW
- **Signal Reading**: Read current pin states
- **Bus Control**: Complete address and data bus manipulation
- **High Performance**: Static inline functions with zero call overhead
- **Macro Integration**: Uses optimized port macros for maximum speed

## Configuration Functions

Set pin direction (INPUT/OUTPUT mode).

- **`static void configWriteRAS(uint8_t outputMode)`** - Configure RAS (Row Address Strobe) pin direction
- **`static void configWriteCAS(uint8_t outputMode)`** - Configure CAS (Column Address Strobe) pin direction
- **`static void configWriteMUX(uint8_t outputMode)`** - Configure MUX (Address Multiplexer) pin direction
- **`static void configWriteRD(uint8_t outputMode)`** - Configure RD (Read signal) pin direction
- **`static void configWriteWR(uint8_t outputMode)`** - Configure WR (Write signal) pin direction
- **`static void configWriteIN(uint8_t outputMode)`** - Configure IN (Input signal) pin direction
- **`static void configWriteOUT(uint8_t outputMode)`** - Configure OUT (Output signal) pin direction
- **`static void configWriteINT(uint8_t outputMode)`** - Configure INT (Interrupt signal) pin direction
- **`static void configWriteTEST(uint8_t outputMode)`** - Configure TEST (Test signal) pin direction
- **`static void configWriteWAIT(uint8_t outputMode)`** - Configure WAIT (Wait signal) pin direction
- **`static void configWriteSYS_RES(uint8_t outputMode)`** - Configure SYS_RES (System Reset) pin direction
- **`static void configWriteINT_ACK(uint8_t outputMode)`** - Configure INT_ACK (Interrupt Acknowledge) pin direction
- **`static void configWriteAddressBus(uint16_t outputMode)`** - Configure all address bus pins
- **`static void configWriteDataBus(uint8_t outputMode)`** - Configure all data bus pins

**Example:**

```cpp
// Configure individual pins
Model1LowLevel::configWriteRAS(OUTPUT);  // Set RAS as output
Model1LowLevel::configWriteCAS(INPUT);   // Set CAS as input

// Configure buses
Model1LowLevel::configWriteAddressBus(0xFFFF);  // All address pins as output
Model1LowLevel::configWriteDataBus(0xFF);       // All data pins as output
```

## Configuration Readers

Read current pin direction configuration.

- **`static uint8_t configReadRAS()`** - Read RAS pin configuration state
- **`static uint8_t configReadCAS()`** - Read CAS pin configuration state
- **`static uint8_t configReadMUX()`** - Read MUX pin configuration state
- **`static uint8_t configReadRD()`** - Read RD pin configuration state
- **`static uint8_t configReadWR()`** - Read WR pin configuration state
- **`static uint8_t configReadIN()`** - Read IN pin configuration state
- **`static uint8_t configReadOUT()`** - Read OUT pin configuration state
- **`static uint8_t configReadINT()`** - Read INT pin configuration state
- **`static uint8_t configReadTEST()`** - Read TEST pin configuration state
- **`static uint8_t configReadWAIT()`** - Read WAIT pin configuration state
- **`static uint8_t configReadSYS_RES()`** - Read SYS_RES pin configuration state
- **`static uint8_t configReadINT_ACK()`** - Read INT_ACK pin configuration state
- **`static uint16_t configReadAddressBus()`** - Read address bus configuration state
- **`static uint8_t configReadDataBus()`** - Read data bus configuration state

**Example:**

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

## Signal Writers

Write pin states (HIGH/LOW). Pin must be configured as OUTPUT first.

- **`static void writeRAS(uint8_t value)`** - Write RAS (Row Address Strobe) signal state
- **`static void writeCAS(uint8_t value)`** - Write CAS (Column Address Strobe) signal state
- **`static void writeMUX(uint8_t value)`** - Write MUX (Address Multiplexer) signal state
- **`static void writeRD(uint8_t value)`** - Write RD (Read signal) state
- **`static void writeWR(uint8_t value)`** - Write WR (Write signal) state
- **`static void writeIN(uint8_t value)`** - Write IN (Input signal) state
- **`static void writeOUT(uint8_t value)`** - Write OUT (Output signal) state
- **`static void writeINT(uint8_t value)`** - Write INT (Interrupt signal) state
- **`static void writeTEST(uint8_t value)`** - Write TEST (Test signal) state
- **`static void writeWAIT(uint8_t value)`** - Write WAIT (Wait signal) state
- **`static void writeSYS_RES(uint8_t value)`** - Write SYS_RES (System Reset) signal state
- **`static void writeINT_ACK(uint8_t value)`** - Write INT_ACK (Interrupt Acknowledge) signal state

## Bus Control Functions

- **`static void writeAddressBus(uint16_t address)`** - Write complete 16-bit address on address bus
- **`static void writeDataBus(uint8_t data)`** - Write 8-bit data on data bus
- **`static uint16_t readAddressBus()`** - Read complete 16-bit address from address bus
- **`static uint8_t readDataBus()`** - Read 8-bit data from data bus

**Example:**

```cpp
// Configure and write individual signals
Model1LowLevel::configWriteRAS(OUTPUT);
Model1LowLevel::writeRAS(HIGH);  // Write RAS high

Model1LowLevel::configWriteWR(OUTPUT);
Model1LowLevel::writeWR(LOW);    // Write WR low

// Write bus values
Model1LowLevel::writeAddressBus(0x1234);  // Write address to 0x1234
Model1LowLevel::writeDataBus(0xFF);       // Write data to 0xFF
```

## Signal Readers

Read current pin states.

- **`static uint8_t readRAS()`** - Read RAS (Row Address Strobe) signal current state
- **`static uint8_t readCAS()`** - Read CAS (Column Address Strobe) signal current state
- **`static uint8_t readMUX()`** - Read MUX (Address Multiplexer) signal current state
- **`static uint8_t readRD()`** - Read RD (Read signal) current state
- **`static uint8_t readWR()`** - Read WR (Write signal) current state
- **`static uint8_t readIN()`** - Read IN (Input signal) current state
- **`static uint8_t readOUT()`** - Read OUT (Output signal) current state
- **`static uint8_t readINT()`** - Read INT (Interrupt signal) current state
- **`static uint8_t readTEST()`** - Read TEST (Test signal) current state
- **`static uint8_t readWAIT()`** - Read WAIT (Wait signal) current state
- **`static uint8_t readSYS_RES()`** - Read SYS_RES (System Reset) signal current state
- **`static uint8_t readINT_ACK()`** - Read INT_ACK (Interrupt Acknowledge) signal current state

**Example:**

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

## Usage Examples

### Basic Signal Control

```cpp
#include "Model1LowLevel.h"

void setup() {
    // Configure RAS as output and write it high
    Model1LowLevel::configWriteRAS(OUTPUT);
    Model1LowLevel::writeRAS(HIGH);

    // Configure WR as input to read external signal
    Model1LowLevel::configWriteWR(INPUT);

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
    Model1LowLevel::configWriteAddressBus(0xFFFF);  // All address pins output
    Model1LowLevel::configWriteDataBus(0xFF);       // All data pins output

    // Write address and data
    Model1LowLevel::writeAddressBus(address);
    Model1LowLevel::writeDataBus(data);

    // Control write signal
    Model1LowLevel::configWriteWR(OUTPUT);
    Model1LowLevel::writeWR(LOW);   // Assert write
    delayMicroseconds(1);           // Hold time
    Model1LowLevel::writeWR(HIGH);  // Deassert write
}

uint8_t readFromMemory(uint16_t address) {
    // Write address
    Model1LowLevel::configWriteAddressBus(0xFFFF);
    Model1LowLevel::writeAddressBus(address);

    // Configure data bus as input
    Model1LowLevel::configWriteDataBus(0x00);

    // Control read signal
    Model1LowLevel::configWriteRD(OUTPUT);
    Model1LowLevel::writeRD(LOW);   // Assert read
    delayMicroseconds(1);           // Access time

    // Read data
    uint8_t data = Model1LowLevel::readDataBus();

    Model1LowLevel::writeRD(HIGH);  // Deassert read

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

## Performance Notes

- **Static Inline Functions**: All functions are `static inline` for maximum performance
- **Zero Call Overhead**: No function call overhead when compiled with optimization
- **Direct Port Manipulation**: Uses optimized port macros for fastest access
- **Time-Critical Suitable**: Appropriate for time-critical applications requiring precise timing
