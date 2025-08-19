/*
 * SimpleContent.h - Minimal ContentScreen Example
 *
 * Bare minimum ContentScreen demonstration:
 * - Header with title
 * - Simple content display
 * - Basic button interaction
 *
 * Authors: Marcel Erz (RetroStack)
 * License: MIT
 */

#ifndef SIMPLE_CONTENT_H
#define SIMPLE_CONTENT_H

#include <ContentScreen.h>

class SimpleContent : public ContentScreen
{
public:
    SimpleContent();

    // Override required ContentScreen methods
    void _drawContent() override;
    void loop() override;
    Screen *actionTaken(ActionTaken action, int8_t offsetX, int8_t offsetY) override;
};

#endif // SIMPLE_CONTENT_H
