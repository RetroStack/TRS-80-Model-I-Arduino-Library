#include "KeyboardTester.h"
#include "MainDemoMenu.h"
#include <Arduino.h>
#include <M1Shield.h>
#include <Model1.h>
#include <Adafruit_GFX.h>
#include "logger.h"

KeyboardTester::KeyboardTester()
    : ContentScreen(), _lastUpdate(0), _keyPressCount(0), _currentMode(MODE_GRAPHICAL), _spaceKeyTested(false), _spacePressed(false), _spacePrevPressed(false), _needsFullRedraw(true), _spaceHighlightTime(0)
{
    // Set up logger for debugging
    _keyboard.setLogger(logger);

    updateModeTitle();
    const char *buttons[] = {"M:Menu", "RT:Reset", "UP:Prev", "DN:Next"};
    _setButtonItems(buttons, 4);

    // Initialize key testing tracking arrays
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            _keyTested[i][j] = false;
            _keyPressed[i][j] = false;
            _keyPrevPressed[i][j] = false;
            _keyHighlightTime[i][j] = 0;
        }
    }
}

bool KeyboardTester::open()
{
    // Activate test signal to allow keyboard access
    Model1.activateTestSignal();
    _keyboard.update(); // Keep keyboard state current

    // Call parent implementation
    return ContentScreen::open();
}

void KeyboardTester::close()
{
    // Deactivate test signal when screen is closed
    Model1.deactivateTestSignal();

    // Call parent implementation
    ContentScreen::close();
}
void KeyboardTester::loop()
{
    // Update keyboard status every 100ms for responsive key detection
    if (millis() - _lastUpdate > 100)
    {
        _lastUpdate = millis();
        displayKeyboardStatus(); // Update status display and track key presses
    }
}

void KeyboardTester::_drawContent()
{
    if (_needsFullRedraw)
    {
        // Clear content area before full redraw
        uint16_t contentLeft = _getContentLeft();
        uint16_t contentTop = _getContentTop();
        uint16_t contentWidth = _getContentWidth();
        uint16_t contentHeight = _getContentHeight();

        Adafruit_GFX &gfx = M1Shield.getGFX();

        // Wrap entire screen redraw in single SPI transaction for better performance
        gfx.startWrite();
        gfx.fillRect(contentLeft, contentTop, contentWidth, contentHeight, M1Shield.convertColor(0x0000));

        switch (_currentMode)
        {
        case MODE_GRAPHICAL:
            drawGraphicalKeyboard();
            break;
        case MODE_MATRIX:
            drawMatrixView();
            break;
        }
        gfx.endWrite();

        _needsFullRedraw = false;
    }
    else
    {
        // Only update keys that have changed state
        switch (_currentMode)
        {
        case MODE_GRAPHICAL:
            updateGraphicalKeyboard();
            break;
        case MODE_MATRIX:
            updateMatrixView();
            break;
        }
    }
}

void KeyboardTester::displayKeyboardStatus()
{
    // Store previous state for change detection
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            _keyPrevPressed[i][j] = _keyPressed[i][j];
        }
    }
    _spacePrevPressed = _spacePressed;

    // Use keyboard iterator to get all key changes
    KeyboardChangeIterator it = _keyboard.changes();
    while (it.hasNext())
    {
        it.next(); // Move to the next changed key first

        uint8_t row = it.row();
        uint8_t col = it.column();
        uint8_t keyValue = it.keyValue();

        // Update the key state directly for this specific position
        if (row < 8 && col < 8)
        {
            bool isPressed = it.isPressed();
            _keyPressed[row][col] = isPressed;

            // Mark key as tested when pressed
            if (isPressed)
            {
                _keyTested[row][col] = true;
            }

            // Handle special case for space key
            if (keyValue == 0x20) // Space key
            {
                _spacePressed = isPressed;
                if (isPressed)
                {
                    _spaceKeyTested = true;
                }
            }
        }

        // Log key activity
        if (it.wasJustPressed())
        {
            String keyName = getKeyName(keyValue);
            Serial.print(F("Key pressed: "));
            Serial.print(keyName);
            Serial.print(F(" (0x"));
            Serial.print(keyValue, HEX);
            Serial.print(F(") at row "));
            Serial.print(row);
            Serial.print(F(", col "));
            Serial.println(col);
        }

        if (it.wasReleased())
        {
            String keyName = getKeyName(keyValue);
            Serial.print(F("Key released: "));
            Serial.print(keyName);
            Serial.print(F(" at row "));
            Serial.print(row);
            Serial.print(F(", col "));
            Serial.println(col);
        }
    }

    // Update the display with only changed keys
    _drawContent();
}

