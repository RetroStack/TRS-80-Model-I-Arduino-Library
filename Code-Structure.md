# Code Structure Reference

This document provides a comprehensive reference of all classes and functions available in the M1Shield Arduino Library.

## M1ShieldClass (M1Shield.h)

- `M1ShieldClass()` // Default constructor
- `~M1ShieldClass()` // Destructor with cleanup
- `bool begin(DisplayProvider &provider)` // Initialize hardware and display
- `void activateJoystick()` // Enable joystick input
- `void deactivateJoystick()` // Disable joystick input
- `bool isDisplayInitialized() const` // Check display initialization status
- `Adafruit_GFX& getGFX()` // Get direct graphics context
- `uint16_t getScreenWidth() const` // Get display width in pixels
- `uint16_t getScreenHeight() const` // Get display height in pixels
- `DisplayProvider& getDisplayProvider() const` // Get display provider reference
- `bool display()` // Update/refresh display
- `uint16_t convertColor(uint16_t color)` // Convert color for current display
- `bool setScreen(Screen *screen)` // Set active screen with lifecycle management
- `void setLogger(ILogger &logger)` // Set logger for debugging output
- `ILogger* getLogger() const` // Get logger instance
- `void setLEDColor(uint8_t r, uint8_t g, uint8_t b) const` // Set RGB LED with individual channels
- `void setLEDColor(LEDColor color, uint8_t intensity = 255) const` // Set LED with predefined color
- `bool isMenuPressed() const` // Check menu button current state
- `bool isLeftPressed() const` // Check left button current state
- `bool isRightPressed() const` // Check right button current state
- `bool isUpPressed() const` // Check up button current state
- `bool isDownPressed() const` // Check down button current state
- `bool isJoystickPressed() const` // Check joystick button current state
- `bool wasMenuPressed()` // Check and consume menu button press event
- `bool wasLeftPressed()` // Check and consume left button press event
- `bool wasRightPressed()` // Check and consume right button press event
- `bool wasUpPressed()` // Check and consume up button press event
- `bool wasDownPressed()` // Check and consume down button press event
- `bool wasJoystickPressed()` // Check and consume joystick button press event
- `JoystickDirection getJoystickDirection() const` // Get 8-directional joystick state
- `bool isJoystickCentered() const` // Check if joystick is in center position
- `uint8_t getJoystickX() const` // Get raw X-axis analog value (0-255)
- `uint8_t getJoystickY() const` // Get raw Y-axis analog value (0-255)
- `void setCR1Mode(bool isOutput) const` // Configure CR1 pin as input/output
- `void setCR2Mode(bool isOutput) const` // Configure CR2 pin as input/output
- `void writeCR1(bool value) const` // Write digital value to CR1 pin
- `void writeCR2(bool value) const` // Write digital value to CR2 pin
- `bool readCR1() const` // Read digital value from CR1 pin
- `bool readCR2() const` // Read digital value from CR2 pin
- `void writeCassetteIn(uint8_t value) const` // Write analog to Model 1 cassette input
- `uint16_t readCassetteOut() const` // Read analog from Model 1 cassette output
- `void loop()` // Main update loop for all shield operations

**Enums:**

- `enum LEDColor` // RGB LED color definitions: BLACK, WHITE, RED, GREEN, BLUE, YELLOW, CYAN, MAGENTA
- `enum JoystickDirection` // 8-directional joystick states: CENTER, UP, DOWN, LEFT, RIGHT, UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT

## Model1Class (Model1.h)

