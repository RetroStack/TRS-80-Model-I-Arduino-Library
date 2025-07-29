/*
 * M1TerminalScreen.cpp - TRS-80 Model I Terminal screen implementation
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#include "M1TerminalScreen.h"
#include "Model1.h"
#include "M1Shield.h"

// ============================================================================
// Colors
// ============================================================================

constexpr uint16_t NO_DATA_COLOR_BG = 0x001F;
constexpr uint16_t NO_DATA_COLOR_FG = 0xFFE0;
constexpr uint16_t TERMINAL_COLOR_BG = 0x0000;
constexpr uint16_t TERMINAL_COLOR_FG = 0xFFFF;

constexpr uint16_t TERMINAL_PADDING = 3;

// ============================================================================
// Character Display Constants
// ============================================================================

#define CHAR_FULL_HEIGHT 12 // Full vertical space allocated to each character including padding
#define CHAR_HEIGHT 8       // Actual height of character bitmap data in pixels
#define CHAR_WIDTH 6        // Width of each character in pixels
#define CHAR_START_BIT 2    // Bit shift applied when rendering character pixels
#define CHAR_COUNT 128      // Number of ASCII characters in each font set
#define CHAR_FONTS 2        // Number of available font sets

// ============================================================================
// TRS-80 Character Font Data
// ============================================================================

/**
 * @brief TRS-80 Model I character font bitmaps stored in program memory
 *
 * Contains two complete font sets, each with 128 characters represented as
 * 8-byte vertical scanlines. Each character is 6 pixels wide by 8 pixels tall.
 * Font data is stored in PROGMEM to conserve RAM on Arduino platforms.
 *
 * ## Font Structure
 * - Font 0: Standard TRS-80 character set
 * - Font 1: Alternative/graphics character set
 * - Each character: 8 bytes (one per horizontal scanline)
 * - Each byte represents 6 pixels (bits 5-0, MSB first)
 *
 * ## Memory Layout
 * ```
 * characters[font][char][scanline] = pixel_data
 * Font:     0-1 (two fonts available)
 * Char:     0-127 (ASCII range)
 * Scanline: 0-7 (top to bottom)
 * ```
 *
 * @note Total size: 2 fonts × 128 chars × 8 bytes = 2,048 bytes in PROGMEM
 */
