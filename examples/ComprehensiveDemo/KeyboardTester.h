#ifndef KEYBOARDTESTER_H
#define KEYBOARDTESTER_H

#include <ContentScreen.h>
#include <Keyboard.h>

class KeyboardTester : public ContentScreen
{
public:
    enum DisplayMode
    {
        MODE_GRAPHICAL = 0, // Visual TRS-80 keyboard layout
        MODE_MATRIX = 1,    // 8x8 matrix grid view
        MODE_COUNT = 2      // Total number of modes
    };

private:
    Keyboard _keyboard;
    unsigned long _lastUpdate;
    int _keyPressCount;
    DisplayMode _currentMode;

    // Key testing tracking - using matrix coordinates [row][col]
    bool _keyTested[8][8];                 // Track which keys have been pressed (8x8 matrix)
    bool _keyPressed[8][8];                // Track current key press state (8x8 matrix)
    bool _keyPrevPressed[8][8];            // Track previous key press state for change detection
    bool _spaceKeyTested;                  // Special tracking for space key
    bool _spacePressed;                    // Current space key press state
    bool _spacePrevPressed;                // Previous space key press state
    bool _needsFullRedraw;                 // Flag to indicate full redraw is needed
    unsigned long _keyHighlightTime[8][8]; // Track when keys were pressed for highlight timing
    unsigned long _spaceHighlightTime;     // Highlight timing for space key

public:
    KeyboardTester();
    bool open() override;
    void close() override;
    void loop() override;
    Screen *actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY) override;

protected:
    void _drawContent() override;

private:
    void displayKeyboardStatus();
    void drawGraphicalKeyboard();
    void drawMatrixView();
    void updateGraphicalKeyboard(); // Update only changed keys in graphical mode
    void updateMatrixView();        // Update only changed keys in matrix mode
    void updateModeTitle();
    void drawKey(int x, int y, int width, int height, const char *label, bool pressed, bool tested, bool highlighted);
    void drawMatrixCell(int x, int y, int width, int height, uint8_t row, uint8_t col, String keyChar, bool pressed, bool tested, bool highlighted);
    void drawKeyboardRow(const char *keys[], int keyCount, int keyMatrix[][2], int startX, int y, int keyWidth, int keyHeight);
    void updateKeyboardRowChanges(const char *keys[], int keyCount, int keyMatrix[][2], int startX, int y, int keyWidth, int keyHeight, bool &anyKeyChanged);

    // Helper methods to reduce duplication
    void calculateGraphicalLayout(int &startX, int &startY, int &keyWidth, int &keyHeight);
    void calculateMatrixLayout(int &startX, int &startY, int &cellWidth, int &cellHeight);
    void processGraphicalKeyboard(bool fullDraw);
    void processMatrixView(bool fullDraw);

    String getKeyName(uint8_t keyValue);
    String getMatrixKeyName(uint8_t row, uint8_t col);
};

#endif
