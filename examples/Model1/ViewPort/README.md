# ViewPort Examples

This folder contains examples demonstrating viewport functionality for the TRS-80 Model I Video class. Viewports allow you to constrain video operations to specific rectangular areas of the screen, enabling windowed interfaces and complex layouts.

## What Are ViewPorts?

A viewport is a rectangular region of the screen that constrains all video operations. When a viewport is active:

- All text output is clipped to the viewport boundaries
- Coordinates become relative to the viewport's top-left corner
- Scrolling only affects the viewport area
- Cursor movement is constrained to the viewport

## Examples

### [SimpleViewPort](SimpleViewPort/README.md)

**Basic viewport operations and concepts**

Learn the fundamentals of viewport usage including:

- Defining viewport boundaries
- Understanding relative coordinates
- Text clipping behavior
- Viewport-constrained scrolling

### [ComprehensiveViewPort](ComprehensiveViewPort/README.md)

**Advanced viewport techniques and multiple viewport management**

Explore advanced viewport capabilities including:

- Multiple simultaneous viewports
- Dynamic viewport switching and animation
- Memory operations within viewports
- Complex layout techniques
- Edge case handling

## Key Concepts

### ViewPort Structure

```cpp
ViewPort viewPort;
viewPort.x = 10;        // Left edge position (screen coordinates)
viewPort.y = 3;         // Top edge position (screen coordinates)
viewPort.width = 44;    // Width in characters
viewPort.height = 10;   // Height in characters
```

### Coordinate Systems

- **Screen Coordinates**: Absolute positions on the full 64x16 screen
- **Viewport Coordinates**: Relative positions within the viewport (0,0 = viewport's top-left)

### Common Use Cases

- **Status bars**: Single-line viewports for system information
- **Dialog boxes**: Small centered viewports for user interaction
- **Multi-panel interfaces**: Multiple viewports for complex layouts
- **Scrolling text areas**: Constrained regions for large text content

## Getting Started

1. **Start with SimpleViewPort**: Learn basic viewport concepts
2. **Understand coordinates**: Practice with relative vs absolute positioning
3. **Experiment with sizes**: Try different viewport dimensions
4. **Advanced techniques**: Move to ComprehensiveViewPort for complex operations

ViewPorts are essential for creating sophisticated user interfaces on the TRS-80 Model I display.
