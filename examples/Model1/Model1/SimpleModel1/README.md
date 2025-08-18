# Simple Model1 Example

This example demonstrates basic Model1 class functionality for TRS-80 Model I hardware interface operations.

## What You'll Learn

- How to initialize the Model1 hardware interface
- Basic bus control and TEST\* signal activation
- Simple memory read and write operations
- Basic I/O port access techniques
- Memory block operations and efficient data transfer
- Safe hardware interface practices

## What It Does

This simple example provides a foundation for TRS-80 hardware interface:

1. **Hardware Initialization**: Sets up Model1 interface and activates TEST\* signal
2. **Basic Memory Operations**: Single byte read/write to TRS-80 memory
3. **I/O Port Access**: Basic input/output port operations
4. **Memory Block Operations**: Efficient block read/write operations
5. **Bus Control**: Demonstrates safe bus control techniques

## Key Features

### Basic Hardware Interface

- **Model1 Initialization**: Proper hardware setup and configuration
- **TEST\* Signal Control**: Safe activation of TRS-80 test mode
- **Bus Management**: Basic bus control and timing
- **Error Handling**: Simple error detection and reporting

### Memory Operations

- **Single Byte Access**: Basic read/write operations
- **Block Operations**: Efficient multi-byte transfers
- **Address Management**: Proper addressing techniques
- **Data Validation**: Basic data verification

### I/O Operations

- **Port Access**: Basic I/O port read/write
- **Control Signals**: Hardware control signal management
- **Timing Control**: Proper timing for I/O operations
- **Status Checking**: Basic hardware status verification

## Technical Implementation

### Basic Setup

```cpp
#include <Model1.h>

void setup() {
    Serial.begin(9600);
    Model1.begin();                 // Initialize hardware
    Model1.activateTestSignal();    // Take bus control
}
```

### Memory Operations

- **Read Memory**: `Model1.readMemory(address)`
- **Write Memory**: `Model1.writeMemory(address, value)`
- **Block Read**: `Model1.readMemoryBlock(address, buffer, length)`
- **Block Write**: `Model1.writeMemoryBlock(address, buffer, length)`

### I/O Operations

- **Read Port**: `Model1.readPort(port)`
- **Write Port**: `Model1.writePort(port, value)`
- **Status Check**: Basic hardware status verification

## Sample Output

```
=== Simple Model1 Example ===
Basic hardware interface operations

--- Basic Memory Operations ---
Original value at 0x3C00: 0x20
Writing test value 0x55...
Read back value: 0x55 ✓
Restoring original value...
Memory test: PASSED

--- Basic I/O Operations ---
Reading keyboard port 0x3800...
Port value: 0xFF
Writing to cassette port 0x3FF8...
I/O test: COMPLETED

--- Memory Block Operations ---
Reading 16-byte block from 0x3C00...
Block read completed successfully
Writing test pattern...
Verifying block write...
Block test: PASSED

--- Bus Control ---
Bus status: ACTIVE
TEST* signal: ENABLED
Clock control: AVAILABLE
Bus control: OPERATIONAL
```

## Hardware Requirements

- Arduino Mega 2560 (required for sufficient GPIO pins)
- TRS-80 Model I with 40-pin edge connector access
- Proper signal level compatibility (TTL levels)
- Stable power supply for both systems

## Memory Map Reference

### TRS-80 Model I Memory Layout

- **0x0000-0x1FFF**: System ROM (8KB) - READ ONLY
- **0x3C00-0x3FFF**: Video RAM (1KB) - Safe for testing
- **0x4000-0x7FFF**: User RAM (16KB) - Safe for testing
- **0x8000+**: Extended RAM (if present)

### Safe Testing Areas

- **Video RAM (0x3C00-0x3FFF)**: Safe for read/write testing
- **User RAM (0x4000+)**: Safe for extensive testing
- **Avoid ROM areas**: Never write to 0x0000-0x1FFF

## Important Safety Notes

### Bus Control

- **Always activate TEST\* signal** before memory operations
- Use `Model1.activateTestSignal()` to take bus control
- Deactivate when finished to return control to TRS-80

### Memory Safety

- **Never write to ROM** areas (0x0000-0x1FFF)
- **Test in Video RAM** first (0x3C00-0x3FFF)
- **Backup important data** before extensive testing
- **Verify operations** with read-back testing

### Hardware Protection

- **Check signal levels** ensure TTL compatibility
- **Monitor power consumption** avoid overloading
- **Use proper timing** respect setup/hold times
- **Graceful shutdown** properly release bus control

## Usage Notes

- Start with video RAM (0x3C00) for safe testing
- Always verify write operations with read-back
- Use Serial Monitor to observe operation results
- Perfect foundation for more complex hardware operations

## Integration with Other Examples

This simple Model1 example pairs well with:

- **ComprehensiveModel1**: Move here after mastering basics
- **ROM/RAM Examples**: Specialized memory operations
- **Logger Examples**: Add debugging to hardware operations

Simple Model1 provides the essential foundation for all TRS-80 Model I hardware interface operations.