- `Model1Class()` // Constructor
- `void begin()` // Initialize TRS-80 interface
- `void end()` // Deinitialize TRS-80 interface
- `void setLogger(ILogger &logger)` // Set logger for debugging
- `uint8_t readMemory(uint16_t address)` // Read byte from memory address
- `void writeMemory(uint16_t address, uint8_t data)` // Write byte to memory address
- `void readMemory(uint16_t address, uint8_t *buffer, uint16_t length)` // Read buffer from memory
- `void writeMemory(uint16_t address, const uint8_t *buffer, uint16_t length)` // Write buffer to memory
- `void fillMemory(uint16_t address, uint8_t data, uint16_t length)` // Fill memory range with value
- `void copyMemory(uint16_t sourceAddress, uint16_t destinationAddress, uint16_t length)` // Copy memory range
- `bool verifyMemory(uint16_t address, const uint8_t *buffer, uint16_t length, uint16_t *errorAddress = nullptr)` // Verify memory contents
- `void clearMemory(uint16_t address, uint16_t length)` // Clear memory range to zero
- `uint8_t readIO(uint8_t address)` // Read from I/O port
- `void writeIO(uint8_t address, uint8_t data)` // Write to I/O port
- `bool readSystemResetSignal()` // Read system reset signal state
- `bool readInterruptAcknowledgeSignal()` // Read interrupt acknowledge signal state
- `bool triggerInterrupt(uint8_t interrupt, uint16_t timeout = 1000)` // Trigger interrupt with timeout
- `void activateInterruptRequestSignal()` // Activate interrupt request signal
- `void deactivateInterruptRequestSignal()` // Deactivate interrupt request signal
- `bool hasActiveTestSignal()` // Check if test signal is active
- `void activateTestSignal()` // Activate test signal (enables bus access)
- `void deactivateTestSignal()` // Deactivate test signal
- `void activateWaitSignal()` // Activate wait signal
- `void deactivateWaitSignal()` // Deactivate wait signal
- `void nextUpdate()` // Execute next system update cycle
- `char* getState()` // Get human-readable state string
- `uint64_t getStateData()` // Get packed state data
- `uint64_t getStateConfigData()` // Get packed configuration data
- `void logState()` // Log current state to logger
- `uint8_t getVersionMajor()` // Get major version number
- `uint8_t getVersionMinor()` // Get minor version number
- `uint8_t getVersionRevision()` // Get revision number
- `char* getVersion()` // Get version string
- `void printMemoryContents(uint16_t start, uint16_t length, PRINT_STYLE style = BOTH, bool relative = false, uint16_t bytesPerLine = 32)` // Print memory contents to Serial
- `void printMemoryContents(Print &output, uint16_t start, uint16_t length, PRINT_STYLE style = BOTH, bool relative = false, uint16_t bytesPerLine = 32)` // Print memory contents to custom output

**Enums:**

- `enum PRINT_STYLE` // Memory display formatting: HEXADECIMAL, ASCII, BOTH

## Screen (Screen.h)

- `Screen()` // Default constructor (inactive state)
- `virtual ~Screen()` // Virtual destructor for proper cleanup
- `bool isActive() const` // Check if screen is currently active
- `bool isSmallDisplay() const` // Check if display is small (height <= 128px)
- `void setLogger(ILogger &logger)` // Set logger for debugging output
- `ILogger* getLogger() const` // Get logger instance
- `void setTitle(const char *title)` // Set screen title/name
- `void setTitle(String title)` // Set screen title from Arduino String
- `void setTitleF(const __FlashStringHelper *title)` // Set screen title from FlashString
- `void clearTitle()` // Clear current title
- `const char* getTitle() const` // Get current screen title
- `void refresh()` // Force complete screen redraw
- `virtual bool open()` // Activate screen and perform initial setup
- `virtual void close()` // Deactivate screen and cleanup
- `virtual void _drawScreen() = 0` // Pure virtual: initial screen rendering
- `virtual void loop() = 0` // Pure virtual: main update loop
- `virtual Screen* actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY) = 0` // Pure virtual: input handling

**Enums:**

- `enum ActionTaken : uint16_t` // Input action bit flags: BUTTON_LEFT, BUTTON_RIGHT, BUTTON_UP, BUTTON_DOWN, BUTTON_MENU, JOYSTICK_LEFT, JOYSTICK_RIGHT, JOYSTICK_UP, JOYSTICK_DOWN, JOYSTICK_BUTTON

## ContentScreen (ContentScreen.h)

