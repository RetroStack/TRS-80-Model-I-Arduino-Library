# TRS-80 Model 1 - Arduino Library - Changelog

## 23rd June 2024 - Initial Version (0.9)

This is the initial version written by Ven Reddy

## 16th September 2024 - Initial Version (0.9.1)

- General cleanup of the codebase
- Adding Arduino library pattern
- Adding cassette interface

## 27th September 2024 - Initial Version (0.9.2)

- General cleanup of the codebase

## 22th July 2025 - Initial Release (1.0.0)

- General cleanup of the codebase

## 22th July 2025 - Updated Release (1.1.0)

- Adding Print features to Video class

## 26th July 2025 - Updated Release (1.2.0)

- Adding M1Sheild and Screen implementations
  - Screen
  - ContentScreen
  - ConsoleScreen
  - MenuScreen
  - M1TerminalScreen
- Adding the ability to request TEST signal state

## 28th July 2025 - Updated Release (1.2.1)

- Added LoggerScreen with ILogger compatibility through adapter pattern
- Added CompositeLogger for multi-destination logging
- Added SerialLogger for Serial Monitor integration
- **BREAKING CHANGE**: LoggerScreen now requires `screen->asLogger()` when used with CompositeLogger instead of passing the screen directly

## 28th July 2025 - Updated Release (1.2.2)

- Added Display_SSD1306 provider for monochrome OLED displays (128x64)
- Added Display_ST7789_320x170 provider for wide landscape format displays
- Added Display_SH1106 provider for monochrome SH1106 OLED displays (128x64)
- Added Display_ST7789_320x240 (renamed from Display_ST7789) provider for alternative landscape format displays

## 16th August 2025 - Updated Release (1.2.3)

- **MAJOR**: Added comprehensive FlashString (F() macro) support across UI classes for memory efficiency
  - Added FlashString variants for ContentScreen: `setTitleF()`, `setButtonItemsF()`, `drawTextF()`
  - Added FlashString variants for MenuScreen: `setMenuItemsF()`, `_getMenuItemConfigValueF()`
  - Enhanced ILogger with FlashString methods: `infoF()`, `warnF()`, `errF()` with variadic format support
  - Added variadic format support to existing String methods: `info()`, `warn()`, `err()`
- **MAJOR**: Added comprehensive console paging functionality to ConsoleScreen
  - New paging modes: Timeout, Button, Both, Off
  - Page status display and user prompts
- **BEHAVIOR CHANGE**: ConsoleScreen paging is now **blocking** for print operations
  - `print()`, `println()`, and `write()` methods block execution during paging waits
  - User input continues to be processed during blocked operations
  - Ensures sequential output order and prevents message loss
  - **Any button press** can continue paging (all buttons + joystick)
- **BREAKING CHANGE**: Removed M1TerminalScreen class and comprehensive demo examples
- **BREAKING CHANGE**: Moved low-level hardware access to dedicated Model1LowLevel class
- **API IMPROVEMENT**: Made various methods public for better accessibility across Screen classes
- Added `keyName()` method to KeyboardChangeIterator for human-readable key names
- General code cleanup, warning fixes, and improved method accessibility

## TBD - Updated Release (1.3.0)

- **NEW FEATURE**: Added `getStateData()` method to Model1 class for efficient packed state access
  - Returns complete TRS-80 system state in a single 64-bit value
  - Byte-aligned bit layout for optimal performance and accessibility
  - Includes address bus (16 bits), data bus (8 bits), memory control signals (8 bits), and system signals (8 bits)
  - Provides efficient alternative to string-based state reporting
- **NEW FEATURE**: Added `getStateConfigData()` method for pin configuration state access
  - Returns complete pin direction configuration in a single 64-bit value
  - Same bit layout as getStateData() but reports INPUT/OUTPUT pin configurations
  - Available in both Model1 and Model1LowLevel classes
  - Enables efficient monitoring of pin direction changes
