#include "SimpleButton.h"
#include "SelectionDisplay.h"
#include <M1Shield.h>

SimpleButton::SimpleButton() : ButtonScreen()
{
    setTitleF(F("Button Screen Demo"));

    // Set up button items - will show 4 at a time with horizontal scrolling
    const char *buttonItems[] = {
        "Audio", "Video", "System", "Debug", "Network", "Reset"};
    setButtonItems(buttonItems, 6);
}

void SimpleButton::_drawContent()
{
    // Draw instructions in the content area
    auto &gfx = M1Shield.getGFX();

    // Get content area dimensions
    uint16_t contentY = _getContentTop();
    uint16_t contentHeight = _getContentHeight();

    // Set text properties
    gfx.setTextColor(M1Shield.convertColor(0xFFFF));
    gfx.setTextSize(1);

    // Draw instruction text centered in content area
    int16_t textY = contentY + (contentHeight / 2) - 12; // Center vertically

    gfx.setCursor(10, textY);
    gfx.print(F("Use arrow keys to navigate buttons"));
    gfx.setCursor(10, textY + 12);
    gfx.print(F("Press up/down to select"));
}

Screen *SimpleButton::_getSelectedButtonItemScreen(int index)
{
    if (index == -1)
    {
        // Exit was requested
        return nullptr;
    }

    // Create appropriate screen for selection
    return new SelectionDisplay(index);
}

const char *SimpleButton::_getButtonItemConfigValue(uint8_t index)
{
    switch (index)
    {
    case 0:
        return "HIGH"; // Audio quality
    case 1:
        return "1024x768"; // Video resolution
    case 2:
        return "v1.3.0"; // System version
    case 3:
        return "OFF"; // Debug mode status
    case 4:
        return "WIFI"; // Network type
    case 5:
        return "READY"; // Reset status
    default:
        return nullptr;
    }
}

bool SimpleButton::_isButtonItemEnabled(uint8_t index) const
{
    // For demo: disable the Reset option to show disabled state
    return index != 5;
}
