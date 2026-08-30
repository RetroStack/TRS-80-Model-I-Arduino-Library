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

## 20th August 2025 - Updated Release (1.3.0)

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

## 21st August 2025 - Updated Release (1.3.1)

- **NEW FEATURE**: Added ButtonScreen class for horizontal button navigation
- **NEW FEATURE**: Added secondary content area support to ContentScreen
- **METHOD NAMING**: Updated layout getters for consistency: `_getHeaderY()` → `_getHeaderTop()`, `_getFooterY()` → `_getFooterTop()`, `_getProgressBarY()` → `_getProgressBarTop()`

## 21st August 2025 - LoggerScreen Buffer Feature (1.3.2)

- **NEW FEATURE**: Added rotational logging buffer to LoggerScreen
- **NEW METHODS**:
  - `setLogBufferSize(uint16_t size)` - Configure buffer size (0 = disabled)
  - `getLogBufferSize()`, `clearLogBuffer()`, `getLogBufferCount()` - Buffer management
- **BEHAVIOR**: Buffered entries automatically replay when screen opens
- **MEMORY**: Dynamic allocation, zero overhead when disabled

## 19th September 2025 - FileBrowser and File Viewer Updates (1.4.0)

- **NEW FEATURE**: Added FileBrowser class for comprehensive SD card file and directory browsing
  - **MenuScreen-based**: Built on MenuScreen framework for consistent navigation and pagination
  - **Dynamic Memory Management**: Uses dynamic arrays that grow as needed, no fixed limits
  - **Unified Constructor**: Single constructor with intelligent parameter handling supports all usage patterns
  - **Directory Navigation**: Full directory traversal with ".." entries and optional root restrictions
  - **File Type Detection**: Automatic file type recognition and appropriate viewer selection
  - **Smart Path Parsing**: Automatically parses full file paths into directory and filename components
  - **Pre-selection Support**: Can pre-select and scroll to specific files upon opening
  - **Configurable Extensions**: Customizable text file extensions for TextFileViewer integration
  - **Arduino Compatible**: No std::vector dependencies, optimized for embedded systems
- **NEW FEATURE**: Added TextFileViewer and BinaryFileViewer integration with FileBrowser
  - **Automatic Selection**: FileBrowser automatically opens appropriate viewer based on file extension
  - **Cross-References**: File viewer examples include helpful comments suggesting FileBrowser for browsing
  - **Seamless Integration**: Viewers work together to provide complete file management solution
- **NEW FEATURE**: Added dedicated SELECT button support throughout the UI framework
  - **Hardware Support**: New BUTTON_SELECT action type with full M1Shield hardware integration and debouncing
  - **Improved Navigation**: SELECT button now handles item selection in MenuScreen and ButtonScreen
  - **Consistent UX**: Unified selection behavior across all UI components using dedicated select action
  - **Input Refinement**: Menu navigation now uses SELECT for item activation, improving user experience
- **HARDWARE UPDATE**: Updated M1Shield pin assignments for new hardware revision
  - **Pin Remapping**: Updated button pins (MENU: 41, SELECT: 40, directional: A8-A11), LED pins (10-13), display pins
  - **Enhanced Features**: Added buzzer support (pin 4) and SD card support (pin 49) with dedicated methods
  - **SD Card Detection**: Added `isSDCardInserted()` method for automatic SD card detection and initialization
  - **Cassette Interface**: Updated cassette remote pins (CR1: 43, CR2: 42) with improved safety warnings
  - **Display Support**: Updated TFT reset pin (38) and chip select configuration for better display compatibility
- **BEHAVIOR IMPROVEMENT**: Refined UI input handling for better user experience
  - **MenuScreen**: Changed from directional button selection to dedicated SELECT button for item activation
  - **ButtonScreen**: Updated to use SELECT button for horizontal button activation instead of vertical navigation
  - **ConsoleScreen**: Simplified exit behavior to only use MENU button (removed LEFT button exit)
- **HARDWARE IMPROVEMENT**: Updated low-level pin configuration for new hardware revision
  - **Signal Mapping**: Updated MUX (Port H/Pin 16), WR (Port J/Pin 15), OUT (Port J/Pin 14) pin assignments
  - **Configuration Cleanup**: Removed deprecated CR1/CR2 port configuration entries
  - **Compatibility**: Maintains backward compatibility through software abstraction layer