static const unsigned char PROGMEM characters[CHAR_FONTS][CHAR_COUNT][CHAR_HEIGHT] =
    {
        {{14, 17, 1, 13, 21, 21, 14, 0},
         {4, 10, 17, 17, 31, 17, 17, 0},
         {30, 9, 9, 14, 9, 9, 30, 0},
         {14, 17, 16, 16, 16, 17, 14, 0},
         {30, 9, 9, 9, 9, 9, 30, 0},
         {31, 16, 16, 28, 16, 16, 31, 0},
         {31, 16, 16, 28, 16, 16, 16, 0},
         {15, 16, 16, 19, 17, 17, 15, 0},
         {17, 17, 17, 31, 17, 17, 17, 0},
         {14, 4, 4, 4, 4, 4, 14, 0},
         {1, 1, 1, 1, 1, 17, 14, 0},
         {17, 18, 20, 24, 20, 18, 17, 0},
         {16, 16, 16, 16, 16, 16, 31, 0},
         {17, 27, 21, 21, 17, 17, 17, 0},
         {17, 25, 21, 19, 17, 17, 17, 0},
         {14, 17, 17, 17, 17, 17, 14, 0},
         {30, 17, 17, 30, 16, 16, 16, 0},
         {14, 17, 17, 17, 21, 18, 13, 0},
         {30, 17, 17, 30, 20, 18, 17, 0},
         {14, 17, 16, 14, 1, 17, 14, 0},
         {31, 4, 4, 4, 4, 4, 4, 0},
         {17, 17, 17, 17, 17, 17, 14, 0},
         {17, 17, 17, 10, 10, 4, 4, 0},
         {17, 17, 17, 17, 21, 27, 17, 0},
         {17, 17, 10, 4, 10, 17, 17, 0},
         {17, 17, 10, 4, 4, 4, 4, 0},
         {31, 1, 2, 4, 8, 16, 31, 0},
         {4, 14, 21, 4, 4, 4, 4, 0},
         {4, 4, 4, 4, 21, 14, 4, 0},
         {0, 4, 8, 31, 8, 4, 0, 0},
         {0, 4, 2, 31, 2, 4, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 31},
         {0, 0, 0, 0, 0, 0, 0, 0},
         {4, 4, 4, 4, 4, 0, 4, 0},
         {10, 10, 10, 0, 0, 0, 0, 0},
         {10, 10, 31, 10, 31, 10, 10, 0},
         {4, 15, 20, 14, 5, 30, 4, 0},
         {24, 25, 2, 4, 8, 19, 3, 0},
         {8, 20, 20, 8, 21, 18, 13, 0},
         {12, 12, 8, 16, 0, 0, 0, 0},
         {2, 4, 8, 8, 8, 4, 2, 0},
         {8, 4, 2, 2, 2, 4, 8, 0},
         {4, 21, 14, 31, 14, 21, 4, 0},
         {0, 4, 4, 31, 4, 4, 0, 0},
         {0, 0, 0, 0, 12, 12, 8, 16},
         {0, 0, 0, 31, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 12, 12, 0},
         {0, 1, 2, 4, 8, 16, 0, 0},
         {14, 17, 19, 21, 25, 17, 14, 0},
         {4, 12, 4, 4, 4, 4, 14, 0},
         {14, 17, 1, 14, 16, 16, 31, 0},
         {14, 17, 1, 6, 1, 17, 14, 0},
         {2, 6, 10, 18, 31, 2, 2, 0},
         {31, 16, 30, 1, 1, 17, 14, 0},
         {6, 8, 16, 30, 17, 17, 14, 0},
         {31, 1, 2, 4, 8, 8, 8, 0},
         {14, 17, 17, 14, 17, 17, 14, 0},
         {14, 17, 17, 15, 1, 2, 12, 0},
         {0, 12, 12, 0, 12, 12, 0, 0},
         {0, 12, 12, 0, 12, 12, 8, 16},
         {2, 4, 8, 16, 8, 4, 2, 0},
         {0, 0, 31, 0, 31, 0, 0, 0},
         {8, 4, 2, 1, 2, 4, 8, 0},
         {14, 17, 1, 2, 4, 0, 4, 0},
         {14, 17, 1, 13, 21, 21, 14, 0},
         {4, 10, 17, 17, 31, 17, 17, 0},
         {30, 9, 9, 14, 9, 9, 30, 0},
         {14, 17, 16, 16, 16, 17, 14, 0},
         {30, 9, 9, 9, 9, 9, 30, 0},
         {31, 16, 16, 28, 16, 16, 31, 0},
         {31, 16, 16, 28, 16, 16, 16, 0},
         {15, 16, 16, 19, 17, 17, 15, 0},
         {17, 17, 17, 31, 17, 17, 17, 0},
         {14, 4, 4, 4, 4, 4, 14, 0},
         {1, 1, 1, 1, 1, 17, 14, 0},
         {17, 18, 20, 24, 20, 18, 17, 0},
         {16, 16, 16, 16, 16, 16, 31, 0},
         {17, 27, 21, 21, 17, 17, 17, 0},
         {17, 25, 21, 19, 17, 17, 17, 0},
         {14, 17, 17, 17, 17, 17, 14, 0},
         {30, 17, 17, 30, 16, 16, 16, 0},
         {14, 17, 17, 17, 21, 18, 13, 0},
         {30, 17, 17, 30, 20, 18, 17, 0},
         {14, 17, 16, 14, 1, 17, 14, 0},
         {31, 4, 4, 4, 4, 4, 4, 0},
         {17, 17, 17, 17, 17, 17, 14, 0},
         {17, 17, 17, 10, 10, 4, 4, 0},
         {17, 17, 17, 17, 21, 27, 17, 0},
         {17, 17, 10, 4, 10, 17, 17, 0},
         {17, 17, 10, 4, 4, 4, 4, 0},
         {31, 1, 2, 4, 8, 16, 31, 0},
         {4, 14, 21, 4, 4, 4, 4, 0},
         {4, 4, 4, 4, 21, 14, 4, 0},
         {0, 4, 8, 31, 8, 4, 0, 0},
         {0, 4, 2, 31, 2, 4, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 31},
         {4, 10, 8, 28, 8, 9, 30, 0},
         {0, 0, 14, 1, 15, 17, 15, 0},
         {16, 16, 22, 25, 17, 25, 22, 0},
         {0, 0, 14, 17, 16, 17, 14, 0},
         {1, 1, 13, 19, 17, 19, 13, 0},
         {0, 0, 14, 17, 31, 16, 14, 0},
         {2, 5, 4, 14, 4, 4, 4, 0},
         {0, 0, 14, 17, 17, 15, 1, 14},
         {16, 16, 22, 25, 17, 17, 17, 0},
         {4, 0, 12, 4, 4, 4, 14, 0},
         {1, 0, 3, 1, 1, 1, 9, 6},
         {8, 8, 9, 10, 12, 10, 9, 0},
         {12, 4, 4, 4, 4, 4, 14, 0},
         {0, 0, 26, 21, 21, 21, 21, 0},
         {0, 0, 22, 25, 17, 17, 17, 0},
         {0, 0, 14, 17, 17, 17, 14, 0},
         {0, 0, 22, 25, 25, 22, 16, 16},
         {0, 0, 13, 19, 19, 13, 1, 1},
         {0, 0, 22, 25, 16, 16, 16, 0},
         {0, 0, 15, 16, 14, 1, 30, 0},
         {4, 4, 14, 4, 4, 5, 2, 0},
         {0, 0, 17, 17, 17, 19, 13, 0},
         {0, 0, 17, 17, 17, 10, 4, 0},
         {0, 0, 17, 17, 21, 21, 10, 0},
         {0, 0, 17, 10, 4, 10, 17, 0},
         {0, 0, 17, 17, 17, 15, 1, 14},
         {0, 0, 31, 2, 4, 8, 31, 0},
         {2, 4, 4, 8, 4, 4, 2, 0},
         {4, 4, 4, 0, 4, 4, 4, 0},
         {8, 4, 4, 2, 4, 4, 8, 0},
         {17, 10, 4, 31, 4, 31, 4, 0},
         {21, 10, 21, 10, 21, 10, 21, 10}},
        {{0, 31, 17, 17, 17, 17, 17, 31},
         {0, 31, 16, 16, 16, 16, 16, 16},
         {0, 4, 4, 4, 4, 4, 4, 31},
         {0, 1, 1, 1, 1, 1, 1, 31},
         {0, 8, 4, 2, 15, 4, 2, 1},
         {0, 31, 17, 27, 21, 27, 17, 31},
         {0, 0, 1, 2, 20, 24, 16, 0},
         {0, 14, 17, 17, 31, 10, 10, 27},
         {0, 4, 8, 30, 9, 5, 1, 1}, // menu
         {0, 0, 4, 2, 31, 2, 4, 0}, // right
         {0, 31, 0, 0, 31, 0, 0, 31},
         {0, 0, 4, 4, 21, 14, 4, 0}, // down
         {0, 4, 21, 14, 4, 21, 14, 4},
         {0, 0, 4, 8, 31, 8, 4, 0}, // left
         {0, 14, 17, 27, 21, 27, 17, 14},
         {0, 14, 17, 17, 21, 17, 17, 14},
         {0, 31, 17, 17, 31, 17, 17, 31},
         {0, 14, 21, 21, 23, 17, 17, 14},
         {0, 14, 17, 17, 23, 21, 21, 14},
         {0, 14, 17, 17, 29, 21, 21, 14},
         {0, 14, 21, 21, 29, 17, 17, 14},
         {0, 0, 5, 2, 21, 24, 16, 0},
         {0, 14, 10, 10, 10, 10, 10, 27},
         {0, 1, 1, 1, 31, 1, 1, 1},
         {0, 31, 17, 10, 4, 10, 17, 31},
         {0, 4, 4, 14, 14, 4, 4, 4}, // up
         {0, 14, 17, 16, 8, 4, 0, 4},
         {0, 14, 17, 17, 31, 17, 17, 14},
         {0, 31, 21, 21, 29, 17, 17, 31},
         {0, 31, 17, 17, 29, 21, 21, 31},
         {0, 31, 17, 17, 23, 21, 21, 31},
         {0, 31, 21, 21, 23, 17, 17, 31},
         {0, 0, 0, 0, 0, 0, 0, 0},
         {0, 4, 4, 4, 4, 4, 0, 4},
         {0, 10, 10, 10, 0, 0, 0, 0},
         {0, 10, 10, 31, 10, 31, 10, 10},
         {0, 4, 15, 20, 14, 5, 30, 4},
         {0, 24, 25, 2, 4, 8, 19, 3},
         {0, 8, 20, 20, 8, 21, 18, 13},
         {0, 12, 12, 8, 16, 0, 0, 0},
         {0, 2, 4, 8, 8, 8, 4, 2},
         {0, 8, 4, 2, 2, 2, 4, 8},
         {0, 4, 21, 14, 31, 14, 21, 4},
         {0, 0, 4, 4, 31, 4, 4, 0},
         {0, 0, 0, 0, 12, 12, 8, 16},
         {0, 0, 0, 0, 31, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 12, 12},
         {0, 0, 1, 2, 4, 8, 16, 0},
         {0, 14, 17, 19, 21, 25, 17, 14},
         {0, 4, 12, 4, 4, 4, 4, 14},
         {0, 14, 17, 1, 14, 16, 16, 31},
         {0, 14, 17, 1, 6, 1, 17, 14},
         {0, 2, 6, 10, 18, 31, 2, 2},
         {0, 31, 16, 30, 1, 1, 17, 14},
         {0, 6, 8, 16, 30, 17, 17, 14},
         {0, 31, 1, 2, 4, 8, 16, 16},
         {0, 14, 17, 17, 14, 17, 17, 14},
         {0, 14, 17, 17, 15, 1, 2, 12},
         {0, 0, 12, 12, 0, 12, 12, 0},
         {0, 12, 12, 0, 12, 12, 8, 16},
         {0, 2, 4, 8, 16, 8, 4, 2},
         {0, 0, 0, 31, 0, 31, 0, 0},
         {0, 8, 4, 2, 1, 2, 4, 8},
         {0, 14, 17, 1, 2, 4, 0, 4},
         {0, 14, 17, 1, 13, 21, 21, 14},
         {0, 4, 10, 17, 17, 31, 17, 17},
         {0, 30, 9, 9, 14, 9, 9, 30},
         {0, 14, 17, 16, 16, 16, 17, 14},
         {0, 30, 9, 9, 9, 9, 9, 30},
         {0, 31, 16, 16, 28, 16, 16, 31},
         {0, 31, 16, 16, 28, 16, 16, 16},
         {0, 15, 16, 16, 19, 17, 17, 15},
         {0, 17, 17, 17, 31, 17, 17, 17},
         {0, 14, 4, 4, 4, 4, 4, 14},
         {0, 1, 1, 1, 1, 1, 17, 14},
         {0, 17, 18, 20, 24, 20, 18, 17},
         {0, 16, 16, 16, 16, 16, 16, 31},
         {0, 17, 27, 21, 21, 17, 17, 17},
         {0, 17, 25, 21, 19, 17, 17, 17},
         {0, 14, 17, 17, 17, 17, 17, 14},
         {0, 30, 17, 17, 30, 16, 16, 16},
         {0, 14, 17, 17, 17, 21, 18, 13},
         {0, 30, 17, 17, 30, 20, 18, 17},
         {0, 14, 17, 16, 14, 1, 17, 14},
         {0, 31, 4, 4, 4, 4, 4, 4},
         {0, 17, 17, 17, 17, 17, 17, 14},
         {0, 17, 17, 17, 10, 10, 4, 4},
         {0, 17, 17, 17, 17, 21, 27, 17},
         {0, 17, 17, 10, 4, 10, 17, 17},
         {0, 17, 17, 10, 4, 4, 4, 4},
         {0, 31, 1, 2, 4, 8, 16, 31},
         {0, 14, 8, 8, 8, 8, 8, 14},
         {0, 0, 16, 8, 4, 2, 1, 0},
         {0, 14, 2, 2, 2, 2, 2, 14},
         {0, 4, 10, 17, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 31},
         {0, 6, 6, 4, 2, 0, 0, 0},
         {0, 14, 1, 15, 17, 15, 0, 0},
         {0, 16, 16, 22, 25, 17, 25, 22},
         {0, 0, 0, 14, 17, 16, 17, 14},
         {0, 1, 1, 13, 19, 17, 19, 13},
         {0, 0, 0, 14, 17, 31, 16, 14},
         {0, 2, 5, 4, 14, 4, 4, 4},
         {0, 13, 19, 19, 13, 1, 17, 14},
         {0, 16, 16, 22, 25, 17, 17, 17},
         {0, 4, 0, 12, 4, 4, 4, 14},
         {0, 1, 0, 1, 1, 1, 17, 14},
         {0, 16, 16, 18, 20, 24, 20, 18},
         {0, 12, 4, 4, 4, 4, 4, 14},
         {0, 0, 0, 26, 21, 21, 21, 21},
         {0, 0, 0, 22, 25, 17, 17, 17},
         {0, 0, 0, 14, 17, 17, 17, 14},
         {0, 22, 25, 17, 25, 22, 16, 16},
         {0, 13, 19, 17, 19, 13, 1, 1},
         {0, 0, 0, 22, 25, 16, 16, 16},
         {0, 0, 0, 15, 16, 14, 1, 30},
         {0, 4, 4, 31, 4, 4, 5, 2},
         {0, 0, 0, 17, 17, 17, 19, 13},
         {0, 0, 0, 17, 17, 17, 10, 4},
         {0, 0, 0, 17, 17, 21, 21, 10},
         {0, 0, 0, 17, 10, 4, 10, 17},
         {0, 17, 17, 17, 15, 1, 17, 14},
         {0, 0, 0, 31, 2, 4, 8, 31},
         {0, 2, 4, 4, 8, 4, 4, 2},
         {0, 4, 4, 4, 0, 4, 4, 4},
         {0, 8, 4, 4, 2, 4, 4, 8},
         {0, 8, 21, 2, 0, 0, 0, 0},
         {0, 10, 21, 10, 21, 10, 21, 10}},
};