- `ContentScreen()` // Constructor with structured layout initialization
- `virtual ~ContentScreen()` // Destructor with memory cleanup
- `virtual void loop() = 0` // Pure virtual: main update loop for content logic
- `virtual Screen* actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY) = 0` // Pure virtual: input handler within structured layout
- `void setProgressValue(int value)` // Set progress bar value (0-100)
- `uint8_t getProgressValue() const` // Get current progress bar value
- `void setButtonItems(const char** buttonItems, uint8_t buttonItemCount)` // Set button labels for footer with dynamic allocation
- `void setButtonItemsF(const __FlashStringHelper** buttonItems, uint8_t buttonItemCount)` // Set button labels from FlashString array
- `void clearButtonItems()` // Clear all button labels and free memory
- `void clearContentArea()` // Clear content area to background color
- `void drawText(uint16_t x, uint16_t y, const char* text, uint16_t color, uint8_t size = 1)` // Draw text within content area with clipping
- `void drawText(uint16_t x, uint16_t y, String text, uint16_t color, uint8_t size = 1)` // Draw text in content area from String object
- `void drawTextF(uint16_t x, uint16_t y, const __FlashStringHelper* text, uint16_t color, uint8_t size = 1)` // Draw text from FlashString
- `void notify(const char* text, unsigned long durationMs = 3000, uint16_t backgroundColor = 0xFFE0)` // Show notification with customizable background color (default: yellow)
- `void notify(String text, unsigned long durationMs = 3000, uint16_t backgroundColor = 0xFFE0)` // Show notification from String object with custom background
- `void notifyF(const __FlashStringHelper* text, unsigned long durationMs = 3000, uint16_t backgroundColor = 0xFFE0)` // Show notification from FlashString with custom background
- `bool isNotificationActive() const` // Check if notification is currently active
- `void dismissNotification()` // Manually dismiss current notification
- `void alert(const char* text)` // Show blocking alert dialog with cyan background
- `void alert(String text)` // Show blocking alert from String object
- `void alertF(const __FlashStringHelper* text)` // Show blocking alert from FlashString
- `ConfirmResult confirm(const char* text, const char* leftText = "Cancel", const char* rightText = "OK")` // Show blocking confirmation dialog with magenta background
- `ConfirmResult confirm(String text, String leftText, String rightText)` // Show confirmation with String objects
- `ConfirmResult confirmF(const __FlashStringHelper* text, const __FlashStringHelper* leftText, const __FlashStringHelper* rightText)` // Show confirmation with FlashStrings

**Enums:**

- `enum ConfirmResult` // Confirmation dialog result: CONFIRM_LEFT, CONFIRM_RIGHT

## MenuScreen (MenuScreen.h)

- `MenuScreen()` // Constructor with default menu state and button labels
- `virtual ~MenuScreen()` // Destructor with dynamic memory cleanup
- `virtual void loop()` // Main loop processing for menu screen updates
- `Screen* actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY)` // Handle user input for menu navigation (up/down/select/back)
- `void setMenuItems(const char** menuItems, uint8_t menuItemCount)` // Set menu items from C-string array with deep copy
- `void setMenuItems(String* menuItems, uint8_t menuItemCount)` // Set menu items from String array with deep copy
- `void setMenuItemsF(const __FlashStringHelper** menuItems, uint8_t menuItemCount)` // Set menu items from FlashString array with deep copy
- `void clearMenuItems()` // Clear and free all dynamically allocated menu items
- `virtual Screen* _getSelectedMenuItemScreen(int index) = 0` // Pure virtual: get screen for selected menu item
- `virtual const char* _getMenuItemConfigValue(uint8_t index)` // Optional: get configuration value string for menu item
- `virtual const __FlashStringHelper* _getMenuItemConfigValueF(uint8_t index)` // Optional: get configuration FlashString for menu item
- `virtual bool _isMenuItemEnabled(uint8_t index) const` // Optional: check if menu item is enabled/selectable

## ConsoleScreen (ConsoleScreen.h)

