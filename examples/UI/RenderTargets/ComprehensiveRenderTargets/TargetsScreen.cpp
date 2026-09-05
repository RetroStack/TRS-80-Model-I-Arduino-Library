/*
 * TargetsScreen.cpp - Lists the registered render targets and toggles them
 *
 * Authors: Marcel Erz (RetroStack)
 * License: MIT
 */

#include <M1Shield.h>
#include <RenderManager.h>
#include <RenderTarget.h>
#include "TargetsScreen.h"

TargetsScreen::TargetsScreen() : ContentScreen(), _selected(0)
{
    setTitleF(F("Render Targets"));

    const char *buttons[] = {"Toggle", "Exit"};
    setButtonItems(buttons, 2);
}

void TargetsScreen::_drawContent()
{
    RenderManager &targets = M1Shield.getRenderManager();
    const uint8_t count = targets.getRenderTargetCount();

    const uint8_t rowHeight = isSmallDisplay() ? 10 : 16;
    const uint8_t textSize = isSmallDisplay() ? 1 : 2;

    uint16_t x = _getContentLeft() + 2;
    uint16_t y = _getContentTop() + 2;

    for (uint8_t i = 0; i < count; i++)
    {
        RenderTarget *target = targets.getRenderTarget(i);
        if (!target)
        {
            continue;
        }

        // "> " marks the highlighted row, "[x]" its enabled state
        String row = (i == _selected) ? F("> ") : F("  ");
        row += target->isEnabled() ? F("[x] ") : F("[ ] ");
        row += target->getName();

        drawText(x, y, row.c_str(), M1Shield.convertColor(0xFFFF), textSize);
        y += rowHeight;

        if (!isSmallDisplay())
        {
            String size = F("      ");
            size += String(target->getScreenWidth());
            size += 'x';
            size += String(target->getScreenHeight());

            drawText(x, y, size.c_str(), M1Shield.convertColor(0x7BEF), 1);
            y += 10;
        }
    }

    if (count == 0)
    {
        drawText(x, y, "No render targets", M1Shield.convertColor(0xFFFF), textSize);
    }
}

void TargetsScreen::loop()
{
    // Nothing to animate here, but the parent still has work to do: notification
    // timeouts never fire without this call.
    ContentScreen::loop();
}

Screen *TargetsScreen::actionTaken(ActionTaken action, int8_t offsetX, int8_t offsetY)
{
    (void)offsetX;
    (void)offsetY;

    RenderManager &targets = M1Shield.getRenderManager();
    const uint8_t count = targets.getRenderTargetCount();
    if (count == 0)
    {
        return nullptr;
    }

    if (action & (BUTTON_UP | JOYSTICK_UP))
    {
        _selected = (_selected == 0) ? (count - 1) : (_selected - 1);
        refresh();
    }
    else if (action & (BUTTON_DOWN | JOYSTICK_DOWN))
    {
        _selected = (_selected + 1) % count;
        refresh();
    }
    else if (action & (BUTTON_SELECT | BUTTON_LEFT))
    {
        RenderTarget *target = targets.getRenderTarget(_selected);
        if (target)
        {
            // Toggling the primary is allowed: it stops being drawn and
            // pushed, but it remains the layout authority.
            target->setEnabled(!target->isEnabled());

            // Redraw so the change is visible on whatever is still enabled
            refresh();
        }
    }

    return nullptr;
}