// ============================================================================
// TRS-80 Graphics Character Constants
// ============================================================================

#define GRAPHICS_HEIGHT 3        // Height in scanlines for base graphics character patterns
#define GRAPHICS_HEIGHT_REPEAT 4 // Repetition factor for graphics scanlines to fill character height
#define GRAPHICS_WIDTH 6         // Width of graphics characters in pixels
#define GRAPHICS_START_BIT 2     // Bit shift applied when rendering graphics characters
#define TERM_COLS 64             // Terminal display dimensions - columns
#define TERM_ROWS 16             // Terminal display dimensions - rows

// ============================================================================
// TRS-80 Graphics Character Data
// ============================================================================

/**
 * @brief TRS-80 Model I graphics character patterns stored in program memory
 *
 * Contains 64 graphics characters used for block graphics, line drawing,
 * and special symbols. Each graphics character is defined as 3 scanlines
 * that are repeated 4 times each to fill the 12-pixel character height.
 *
 * ## Graphics Structure
 * - 64 graphics characters (indices 128-191 in character set)
 * - Each character: 3 bytes (one per base scanline)
 * - Each scanline repeated 4 times for full height
 * - 6 pixels wide (bits 5-0, MSB first)
 *
 * ## Usage
 * Graphics characters are accessed when character code >= 128:
 * ```
 * graphicIndex = characterCode % 64  // Maps 128-191 to 0-63
 * ```
 *
 * @note Total size: 64 chars × 3 bytes = 192 bytes in PROGMEM
 */
