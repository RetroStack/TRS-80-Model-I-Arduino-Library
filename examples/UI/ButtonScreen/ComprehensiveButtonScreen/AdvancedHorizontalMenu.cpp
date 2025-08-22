#include "AdvancedHorizontalMenu.h"
#include "SettingsScreen.h"
#include <M1Shield.h>
#include "ComprehensiveButton.h"

AdvancedHorizontalMenu::AdvancedHorizontalMenu() : ButtonScreen()
{
    setTitleF(F("Advanced Settings"));
    _lastUpdate = 0;
    _animationFrame = 0;

    const char *buttonItems[] = {
        "Audio", "Bright", "Volume", "Network", "View", "Debug"};
    setButtonItems(buttonItems, 6);
}

void AdvancedHorizontalMenu::_drawContent()
{
    auto &gfx = M1Shield.getGFX();

    uint16_t contentY = _getContentTop();
    uint16_t contentHeight = _getContentHeight();

    gfx.setTextColor(M1Shield.convertColor(0xFFFF));
    gfx.setTextSize(1);

    // Draw animated title and instructions
    int16_t y = contentY + 8;

    // Add some animation to show the advanced features
    static const char *tips[] = {
        "Press buttons to toggle values",
        "Settings update in real-time",
        "Use arrows to navigate options"};

    uint8_t tipIndex = (_animationFrame / 60) % 3; // Change tip every 60 frames

    gfx.setCursor(10, y);
    gfx.print(F("Configuration Panel"));
    y += 15;

    gfx.setCursor(10, y);
    gfx.print(F("Tip: "));
    gfx.print(tips[tipIndex]);
    y += 15;

    // Show current selection info
    gfx.setCursor(10, y);
    gfx.print(F("Selected: "));
    uint8_t selectedIndex = _getSelectedButtonItemIndex();
    const char *buttonItem = _getButtonItem(selectedIndex);
    if (buttonItem)
    {
        gfx.print(buttonItem);
    }
}

Screen *AdvancedHorizontalMenu::_getSelectedButtonItemScreen(int index)
{
    if (index == -1)
    {
        // Exit was requested
        return new ComprehensiveButton();
    }

    // Handle toggling/cycling values directly instead of navigating to other screens
    switch (index)
    {
    case 0: // Audio - toggle on/off
        settings.audioEnabled = !settings.audioEnabled;
        refreshButtons(); // Update display to show new value
        break;

    case 1: // Brightness - cycle through values
        settings.brightness += 25;
        if (settings.brightness > 100)
            settings.brightness = 25;
        refreshButtons();
        break;

    case 2: // Volume - adjust if audio is enabled
        if (settings.audioEnabled)
        {
            settings.volume += 25;
            if (settings.volume > 100)
                settings.volume = 25;
            refreshButtons();
        }
        break;

    case 3: // Network - toggle WiFi
        settings.wifiEnabled = !settings.wifiEnabled;
        refreshButtons();
        break;

    case 4: // View (Contrast) - cycle if brightness allows
        if (settings.brightness > 10)
        {
            settings.contrast += 25;
            if (settings.contrast > 100)
                settings.contrast = 25;
            refreshButtons();
        }
        break;

    case 5: // Debug - toggle debug mode
        settings.debugMode = !settings.debugMode;
        refreshButtons();
        break;
    }

    return nullptr; // Stay on current screen after toggling
}

const char *AdvancedHorizontalMenu::_getButtonItemConfigValue(uint8_t index)
{
    static char buffer[16]; // Static buffer for return values

    switch (index)
    {
    case 0: // Audio
        strcpy_P(buffer, settings.audioEnabled ? PSTR("ON") : PSTR("OFF"));
        break;

    case 1: // Brightness
        snprintf_P(buffer, sizeof(buffer), PSTR("%d%%"), settings.brightness);
        break;

    case 2: // Volume
        if (settings.audioEnabled)
        {
            snprintf_P(buffer, sizeof(buffer), PSTR("%d%%"), settings.volume);
        }
        else
        {
            strcpy_P(buffer, PSTR("N/A"));
        }
        break;

    case 3: // Network
        strcpy_P(buffer, settings.wifiEnabled ? PSTR("WIFI") : PSTR("OFF"));
        break;

    case 4: // View (Contrast)
        snprintf_P(buffer, sizeof(buffer), PSTR("%d%%"), settings.contrast);
        break;

    case 5: // Debug - animated status
        if (settings.debugMode)
        {
            const char *frames[] = {"ACT.", "ACT-", "ACT\\", "ACT|"};
            strcpy(buffer, frames[_animationFrame % 4]);
        }
        else
        {
            strcpy_P(buffer, PSTR("OFF"));
        }
        break;

    default:
        return nullptr;
    }

    return buffer;
}

// Dynamic enable/disable based on current state
bool AdvancedHorizontalMenu::_isButtonItemEnabled(uint8_t index) const
{
    switch (index)
    {
    case 2: // Volume - only enabled if audio is on
        return settings.audioEnabled;

    case 4: // View options - disabled if brightness too low
        return settings.brightness > 10;

    default:
        return true;
    }
}

void AdvancedHorizontalMenu::loop()
{
    ButtonScreen::loop();

    // Update animation and refresh menu periodically
    unsigned long now = millis();
    if (now - _lastUpdate >= 500)
    {
        _lastUpdate = now;
        _animationFrame++;
        // Force a redraw to update animated config values
        // _needsRedraw = true;  // This variable may not exist in ButtonScreen
        refreshButtons(); // Use the public refresh method instead
    }
}
