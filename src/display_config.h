/*
 * M1Shield.h - Display configuration and hardware abstraction layer for TRS-80 Model I Arduino Shield
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef DISPLAY_CONFIG_H
#define DISPLAY_CONFIG_H

/**
 * Display Configuration for M1Shield
 *
 * Define ONE of the following before including M1Shield.h:
 * - USE_ST7789    (240x320 TFT - Default)
 * - USE_ST7735    (128x160 TFT)
 * - USE_ILI9341   (240x320 TFT)
 * - USE_ILI9488   (320x480 TFT)
 * - USE_HX8357    (320x480 TFT)
 * - USE_ILI9325   (240x320 TFT)
 *
 * Example usage in your sketch:
 * #define USE_ST7789
 * #include "M1Shield.h"
 */

// Display pin definitions (can be overridden before including this file)
#ifndef TFT_CS
constexpr uint8_t TFT_CS_DEFAULT = 9; // Chip Select
#define TFT_CS TFT_CS_DEFAULT
#endif

#ifndef TFT_DC
constexpr uint8_t TFT_DC_DEFAULT = 8; // Data/Command
#define TFT_DC TFT_DC_DEFAULT
#endif

#ifndef TFT_RST
constexpr int8_t TFT_RST_DEFAULT = -1; // Reset pin (-1 if not used)
#define TFT_RST TFT_RST_DEFAULT
#endif

// Display-specific configurations
// Configure display dimensions and name string at compile time
// based on which display driver is enabled

#if defined(USE_ST7789)
constexpr uint16_t DISPLAY_WIDTH = 240;
constexpr uint16_t DISPLAY_HEIGHT = 320;
constexpr const char *DISPLAY_NAME = "ST7789";

#elif defined(USE_ST7789_240x240)
constexpr uint16_t DISPLAY_WIDTH = 240;
constexpr uint16_t DISPLAY_HEIGHT = 240;
constexpr const char *DISPLAY_NAME = "ST7789 (240x240)";

#elif defined(USE_ST7735)
constexpr uint16_t DISPLAY_WIDTH = 128;
constexpr uint16_t DISPLAY_HEIGHT = 160;
constexpr const char *DISPLAY_NAME = "ST7735";

#elif defined(USE_ILI9341)
constexpr uint16_t DISPLAY_WIDTH = 240;
constexpr uint16_t DISPLAY_HEIGHT = 320;
constexpr const char *DISPLAY_NAME = "ILI9341";

#elif defined(USE_ILI9488)
constexpr uint16_t DISPLAY_WIDTH = 320;
constexpr uint16_t DISPLAY_HEIGHT = 480;
constexpr const char *DISPLAY_NAME = "ILI9488";

#elif defined(USE_HX8357)
constexpr uint16_t DISPLAY_WIDTH = 320;
constexpr uint16_t DISPLAY_HEIGHT = 480;
constexpr const char *DISPLAY_NAME = "HX8357";

#elif defined(USE_ILI9325)
constexpr uint16_t DISPLAY_WIDTH = 240;
constexpr uint16_t DISPLAY_HEIGHT = 320;
constexpr const char *DISPLAY_NAME = "ILI9325";

#else
// Fallback to ST7789 if no display type was defined
#define USE_ST7789
constexpr uint16_t DISPLAY_WIDTH = 320;
constexpr uint16_t DISPLAY_HEIGHT = 240;
constexpr const char *DISPLAY_NAME = "ST7789 (Default)";
#endif

#endif /* DISPLAY_CONFIG_H */