static const unsigned char PROGMEM graphics[][GRAPHICS_HEIGHT] =
    {
        {0, 0, 0},
        {56, 0, 0},
        {7, 0, 0},
        {63, 0, 0},

        {0, 56, 0},
        {56, 56, 0},
        {7, 56, 0},
        {63, 56, 0},

        {0, 7, 0},
        {56, 7, 0},
        {7, 7, 0},
        {63, 7, 0},

        {0, 63, 0},
        {56, 63, 0},
        {7, 63, 0},
        {63, 63, 0},

        {0, 0, 56},
        {56, 0, 56},
        {7, 0, 56},
        {63, 0, 56},

        {0, 56, 56},
        {56, 56, 56},
        {7, 56, 56},
        {63, 56, 56},

        {0, 7, 56},
        {56, 7, 56},
        {7, 7, 56},
        {63, 7, 56},

        {0, 63, 56},
        {56, 63, 56},
        {7, 63, 56},
        {63, 63, 56},

        {0, 0, 7},
        {56, 0, 7},
        {7, 0, 7},
        {63, 0, 7},

        {0, 56, 7},
        {56, 56, 7},
        {7, 56, 7},
        {63, 56, 7},

        {0, 7, 7},
        {56, 7, 7},
        {7, 7, 7},
        {63, 7, 7},

        {0, 63, 7},
        {56, 63, 7},
        {7, 63, 7},
        {63, 63, 7},

        {0, 0, 63},
        {56, 0, 63},
        {7, 0, 63},
        {63, 0, 63},

        {0, 56, 63},
        {56, 56, 63},
        {7, 56, 63},
        {63, 56, 63},

        {0, 7, 63},
        {56, 7, 63},
        {7, 7, 63},
        {63, 7, 63},

        {0, 63, 63},
        {56, 63, 63},
        {7, 63, 63},
        {63, 63, 63},
};

// ============================================================================
// M1TerminalScreen Implementation
// ============================================================================

/**
 * @brief Constructor initializing terminal screen with default configuration
 *
 * Sets up the terminal display with initial state, clears video memory,
 * configures the content screen interface, and initializes control buttons.
 * The terminal starts in left-view mode with font 0 selected.
 *
 * ## Initialization Process
 * 1. Reset cursor position and display state
 * 2. Configure ContentScreen title and buttons
 *
 * ## Default State
 * - Position: (0,0) top-left corner
 * - Font: 0 (standard character set)
 * - Scroll: No offset (0,0)
 * - Active: Initially inactive until first draw
 */
M1TerminalScreen::M1TerminalScreen() : ContentScreen()
{
    // Initialize rendering state
    _currentUpdateIndex = 0;     // Start at first character position
    _xCoordinate = 0;            // Cursor at column 0
    _yCoordinate = 0;            // Cursor at row 0
    _fontIndex = 0;              // Use default font
    _horizontalScrollOffset = 0; // Start with no horizontal scroll
    _verticalScrollOffset = 0;   // Start with no vertical scroll

    // Initialize display state
    _model1VideoLoadTime = 0; // Model1 content not yet loaded
    _redrawIndex = -1;        // No redraw in progress

    // Set initial content area (uninitialized)
    _contentLeft = 0;
    _contentTop = 0;
    _contentWidth = 0;
    _contentHeight = 0;

    // Configure ContentScreen interface
    _setTitle("TRS-80 Terminal");
    _setProgressValue(0);

    // Configure control buttons for terminal operations
    const char *buttonItems[2] = {
        "[M] Exit",   // Return to menu
        "[<&>] Font", // Cycle fonts (Left + Right)
    };
    _setButtonItems(buttonItems, 2);
}

/**
 * @brief Extract pixel data for a specific scanline of a character or graphics element
 *
 * Retrieves the horizontal pixel pattern for a given scanline within a character,
 * handling both ASCII characters (0-127) and graphics characters (128+). The method
 * automatically selects between character font data and graphics patterns based on
 * the character index.
 *
 * ## Character Selection Logic
 * - **ASCII Characters (0-127)**: Use font bitmap data from characters array
 * - **Graphics Characters (128+)**: Use graphics pattern data with repetition
 *
 * ## Coordinate System
 * - **charY**: Scanline within 8-pixel character height (0-7)
 * - **graphicY**: Base scanline within 3-line graphics pattern (0-2)
 *
 * ## Pixel Format
 * The returned byte represents 6 pixels shifted left by CHAR_START_BIT:
 * ```
 * Bit 7: Leftmost pixel  (after shift)
 * Bit 6: Pixel 1
 * Bit 5: Pixel 2
 * Bit 4: Pixel 3
 * Bit 3: Pixel 4
 * Bit 2: Rightmost pixel (after shift)
 * Bit 1-0: Unused (padding)
 * ```
 *
 * @param charIndex Character code (0-255) determining character or graphics mode
 * @param graphicIndex Graphics character index (charIndex % 64) for graphics lookup
 * @param charY Vertical scanline position within character height (0-7)
 * @param graphicY Vertical position within graphics base pattern (0-2)
 * @return 8-bit pixel pattern with 6 pixels shifted left by CHAR_START_BIT
 *
 * @note Character data is read from PROGMEM using pgm_read_byte()
 * @note Returns 0 for scanlines beyond character height, meaning no pixel to be drawn
 */
uint8_t M1TerminalScreen::_getPixelLine(uint8_t charIndex, uint8_t graphicIndex, uint8_t charY, uint8_t graphicY)
{
    if (charIndex < 128)
    {
        // ASCII character: use font bitmap data
        if (charY < CHAR_HEIGHT)
        {
            // Read character scanline from PROGMEM and apply bit shift
            return pgm_read_byte(&(characters[_fontIndex][charIndex][charY])) << CHAR_START_BIT;
        }
        else
        {
            // Beyond character height: return blank scanline
            return 0;
        }
    }
    else
    {
        // Graphics character: use graphics pattern data
        return pgm_read_byte(&(graphics[graphicIndex][graphicY])) << GRAPHICS_START_BIT;
    }
}

