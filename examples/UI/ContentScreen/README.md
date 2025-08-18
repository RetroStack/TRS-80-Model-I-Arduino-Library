# ContentScreen Examples

This folder contains examples for the **ContentScreen** class - a structured screen layout providing automatic header, footer, content area, and progress bar management.

## Available Examples

### SimpleContent/

**Perfect for getting started with ContentScreen!** A basic example showing:

- Simple content layout with automatic header/footer
- Progress bar that reflects current state
- User feedback systems (alerts, confirmations, notifications)
- Basic button handling and navigation
- Clean, structured screen organization

**Files:**

- `SimpleContent.ino` - Main Arduino sketch
- `SimpleContent.h/.cpp` - Basic content screen implementation
- `README.md` - Complete documentation and tutorial

### ComprehensiveContent/

**Complete ContentScreen demonstration** showing ALL capabilities:

- Advanced content rendering and management
- Dynamic progress bar updates
- Multiple content sections and layouts
- Advanced user feedback and interaction
- Real-time data display and updates
- Custom drawing within the content area
- Complex state management

**Files:**

- `ComprehensiveContent.ino` - Main Arduino sketch
- `ComprehensiveContent.h/.cpp` - Advanced content screen implementation
- `README.md` - Complete documentation and learning guide

## What is ContentScreen?

ContentScreen is a structured layout class that automatically manages:

- **Header Area**: Title and status information
- **Footer Area**: Button hints and navigation info
- **Content Area**: Your custom content (text, graphics, data)
- **Progress Bar**: Visual indication of progress or selection

## Why Use ContentScreen?

- **Consistent Layout**: Automatic header/footer positioning
- **Built-in Feedback**: Alert, confirm, and notify systems
- **Progress Tracking**: Visual progress indication
- **Clean Organization**: Separates layout from content logic
- **Easy Customization**: Focus on your content, not layout management

## Getting Started

1. **Start with SimpleContent/** to learn the basics
2. **Explore ComprehensiveContent/** for advanced features
3. **Read the README.md** files for step-by-step tutorials
4. **Modify the examples** to fit your project needs

## Hardware Requirements

- **M1Shield** with display
- **Arduino Mega 2560**
- Any **supported display** (ST7789, ILI9341, ST7735, etc.)
- **Working buttons/joystick** for navigation

## Key ContentScreen Features

### Layout Management

- Automatic header, footer, and content area positioning
- Display size adaptation (small/large display support)
- Consistent visual organization

### User Feedback

- **Alerts**: Important information with OK button
- **Confirmations**: Yes/No choices with clear options
- **Notifications**: Auto-expiring status messages

### Progress Indication

- Visual progress bar in header area
- Automatic scaling and positioning
- Custom progress value setting

### Content Flexibility

- Draw custom content in the content area
- Text rendering with automatic wrapping
- Graphics and data visualization support

## Learning Path

1. **Read SimpleContent README.md** - Understand the basics
2. **Upload SimpleContent example** - See it in action
3. **Modify SimpleContent** - Try your own content
4. **Explore ComprehensiveContent** - Learn advanced techniques
5. **Build your own ContentScreen** - Apply to your projects
