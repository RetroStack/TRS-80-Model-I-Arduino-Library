/**
 * ===================================================================
 * Comprehensive MenuScreen Example
 * ===================================================================
 *
 * This is a complete example showing ALL MenuScreen features:
 * 1. Basic menu navigation and selection
 * 2. Configuration values displayed on the right
 * 3. Disabled menu items with conditional logic
 * 4. User feedback systems (alerts, confirmations, notifications)
 * 5. Menu interdependencies and real-time updates
 * 6. Screen navigation (to submenus and back)
 *
 * WHAT IS A MENUSCREEN?
 * MenuScreen extends ContentScreen with automatic menu functionality:
 * - Handles navigation, selection, and visual feedback automatically
 * - Supports pagination for large menus
 * - Can display configuration values next to each item
 * - Can disable items conditionally with visual graying
 * - Can navigate to other screens and return via back navigation
 * - Includes all ContentScreen features (header, footer, progress, user feedback)
 *
 * LEARNING OBJECTIVES:
 * - Understand MenuScreen vs ContentScreen vs Screen hierarchy
 * - Learn how to implement all MenuScreen virtual methods
 * - See how configuration values and disabled items work together
 * - Practice creating complex menu logic with interdependencies
 * - Master user feedback integration in menu systems
 * - Understand screen navigation and back button handling
 */

#ifndef COMPREHENSIVE_MENU_SCREEN_H
#define COMPREHENSIVE_MENU_SCREEN_H

#include <MenuScreen.h>

class ComprehensiveMenuScreen : public MenuScreen
{
private:
    // Configuration settings that will be displayed and managed
    bool _soundEnabled;  // Sound effects on/off
    uint8_t _difficulty; // 0=Easy, 1=Normal, 2=Hard
    uint16_t _timeout;   // Screen timeout in seconds (5-60)
    uint8_t _brightness; // Brightness percentage (10-100)
    bool _autoSave;      // Auto save feature on/off
    uint8_t _language;   // 0=English, 1=Spanish, 2=French

    // Buffer for numeric values (shared to save memory)
    char _numericBuffer[16];

    // Difficulty level names for display
    const char *_difficultyNames[3] = {"Easy", "Normal", "Hard"};

    // Language names for display
    const char *_languageNames[3] = {"English", "Spanish", "French"};

    // Helper methods for setting adjustments
    void _toggleSound();
    void _cycleDifficulty();
    void _adjustTimeout(int delta);
    void _adjustBrightness(int delta);
    void _toggleAutoSave();
    void _cycleLanguage();
    void _resetToDefaults();
    void _showUserFeedbackDemo();

protected:
    // Required MenuScreen virtual method - handles item selection
    Screen *_getSelectedMenuItemScreen(int index) override;

    // Optional MenuScreen virtual method - provides configuration values
    const char *_getMenuItemConfigValue(uint8_t index) override;

    // Optional MenuScreen virtual method - controls item enabled/disabled state
    bool _isMenuItemEnabled(uint8_t index) const override;

public:
    // Constructor - sets up the menu with all items and initial settings
    ComprehensiveMenuScreen();

    // Destructor - default is fine since we don't allocate anything special
    virtual ~ComprehensiveMenuScreen() = default;

    // Optional MenuScreen virtual method - custom loop processing
    void loop() override;
};

#endif /* COMPREHENSIVE_MENU_SCREEN_H */