/**
 * @brief Render a character at specific terminal coordinates with change detection
 *
 * Updates the display for a single character position, comparing the new character
 * against the previously rendered character to optimize drawing. Only pixel lines
 * that have actually changed are updated, providing efficient incremental rendering.
 *
 * ## Rendering Process
 * 1. Calculate graphics indices for both current and previous characters
 * 2. Iterate through all scanlines
 * 3. Extract pixel data for each scanline using _getPixelLine()
 * 4. Update only scanlines where pixel patterns have changed
 *
 * ## Optimization Features
 * - **Change Detection**: Compare current vs previous character
 * - **Pixel-Level Updates**: Only changed scanlines are redrawn
 * - **Batch Operations**: Uses GFX startWrite()/endWrite() for efficiency
 *
 * @param terminalX Character column position (0-63)
 * @param terminalY Character row position (0-15)
 * @param currentCharIndex New character code to display (0-255)
 * @param previousCharIndex Previous character code for comparison (0-255, or 255 for force)
 *
 * @note previousCharIndex of 255 forces a complete redraw (no previous data)
 * @note Uses graphics repetition: each of 3 base lines repeated 4 times
 * @note GFX batch writing optimizes display communication
 */
void M1TerminalScreen::_updateXY(uint16_t terminalX, uint16_t terminalY, uint8_t currentCharIndex, uint8_t previousCharIndex)
{
    // Get display reference and begin batch update
    Adafruit_GFX &gfx = M1Shield.getGFX();
    gfx.startWrite();

    // Calculate graphics character indices (0-63 range)
    uint8_t currentGraphicIndex = currentCharIndex % 64;
    uint8_t previousGraphicIndex = previousCharIndex % 64;

    // Render all scanlines for this character position
    // Graphics: 3 base lines × 4 repetitions = 12 total lines
    for (int16_t graphicY = 0, charY = 0; graphicY < GRAPHICS_HEIGHT; graphicY++)
    {
        for (int16_t i = 0; i < GRAPHICS_HEIGHT_REPEAT; i++, charY++)
        {
            // Extract pixel patterns for current and previous characters
            uint8_t currentPixelLine = _getPixelLine(currentCharIndex, currentGraphicIndex, charY, graphicY);
            uint8_t previousPixelLine = previousCharIndex == 255 ? 0 : _getPixelLine(previousCharIndex, previousGraphicIndex, charY, graphicY);

            // Update this scanline with differential rendering
            _updatePixelLine(gfx, currentPixelLine, previousPixelLine, terminalX, terminalY, charY);
        }
    }

    // Complete batch update
    gfx.endWrite();
}

/**
 * @brief Render a single horizontal scanline with pixel-level change detection
 *
 * Updates one horizontal line within a character, comparing new and previous pixel
 * patterns to draw only pixels that have actually changed. This provides the finest
 * level of rendering granularity for maximum display efficiency.
 *
 * ## Pixel Processing
 * 1. Calculate absolute screen coordinates within content area
 * 2. Iterate through all pixels in the scanline (MSB to LSB)
 * 3. Compare each pixel bit between current and previous patterns
 * 4. Update only pixels that have changed state (on→off or off→on)
 *
 * ## Coordinate System
 * - **terminalX/Y**: Character position in pixels relative to content area
 * - **xCoord/yCoord**: Absolute screen coordinates including content offset
 * - **Bounds Checking**: Ensures all pixels are within content area
 *
 * ## Optimization Features
 * - **Differential Updates**: Only changed pixels are drawn
 * - **Bounds Safety**: Clips rendering to content area boundaries
 * - **Bit-Level Processing**: Examines each pixel individually
 * - **Force Redraw**: Honors global redraw requests
 *
 * ## Pixel Encoding
 * Each byte represents 6 horizontal pixels:
 * ```
 * Bit 7: Leftmost pixel  (drawn first)
 * Bit 6: Pixel 1
 * Bit 5: Pixel 2
 * Bit 4: Pixel 3
 * Bit 3: Pixel 4
 * Bit 2: Rightmost pixel (drawn last)
 * ```
 *
 * @param gfx Reference to Adafruit_GFX object for direct pixel writing
 * @param currentPixelLine New pixel pattern (6 bits in MSB positions)
 * @param previousPixelLine Previous pixel pattern for comparison
 * @param terminalX Character X position in pixels relative to content area
 * @param terminalY Character Y position in pixels relative to content area
 * @param y Scanline offset within character (0-11)
 *
 * @note Pixel colors: 1 bit = FG, 0 bit = BG
 * @note Uses left-shift to process pixels from MSB to LSB
 * @note Respects content area boundaries for clipping
 */
void M1TerminalScreen::_updatePixelLine(Adafruit_GFX &gfx, uint8_t currentPixelLine, uint8_t previousPixelLine, int16_t terminalX, int16_t terminalY, uint16_t y)
{
    // Calculate absolute screen coordinates
    int16_t xCoord = _contentLeft + terminalX;
    int16_t yCoord = _contentTop + terminalY + y;

    uint16_t fgColor = M1Shield.convertColor(TERMINAL_COLOR_FG);
    uint16_t bgColor = M1Shield.convertColor(TERMINAL_COLOR_BG);

    // Process all 6 pixels in this scanline
    for (int16_t x = 0; x < CHAR_WIDTH; x++)
    {
        // Verify pixel is within content area bounds
        if (terminalX >= 0 && terminalX + x <= _contentWidth &&
            terminalY >= 0 && terminalY + y <= _contentHeight)
        {
            // Extract current and previous pixel states (MSB = leftmost)
            bool hasPixel = (currentPixelLine & 0x80);
            bool wasPixel = (previousPixelLine & 0x80);

            // Update pixel only if it changed or during forced redraw
            if (_redrawIndex != -1 || (hasPixel != wasPixel))
            {
                gfx.writePixel(xCoord + x, yCoord, hasPixel ? fgColor : bgColor);
            }

            // Shift to next pixel bit (left to right processing)
            currentPixelLine <<= 1;
            previousPixelLine <<= 1;
        }
    }
}

