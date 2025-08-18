# ComprehensiveViewPort Example

This example demonstrates advanced viewport techniques including multiple viewports, dynamic switching, memory operations, and edge case handling.

## What It Does

- **Multiple Viewports**: Creates and manages several different viewport configurations
- **Dynamic Switching**: Animates viewport size and position changes
- **Coordinate Analysis**: Detailed examination of viewport coordinate systems
- **Memory Operations**: Direct memory access within viewport constraints
- **Edge Case Testing**: Tests minimum sizes and boundary conditions
- **Advanced Layout**: Creates complex multi-panel interfaces

## What You'll Learn

- Managing multiple viewport configurations
- Dynamic viewport switching techniques
- Coordinate translation between viewport and screen space
- Memory address calculation within viewports
- Advanced layout techniques for complex interfaces
- Edge case handling and viewport limitations
- Performance considerations with frequent viewport changes

## Hardware Requirements

- Arduino Mega 2560
- TRS-80 Model I with 40-pin edge connector interface
- Serial monitor for detailed analysis output
- Proper bus control via TEST signal

## Key Functions Demonstrated

- Multiple `ViewPort` structure definitions
- `video.getAbsoluteX()` / `video.getAbsoluteY()` - Coordinate translation
- `video.getAddress()` - Memory address calculation within viewports
- `video.read()` - Reading text from viewport areas
- Dynamic viewport animation and switching
- Memory dump operations within viewport constraints

## Viewport Configurations

This example demonstrates several viewport types:

- **Full Screen**: 64x16 - Complete screen access
- **Main Window**: 54x12 - Primary content area with border
- **Small Window**: 24x6 - Overlay window
- **Narrow Column**: 4x16 - Side panel
- **Status Bar**: 64x1 - Bottom status line
- **Dynamic Viewports**: Animated size changes

## Serial Output

Detailed analysis output includes:

- Coordinate mapping tables
- Memory address calculations
- Viewport boundary analysis
- Memory content dumps
- Performance timing information

## Usage

1. Connect your Arduino to the TRS-80 Model I edge connector
2. Open the Serial Monitor at 115200 baud for detailed analysis
3. Upload this sketch to your Arduino Mega 2560
4. Watch the complex multi-viewport interface demonstration
5. Observe dynamic viewport changes and memory operations

This example is perfect for developers creating sophisticated user interfaces, diagnostic tools, or applications requiring complex screen layouts on the TRS-80.