void KeyboardTester::drawGraphicalKeyboard()
{
    processGraphicalKeyboard(true);
}

void KeyboardTester::updateGraphicalKeyboard()
{
    processGraphicalKeyboard(false);
}

void KeyboardTester::drawKeyboardRow(const char *keys[], int keyCount, int keyMatrix[][2], int startX, int y, int keyWidth, int keyHeight)
{
    for (int i = 0; i < keyCount; i++)
    {
        int x = startX + i * (keyWidth + 2); // 2 pixel spacing between keys

        // Get matrix position for this key
        int matrixByte = keyMatrix[i][0];
        int matrixBit = keyMatrix[i][1];
        bool keyPressed = false;
        bool isTested = false;

        if (matrixByte >= 0 && matrixBit >= 0 && matrixByte < 8 && matrixBit < 8)
        {
            keyPressed = _keyPressed[matrixByte][matrixBit];
            isTested = _keyTested[matrixByte][matrixBit];
        }

        // Draw key with current state (highlighting happens when key is pressed)
        drawKey(x, y, keyWidth, keyHeight, keys[i], keyPressed, isTested, keyPressed);
    }
}

void KeyboardTester::updateKeyboardRowChanges(const char *keys[], int keyCount, int keyMatrix[][2], int startX, int y, int keyWidth, int keyHeight, bool &anyKeyChanged)
{
    for (int i = 0; i < keyCount; i++)
    {
        int x = startX + i * (keyWidth + 2); // 2 pixel spacing between keys

        // Get matrix position for this key
        int matrixByte = keyMatrix[i][0];
        int matrixBit = keyMatrix[i][1];

        if (matrixByte >= 0 && matrixBit >= 0 && matrixByte < 8 && matrixBit < 8)
        {
            // Check if this key's state has changed
            bool currentPressed = _keyPressed[matrixByte][matrixBit];
            bool prevPressed = _keyPrevPressed[matrixByte][matrixBit];

            if (currentPressed != prevPressed)
            {
                anyKeyChanged = true;
                bool isTested = _keyTested[matrixByte][matrixBit];

                // Redraw only this key with its new state
                drawKey(x, y, keyWidth, keyHeight, keys[i], currentPressed, isTested, currentPressed);
            }
        }
    }
}

void KeyboardTester::drawKey(int x, int y, int width, int height, const char *label, bool pressed, bool tested, bool highlighted)
{
    Adafruit_GFX &gfx = M1Shield.getGFX();

    // Determine colors based on key state
    uint16_t bgColor, textColor, borderColor = 0x5AEB; // Light gray border

    if (highlighted)
    {
        // Currently pressed - white background, black text
        bgColor = 0xFFFF;   // White
        textColor = 0x0000; // Black
    }
    else if (tested)
    {
        // Key has been tested - dark green background, white text
        bgColor = 0x0320;   // Dark green
        textColor = 0xFFFF; // White
    }
    else
    {
        // Key not yet tested - dark red background, white text
        bgColor = 0x8000;   // Dark red
        textColor = 0xFFFF; // White
    }

    // Draw key rectangle with border
    gfx.fillRect(x, y, width, height, M1Shield.convertColor(bgColor));
    gfx.drawRect(x, y, width, height, M1Shield.convertColor(borderColor));

    // Draw key label centered
    gfx.setTextColor(M1Shield.convertColor(textColor));
    gfx.setTextSize(1);

    // Calculate text position to center it in the key
    int textWidth = strlen(label) * 6; // Approximate character width
    int textX = x + (width - textWidth) / 2;
    int textY = y + (height - 8) / 2; // 8 is approximate character height

    gfx.setCursor(textX, textY);
    gfx.print(label);
}

void KeyboardTester::calculateGraphicalLayout(int &startX, int &startY, int &keyWidth, int &keyHeight)
{
    // Get content area bounds
    uint16_t contentLeft = _getContentLeft();
    uint16_t contentTop = _getContentTop();
    uint16_t contentWidth = _getContentWidth();
    uint16_t contentHeight = _getContentHeight();

    // Define key dimensions and spacing
    keyWidth = 22;
    keyHeight = 20;
    const int keySpacing = 2;

    // Calculate keyboard dimensions for centering
    const int totalKeyboardWidth = 14 * (keyWidth + keySpacing) - keySpacing; // 14 keys in longest row
    const int totalKeyboardHeight = 5 * (keyHeight + 5) + 20;                 // 4 rows + spacebar + spacing

    // Center the keyboard in the content area
    startX = contentLeft + (contentWidth - totalKeyboardWidth) / 2;
    startY = contentTop + (contentHeight - totalKeyboardHeight) / 2;
}