- `ConsoleScreen()` // Constructor with default console settings
- `virtual ~ConsoleScreen()` // Destructor
- `bool open()` // Override to initialize timing for one-time execution
- `void loop()` // Main loop processing for console screen updates
- `Screen* actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY)` // Handle user input actions with standard navigation
- `size_t write(uint8_t c)` // Write single character to console (Print interface requirement)
- `size_t write(const uint8_t* buffer, size_t size)` // Write buffer of characters for bulk operations
- `void cls()` // Clear screen and reset cursor to top-left
- `void refresh()` // Refresh screen and reset cursor to top-left
- `void setTextColor(uint16_t foreground, uint16_t background = 0x0000)` // Set text color for subsequent output
- `void setConsoleBackground(uint16_t color)` // Set console background color
- `void setTextSize(uint8_t size)` // Set text size multiplier for subsequent output
- `void setTabSize(uint8_t size)` // Set tab size in characters
- `void setPagingMode(ConsolePagingMode mode)` // Set console paging behavior mode
- `void setPagingTimeout(uint16_t timeoutMs)` // Set paging timeout duration
- `ConsolePagingMode getPagingMode() const` // Get current paging mode
- `uint16_t getPagingTimeout() const` // Get current paging timeout
- `bool isWaitingForPaging() const` // Check if console is waiting for paging action
- `void continuePaging()` // Manually trigger page continuation
- `void setAutoForward(bool enabled, unsigned long delayMs = 5000)` // Enable/disable auto-forward functionality
- `bool isAutoForwardEnabled() const` // Check if auto-forward is enabled
- `unsigned long getAutoForwardDelay() const` // Get current auto-forward delay
- `virtual void _executeOnce()` // Optional one-time execution method called 1 second after console opens

**Enums:**

- `enum ConsolePagingMode` // Auto-paging behavior: PAGING_AUTO_CLEAR, PAGING_WAIT_TIMEOUT, PAGING_WAIT_BUTTON, PAGING_WAIT_BOTH

## LoggerScreen (LoggerScreen.h)

- `LoggerScreen(const char* title = "Logger")` // Constructor with optional title
- `virtual ~LoggerScreen()` // Destructor
- `void setTimestampEnabled(bool enabled)` // Enable/disable timestamp display in log messages
- `bool isTimestampEnabled() const` // Check if timestamps are enabled
- `void setColorCodingEnabled(bool enabled)` // Enable/disable color coding for different log levels
- `bool isColorCodingEnabled() const` // Check if color coding is enabled
- `void resetTimestamp()` // Reset timestamp reference point to current time
- `void info(const char* fmt, ...)` // Log info message with printf-style formatting
- `void warn(const char* fmt, ...)` // Log warning message with printf-style formatting
- `void err(const char* fmt, ...)` // Log error message with printf-style formatting
- `void debug(const char* fmt, ...)` // Log debug message with printf-style formatting
- `ILogger* asLogger()` // Get ILogger adapter for CompositeLogger compatibility

## Keyboard (Keyboard.h)

- `Keyboard()` // Constructor
- `void setLogger(ILogger& logger)` // Set logger for debugging output
- `bool isKeyPressed() const` // Check if any key is currently pressed
- `void update()` // Update keyboard state and detect changes
- `uint8_t getFirstJustPressedKey()` // Get first key that was just pressed
- `KeyboardChangeIterator changes()` // Get iterator for recent keyboard changes

## KeyboardChangeIterator (KeyboardChangeIterator.h)

- `KeyboardChangeIterator(const uint8_t* previous, const uint8_t* current)` // Constructor with previous and current keyboard states
- `bool hasNext() const` // Check if more changes are available
- `void next()` // Advance to next keyboard change
- `uint8_t keyIndex() const` // Get current key index in iteration
- `uint8_t row() const` // Get keyboard row for current key
- `uint8_t column() const` // Get keyboard column for current key
- `uint8_t keyValue() const` // Get ASCII value for current key
- `String keyName() const` // Get human-readable name for current key
- `bool wasPressed() const` // Check if key was pressed in previous state
- `bool isPressed() const` // Check if key is pressed in current state
- `bool wasJustPressed() const` // Check if key was just pressed (transition from released to pressed)
- `bool wasReleased() const` // Check if key was just released (transition from pressed to released)
- `bool isShiftPressed() const` // Check if shift key is currently pressed

## Video (Video.h)

