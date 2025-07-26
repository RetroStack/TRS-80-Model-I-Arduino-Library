# TRS-80 Model I Arduino Library

This library allows an Arduino to control the TRS-80 Model I mainboard through its edge connector.

![Overview](..//Images/Overview.png)

## Model 1 Classes

- [**Model1**](Model1.md) — Main interface to control bus signals, memory, and interrupts.
- [**Cassette**](Cassette.md) — Cassette output and video mode control.
- [**Keyboard**](Keyboard.md) — Reading the keyboard matrix.
- [**Video**](Video.md) — Video memory management and text display.
- [**ROM**](ROM.md) — ROM reading, checksumming, and identification.

## Logging Classes

- [**ILogger**](ILogger.md) — Logging interface.
- [**SerialLogger**](SerialLogger.md) — Logging implementation to Serial output.

## Shield Access

![M1Shield](..//Images/M1Shield.png)

- [**M1Shield**](M1Shield.md) — Hardware abstraction layer for display, input, LED control, and screen management.

## Screen Management

- [**Screen**](Screen.md) — Abstract base class for all user interface screens with lifecycle and input handling.
- [**ContentScreen**](ContentScreen.md) — Structured layout template with header, content, footer, and progress regions.
- [**ConsoleScreen**](ConsoleScreen.md) — Console implementation with print, println, and cls features for easy views.
- [**MenuScreen**](MenuScreen.md) — Paginated menu framework with automatic navigation and selection handling.
- [**M1TerminalScreen**](M1TerminalScreen.md) — TRS-80 Model I terminal emulation with 64×16 character grid and Model1 integration.