void KeyboardTester::calculateMatrixLayout(int &startX, int &startY, int &cellWidth, int &cellHeight)
{
    // Get content area bounds
    uint16_t contentLeft = _getContentLeft();
    uint16_t contentTop = _getContentTop();
    uint16_t contentWidth = _getContentWidth();
    uint16_t contentHeight = _getContentHeight();

    // Matrix grid dimensions
    cellWidth = 35;
    cellHeight = 20;
    const int headerSpace = 20; // Space for row/column headers

    // Calculate matrix dimensions for centering
    const int totalMatrixWidth = 8 * cellWidth + headerSpace;
    const int totalMatrixHeight = 8 * cellHeight + headerSpace;

    // Center the matrix in the content area
    startX = contentLeft + (contentWidth - totalMatrixWidth) / 2 + headerSpace;
    startY = contentTop + (contentHeight - totalMatrixHeight) / 2 + headerSpace;
}

void KeyboardTester::processGraphicalKeyboard(bool fullDraw)
{
    int startX, startY, keyWidth, keyHeight;
    calculateGraphicalLayout(startX, startY, keyWidth, keyHeight);

    // Get content area bounds for spacebar calculation
    uint16_t contentLeft = _getContentLeft();
    uint16_t contentWidth = _getContentWidth();

    Adafruit_GFX &gfx = M1Shield.getGFX();

    // Define key layout arrays and matrix mappings (shared between full draw and updates)
    const char *row1[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0", ":", "-", "BRK"};
    const char *row2[] = {"UP", "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "@", "LF", "RT"};
    const char *row3[] = {"DN", "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "ENT", "CLR"};
    const char *row4[] = {"SHF", "Z", "X", "C", "V", "B", "N", "M", ",", ".", "/", "SHF"};

    int keyMatrix1[][2] = {{4, 1}, {4, 2}, {4, 3}, {4, 4}, {4, 5}, {4, 6}, {4, 7}, {5, 0}, {5, 1}, {4, 0}, {5, 2}, {5, 5}, {6, 2}};
    int keyMatrix2[][2] = {{6, 3}, {2, 1}, {2, 2}, {2, 3}, {2, 4}, {2, 5}, {2, 6}, {2, 7}, {1, 1}, {1, 7}, {2, 0}, {0, 0}, {6, 5}, {6, 6}};
    int keyMatrix3[][2] = {{6, 4}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {1, 2}, {1, 3}, {1, 4}, {5, 3}, {6, 0}, {6, 1}};
    int keyMatrix4[][2] = {{7, 0}, {3, 2}, {3, 0}, {0, 3}, {2, 6}, {0, 5}, {1, 6}, {1, 5}, {5, 4}, {5, 6}, {5, 7}, {7, 0}};

    bool anyKeyChanged = false;

    if (!fullDraw)
    {
        gfx.startWrite();
    }

    if (fullDraw)
    {
        // Draw all rows
        drawKeyboardRow(row1, 13, keyMatrix1, startX + 30, startY, keyWidth, keyHeight);
        drawKeyboardRow(row2, 14, keyMatrix2, startX + 5, startY + 25, keyWidth, keyHeight);
        drawKeyboardRow(row3, 13, keyMatrix3, startX + 15, startY + 50, keyWidth, keyHeight);
        drawKeyboardRow(row4, 12, keyMatrix4, startX + 15, startY + 75, keyWidth, keyHeight);

        // Always draw spacebar in full draw
        int spaceWidth = 120;
        int spaceX = contentLeft + (contentWidth - spaceWidth) / 2;
        int spaceY = startY + 100;
        drawKey(spaceX, spaceY, spaceWidth, keyHeight, "SPACE", _spacePressed, _spaceKeyTested, _spacePressed);
    }
    else
    {
        // Check each row for changes
        updateKeyboardRowChanges(row1, 13, keyMatrix1, startX + 30, startY, keyWidth, keyHeight, anyKeyChanged);
        updateKeyboardRowChanges(row2, 14, keyMatrix2, startX + 5, startY + 25, keyWidth, keyHeight, anyKeyChanged);
        updateKeyboardRowChanges(row3, 13, keyMatrix3, startX + 15, startY + 50, keyWidth, keyHeight, anyKeyChanged);
        updateKeyboardRowChanges(row4, 12, keyMatrix4, startX + 15, startY + 75, keyWidth, keyHeight, anyKeyChanged);

        // Check spacebar for changes
        if (_spacePressed != _spacePrevPressed)
        {
            anyKeyChanged = true;
            int spaceWidth = 120;
            int spaceX = contentLeft + (contentWidth - spaceWidth) / 2;
            int spaceY = startY + 100;
            drawKey(spaceX, spaceY, spaceWidth, keyHeight, "SPACE", _spacePressed, _spaceKeyTested, _spacePressed);
        }
    }

    if (!fullDraw)
    {
        gfx.endWrite();
        if (anyKeyChanged)
        {
            M1Shield.display();
        }
    }
    else
    {
        M1Shield.display();
    }
}

void KeyboardTester::processMatrixView(bool fullDraw)
{
    int startX, startY, cellWidth, cellHeight;
    calculateMatrixLayout(startX, startY, cellWidth, cellHeight);

    Adafruit_GFX &gfx = M1Shield.getGFX();
    bool anyKeyChanged = false;

    if (!fullDraw)
    {
        gfx.startWrite();
    }

    if (fullDraw)
    {
        // Draw column headers (0-7)
        for (int col = 0; col < 8; col++)
        {
            int x = startX + col * cellWidth + cellWidth / 2 - 3;
            gfx.setCursor(x, startY - 15);
            gfx.setTextColor(M1Shield.convertColor(0x07FF));
            gfx.setTextSize(1);
            gfx.print(col);
        }

        // Draw row headers and all matrix cells
        for (int row = 0; row < 8; row++)
        {
            int y = startY + row * cellHeight;

            // Row header
            gfx.setCursor(startX - 15, y + 6);
            gfx.setTextColor(M1Shield.convertColor(0x07FF));
            gfx.setTextSize(1);
            gfx.print(row);

            for (int col = 0; col < 8; col++)
            {
                int x = startX + col * cellWidth;
                String keyChar = getMatrixKeyName(row, col);
                bool keyPressed = _keyPressed[row][col];
                bool isTested = _keyTested[row][col];
                drawMatrixCell(x, y, cellWidth - 1, cellHeight - 1, row, col, keyChar, keyPressed, isTested, keyPressed);
            }
        }
    }
    else
    {
        // Check each matrix cell for changes
        for (int row = 0; row < 8; row++)
        {
            for (int col = 0; col < 8; col++)
            {
                bool currentPressed = _keyPressed[row][col];
                bool prevPressed = _keyPrevPressed[row][col];

                if (currentPressed != prevPressed)
                {
                    anyKeyChanged = true;
                    int x = startX + col * cellWidth;
                    int y = startY + row * cellHeight;
                    String keyChar = getMatrixKeyName(row, col);
                    bool isTested = _keyTested[row][col];
                    drawMatrixCell(x, y, cellWidth - 1, cellHeight - 1, row, col, keyChar, currentPressed, isTested, currentPressed);
                }
            }
        }
    }

    if (!fullDraw)
    {
        gfx.endWrite();
        if (anyKeyChanged)
        {
            M1Shield.display();
        }
    }
}

String KeyboardTester::getKeyName(uint8_t keyValue)
{
    // Handle special keys
    switch (keyValue)
    {
    case 0xB0:
        return "ENTER";
    case 0xB2:
        return "CLEAR";
    case 0xB1:
        return "BREAK";
    case 0xDA:
        return "UP";
    case 0xD9:
        return "DOWN";
    case 0xD8:
        return "LEFT";
    case 0xD7:
        return "RIGHT";
    case 0x20:
        return "SPACE";
    case 0x81:
        return "SHIFT";
    default:
        // For regular ASCII characters
        if (keyValue >= 32 && keyValue <= 126)
        {
            return String((char)keyValue);
        }
        else
        {
            // For unknown/special keys, show hex value
            String hex = "0x";
            if (keyValue < 16)
                hex += "0";
            hex += String(keyValue, HEX);
            return hex;
        }
    }
}

void KeyboardTester::updateModeTitle()
{
    switch (_currentMode)
    {
    case MODE_GRAPHICAL:
        _setTitle("Keyboard Test - Graphical");
        break;
    case MODE_MATRIX:
        _setTitle("Keyboard Test - Matrix");
        break;
    }
}

void KeyboardTester::drawMatrixView()
{
    processMatrixView(true);
}

void KeyboardTester::updateMatrixView()
{
    processMatrixView(false);
}

String KeyboardTester::getMatrixKeyName(uint8_t row, uint8_t col)
{
    // TRS-80 Model 1 keyboard matrix layout - matches KeyboardChangeIterator lookup table
    // This is the actual hardware matrix mapping used by the library
    static const char *keyMatrix[8][8] = {
        {"@", "A", "B", "C", "D", "E", "F", "G"},             // Row 0 (0x3801)
        {"H", "I", "J", "K", "L", "M", "N", "O"},             // Row 1 (0x3802)
        {"P", "Q", "R", "S", "T", "U", "V", "W"},             // Row 2 (0x3804)
        {"X", "Y", "Z", "", "", "", "", ""},                  // Row 3 (0x3808) - unused positions 3-7
        {"0", "1", "2", "3", "4", "5", "6", "7"},             // Row 4 (0x3810)
        {"8", "9", ":", ";", ",", "-", ".", "/"},             // Row 5 (0x3820)
        {"ENT", "CLR", "BRK", "UP", "DN", "LF", "RT", "SPC"}, // Row 6 (0x3840) - special keys
        {"SHF", "", "", "", "", "", "", ""}                   // Row 7 (0x3860) - shift and unused
    };

    if (row < 8 && col < 8)
    {
        const char *key = keyMatrix[row][col];
        return String(key);
    }

    // For empty positions, show coordinates
    return "";
}

void KeyboardTester::drawMatrixCell(int x, int y, int width, int height, uint8_t row, uint8_t col, String keyChar, bool pressed, bool tested, bool highlighted)
{
    Adafruit_GFX &gfx = M1Shield.getGFX();

    uint16_t bgColor, textColor, borderColor = 0xFFFF;

    if (highlighted)
    {
        // Currently pressed - white background, black text
        bgColor = 0xFFFF;   // White
        textColor = 0x0000; // Black
    }
    else if (tested)
    {
        // Key has been tested - dark green background, white text
        bgColor = 0x0320;   // Dark green
        textColor = 0xFFFF; // White
    }
    else
    {
        // Key not yet tested - dark red background, white text
        bgColor = 0x8000;   // Dark red
        textColor = 0xFFFF; // White
    }

    // Draw cell rectangle with border
    gfx.fillRect(x, y, width, height, M1Shield.convertColor(bgColor));
    gfx.drawRect(x, y, width, height, M1Shield.convertColor(borderColor));

    // Draw key name centered in cell
    gfx.setTextColor(M1Shield.convertColor(textColor));
    gfx.setTextSize(1);

    // For longer key names, use smaller text or abbreviate
    String displayText = keyChar;
    if (keyChar.length() > 4)
    {
        // Abbreviate longer key names
        if (keyChar == "ENTER")
            displayText = "ENT";
        else if (keyChar == "CLEAR")
            displayText = "CLR";
        else if (keyChar == "BREAK")
            displayText = "BRK";
        else if (keyChar == "SHIFT")
            displayText = "SHF";
        else
            displayText = keyChar.substring(0, 3);
    }

    int textWidth = displayText.length() * 6; // Approximate character width
    int textX = x + (width - textWidth) / 2;
    int textY = y + (height - 8) / 2; // 8 is approximate character height

    gfx.setCursor(textX, textY);
    gfx.print(displayText);
}

Screen *KeyboardTester::actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY)
{
    if (action & BUTTON_MENU)
    {
        Serial.println(F("Returning to main menu from Keyboard Tester"));
        return new MainDemoMenu();
    }

    if (action & BUTTON_RIGHT)
    {
        // Reset all key testing status
        Serial.println(F("Resetting key test status"));
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                _keyTested[i][j] = false;
                _keyPressed[i][j] = false;
                _keyPrevPressed[i][j] = false;
                _keyHighlightTime[i][j] = 0;
            }
        }
        _spaceKeyTested = false;
        _spacePressed = false;
        _spacePrevPressed = false;
        _spaceHighlightTime = 0;
        _keyPressCount = 0;
        _needsFullRedraw = true; // Force full redraw after reset
    }

    if (action & UP_ANY)
    {
        // Cycle to previous mode
        _currentMode = (DisplayMode)((_currentMode + MODE_COUNT - 1) % MODE_COUNT);
        updateModeTitle();
        _needsFullRedraw = true; // Force full redraw for mode change
        _drawContent();          // Redraw the screen with the new mode
        Serial.print(F("Switched to mode: "));
        Serial.println((int)_currentMode);
    }

    if (action & DOWN_ANY)
    {
        // Cycle to next mode
        _currentMode = (DisplayMode)((_currentMode + 1) % MODE_COUNT);
        updateModeTitle();
        _needsFullRedraw = true; // Force full redraw for mode change
        _drawContent();          // Redraw the screen with the new mode
        Serial.print(F("Switched to mode: "));
        Serial.println((int)_currentMode);
    }

    return nullptr;
}