- `Video()` // Constructor
- `void setLogger(ILogger& logger)` // Set logger for debugging output
- `void setViewPort(ViewPort viewPort)` // Set display viewport area
- `uint16_t getRowAddress(uint8_t y)` // Get video memory address for row
- `uint16_t getColumnAddress(uint16_t rowAddress, uint8_t x)` // Get column address within row
- `uint16_t getAddress(uint8_t x, uint8_t y)` // Get video memory address for coordinates
- `uint8_t getX()` // Get current cursor X position
- `void setX(uint8_t x)` // Set cursor X position
- `uint8_t getY()` // Get current cursor Y position
- `void setY(uint8_t y)` // Set cursor Y position
- `void setXY(uint8_t x, uint8_t y)` // Set cursor X and Y position
- `uint8_t getStartX()` // Get viewport start X coordinate
- `uint8_t getEndX()` // Get viewport end X coordinate
- `uint8_t getStartY()` // Get viewport start Y coordinate
- `uint8_t getEndY()` // Get viewport end Y coordinate
- `uint8_t getWidth()` // Get viewport width
- `uint8_t getHeight()` // Get viewport height
- `uint16_t getSize()` // Get total viewport size
- `uint8_t getAbsoluteX(uint8_t x)` // Convert relative to absolute X coordinate
- `uint8_t getAbsoluteY(uint8_t y)` // Convert relative to absolute Y coordinate
- `void cls()` // Clear screen with spaces
- `void cls(char character)` // Clear screen with specific character
- `void cls(char* characters)` // Clear screen with character array
- `void cls(char* characters, uint16_t length)` // Clear screen with character array of specific length
- `void scroll()` // Scroll screen up by one row
- `void scroll(uint8_t rows)` // Scroll screen up by specified rows
- `size_t write(uint8_t ch)` // Write single character (Print interface)
- `size_t write(const uint8_t* buffer, size_t size)` // Write character buffer (Print interface)
- `void print(const char character, bool raw)` // Print character with optional raw mode
- `void print(uint8_t x, uint8_t y, const char* str)` // Print string at specific coordinates
- `void print(uint8_t x, uint8_t y, const char* str, uint16_t length)` // Print string with length limit
- `char* read(uint8_t x, uint8_t y, uint16_t length, bool raw)` // Read characters from video memory
- `void setAutoScroll(bool autoScroll)` // Enable/disable automatic scrolling
- `void setLowerCaseMod(bool hasLowerCaseMod)` // Configure lowercase character support
- `char convertModel1CharacterToLocal(char character)` // Convert Model 1 character to local encoding
- `char convertLocalCharacterToModel1(char character)` // Convert local character to Model 1 encoding

**Structs:**

- `struct ViewPort` // Viewport area definition with x, y, width, height

## Cassette (Cassette.h)

- `Cassette()` // Constructor
- `void setLogger(ILogger& logger)` // Set logger for debugging output
- `void update()` // Update cassette state
- `void writeRaw(bool value1, bool value2)` // Write raw values to cassette interface
- `bool readRaw()` // Read raw value from cassette interface
- `void play(uint16_t frequency, uint32_t duration)` // Play tone at specific frequency and duration
- `void playSong(int* melody, float* durations, size_t numNotes, int bpm)` // Play song from RAM arrays
- `void playSongPGM(const int* melody, const float* durations, size_t numNotes, int bpm)` // Play song from PROGMEM arrays
- `void activateRemote()` // Activate cassette remote control
- `void deactivateRemote()` // Deactivate cassette remote control
- `void setCharGenA()` // Set character generator to set A
- `void setCharGenB()` // Set character generator to set B
- `bool is64CharacterMode()` // Check if in 64-character display mode
- `void set32CharacterMode()` // Set 32-character display mode
- `void set64CharacterMode()` // Set 64-character display mode

## ROM (ROM.h)

- `ROM()` // Constructor
- `void setLogger(ILogger& logger)` // Set logger for debugging output
- `uint16_t getROMStartAddress(uint8_t rom)` // Get start address for ROM number
- `uint16_t getROMLength(uint8_t rom)` // Get length for ROM number
- `uint32_t getChecksum(uint8_t rom)` // Calculate checksum for ROM number
- `const __FlashStringHelper* identifyROM()` // Identify ROM type from checksum
- `void printROMContents(uint8_t rom, PRINT_STYLE style = BOTH, bool relative = true, uint16_t bytesPerLine = 32)` // Print ROM contents to Serial