- **HARDWARE FIX**: Corrected `PIN_WR_ON` / `PIN_WR_OFF` definitions in `port_config.h`
  - **Signal Deassertion**: `PIN_WR_OFF` was defined identically to `PIN_WR_ON`, so the WR strobe could not be deasserted through the mask; it is now the complement
- **HARDWARE**: Added shield PCB (KiCad) and enclosure (STL) design files under `Shield/` and `Case/`
- **STABILITY**: Fixed two crashes reachable through ordinary use
  - **Screen Dispatch**: `M1Shield::loop()` dispatched to `_screen` after `setScreen()` may have cleared it, so opening a file the viewer could not read jumped through a null vtable pointer
  - **Binary Viewer**: `BinaryFileViewer` kept a non-zero buffer size after a failed allocation, so the next page turn passed a null destination to `File::read()`
  - **Allocation Checks**: `FileBrowser` and `TextFileViewer` now check `new[]` results before writing through them, and `FileBrowser` no longer overruns its array on directories with more than 255 visible entries
- **FIX**: Corrected several functions that produced wrong results
  - **`Model1::fillMemory()`**: No longer writes one byte past the region; its fourth parameter is now named `length_in_bytes` in the header to match the implementation, which has always treated it as a byte count rather than an end address
  - **`Video::setX()` / `setY()`**: A cursor at exactly the viewport width or height was accepted, placing subsequent output outside video RAM
  - **`Video` tab expansion**: Tabs advanced by the distance past the previous tab stop instead of the distance to the next one
  - **Diagonal Joystick Input**: `MenuScreen` and `ButtonScreen` compared signed joystick offsets rather than magnitudes, so three of the four diagonal directions resolved to the wrong axis
  - **`TextFileViewer::nextPage()`**: Underflowed on a file with no pages
  - **Layout Math**: Clamped unsigned subtractions in `MenuScreen` rows and the `ContentScreen` confirm dialog that underflowed on narrow displays and defeated text truncation
  - **`ButtonScreen::refreshButtons()`**: Now pushes the framebuffer, so navigation is visible on SSD1306 and SH1106 panels
- **FIX**: `M1ShieldClass::begin()` was declared but never defined, so sketches calling the no-argument overload failed to link
  - **Standard Entry Point**: `begin()` is the library's no-argument initialization entry point and performs the display-independent setup; `begin(DisplayProvider&)` now calls it first
  - **Failure Handling**: `begin(DisplayProvider&)` no longer adopts a display provider whose `create()` failed
- **PACKAGING**: `library.properties` now declares its dependencies via `depends=`; previously an Arduino IDE install pulled in nothing, not even Adafruit GFX
- **PACKAGING**: `architectures` narrowed from `*` to `avr`, with an `#error` in `port_config.h` naming the ATmega2560 requirement instead of failing deep inside the port macros
- **DOCUMENTATION**: Corrected API names that never existed, including `Model1.readByte()`/`writeByte()` in the README quickstart, `M1Shield.processInput()`/`updateScreen()`/`renderScreen()`, and `beginWithDisplay()`/`updateDisplay()`
- **DOCUMENTATION**: `docs/MenuScreen.md` examples no longer cache child screens as members and delete them in the menu destructor; `setScreen()` takes ownership and deletes the outgoing menu, so that pattern freed the screen being activated
- **NEW FEATURE**: Completed the render target abstraction introduced alongside `RenderManager`
  - **Real Interface**: `RenderTarget` now declares the drawing surface itself - `getGFX()`, `getScreenWidth()`, `getScreenHeight()`, `convertColor()` and `display()` - so targets are genuinely interchangeable rather than only nameable
  - **Fan-out**: `RenderManager::displayAll()` pushes every enabled target, and `M1Shield.display()` now goes through it; `getPrimaryRenderTarget()` names target 0
  - **Delegation**: `M1Shield`'s display accessors read through the primary render target instead of the display provider, so the seam is load-bearing rather than decorative
  - **Non-nullable Provider**: `DisplayRenderTarget` takes its `DisplayProvider` by reference, removing a `getGFX()` path that deliberately returned a dereferenced null pointer
  - **Single Rule**: `isSmallDisplay()` lives once, on `RenderTarget`
  - **Unchanged for Screens**: screens keep calling `M1Shield.getGFX()` and `M1Shield.display()`; no screen code changes
