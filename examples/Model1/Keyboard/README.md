# Keyboard Examples

This folder contains examples demonstrating keyboard input handling for the TRS-80 Model I.

## Overview

The TRS-80 Model I uses a matrix-scanned keyboard that can be read through I/O ports. These examples show how to detect key presses, handle key combinations, and analyze keyboard input patterns.

## TRS-80 Model I Keyboard Layout

The TRS-80 keyboard is organized as:

- **8 x 8 Matrix**: 64 possible key positions
- **Special Keys**: SHIFT, CTRL, numeric keypad
- **ASCII Mapping**: Standard ASCII character output
- **Key Combinations**: SHIFT and CTRL modifiers

## Examples

### [SimpleKeyboard](SimpleKeyboard/README.md)

**Basic keyboard input detection**

Learn fundamental keyboard concepts including:

- Basic key press detection
- ASCII character conversion
- Simple key state monitoring
- Basic input polling

### [ComprehensiveKeyboard](ComprehensiveKeyboard/README.md)

**Advanced keyboard analysis and statistics**

Explore advanced keyboard capabilities including:

- Real-time key combination detection
- Typing pattern analysis
- Key press timing and statistics
- Advanced input handling techniques
- Keyboard diagnostic testing

## Key Concepts

### Matrix Scanning

The TRS-80 keyboard uses matrix scanning:

- **Row Selection**: Activate one row at a time
- **Column Reading**: Read column states for active row
- **Key Identification**: Row/column intersection identifies key
- **Debouncing**: Handle mechanical key bounce

### Key State Detection

Track keyboard state changes:

- **Press Detection**: Key goes from up to down
- **Release Detection**: Key goes from down to up
- **Repeat Handling**: Continuous key press detection
- **State Tracking**: Monitor multiple keys simultaneously

### Character Conversion

Convert key positions to characters:

- **ASCII Mapping**: Standard character conversion
- **Modifier Keys**: SHIFT and CTRL key effects
- **Special Characters**: Handle non-printing keys
- **Key Combinations**: Multi-key sequences

## Important Considerations

### Input Handling

- **Polling vs Interrupt**: Different detection methods
- **Debouncing**: Handle mechanical key bounce
- **Response Time**: Balance between speed and accuracy
- **Multiple Keys**: Handle simultaneous key presses

### System Integration

- **Non-Blocking**: Don't interfere with TRS-80 operation
- **Clean Integration**: Minimal impact on system performance
- **Error Handling**: Graceful handling of invalid states
- **State Consistency**: Maintain accurate key state tracking

## Common Use Cases

- **Input Monitoring**: Monitor user keyboard activity
- **Key Mapping**: Create custom key assignments
- **Typing Analysis**: Study typing patterns and speed
- **Keyboard Testing**: Verify keyboard functionality
- **Game Controllers**: Use keyboard for game input
- **Security Monitoring**: Log keyboard activity

## Getting Started

1. **Start with SimpleKeyboard**: Learn basic key detection
2. **Understand matrix scanning**: How the hardware works
3. **Practice state tracking**: Monitor key press patterns
4. **Advanced analysis**: Move to ComprehensiveKeyboard for detailed statistics

The keyboard interface provides a direct connection to user input - understanding it enables powerful interactive applications.
