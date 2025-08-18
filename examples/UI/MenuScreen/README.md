# MenuScreen Examples

This folder contains examples for the **MenuScreen** class - an advanced screen type that provides automatic menu navigation and management.

## Available Examples

### SimpleMenu/

**Perfect for getting started with menus!** A basic MenuScreen example showing:

- Simple menu creation with 5 items
- Up/Down navigation and item selection
- LED color control through menu choices
- Basic menu patterns and user feedback

**Files:**

- `SimpleMenu.ino` - Main Arduino sketch
- `SimpleMenu.h/.cpp` - Basic menu implementation

### ComprehensiveMenu/

**Complete MenuScreen demonstration** showing ALL capabilities:

- Multi-level menu hierarchies with sub-menus
- Configuration values displayed next to menu items
- Disabled menu items with conditional logic
- Advanced user feedback (alerts, confirmations, notifications)
- Screen navigation and memory management
- All ContentScreen features (headers, footers, progress bars)

**Files:**

- `ComprehensiveMenu.ino` - Main Arduino sketch
- `ComprehensiveMenuScreen.h/.cpp` - Advanced menu with all features
- `SimpleSubmenu.h/.cpp` - Example submenu implementation

## Learning Path

1. **Start with SimpleMenu** - Learn basic menu concepts
2. **Move to ComprehensiveMenu** - See all advanced features
3. **Study the submenu pattern** - Learn navigation techniques

## MenuScreen Features

MenuScreen builds on ContentScreen and adds:

- **Automatic menu navigation** - Up/Down buttons handled automatically
- **Item highlighting** - Current selection visually indicated
- **Pagination** - Long menus automatically scroll
- **Configuration display** - Show current values next to menu items
- **Disabled items** - Items can be conditionally grayed out
- **Selection handling** - Easy override for item selection logic

## Class Hierarchy

```
Screen (base class)
+-- ContentScreen (adds header/footer/progress)
    +-- MenuScreen (adds menu navigation)
```

MenuScreen inherits ALL ContentScreen features:

- User feedback systems (alert, confirm, notify)
- Automatic header and footer management
- Progress bar support
- Adaptive layouts for different display sizes