## AddressBus (AddressBus.h)

- `AddressBus()` // Constructor
- `void begin()` // Initialize address bus
- `void end()` // Deinitialize address bus
- `void setLogger(ILogger& logger)` // Set logger for debugging output
- `bool isReadable()` // Check if bus is in read mode
- `bool isWritable()` // Check if bus is in write mode
- `void setAsReadable()` // Set bus to read mode
- `void setAsWritable()` // Set bus to write mode
- `uint16_t readMemoryAddress()` // Read 16-bit memory address from bus
- `void writeMemoryAddress(uint16_t address)` // Write 16-bit memory address to bus
- `void writeRefreshAddress(uint8_t address)` // Write refresh address to bus
- `uint8_t readIOAddress()` // Read 8-bit I/O address from bus
- `void writeIOAddress(uint8_t address)` // Write 8-bit I/O address to bus
- `char* getState()` // Get human-readable bus state string

## DataBus (DataBus.h)

- `DataBus()` // Constructor
- `void begin()` // Initialize data bus
- `void end()` // Deinitialize data bus
- `void setLogger(ILogger& logger)` // Set logger for debugging output
- `bool isReadable()` // Check if bus is in read mode
- `bool isWritable()` // Check if bus is in write mode
- `void setAsReadable()` // Set bus to read mode
- `void setAsWritable()` // Set bus to write mode
- `uint8_t read()` // Read 8-bit data from bus
- `void write(uint8_t data)` // Write 8-bit data to bus
- `char* getState()` // Get human-readable bus state string

## Model1LowLevel (Model1LowLevel.h)