- **ARCHITECTURE IMPROVEMENT**: Model1LowLevel is now the authoritative source for state data
  - Implementation moved from Model1 to Model1LowLevel for consistency
  - Model1.getStateData() and Model1.getStateConfigData() delegate to Model1LowLevel
  - Ensures single source of truth for low-level state access
- **NEW FEATURE**: Added auto-forward functionality to ConsoleScreen for automated navigation
  - Auto-forward automatically triggers actionTaken(BUTTON_MENU) after \_executeOnce() completes
  - Configurable delay (default 5 seconds) before auto-forward activates
  - User interaction cancels pending auto-forward to maintain user control
  - Disabled by default - must be explicitly enabled with setAutoForward()
- **UI IMPROVEMENT**: Updated MenuScreen button behavior for better navigation
  - LEFT button now performs selection action (same as joystick press and RIGHT)
  - MENU button is now the only way to exit menus (no more RIGHT accepted for it)
- **NEW FEATURE**: Added TRS-80 Model I cassette interface support to M1Shield
  - **WARNING: ADVANCED FEATURE**: Direct access to cassette remote control (CR1/CR2) and audio interface
  - Cassette remote methods: `setCR1Mode()`, `setCR2Mode()`, `writeCR1()`, `writeCR2()`, `readCR1()`, `readCR2()`
  - Audio interface methods: `writeCassetteIn()` (Arduino>Model I), `readCassetteOut()` (Model I>Arduino)
  - **SAFETY WARNING**: Incorrect usage can damage Arduino - CR1/CR2 may be connected. Setting them both to write in different values (HIGH/LOW) may result in a short.
  - Pin assignments: CR1 (pin 2), CR2 (pin 3), Cassette Input (A14), Cassette Output (A15)
- **UI ENHANCEMENT**: Added paging pause/resume functionality to ConsoleScreen
  - **LEFT button**: Pauses automatic timeout countdown indefinitely for extended reading time
  - **RIGHT button**: Immediately skips to next page, or continues from pause state
  - **Smart messages**: Context-aware prompts show available actions based on current state
  - **Pause persistence**: Timeout can be paused indefinitely until user resumes
  - Works in both `PAGING_WAIT_TIMEOUT` and `PAGING_WAIT_BOTH` modes
- **NEW FEATURE**: Added notification system to ContentScreen for user feedback
  - **Temporary messages**: Notifications replace footer area with yellow background and black text
  - **Auto-expiration**: Configurable duration (default 3 seconds) with automatic footer restoration
  - **Memory efficient**: `notify()` for dynamic text, `notifyF()` for FlashString static text
  - **Manual control**: `isNotificationActive()` to check state, `dismissNotification()` for early dismissal
  - Complete API: `notify()`, `notifyF()`, `isNotificationActive()`, `dismissNotification()`
- **NEW FEATURE**: Added blocking alert and confirmation dialog system to ContentScreen
  - **Alert dialogs**: `alert()` and `alertF()` for critical messages requiring acknowledgment (cyan background)
  - **Confirmation dialogs**: `confirm()` and `confirmF()` for user decision prompts (magenta background)
  - **Blocking execution**: Functions halt program flow until user responds with LEFT/RIGHT buttons
  - **Visual design**: Magenta background with black text for high attention and urgency
  - **Custom button labels**: Configurable left/right button text (default: "Cancel"/"OK")
  - **Return values**: `CONFIRM_LEFT`/`CONFIRM_RIGHT` enum for decision handling
  - **Memory efficient**: FlashString variants available for static text
  - **Multi-step workflows**: Chainable dialogs for complex confirmation sequences
  - **Safe defaults**: Graceful handling on small displays and allocation failures
  - **Performance optimized**: Only redraws footer area when dialogs close (not entire screen)
- Added `refreshMenu()` method to MenuScreen class for efficient menu content updates