/**
 * @brief Process the next character in the incremental rendering cycle
 *
 * Advances through the video memory buffer, performing change detection and
 * rendering for one character position per call. This method distributes the
 * rendering workload across multiple frames to maintain smooth performance.
 *
 * ## Incremental Rendering Process
 * 1. **Change Detection**: Compare current vs shadow buffer at current index
 * 2. **Coordinate Calculation**: Convert buffer index to pixel coordinates
 * 3. **Scroll Adjustment**: Apply horizontal and vertical scroll offsets
 * 4. **Viewport Culling**: Only render characters within visible area
 * 5. **Character Update**: Render character if changed or during redraw
 * 6. **Position Advance**: Move to next character with wraparound
 *
 * ## Coordinate Conversion
 * ```
 * Buffer Index → Grid Position:
 * x = index % TERM_COLS  (0-63)
 * y = index / TERM_COLS  (0-15)
 *
 * Grid Position → Pixel Position:
 * pixelX = (x - _horizontalScrollOffset) * CHAR_WIDTH
 * pixelY = (y - _verticalScrollOffset) * CHAR_FULL_HEIGHT
 * ```
 *
 * ## Scrollable Viewport
 * - **Horizontal**: Scroll offset determines visible columns
 * - **Vertical**: Vertical offset determines visible rows based on content area
 * - **Viewport Culling**: Only characters within display bounds are rendered
 *
 * ## State Management
 * - **Buffer Tracking**: Updates shadow buffer after successful render
 * - **Cursor Advancement**: Maintains current X/Y coordinates
 * - **Wraparound**: Resets to (0,0) after processing all 1024 positions
 * - **Redraw Completion**: Clears redraw flag when cycle completes
 *
 * @note Called once per frame to maintain smooth animation
 * @note Only processes one character per call for performance
 * @note Handles both incremental updates and full redraws
 */
void M1TerminalScreen::_updateNext()
{
    if (!isActive())
        return;

    // Check if character needs updating (changed or during redraw)
    if (_redrawIndex != -1 || _bufferedVidMem[_currentUpdateIndex] != _writtenVidMem[_currentUpdateIndex])
    {
        // Convert character grid coordinates to pixel coordinates
        int16_t terminalX = (_xCoordinate - _horizontalScrollOffset) * CHAR_WIDTH;     // Apply horizontal scroll
        int16_t terminalY = (_yCoordinate - _verticalScrollOffset) * CHAR_FULL_HEIGHT; // Apply vertical scroll

        // Only render if character is within the visible viewport
        if (terminalX >= -CHAR_WIDTH && terminalX < (int16_t)_contentWidth &&
            terminalY >= -CHAR_FULL_HEIGHT && terminalY < (int16_t)_contentHeight)
        {
            _updateXY(terminalX, terminalY, _bufferedVidMem[_currentUpdateIndex], _writtenVidMem[_currentUpdateIndex]);
        }

        // Update shadow buffer to reflect rendered state
        _writtenVidMem[_currentUpdateIndex] = _bufferedVidMem[_currentUpdateIndex];
    }

    // Advance to next character position
    _currentUpdateIndex++;

    // Handle end-of-row wraparound
    _xCoordinate++;
    if (_xCoordinate >= TERM_COLS)
    {
        _xCoordinate = 0; // Reset to left column
        _yCoordinate++;   // Move to next row
    }

    // Handle end-of-screen wraparound
    if (_yCoordinate >= TERM_ROWS)
    {
        _yCoordinate = 0;        // Reset to top row
        _currentUpdateIndex = 0; // Reset to first character
    }

    // Check if full redraw cycle is complete
    if (_currentUpdateIndex == _redrawIndex)
    {
        _redrawIndex = -1; // Clear redraw flag
    }
}

/**
 * @brief Schedule a complete screen redraw over multiple frames
 *
 * Initiates a full redraw operation that will be completed incrementally
 * over multiple rendering cycles. This provides complete display refresh
 * while maintaining smooth frame rates.
 *
 * ## Redraw Process
 * 1. Sets redraw marker to current position
 * 2. Incremental rendering continues from current position
 * 3. All characters are redrawn as the cycle progresses
 * 4. Redraw completes when cycle returns to starting position
 *
 * ## Performance Benefits
 * - Distributes rendering load across multiple frames
 * - Maintains responsive user interface during refresh
 * - Avoids display freezing during full updates
 *
 * @note Redraw progresses with normal incremental rendering
 * @note Call after major state changes (font, view mode, etc.)
 */
void M1TerminalScreen::_redraw()
{
    // Mark current position as redraw target
    // Incremental rendering will continue until it loops back to this position
    _redrawIndex = _currentUpdateIndex;
}

/**
 * @brief Scroll the viewport left by up to N characters with dynamic bounds checking
 *
 * Moves the viewport left within the 64-column terminal display, adapting to
 * the available content area width. The scroll amount is clamped to maintain
 * viewport boundaries and provide smooth scrolling experience.
 *
 * ## Scroll Behavior
 * - Maximum step: N characters per call (depends on screen size)
 * - Minimum offset: 0 (leftmost position)
 * - Triggers redraw to update display with new viewport
 *
 * ## Dynamic Viewport Calculation
 * The visible column range is calculated based on content area width:
 * - Visible columns = contentWidth / CHAR_WIDTH
 * - Scroll range: 0 to max(0, 64 - visibleColumns)
 *
 * @note Scroll amount is limited to N characters maximum
 * @note Triggers full redraw to update display with new viewport
 */
void M1TerminalScreen::_scrollLeft()
{
    // Determine scroll amount based on screen size
    uint8_t visibleColumns = _contentWidth / CHAR_WIDTH;
    uint8_t scrollAmount = (visibleColumns >= 30) ? 15 : 10;

    // Scroll left by up to scrollAmount characters, but not below 0
    if (_horizontalScrollOffset >= scrollAmount)
    {
        _horizontalScrollOffset -= scrollAmount;
    }
    else
    {
        _horizontalScrollOffset = 0;
    }
    _redraw(); // Schedule full redraw with new viewport
}

/**
 * @brief Scroll the viewport right by up to N characters with dynamic bounds checking
 *
 * Moves the viewport right within the 64-column terminal display, adapting to
 * the available content area width. The scroll amount is clamped to maintain
 * viewport boundaries and provide smooth scrolling experience.
 *
 * ## Scroll Behavior
 * - Maximum step: N characters per call (depends on screen size)
 * - Maximum offset: Dynamically calculated from content area width
 * - Triggers redraw to update display with new viewport
 *
 * ## Dynamic Viewport Calculation
 * The visible column range is calculated based on content area width:
 * - Visible columns = contentWidth / CHAR_WIDTH
 * - Maximum scroll = max(0, 64 - visibleColumns)
 *
 * @note Scroll amount is limited to N characters maximum
 * @note Triggers full redraw to update display with new viewport
 */