- `static void configWriteRAS(uint8_t outputMode)` // Configure RAS pin as INPUT or OUTPUT
- `static void configWriteCAS(uint8_t outputMode)` // Configure CAS pin as INPUT or OUTPUT
- `static void configWriteMUX(uint8_t outputMode)` // Configure MUX pin as INPUT or OUTPUT
- `static void configWriteRD(uint8_t outputMode)` // Configure RD pin as INPUT or OUTPUT
- `static void configWriteWR(uint8_t outputMode)` // Configure WR pin as INPUT or OUTPUT
- `static void configWriteIN(uint8_t outputMode)` // Configure IN pin as INPUT or OUTPUT
- `static void configWriteOUT(uint8_t outputMode)` // Configure OUT pin as INPUT or OUTPUT
- `static void configWriteINT(uint8_t outputMode)` // Configure INT pin as INPUT or OUTPUT
- `static void configWriteTEST(uint8_t outputMode)` // Configure TEST pin as INPUT or OUTPUT
- `static void configWriteWAIT(uint8_t outputMode)` // Configure WAIT pin as INPUT or OUTPUT
- `static void configWriteSYS_RES(uint8_t outputMode)` // Configure SYS_RES pin as INPUT or OUTPUT
- `static void configWriteINT_ACK(uint8_t outputMode)` // Configure INT_ACK pin as INPUT or OUTPUT
- `static void writeRAS(uint8_t value)` // Set RAS signal HIGH or LOW
- `static void writeCAS(uint8_t value)` // Set CAS signal HIGH or LOW
- `static void writeMUX(uint8_t value)` // Set MUX signal HIGH or LOW
- `static void writeRD(uint8_t value)` // Set RD signal HIGH or LOW
- `static void writeWR(uint8_t value)` // Set WR signal HIGH or LOW
- `static void writeIN(uint8_t value)` // Set IN signal HIGH or LOW
- `static void writeOUT(uint8_t value)` // Set OUT signal HIGH or LOW
- `static void writeINT(uint8_t value)` // Set INT signal HIGH or LOW
- `static void writeTEST(uint8_t value)` // Set TEST signal HIGH or LOW
- `static void writeWAIT(uint8_t value)` // Set WAIT signal HIGH or LOW
- `static void writeSYS_RES(uint8_t value)` // Set SYS_RES signal HIGH or LOW
- `static void writeINT_ACK(uint8_t value)` // Set INT_ACK signal HIGH or LOW
- `static uint8_t configReadRAS()` // Read RAS pin configuration (INPUT/OUTPUT)
- `static uint8_t configReadCAS()` // Read CAS pin configuration (INPUT/OUTPUT)
- `static uint8_t configReadMUX()` // Read MUX pin configuration (INPUT/OUTPUT)
- `static uint8_t configReadRD()` // Read RD pin configuration (INPUT/OUTPUT)
- `static uint8_t configReadWR()` // Read WR pin configuration (INPUT/OUTPUT)
- `static uint8_t configReadIN()` // Read IN pin configuration (INPUT/OUTPUT)
- `static uint8_t configReadOUT()` // Read OUT pin configuration (INPUT/OUTPUT)
- `static uint8_t configReadINT()` // Read INT pin configuration (INPUT/OUTPUT)
- `static uint8_t configReadTEST()` // Read TEST pin configuration (INPUT/OUTPUT)
- `static uint8_t configReadWAIT()` // Read WAIT pin configuration (INPUT/OUTPUT)
- `static uint8_t configReadSYS_RES()` // Read SYS_RES pin configuration (INPUT/OUTPUT)
- `static uint8_t configReadINT_ACK()` // Read INT_ACK pin configuration (INPUT/OUTPUT)
- `static uint8_t readRAS()` // Read RAS signal current state (HIGH/LOW)
- `static uint8_t readCAS()` // Read CAS signal current state (HIGH/LOW)
- `static uint8_t readMUX()` // Read MUX signal current state (HIGH/LOW)
- `static uint8_t readRD()` // Read RD signal current state (HIGH/LOW)
- `static uint8_t readWR()` // Read WR signal current state (HIGH/LOW)
- `static uint8_t readIN()` // Read IN signal current state (HIGH/LOW)
- `static uint8_t readOUT()` // Read OUT signal current state (HIGH/LOW)
- `static uint8_t readINT()` // Read INT signal current state (HIGH/LOW)
- `static uint8_t readTEST()` // Read TEST signal current state (HIGH/LOW)
- `static uint8_t readWAIT()` // Read WAIT signal current state (HIGH/LOW)
- `static uint8_t readSYS_RES()` // Read SYS_RES signal current state (HIGH/LOW)
- `static uint8_t readINT_ACK()` // Read INT_ACK signal current state (HIGH/LOW)
- `static void configWriteAddressBus(uint16_t config)` // Configure address bus pin directions
- `static void configWriteDataBus(uint8_t config)` // Configure data bus pin directions
- `static uint16_t configReadAddressBus()` // Read address bus pin configurations
- `static uint8_t configReadDataBus()` // Read data bus pin configurations
- `static void writeAddressBus(uint16_t address)` // Write 16-bit address to address bus
- `static void writeDataBus(uint8_t data)` // Write 8-bit data to data bus
- `static uint16_t readAddressBus()` // Read 16-bit address from address bus
- `static uint8_t readDataBus()` // Read 8-bit data from data bus
- `static uint64_t getStateData()` // Get packed 64-bit state data
- `static uint64_t getStateConfigData()` // Get packed 64-bit configuration data

## ILogger (ILogger.h)

- `virtual void info(const char* fmt, ...) = 0` // Log info message with printf-style formatting
- `virtual void warn(const char* fmt, ...) = 0` // Log warning message with printf-style formatting
- `virtual void err(const char* fmt, ...) = 0` // Log error message with printf-style formatting
- `virtual void debug(const char* fmt, ...) = 0` // Log debug message with printf-style formatting
- `void info(const String& fmt, ...)` // Log info message from String object
- `void warn(const String& fmt, ...)` // Log warning message from String object
- `void err(const String& fmt, ...)` // Log error message from String object
- `void debug(const String& fmt, ...)` // Log debug message from String object

## SerialLogger (SerialLogger.h)

