/*
 * M1TerminalScreen.h - TRS-80 Model 1 Terminal screen implementation
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef M1_TERMINAL_SCREEN_H
#define M1_TERMINAL_SCREEN_H

#include <Arduino.h>
#include "ContentScreen.h"
#include <Adafruit_GFX.h>

/**
 * @brief TRS-80 Model I terminal screen with character grid rendering and dual display support
 *
 * M1TerminalScreen provides a complete TRS-80 Model I terminal emulation with a 64x16
 * character grid display. It implements efficient incremental rendering, dual font support,
 * and split-screen viewing capabilities to authentically recreate the original TRS-80
 * terminal experience.
 *
 * ## Key Features
 *
 * - **64x16 Character Grid**: Full TRS-80 Model I display resolution
 * - **Incremental Rendering**: Only changed characters are redrawn for efficiency
 * - **Dual Buffer System**: Backing buffer + shadow buffer for change detection
 * - **Horizontal/Vertical Scrolling**: Arrow key navigation through 64-column display
 * - **Multiple Font Sets**: Switch between different character fonts
 * - **Pixel-Perfect Accuracy**: Authentic TRS-80 character rendering
 * - **Memory Efficient**: Uses differential updates to minimize redraw overhead
 *
 * ## Display Architecture
 *
 * ```
 * Full 64x16 Grid:           Scrollable View:
 * ┌─────────────────────┐    ┌──────────────────────┐
 * │ Character Grid      │    │ Viewport (adaptive)  │
 * │ 64 columns          │ →  │ ← → H. Scrollable    │
 * │ 16 rows             │    │ ↑ ↓ V. Scrollable    │
 * │ 1024 total chars    │    │ Size: Content Area   │
 * └─────────────────────┘    └──────────────────────┘
 * ```
 *
 * ## Buffer Management
 *
 * The terminal uses a dual-buffer system for efficient rendering:
 * - **Backing Buffer**: `_bufferedVidMem[1024]` - Current character data
 * - **Shadow Buffer**: `_writtenVidMem[1024]` - Previously rendered characters
 * - **Change Detection**: Compares buffers to identify characters needing updates
 *
 * ## Memory Layout
 *
 * ```
 * Video Memory Layout (1024 bytes):
 * Row 0:  [0-63]   - Characters 0-63 (top row)
 * Row 1:  [64-127] - Characters 64-127
 * ...
 * Row 15: [960-1023] - Characters 960-1023 (bottom row)
 *
 * Index Calculation: index = (y * 64) + x
 * ```
 *
 * ## Performance Optimization
 *
 * - **Incremental Updates**: Only changed characters trigger pixel updates
 * - **Line-by-Line Rendering**: Characters updated one pixel line at a time
 * - **Differential Drawing**: Only modified pixels within characters are redrawn
 * - **Frame-Rate Control**: Updates distributed across multiple frames for smooth animation
 * - **Horizontal/Vertical Scrolling**: Arrow keys scroll viewport dynamically through 64-column display based on content area width
 * - **Viewport Culling**: Only renders characters within visible display area
 *
 * ## Font System
 *
 * Supports multiple TRS-80 character sets:
 * - Standard ASCII characters
 * - TRS-80 specific graphics characters
 *
 * ## Model1 Integration
 *
 * The terminal can automatically load content from the Model1 system:
 * - Reads video memory from Model1 (0x3C00-0x3FFF) when test signal is active
 * - Syncs with actual TRS-80 Model I display state
 * - Falls back to demo pattern when Model1 is not available
 *
 * @see ContentScreen For base screen functionality
 * @see Screen For screen interface
 */
class M1TerminalScreen : public ContentScreen
{
private:
    uint8_t _bufferedVidMem[1024]; // Video memory buffer storing current character data (64x16 = 1024 bytes)
    uint8_t _writtenVidMem[1024];  // Shadow buffer tracking previously rendered characters for change detection

    int _currentUpdateIndex; // Current index for incremental character updates (0-1023)

    uint8_t _xCoordinate; // Current X coordinate for character positioning (0-63)
    uint8_t _yCoordinate; // Current Y coordinate for character positioning (0-15)