void M1TerminalScreen::_scrollRight()
{
    // Calculate maximum scroll offset based on content area width
    uint8_t visibleColumns = _contentWidth / CHAR_WIDTH;
    uint8_t maxScrollOffset = (TERM_COLS > visibleColumns) ? (TERM_COLS - visibleColumns) : 0;
    uint8_t scrollAmount = (visibleColumns >= 30) ? 15 : 10;

    // Scroll right by up to scrollAmount characters, but not beyond maximum
    if (maxScrollOffset >= scrollAmount && _horizontalScrollOffset <= maxScrollOffset - scrollAmount)
    {
        _horizontalScrollOffset += scrollAmount;
    }
    else
    {
        _horizontalScrollOffset = maxScrollOffset;
    }
    _redraw(); // Schedule full redraw with new viewport
}

/**
 * @brief Scroll the viewport up by up to N rows with dynamic bounds checking
 *
 * Moves the viewport up within the 16-row terminal display based on available
 * content area height. The scroll amount is calculated dynamically and limited
 * to maintain viewport boundaries.
 *
 * ## Scroll Behavior
 * - Maximum step: N rows per call (depends on screen size)
 * - Minimum offset: 0 (topmost position)
 * - Triggers redraw to update display with new viewport
 *
 * ## Dynamic Content Area
 * The maximum scroll range depends on content area height:
 * - Available rows = contentHeight / CHAR_FULL_HEIGHT
 * - If all 16 rows fit, no scrolling is needed
 * - Otherwise, scroll to show different sections of the 16-row grid
 *
 * @note Scroll amount is limited to N rows maximum
 * @note Triggers full redraw to update display with new viewport
 */
void M1TerminalScreen::_scrollUp()
{
    // Determine scroll amount based on screen size
    uint8_t visibleRows = _contentHeight / CHAR_FULL_HEIGHT;
    uint8_t scrollAmount = (visibleRows >= 12) ? 10 : 3;

    // Scroll up by up to scrollAmount rows, but not below 0
    if (_verticalScrollOffset >= scrollAmount)
    {
        _verticalScrollOffset -= scrollAmount;
    }
    else
    {
        _verticalScrollOffset = 0;
    }
    _redraw(); // Schedule full redraw with new viewport
}

/**
 * @brief Scroll the viewport down by up to N rows with dynamic bounds checking
 *
 * Moves the viewport down within the 16-row terminal display based on available
 * content area height. The scroll amount is calculated dynamically and limited
 * to maintain viewport boundaries.
 *
 * ## Scroll Behavior
 * - Maximum step: N rows per call (depends on screen size)
 * - Maximum offset: Calculated based on content area height
 * - Triggers redraw to update display with new viewport
 *
 * ## Dynamic Content Area
 * The maximum scroll range depends on content area height:
 * - Available rows = contentHeight / CHAR_FULL_HEIGHT
 * - Maximum offset = max(0, 16 - available rows)
 * - Ensures all displayable content remains accessible
 *
 * @note Scroll amount is limited to N rows maximum
 * @note Triggers full redraw to update display with new viewport
 */
void M1TerminalScreen::_scrollDown()
{
    // Calculate maximum scroll based on content area height
    uint8_t visibleRows = _contentHeight / CHAR_FULL_HEIGHT;
    uint8_t maxVerticalScroll = (TERM_ROWS > visibleRows) ? (TERM_ROWS - visibleRows) - 1 : 0;
    uint8_t scrollAmount = (visibleRows >= 12) ? 10 : 3;

    // Scroll down by up to scrollAmount rows, but not beyond maximum
    if (_verticalScrollOffset + scrollAmount <= maxVerticalScroll)
    {
        _verticalScrollOffset += scrollAmount;
    }
    else
    {
        _verticalScrollOffset = maxVerticalScroll;
    }
    _redraw(); // Schedule full redraw with new viewport
}

/**
 * @brief Cycle to the next available character font
 *
 * Advances to the next font in the available font collection, wrapping
 * around to the first font after the last. This allows switching between
 * different TRS-80 character sets and display styles.
 *
 * ## Font Management
 * - Current font stored in _fontIndex (0 to CHAR_FONTS-1)
 * - Modulo arithmetic ensures wraparound: (index + 1) % CHAR_FONTS
 * - Font change affects character rendering in _getPixelLine()
 *
 * @note Triggers full redraw to display characters with new font
 * @note Font change is visible immediately during next render cycle
 */
void M1TerminalScreen::_nextFont()
{
    _fontIndex = (_fontIndex + 1) % CHAR_FONTS;
    _redraw(); // Schedule full redraw with new font
}

/**
 * @brief Load video memory content from Model1 system when test signal is active
 *
 * Reads the TRS-80 Model I video memory (0x3C00-0x3FFF) from the Model1
 * system and copies it to the terminal's video buffer. This synchronizes
 * the terminal display with the actual Model1 state.
 *
 * ## Integration Process
 * 1. Check if Model1 test signal is active using hasActiveTestSignal()
 * 2. Read 1024 bytes from Model1 video memory range (0x3C00-0x3FFF)
 * 3. Copy data to local video buffer using readMemory()
 * 4. Clean up allocated memory and mark display for redraw
 *
 * ## Video Memory Mapping
 * - **Model1 Range**: 0x3C00-0x3FFF (1024 bytes)
 * - **Local Buffer**: _bufferedVidMem[1024]
 * - **Character Layout**: Same 64×16 grid as TRS-80 Model I
 *
 * ## Fallback Behavior
 * If Model1 test signal is not active or memory reading fails, the method
 * exits gracefully without changes, preserving existing content.
 *
 * @note Only loads when Model1.hasActiveTestSignal() returns true
 * @note Uses Model1.readMemory() to fetch video memory content
 */
void M1TerminalScreen::_loadFromModel1()
{
    // Check if Model1 test signal is active
    // if (!Model1.hasActiveTestSignal())
    // {
    //     return; // Model1 not available or test signal inactive
    // }

    // Read video memory from Model1 system (0x3C00-0x3FFF = 1024 bytes)
    Model1.activateTestSignal();
    uint8_t *videoData = Model1.readMemory(0x3C00, 1024);
    Model1.deactivateTestSignal();

    if (videoData != nullptr)
    {
        // Copy Model1 video memory to local buffer
        for (int i = 0; i < 1024; i++)
        {
            _bufferedVidMem[i] = videoData[i];
        }

        // Clean up allocated memory
        delete[] videoData;
    }

    _model1VideoLoadTime = millis(); // Mark video memory as loaded
}

