# SimpleROM Example

This example demonstrates basic ROM reading operations for the TRS-80 Model I system ROMs.

## What It Does

- **System Identification**: Automatically identifies the TRS-80 ROM version
- **Checksum Calculation**: Calculates and displays checksums for all ROM slots (A, B, C, D)
- **ROM Verification**: Checks if ROMs are present and accessible
- **Basic Diagnostics**: Provides simple status information about the ROM system

## What You'll Learn

- How to use the ROM class for system identification
- ROM checksum calculation and verification
- Understanding TRS-80 ROM slot organization
- Basic ROM health checking techniques
- System compatibility detection

## Hardware Requirements

- Arduino Mega 2560
- TRS-80 Model I with 40-pin edge connector interface
- Serial monitor for ROM information display
- Proper bus control via TEST signal

## Key Functions Demonstrated

- `rom.identifyROM()` - Automatic system ROM identification
- `rom.getChecksum(slot)` - Calculate ROM checksums
- Basic ROM presence detection
- Serial output formatting for ROM data

## Serial Output

This example produces clear serial output including:

- Detected system name (if recognized)
- Individual ROM checksums for slots A through D
- ROM validation status
- System readiness assessment

## Usage

1. Connect your Arduino to the TRS-80 Model I edge connector
2. Open the Serial Monitor at 9600 baud
3. Upload this sketch to your Arduino Mega 2560
4. Watch the serial output for ROM identification and status
5. The cycle repeats every few seconds for continuous monitoring

This example is perfect for quickly checking ROM health and system compatibility before running more complex operations.
