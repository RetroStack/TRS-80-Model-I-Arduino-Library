# Video Examples

This folder contains examples demonstrating basic video operations for the TRS-80 Model I using the full screen without viewport constraints.

## What Are Basic Video Operations?

Basic video operations use the full 64x16 character screen of the TRS-80 Model I. These operations include:

- Text output and positioning
- Screen clearing with various patterns
- Cursor management and tracking
- Scrolling and character mode switching
- Memory reading and character conversion

## Examples

### [SimpleVideo](SimpleVideo/README.md)

**Fundamental video operations**

Learn the basics of TRS-80 video control including:

- Screen clearing techniques
- Basic text output and positioning
- Cursor movement and tracking
- Simple scrolling operations

### [ComprehensiveVideo](ComprehensiveVideo/README.md)

**Advanced video features and memory operations**

Explore advanced video capabilities including:

- Memory reading and analysis
- Character set conversion
- Performance optimization
- Detailed logging and debugging
- Character mode switching effects

## Key Features

### Screen Management

- **Full screen access**: 64 characters × 16 rows
- **Multiple clearing modes**: Spaces, characters, or patterns
- **Automatic wrapping**: Text flows naturally across rows

### Character Modes

- **64-character mode**: Standard full-width display
- **32-character mode**: Double-width characters for accessibility

### Memory Operations

- **Direct memory access**: Read video RAM at 0x3C00-0x3FFF
- **Character conversion**: Between local ASCII and TRS-80 character sets
- **Address calculation**: Automatic row/column to memory mapping

### Performance Features

- **Auto-scroll control**: Enable/disable automatic scrolling
- **Efficient memory operations**: Optimized for speed
- **Logging integration**: Debug and monitor video operations

## Getting Started

1. **Start with SimpleVideo**: Learn fundamental concepts
2. **Practice positioning**: Experiment with cursor and text placement
3. **Understand scrolling**: See how text flows and scrolls
4. **Advanced features**: Move to ComprehensiveVideo for memory operations

These examples provide the foundation for all TRS-80 video programming and are essential before working with viewport-constrained operations.