/**
 * @brief Main update loop for terminal processing and incremental rendering
 *
 * Handles the terminal's frame-by-frame processing, delegating to the parent
 * ContentScreen for base functionality and managing incremental character
 * rendering when the terminal is active.
 *
 * ## Processing Flow
 * 1. **Parent Processing**: Call ContentScreen::loop() for base screen management
 * 2. **Active Check**: Only process rendering when terminal is active
 * 3. **Data Loading**: Loads data from the Model 1 when necessary
 * 4. **Incremental Update**: Process one character per frame via _updateNext()
 *
 * ## Performance Design
 * The loop processes only one character per frame to maintain smooth
 * performance. Over 1024 frames, the entire screen will be updated, but
 * each individual frame remains responsive.
 *
 * ## State Management
 * - Active state prevents unnecessary processing when terminal is hidden
 * - Incremental rendering distributes CPU load evenly across frames
 * - Parent loop handles input processing and screen management
 *
 * @note Called once per application frame
 * @note Processes exactly one character position per call when active
 * @see _updateNext() For character rendering implementation
 */
void M1TerminalScreen::loop()
{
    // Process incremental character rendering when terminal is active
    if (!isActive())
        return;

    if (_model1VideoLoadTime == 0 || (_currentUpdateIndex == 0 && _model1VideoLoadTime + 500 < millis()))
    {
        _loadFromModel1();

        // First time setting content area dimensions
        if (_contentHeight == 0 || _contentWidth == 0)
        {
            _contentLeft = _getContentLeft() + TERMINAL_PADDING;
            _contentTop = _getContentTop() + TERMINAL_PADDING;
            _contentWidth = _getContentWidth() - TERMINAL_PADDING - TERMINAL_PADDING;
            _contentHeight = _getContentHeight() - TERMINAL_PADDING - TERMINAL_PADDING;

            refresh();
        }
        else
        {
            _updateNext(); // Update one character position per frame
            M1Shield.display();
        }
    }
    else
    {
        _updateNext(); // Update one character position per frame
        M1Shield.display();
    }
}

/**
 * @brief Initialize terminal content area and begin active rendering
 *
 * Implementation of ContentScreen's pure virtual method. Sets up the terminal
 * for active display by scheduling a full redraw and enabling the incremental
 * rendering system.
 *
 * ## ContentScreen Integration
 * This method is called automatically by ContentScreen when the terminal
 * becomes the active content. The actual drawing is performed incrementally
 * by _updateNext() during subsequent loop() calls.
 *
 * @note Called automatically when screen becomes active
 * @note Actual rendering occurs incrementally via loop() → _updateNext()
 * @see loop() For ongoing rendering process
 */
void M1TerminalScreen::_drawContent()
{
    if (!isActive())
        return;

    uint16_t top = _getContentTop();
    uint16_t left = _getContentLeft();
    uint16_t width = _getContentWidth();
    uint16_t height = _getContentHeight();

    Adafruit_GFX &gfx = M1Shield.getGFX();

    if (_model1VideoLoadTime > 0)
    {
        // Clear with normal background color
        gfx.fillRect(left, top, width, height, M1Shield.convertColor(TERMINAL_COLOR_BG));

        // Schedule complete terminal redraw
        _redraw();
    }
    else
    {
        // Clear with normal background color
        gfx.fillRect(left, top, width, height, M1Shield.convertColor(NO_DATA_COLOR_BG));

        // Show that there is no data available
        gfx.setTextColor(M1Shield.convertColor(NO_DATA_COLOR_FG));
        gfx.setTextSize(2);
        gfx.setCursor((width - 130) / 2, (height - 5) / 2);
        gfx.print("No Content");
    }
}

/**
 * @brief Process user input for terminal control and navigation
 *
 * Handles input actions specific to terminal operation, including 2D scrolling,
 * font cycling, redraw requests, and menu navigation. Input is only processed
 * when the terminal is the active screen.
 *
 * ## Input Mapping
 * - **BUTTON_RIGHT**: Scroll viewport right (→ key equivalent)
 * - **BUTTON_LEFT**: Scroll viewport left (← key equivalent)
 * - **BUTTON_UP**: Scroll viewport up (↑ key equivalent)
 * - **BUTTON_DOWN**: Scroll viewport down (↓ key equivalent)
 * - **BUTTON_LEFT + BUTTON_RIGHT**: Switch font
 * - **BUTTON_MENU**: Exit terminal (M key equivalent)
 *
 * ## State Management
 * All terminal control actions trigger appropriate state changes and
 * screen updates. 2D scrolling actions adjust both horizontal and vertical
 * viewport offsets while navigation actions may return different screens.
 *
 * ## Performance Considerations
 * Scroll changes trigger automatic _redraw() for smooth visual updates.
 * Input processing is lightweight and responds immediately.
 *
 * @param action Bitfield of input actions that occurred
 * @param offsetX Horizontal analog input offset
 * @param offsetY Vertical analog input offset
 * @return Screen pointer for navigation (nullptr = stay on terminal)
 *
 * @note Only processes input when terminal isActive()
 * @note Returns nullptr for all actions (stays on terminal)
 * @note All scroll changes trigger automatic redraw
 * @note Returning the parent for the Menu button needs to be handled by a specific screen
 */
Screen *M1TerminalScreen::actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY)
{
    // Only process input when terminal is active
    if (!isActive())
    {
        return nullptr;
    }

    // Font switching (Left + Right combination)
    if (((action & BUTTON_LEFT) && (action & BUTTON_RIGHT)) || action & BUTTON_JOYSTICK)
    {
        _nextFont();
        return nullptr;
    }

    // Scroll right (Right button / → key)
    if (action & RIGHT_ANY)
    {
        _scrollRight();
        return nullptr;
    }

    // Scroll left (Left button / ← key)
    if (action & LEFT_ANY)
    {
        if (_horizontalScrollOffset == 0)
        {
            _nextFont();
        }
        else
        {
            _scrollLeft();
        }
        return nullptr;
    }

    // Scroll up (Up button / ↑ key)
    if (action & UP_ANY)
    {
        if (_verticalScrollOffset == 0)
        {
            _nextFont();
        }
        else
        {
            _scrollUp();
        }
        return nullptr;
    }

    // Scroll down (Down button / ↓ key)
    if (action & DOWN_ANY)
    {
        _scrollDown();
        return nullptr;
    }

    // No screen navigation occurred
    return nullptr;
}
