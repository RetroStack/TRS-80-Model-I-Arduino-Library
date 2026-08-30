/*
 * M1Shield Render Targets Example
 *
 * Draws one UI to several destinations at once and lets you switch each on
 * and off at runtime. See README.md for complete documentation.
 *
 * Out of the box this runs on a single panel plus a "serial mirror" target,
 * so it is useful with no extra hardware. Set SECOND_PANEL_PRESENT to true
 * once you have wired a second panel.
 *
 * Authors: Marcel Erz (RetroStack)
 * License: MIT
 */

// ---------------------------------------------------------------------------
// STEP 1: the primary display
// ---------------------------------------------------------------------------

// For ST7789 320x240 displays (landscape)
#include <Display_ST7789_320x240.h>
Display_ST7789_320x240 displayProvider;

// For ST7789 240x240 square displays
// #include <Display_ST7789_240x240.h>
// Display_ST7789_240x240 displayProvider;

// For smaller ST7735 128x160 displays
// #include <Display_ST7735.h>
// Display_ST7735 displayProvider;

#include <M1Shield.h>
#include <DisplayRenderTarget.h>
#include "MirrorTarget.h"
#include "TargetsScreen.h"

// ---------------------------------------------------------------------------
// STEP 2: a second physical panel (optional)
// ---------------------------------------------------------------------------
//
// Set this to true once a second panel is wired to the pins below.
// It is a constexpr rather than an #ifdef so the compiler still type-checks
// the code below even when it is false, and -Os removes the dead branch.
constexpr bool SECOND_PANEL_PRESENT = false;

// Pins for the second panel.
//
// These are free in the ATmega2560's pin assignment: the TRS-80 bus claims
// ports A, C, F, L, J, H, E and D, the shield claims 4, 8-13, 38-43, 49 and
// A8-A15, and 50-53 are the hardware SPI bus. That leaves 5, 6, 7, 17, 20,
// 21 and 53 unclaimed at the MCU level.
//
// WHETHER THEY ARE PHYSICALLY BROKEN OUT ON YOUR BOARD IS YOURS TO VERIFY -
// check your shield before wiring anything.
constexpr int8_t SECOND_PANEL_CS = 5;
constexpr int8_t SECOND_PANEL_DC = 6;

// The second panel needs its own reset line, or none at all (-1). It must not
// share the primary's reset pin: the Adafruit driver toggles reset while
// initializing, which would reset the primary panel and leave it blank.
constexpr int8_t SECOND_PANEL_RST = 7;

// The second panel's driver. Included unconditionally so the compiler (and CI)
// type-check the registration below even while SECOND_PANEL_PRESENT is false.
#include <Display_ST7735.h>
Display_ST7735 secondPanelProvider;
DisplayRenderTarget secondPanel(secondPanelProvider);

// ---------------------------------------------------------------------------
// STEP 3: a custom target - mirrors the UI to the Serial Monitor
// ---------------------------------------------------------------------------
//
// Costs 1024 bytes of SRAM for its 128x64 canvas. Use MirrorTarget(64, 32)
// for 256 bytes if you need the room.
MirrorTarget serialMirror(128, 64);

void setup()
{
    Serial.begin(115200);
    Serial.println(F("=== Render Targets Example ==="));

    // The primary panel registers itself as render target 0
    M1Shield.begin(displayProvider);

    // A second physical panel, if one is wired
    if (SECOND_PANEL_PRESENT)
    {
        if (M1Shield.addDisplay(secondPanel, SECOND_PANEL_CS, SECOND_PANEL_DC, SECOND_PANEL_RST))
        {
            Serial.println(F("Second panel registered"));
        }
        else
        {
            Serial.println(F("Second panel failed to initialize"));
        }
    }

    // The serial mirror needs no hardware at all
    if (serialMirror.isValid())
    {
        M1Shield.getRenderManager().addRenderTarget(&serialMirror);
        Serial.println(F("Serial mirror registered"));
    }
    else
    {
        Serial.println(F("Serial mirror canvas failed to allocate - skipped"));
    }

    Serial.print(F("Render targets registered: "));
    Serial.println(M1Shield.getRenderManager().getRenderTargetCount());

    M1Shield.setScreen(new TargetsScreen());
}

void loop()
{
    // Handle all M1Shield operations
    M1Shield.loop();
}
