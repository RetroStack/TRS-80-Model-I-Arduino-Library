# RAM Examples

This folder contains examples demonstrating RAM (Random Access Memory) operations and testing for the TRS-80 Model I.

## Overview

The TRS-80 Model I uses dynamic RAM (DRAM) which requires periodic refresh to maintain data integrity. These examples show how to properly manage DRAM refresh, perform memory operations, and test memory reliability.

## TRS-80 Model I Memory Layout

- **System ROM** (0x0000-0x1FFF): 8KB BASIC interpreter (read-only)
- **Video RAM** (0x3C00-0x3FFF): 1KB display memory
- **User RAM** (0x4000-0x7FFF): 16KB user program space
- **Extended RAM** (0x8000+): Additional memory in expanded systems

## Examples

### [SimpleRAM](SimpleRAM/README.md)

**Basic RAM operations and DRAM refresh**

Learn fundamental RAM concepts including:

- DRAM refresh setup with Timer interrupts
- Basic memory read/write operations
- Memory pattern testing
- Proper bus control procedures

### [ComprehensiveRAM](ComprehensiveRAM/README.md)

**Advanced memory testing and analysis**

Explore advanced RAM capabilities including:

- Comprehensive memory region analysis
- Advanced memory testing patterns
- Performance benchmarking
- Long-term stability testing
- Memory mapping verification

## Key Concepts

### DRAM Refresh

Dynamic RAM requires periodic refresh to maintain data:

- **Refresh Rate**: Typically every 2ms for TRS-80 DRAM
- **Timer Interrupt**: Arduino Timer 2 handles automatic refresh
- **Critical Timing**: Missing refresh cycles cause data loss

### Memory Testing

Comprehensive testing includes:

- **Pattern Tests**: Various bit patterns to test all memory cells
- **Walking Bit Tests**: Isolate individual bit failures
- **Address Tests**: Verify correct address decoding
- **Retention Tests**: Check data stability over time

### Memory Regions

Different memory areas have different characteristics:

- **Video RAM**: Directly controls display output
- **User RAM**: Available for programs and data
- **System Areas**: Reserved for system operation

## Important Considerations

### DRAM Refresh Requirements

- **Always enable refresh** when taking bus control
- Use Timer 2 interrupt for automatic refresh
- Monitor refresh timing for reliability
- Test refresh effectiveness with stability tests

### Memory Safety

- Avoid writing to ROM areas (0x0000-0x1FFF)
- Be careful with video RAM (affects display)
- Use user RAM areas for safe testing
- Always restore original values when possible

## Common Use Cases

- **Memory Diagnostics**: Test RAM reliability and performance
- **System Validation**: Verify memory system operation
- **Debugging Tools**: Identify memory-related issues
- **Performance Analysis**: Measure memory access speeds
- **Repair Assistance**: Locate faulty memory components

## Getting Started

1. **Start with SimpleRAM**: Learn DRAM refresh and basic operations
2. **Understand refresh timing**: Critical for data integrity
3. **Practice memory testing**: Learn pattern-based verification
4. **Advanced diagnostics**: Move to ComprehensiveRAM for detailed analysis

Always ensure proper DRAM refresh is active when performing memory operations on the TRS-80.
