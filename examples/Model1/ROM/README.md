# ROM Examples

This folder contains examples demonstrating ROM (Read-Only Memory) operations for the TRS-80 Model I system ROMs.

## Overview

The ROM class provides functionality for reading and analyzing the TRS-80 Model I system ROMs. These ROMs contain the system BASIC interpreter and are essential for system operation. The examples show how to identify ROM versions, calculate checksums, and perform diagnostic operations.

## TRS-80 Model I ROM Layout

- **ROM A** (0x0000-0x07FF): System bootstrap and core routines
- **ROM B** (0x0800-0x0FFF): BASIC interpreter (part 1)
- **ROM C** (0x1000-0x17FF): BASIC interpreter (part 2)
- **ROM D** (0x1800-0x1FFF): BASIC interpreter (part 3)

## Examples

### [SimpleROM](SimpleROM/README.md)

**Basic ROM identification and verification**

Learn fundamental ROM operations including:

- Automatic system ROM identification
- ROM checksum calculation and verification
- Basic ROM health checking
- System compatibility detection

### [ComprehensiveROM](ComprehensiveROM/README.md)

**Advanced ROM analysis and diagnostics**

Explore advanced ROM capabilities including:

- Detailed ROM content analysis and hex dumps
- Memory mapping verification
- ROM integrity checking with multiple verification passes
- Comprehensive system diagnostics and comparison analysis

## Key Concepts

### ROM Identification

The library can automatically identify common TRS-80 ROM versions including:

- Level I BASIC ROMs
- Level II BASIC ROMs
- Custom or modified ROM sets

### Checksums

ROM checksums provide a quick way to:

- Verify ROM integrity
- Identify specific ROM versions
- Detect corruption or modification
- Compare ROM sets across systems

### Memory Mapping

Understanding ROM memory mapping is crucial for:

- System-level programming
- Memory management
- Debugging system issues
- Creating ROM-based utilities

## Common Use Cases

- **System Diagnostics**: Verify ROM health and compatibility
- **ROM Dumping**: Create backup copies of system ROMs
- **Version Identification**: Determine which BASIC version is installed
- **Repair Assistance**: Identify faulty or missing ROMs
- **Development Tools**: Analyze ROM contents for system programming

## Getting Started

1. **Start with SimpleROM**: Learn basic ROM operations and identification
2. **Understand checksums**: Practice ROM verification techniques
3. **Explore memory mapping**: Learn how ROMs are organized in memory
4. **Advanced analysis**: Move to ComprehensiveROM for detailed diagnostics

ROM operations are read-only and safe to perform on any TRS-80 system.
