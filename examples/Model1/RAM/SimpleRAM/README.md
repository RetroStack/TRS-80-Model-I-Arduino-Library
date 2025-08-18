# SimpleRAM Example

This example demonstrates basic RAM operations and DRAM refresh functionality for the TRS-80 Model I.

## What It Does

- **Basic Memory Operations**: Read and write single bytes to RAM
- **DRAM Refresh**: Demonstrates automatic DRAM refresh using Timer 2
- **Memory Testing**: Simple pattern-based memory verification
- **Bus Control**: Shows proper TEST signal activation/deactivation

## What You'll Learn

- How to configure DRAM refresh with Timer interrupts
- Basic memory read/write operations
- Understanding DRAM refresh requirements
- Memory pattern testing techniques
- Proper bus control procedures
- Timer interrupt setup for DRAM maintenance

## Hardware Requirements

- Arduino Mega 2560
- TRS-80 Model I with 40-pin edge connector interface
- Serial monitor for test results
- Proper bus control via TEST signal

## Key Functions Demonstrated

- `Model1.begin(2)` - Initialize with Timer 2 for DRAM refresh
- `Model1.writeMemory()` / `Model1.readMemory()` - Basic memory operations
- `Model1.activateTestSignal()` / `Model1.deactivateTestSignal()` - Bus control
- Timer interrupt service routine for DRAM refresh
- Simple memory pattern testing

## Important Notes

**DRAM Refresh**: This example includes critical DRAM refresh functionality. The Timer 2 interrupt ensures that dynamic RAM contents are preserved during Arduino control.

**Memory Areas**: Tests are performed in user RAM area (0x4000+) to avoid interfering with system ROM or video memory.

## Serial Output

- Memory operation results
- DRAM refresh status
- Memory test pass/fail results
- Bus control status

## Usage

1. Connect your Arduino to the TRS-80 Model I edge connector
2. Open the Serial Monitor at 115200 baud
3. Upload this sketch to your Arduino Mega 2560
4. Watch the memory operations and refresh demonstrations
5. Observe how DRAM refresh maintains data integrity

This example is essential for understanding TRS-80 memory management and is a prerequisite for more advanced memory operations.