- `void info(const char* fmt, ...)` // Log info message to Serial
- `void warn(const char* fmt, ...)` // Log warning message to Serial
- `void err(const char* fmt, ...)` // Log error message to Serial
- `void debug(const char* fmt, ...)` // Log debug message to Serial
- `size_t write(uint8_t ch)` // Write single character to Serial
- `size_t write(const uint8_t* buffer, size_t size)` // Write character buffer to Serial
- `void mute()` // Disable all logging output
- `void unmute()` // Re-enable logging output

## CompositeLogger (CompositeLogger.h)

- `CompositeLogger()` // Constructor initializing empty logger list
- `bool addLogger(ILogger* logger)` // Add logger to composite (max 8 loggers)
- `bool removeLogger(ILogger* logger)` // Remove specific logger from composite
- `void clearLoggers()` // Remove all registered loggers
- `uint8_t getLoggerCount() const` // Get number of registered loggers
- `bool hasLogger(ILogger* logger) const` // Check if specific logger is registered
- `void info(const char* fmt, ...)` // Forward info message to all registered loggers
- `void warn(const char* fmt, ...)` // Forward warning message to all registered loggers
- `void err(const char* fmt, ...)` // Forward error message to all registered loggers
- `void debug(const char* fmt, ...)` // Forward debug message to all registered loggers
- `size_t write(uint8_t ch)` // Forward character write to all registered loggers
- `size_t write(const uint8_t* buffer, size_t size)` // Forward buffer write to all registered loggers

## DisplayProvider (DisplayProvider.h)

- `virtual bool create(int8_t cs, int8_t dc, int8_t rst) = 0` // Create display instance with SPI pins
- `virtual void destroy() = 0` // Destroy display instance and free resources
- `virtual Adafruit_GFX& getGFX() = 0` // Get Adafruit_GFX interface for drawing
- `virtual bool display() = 0` // Update display with buffered content (for buffered displays)
- `virtual uint16_t convertColor(uint16_t color) = 0` // Convert RGB565 color to display-specific format
- `virtual const char* name() const = 0` // Get display driver name
- `virtual uint16_t width() const = 0` // Get display width in pixels
- `virtual uint16_t height() const = 0` // Get display height in pixels

**Available Display Drivers:**

- `Display_ST7789_240x240, Display_ST7789_320x170, Display_ST7789_320x240, Display_ST7796, Display_ST7735, Display_ILI9341, Display_ILI9325, Display_HX8357, Display_SSD1306, Display_SH1106`

## Port Configuration (port_config.h)

Hardware pin mappings and port bit masks for TRS-80 Model 1 signal connections to Arduino Mega 2560 pins.

## Port Macros (port_macros.h)

- `pinConfigWrite(_pin, _mode)` // Configure pin as INPUT or OUTPUT using direct port manipulation
- `pinConfigRead(_pin)` // Read pin configuration state using direct port access
- `pinWrite(_pin, _value)` // Write HIGH/LOW to pin using direct port manipulation
- `pinRead(_pin)` // Read HIGH/LOW from pin using direct port access
- `busConfigWrite(_bus, _config)` // Configure entire bus direction using direct port manipulation
- `busConfigRead(_bus)` // Read entire bus configuration using direct port access
- `busWrite(_bus, _value)` // Write value to entire bus using direct port manipulation
- `busRead(_bus)` // Read value from entire bus using direct port access
- `asmShortNoop()` // Single CPU cycle delay (62.5ns @ 16MHz)
- `asmNoop()` // Two CPU cycle delay (125ns @ 16MHz)

## Utils (utils.h)

- `asmShortNoop()` // Wait exactly 1 CPU cycle (62.5ns @ 16MHz)
- `asmNoop()` // Wait exactly 2 CPU cycles (125ns @ 16MHz)
- `void asmWait(uint16_t wait)` // Assembly-based precise delay
- `void asmWait(uint16_t outerLoopCount, uint16_t innerLoopCount)` // Nested loop delay
- `char* uint8ToBinary(uint8_t value, char* buffer)` // Convert 8-bit value to binary string
- `char* uint16ToBinary(uint16_t value, char* buffer)` // Convert 16-bit value to binary string
- `char pinStatus(bool value)` // Convert boolean to pin status character
- `char busStatus(uint8_t value)` // Convert bus value to status character
