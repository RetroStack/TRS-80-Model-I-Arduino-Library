# SimpleViewPort Example

This example demonstrates basic viewport functionality, showing how to constrain video operations to a specific rectangular area of the screen.

## What It Does

- **Viewport Definition**: Creates a 44x10 character window within the full screen
- **Boundary Visualization**: Shows how viewport constraints work
- **Clipped Operations**: Demonstrates text clipping at viewport edges
- **Relative Coordinates**: Shows how coordinates work within a viewport
- **Viewport Scrolling**: Demonstrates scrolling within the constrained area

## What You'll Learn

- How to define and apply a viewport using `ViewPort` structure
- Understanding relative vs absolute coordinate systems
- How text automatically clips at viewport boundaries
- Viewport-constrained scrolling behavior
- Switching between different viewport configurations
- Visual representation of viewport boundaries

## Hardware Requirements

- Arduino Mega 2560
- TRS-80 Model I with 40-pin edge connector interface
- Proper bus control via TEST signal

## Key Functions Demonstrated

- `video.setViewPort()` - Apply viewport constraints
- Viewport structure definition (`ViewPort.x`, `.y`, `.width`, `.height`)
- Automatic text clipping and wrapping within viewport
- Coordinate translation between viewport and screen space
- Scrolling behavior within constrained area

## Viewport Configuration

This example uses a viewport positioned at:

- **Position**: (10, 3) - 10 characters from left, 3 rows from top
- **Size**: 44 characters wide by 10 rows tall
- **Coverage**: Center portion of the 64x16 screen

## Usage

1. Connect your Arduino to the TRS-80 Model I edge connector
2. Upload this sketch to your Arduino Mega 2560
3. Watch how text operations are constrained to the viewport area
4. Observe the visual boundaries and clipping behavior
5. See how scrolling only affects the viewport region

This example provides a foundation for creating windowed interfaces, status bars, or multi-panel displays on the TRS-80.
