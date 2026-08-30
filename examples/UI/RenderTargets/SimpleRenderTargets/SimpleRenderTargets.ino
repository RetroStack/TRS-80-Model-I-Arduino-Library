/*
 * SimpleRenderTargets.ino - Drawing to more than one target
 *
 * M1Shield keeps a list of render targets. The panel is target 0; anything
 * else registered is drawn to as well, from the same drawing code. This shows
 * the smallest case: the panel, plus a target that mirrors the UI to Serial.
 *
 * See ComprehensiveRenderTargets for a second physical panel, enabling and
 * disabling targets at runtime, and what the layout authority means.
 *
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#include <M1Shield.h>
#include <ConsoleScreen.h>

// First, tell the system which display you have.
// Uncomment exactly one and comment out the rest.

// For ST7789 240x320 displays (most common, landscape becomes 320x240)
#include <Display_ST7789_320x240.h>
Display_ST7789_320x240 displayProvider;

// For ST7789 240x240 square displays
// #include <Display_ST7789_240x240.h>
// Display_ST7789_240x240 displayProvider;

// For ST7789 320x170 wide displays (landscape)
// #include <Display_ST7789_320x170.h>
// Display_ST7789_320x170 displayProvider;

// For smaller ST7735 128x160 displays
// #include <Display_ST7735.h>
// Display_ST7735 displayProvider;

// For large ST7796 320x480 displays (landscape becomes 480x320)
// #include <Display_ST7796.h>
// Display_ST7796 displayProvider;

// For parallel ILI9325 240x320 displays (landscape becomes 320x240)
// #include <Display_ILI9325.h>
// Display_ILI9325 displayProvider;

// For ILI9341 240x320 displays (landscape becomes 320x240)
// #include <Display_ILI9341.h>
// Display_ILI9341 displayProvider;

// For large HX8357 320x480 displays
// #include <Display_HX8357.h>
// Display_HX8357 displayProvider;

// For monochrome SSD1306 OLED displays (128x64)
// #include <Display_SSD1306.h>
// Display_SSD1306 displayProvider;

// For monochrome SH1106 OLED displays (128x64)
// #include <Display_SH1106.h>
// Display_SH1106 displayProvider;

// A console is enough to show that one write reaches every target.
class MirrorConsole : public ConsoleScreen
{
public:
    MirrorConsole() : ConsoleScreen()
    {
        setTitleF(F("Render Targets"));
        setTextSize(1);
        // No drawing in the constructor: the screen is not active yet.
    }

protected:
    void _executeOnce() override
    {
        cls();
        println(F("This text is drawn once"));
        println(F("and pushed to every"));
        println(F("registered render target."));
    }
};

void setup()
{
    Serial.begin(115200);

    // begin() registers the panel as target 0, the layout authority: every
    // size the UI computes comes from it, whatever else is registered later.
    M1Shield.begin(displayProvider);

    M1Shield.setScreen(new MirrorConsole());

    Serial.println(F("Targets registered: "));
    Serial.println(M1Shield.getRenderManager().getRenderTargetCount());
}

void loop()
{
    M1Shield.loop();
}
