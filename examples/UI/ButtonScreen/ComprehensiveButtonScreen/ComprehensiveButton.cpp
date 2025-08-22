#include "ComprehensiveButton.h"
#include "SettingsScreen.h"
#include "AdvancedHorizontalMenu.h"
#include "InfoScreen.h"
#include <ConsoleScreen.h>
#include <M1Shield.h>

// Global settings that button items can modify
GlobalSettings settings;

ComprehensiveButton::ComprehensiveButton() : ButtonScreen()
{
    setTitleF(F("Comprehensive Demo"));

    const char *buttonItems[] = {
        "Settings", "Info", "About", "Test", "Audio", "Debug"};
    setButtonItems(buttonItems, 6);
}

void ComprehensiveButton::_drawContent()
{
    auto &gfx = M1Shield.getGFX();

    uint16_t contentY = _getContentTop();
    uint16_t contentHeight = _getContentHeight();

    gfx.setTextColor(M1Shield.convertColor(0xFFFF));
    gfx.setTextSize(1);

    // Show current system status in content area
    int16_t y = contentY + 8;

    gfx.setCursor(10, y);
    gfx.print(F("System Status:"));
    y += 12;

    gfx.setCursor(15, y);
    gfx.print(F("Audio: "));
    gfx.print(settings.audioEnabled ? F("ON") : F("OFF"));
    y += 10;

    gfx.setCursor(15, y);
    gfx.print(F("WiFi: "));
    gfx.print(settings.wifiEnabled ? F("CONNECTED") : F("DISCONNECTED"));
    y += 10;

    gfx.setCursor(15, y);
    gfx.print(F("Debug: "));
    gfx.print(settings.debugMode ? F("ENABLED") : F("DISABLED"));
    y += 10;

    gfx.setCursor(15, y);
    gfx.print(F("Volume: "));
    gfx.print(settings.volume);
    gfx.print(F("%"));
}

Screen *ComprehensiveButton::_getSelectedButtonItemScreen(int index)
{
    switch (index)
    {
    case 0: // Settings
        return new AdvancedHorizontalMenu();
    case 1: // Info
        return new InfoScreen();
    case 2: // About
    {
        // Create a simple about screen that shows some info
        ConsoleScreen *aboutScreen = new ConsoleScreen();
        return aboutScreen;
    }
    case 3: // Test - disabled, so stay on current screen
        return nullptr;
    case 4: // Audio - will be disabled when audio is off
        return nullptr;
    case 5: // Debug
        return nullptr;
    default:
        return nullptr;
    }
}

const char *ComprehensiveButton::_getButtonItemConfigValue(uint8_t index)
{
    static char buffer[16]; // Static buffer for dynamic values

    switch (index)
    {
    case 0:
        return "MANAGE";
    case 1:
        return "VIEW";
    case 2:
        return "v1.0";
    case 3:
        return "N/A"; // Disabled item
    case 4:
        // Dynamic config based on audio settings
        if (settings.audioEnabled)
        {
            snprintf_P(buffer, sizeof(buffer), PSTR("%d%%"), settings.volume);
            return buffer;
        }
        else
        {
            return "OFF";
        }
    case 5:
        // Show debug status
        return settings.debugMode ? "ON" : "OFF";
    default:
        return nullptr;
    }
}

// Demonstrate dynamic enable/disable functionality
bool ComprehensiveButton::_isButtonItemEnabled(uint8_t index) const
{
    switch (index)
    {
    case 3: // Test item - always disabled for demo
        return false;
    case 4: // Audio item - only enabled if audio is on
        return settings.audioEnabled;
    default:
        return true;
    }
}
