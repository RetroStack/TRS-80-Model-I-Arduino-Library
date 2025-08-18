/**
 * ===================================================================
 * Simple Content Screen
 * ===================================================================
 *
 * This is the SIMPLEST possible example of using ContentScreen!
 * Perfect for learning how the M1Shield content framework works.
 *
 * WHAT IS A CONTENTSCREEN?
 * - A ContentScreen is a special type of screen that has built-in:
 *   * Header (title area at the top)
 *   * Footer (buttons at the bottom)
 *   * Progress bar (shows completion/selection)
 *   * Content area (middle section for your custom content)
 *
 * WHY USE CONTENTSCREEN?
 * - Saves you time - no need to manually draw headers/footers
 * - Consistent look across your app
 * - Built-in button handling
 * - Automatic layout management
 *
 * LEARNING OBJECTIVES:
 * - Understand ContentScreen vs regular Screen
 * - Learn how to draw content in the middle area
 * - See how header/footer are managed automatically
 * - Practice basic menu navigation
 */

#ifndef SIMPLE_CONTENT_SCREEN_H
#define SIMPLE_CONTENT_SCREEN_H

#include <ContentScreen.h> // This gives us the ContentScreen base class

class SimpleContentScreen : public ContentScreen
{
private:
    // Simple menu state - just track which item is selected
    int _selectedItem;       // Which menu item is highlighted (0, 1, 2, etc.)
    int _totalItems;         // How many menu items we have
    const char **_menuItems; // Array of menu text (like "Settings", "Games", etc.)

protected:
    // These 3 methods are REQUIRED when using ContentScreen:

    void _drawContent() override; // Called to draw the middle content area

public:
    // Constructor - creates the screen with a title and menu items
    SimpleContentScreen(const char *title, const char **menuItems, int itemCount);

    // These 2 methods are REQUIRED for any Screen:
    void loop() override;                                                               // Called continuously
    Screen *actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY) override; // Called when user presses buttons
};

#endif /* SIMPLE_CONTENT_SCREEN_H */