    uint8_t _fontIndex; // Current font index for character set selection

    uint8_t _horizontalScrollOffset; // Horizontal scroll offset
    uint8_t _verticalScrollOffset;   // Vertical scroll offset

    unsigned long _model1VideoLoadTime; // Timestamp for when Model1 video memory was loaded

    int _redrawIndex; // Index for full screen redraw operations

    uint16_t _contentLeft;
    uint16_t _contentTop;
    uint16_t _contentWidth;
    uint16_t _contentHeight;

    /**
     * @brief Extract a specific pixel line from a character's bitmap data
     *
     * Retrieves the pixel pattern for a specific horizontal line within a character,
     * supporting both normal characters and graphics characters with proper font
     * selection and coordinate mapping.
     *
     * @param charIndex Character code from video memory (0-255)
     * @param graphicIndex Graphics character index for block/line characters
     * @param y Pixel line within the character (0-7 for 8-pixel tall characters)
     * @param graphicY Y coordinate for graphics character positioning
     * @return 8-bit pixel pattern for the requested line (bit 7 = leftmost pixel)
     *
     * @note Handles font selection and character set mapping
     * @note Supports two TRS-80 specific character sets
     */
    uint8_t _getPixelLine(uint8_t charIndex, uint8_t graphicIndex, uint8_t y, uint8_t graphicY);

    /**
     * @brief Update character display at terminal coordinates with change detection
     *
     * Renders a character at the specified terminal position, comparing against
     * the shadow buffer to determine if an update is needed. Handles coordinate
     * conversion and pixel-level change detection for efficient updates.
     *
     * @param terminalX Terminal X coordinate (0-63)
     * @param terminalY Terminal Y coordinate (0-15)
     * @param index Character code to display
     * @param previousIndex Previously displayed character code for comparison
     *
     * @note Only updates pixels that have actually changed
     * @note Handles horizontal scrolling coordinate mapping based on _horizontalScrollOffset
     */
    void _updateXY(uint16_t terminalX, uint16_t terminalY, uint8_t index, uint8_t previousIndex);

    /**
     * @brief Update a single pixel line within a character with differential rendering
     *
     * Renders one horizontal line of pixels within a character, comparing new and
     * previous pixel patterns to only update changed pixels. This method provides
     * the finest level of rendering granularity for maximum efficiency.
     *
     * @param gfx Reference to Adafruit_GFX object for pixel drawing
     * @param currentPixelLine New pixel pattern (8 bits representing 8 horizontal pixels)
     * @param previousPixelLine Previous pixel pattern for comparison
     * @param screenX Screen X coordinate for the pixel line
     * @param screenY Screen Y coordinate for the pixel line
     * @param y Pixel line offset within the character (0-7)
     *
     * @note Each bit in currentPixelLine/previousPixelLine represents one pixel (bit 7 = leftmost)
     * @note Only changed pixels are actually drawn to the display
     */
    void _updatePixelLine(Adafruit_GFX &gfx, uint8_t currentPixelLine, uint8_t previousPixelLine, int16_t screenX, int16_t screenY, uint16_t y);

protected:
    /**
     * @brief Process the next character in the incremental update cycle
     *
     * Advances the update system to the next character in the video memory,
     * performing change detection and rendering as needed. This method is
     * called each frame to distribute rendering work across multiple frames.
     *
     * @note Wraps around after processing all 1024 characters
     * @note Updates _currentUpdateIndex, _xCoordinate, and _yCoordinate
     */
    void _updateNext();

    /**
     * @brief Render terminal content within the designated content area
     *
     * Implementation of ContentScreen's pure virtual method. Handles the
     * incremental character rendering system, processing a limited number
     * of characters per frame to maintain smooth animation performance.
     *
     * @note Called automatically by ContentScreen's rendering system
     * @note Processes characters incrementally rather than full-screen updates
     */
    void _drawContent() override;

    /**
     * @brief Initiate a full screen redraw process
     *
     * Starts a full redraw operation that will be completed over multiple
     * frames.
     *
     * @note Redraw occurs incrementally over multiple frames
     */
    void _redraw();

    /**
     * @brief Scroll the viewport left by up to 5 characters
     *
     * Moves the viewport left within the 64-column terminal display, adapting
     * to the available content area width. The scroll amount is limited to
     * maintain viewport boundaries and provide smooth scrolling with a maximum
     * step of 5 characters.
     *
     * @note Scroll offset is clamped to valid range (0 to max displayable columns)
     * @note Maximum scroll dynamically calculated from content area width
     * @note Triggers redraw to update display with new viewport
     */
    void _scrollLeft();

    /**
     * @brief Scroll the viewport right by up to 5 characters
     *
     * Moves the viewport right within the 64-column terminal display, adapting
     * to the available content area width. The scroll amount is limited to
     * maintain viewport boundaries and provide smooth scrolling with a maximum
     * step of 5 characters.
     *
     * @note Scroll offset is clamped to valid range (0 to max displayable columns)
     * @note Maximum scroll dynamically calculated from content area width
     * @note Triggers redraw to update display with new viewport
     */
    void _scrollRight();

    /**
     * @brief Scroll the viewport up by up to 3 rows
     *
     * Moves the viewport up within the 16-row terminal display based on
     * available content area height. The scroll amount is calculated dynamically
     * and limited to maintain viewport boundaries.
     *
     * @note Scroll offset is clamped to valid range (0 to max displayable rows)
     * @note Triggers redraw to update display with new viewport
     * @note Scroll amount depends on content area height
     */
    void _scrollUp();

    /**
     * @brief Scroll the viewport down by up to 3 rows
     *
     * Moves the viewport down within the 16-row terminal display based on
     * available content area height. The scroll amount is calculated dynamically
     * and limited to maintain viewport boundaries.
     *
     * @note Scroll offset is clamped to valid range (0 to max displayable rows)
     * @note Triggers redraw to update display with new viewport
     * @note Scroll amount depends on content area height
     */
    void _scrollDown();

    /**
     * @brief Cycle to the next available font set
     *
     * Advances to the next character font in the available font collection.
     * Allows switching between different TRS-80 character sets and styles.
     *
     * @note Cycles through all available fonts and wraps to first
     * @note May trigger partial redraw to show font changes
     */
    void _nextFont();

    /**
     * @brief Load video memory content from Model1 system
     *
     * Reads the TRS-80 Model I video memory (0x3C00-0x3FFF) from the Model1
     * system and copies it to the terminal's video buffer. This synchronizes
     * the terminal display with the actual Model1 state.
     *
     * ## Integration Process
     * 1. Check if Model1 test signal is active
     * 2. Read 1024 bytes from Model1 video memory range
     * 3. Copy data to local video buffer
     * 4. Mark display for redraw to show new content
     *
     * @note Only loads when Model1 test signal is active
     * @note Called automatically during initialization when index 1 is reached
     * @note Falls back to demo pattern if Model1 is unavailable
     */
    void _loadFromModel1();

public:
    /**
     * @brief Constructor initializing terminal with default state
     *
     * Sets up the terminal with cleared video memory, reset coordinates,
     * and default display settings (left view, font 0).
     */
    M1TerminalScreen();

    /**
     * @brief Virtual destructor for proper cleanup
     */
    virtual ~M1TerminalScreen() = default;

    // Screen Interface Implementation

    /**
     * @brief Main update loop for terminal processing
     *
     * Handles incremental character rendering and any terminal-specific
     * processing that needs to occur each frame. Called repeatedly by
     * the main application loop.
     *
     * @note Delegates to ContentScreen for basic screen management
     * @note Processes incremental character updates for smooth performance
     */
    void loop() override;

    /**
     * @brief Handle user input for terminal control and navigation
     *
     * Processes input actions for terminal-specific controls including
     * horizontal scrolling, font changes, and other terminal functions.
     *
     * @param action Input action flags indicating what occurred
     * @param offsetX Horizontal offset for analog input
     * @param offsetY Vertical offset for analog input
     * @return Screen navigation result (nullptr = stay, other = navigate)
     *
     * @note Supports arrow key scrolling and terminal control
     * @note May trigger display scrolling or font switching
     */
    Screen *actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY) override;
};

#endif /* M1_TERMINAL_SCREEN_H */