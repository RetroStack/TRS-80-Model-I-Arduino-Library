#include <Arduino.h>
#include <Model1.h>
#include <Keyboard.h>

// Keyboard class instance for TRS-80 keyboard input
Keyboard keyboard;

// Keyboard analysis structures
struct KeyStatistics
{
    uint32_t totalKeyPresses;
    uint32_t alphaKeys;
    uint32_t numericKeys;
    uint32_t specialKeys;
    uint32_t controlKeys;
    unsigned long lastKeyTime;
    unsigned long sessionStart;
};

struct KeyboardState
{
    uint8_t currentKeys[64]; // Track all currently pressed keys
    uint8_t keyCount;
    bool shiftPressed;
    bool ctrlPressed;
    unsigned long lastScanTime;
};

KeyStatistics stats = {};
KeyboardState keyState = {};

void setup()
{
    // Initialize serial communication at high speed
    Serial.begin(115200);
    delay(1000);

    // Initialize the Model 1 hardware interface
    Model1.begin();

    // Take control of the TRS-80 bus
    Model1.activateTestSignal();

    // Initialize keyboard state
    keyboard.update();

    // Initialize statistics
    stats.sessionStart = millis();
    keyState.lastScanTime = millis();

    Serial.println(F("=== Comprehensive Keyboard Example ==="));
    Serial.println(F("Advanced keyboard analysis and diagnostics"));
    Serial.println(F("Press keys on the TRS-80 keyboard for detailed analysis..."));
    Serial.println();
}

void loop()
{
    // Advanced keyboard state analysis
    demonstrateAdvancedKeyboardAnalysis();

    // Key combination detection
    demonstrateKeyCombinations();

    // Typing pattern analysis
    demonstrateTypingAnalysis();

    // Keyboard diagnostics
    demonstrateKeyboardDiagnostics();

    // Display periodic statistics
    displayPeriodicStatistics();

    // Update scan timing
    keyState.lastScanTime = millis();

    // Controlled scan rate
    delay(10);
}

void demonstrateAdvancedKeyboardAnalysis()
{
    // Get all keyboard changes with detailed analysis
    KeyboardChangeIterator it = keyboard.changes();

    while (it.hasNext())
    {
        uint8_t keyValue = it.keyValue();

        if (it.wasJustPressed())
        {
            handleKeyPress(keyValue);
        }
        else if (it.wasJustReleased())
        {
            handleKeyRelease(keyValue);
        }

        it.next();
    }
}

void handleKeyPress(uint8_t keyValue)
{
    // Update statistics
    stats.totalKeyPresses++;
    stats.lastKeyTime = millis();

    // Categorize the key
    categorizeKey(keyValue);

    // Add to current keys list
    if (keyState.keyCount < 64)
    {
        keyState.currentKeys[keyState.keyCount] = keyValue;
        keyState.keyCount++;
    }

    // Update modifier states
    updateModifierState(keyValue, true);

    // Display detailed key information
    displayKeyPressDetails(keyValue);
}

void handleKeyRelease(uint8_t keyValue)
{
    // Remove from current keys list
    for (int i = 0; i < keyState.keyCount; i++)
    {
        if (keyState.currentKeys[i] == keyValue)
        {
            // Shift remaining keys down
            for (int j = i; j < keyState.keyCount - 1; j++)
            {
                keyState.currentKeys[j] = keyState.currentKeys[j + 1];
            }
            keyState.keyCount--;
            break;
        }
    }

    // Update modifier states
    updateModifierState(keyValue, false);

    Serial.print(F("Key released: "));
    displayKeyInfo(keyValue);
    Serial.println();
}

void categorizeKey(uint8_t keyValue)
{
    if (keyValue >= 'A' && keyValue <= 'Z')
    {
        stats.alphaKeys++;
    }
    else if (keyValue >= 'a' && keyValue <= 'z')
    {
        stats.alphaKeys++;
    }
    else if (keyValue >= '0' && keyValue <= '9')
    {
        stats.numericKeys++;
    }
    else if (keyValue < 32)
    {
        stats.controlKeys++;
    }
    else
    {
        stats.specialKeys++;
    }
}

void updateModifierState(uint8_t keyValue, bool pressed)
{
    // Update shift state (TRS-80 shift key codes may vary)
    if (keyValue == 0x10 || keyValue == 0x11) // Adjust these values as needed
    {
        keyState.shiftPressed = pressed;
    }

    // Update control state
    if (keyValue == 0x03 || keyValue == 0x17) // Adjust these values as needed
    {
        keyState.ctrlPressed = pressed;
    }
}

void displayKeyPressDetails(uint8_t keyValue)
{
    unsigned long currentTime = millis();
    unsigned long timeSinceLastKey = currentTime - stats.lastKeyTime;

    Serial.print(F("Key press #"));
    Serial.print(stats.totalKeyPresses);
    Serial.print(F(": "));
    displayKeyInfo(keyValue);

    Serial.print(F(" | Time: +"));
    Serial.print(timeSinceLastKey);
    Serial.print(F("ms"));

    if (keyState.keyCount > 1)
    {
        Serial.print(F(" | Chord: "));
        Serial.print(keyState.keyCount);
        Serial.print(F(" keys"));
    }

    if (keyState.shiftPressed)
    {
        Serial.print(F(" | +SHIFT"));
    }

    if (keyState.ctrlPressed)
    {
        Serial.print(F(" | +CTRL"));
    }

    Serial.println();
}

void displayKeyInfo(uint8_t keyValue)
{
    // Display character if printable
    if (keyValue >= 32 && keyValue <= 126)
    {
        Serial.print(F("'"));
        Serial.print((char)keyValue);
        Serial.print(F("'"));
    }
    else
    {
        Serial.print(F("["));
        Serial.print(getKeyName(keyValue));
        Serial.print(F("]"));
    }

    Serial.print(F(" (0x"));
    if (keyValue < 0x10)
        Serial.print('0');
    Serial.print(keyValue, HEX);
    Serial.print(F(")"));
}

const char *getKeyName(uint8_t keyValue)
{
    switch (keyValue)
    {
    case 0x0D:
        return "ENTER";
    case 0x08:
        return "BACKSPACE";
    case 0x20:
        return "SPACE";
    case 0x1B:
        return "BREAK";
    case 0x09:
        return "TAB";
    case 0x0C:
        return "CLEAR";
    case 0x1E:
        return "UP";
    case 0x1F:
        return "DOWN";
    case 0x1C:
        return "RIGHT";
    case 0x1D:
        return "LEFT";
    case 0x10:
        return "SHIFT";
    case 0x11:
        return "SHIFT";
    case 0x03:
        return "CTRL";
    case 0x17:
        return "CTRL";
    default:
        return "UNKNOWN";
    }
}

void demonstrateKeyCombinations()
{
    // Only check for combinations when multiple keys are pressed
    if (keyState.keyCount > 1)
    {
        static unsigned long lastComboReport = 0;
        unsigned long currentTime = millis();

        // Report combinations every 100ms to avoid spam
        if (currentTime - lastComboReport > 100)
        {
            Serial.print(F("Key combination detected: "));
            for (int i = 0; i < keyState.keyCount; i++)
            {
                if (i > 0)
                    Serial.print(F(" + "));
                displayKeyInfo(keyState.currentKeys[i]);
            }
            Serial.println();

            lastComboReport = currentTime;
        }
    }
}

void demonstrateTypingAnalysis()
{
    static unsigned long lastAnalysis = 0;
    static uint32_t lastKeyCount = 0;
    unsigned long currentTime = millis();

    // Perform typing analysis every 5 seconds
    if (currentTime - lastAnalysis > 5000 && stats.totalKeyPresses > lastKeyCount)
    {
        unsigned long sessionTime = currentTime - stats.sessionStart;
        float keysPerMinute = (float)(stats.totalKeyPresses * 60000) / sessionTime;
        float keysPerSecond = (float)stats.totalKeyPresses / (sessionTime / 1000.0);

        Serial.println(F("\\n--- Typing Analysis ---"));
        Serial.print(F("Keys per minute: "));
        Serial.println(keysPerMinute, 1);
        Serial.print(F("Keys per second: "));
        Serial.println(keysPerSecond, 2);

        // Calculate key distribution
        float alphaPercent = (float)stats.alphaKeys / stats.totalKeyPresses * 100;
        float numericPercent = (float)stats.numericKeys / stats.totalKeyPresses * 100;
        float specialPercent = (float)stats.specialKeys / stats.totalKeyPresses * 100;
        float controlPercent = (float)stats.controlKeys / stats.totalKeyPresses * 100;

        Serial.print(F("Key distribution - Alpha: "));
        Serial.print(alphaPercent, 1);
        Serial.print(F("%, Numeric: "));
        Serial.print(numericPercent, 1);
        Serial.print(F("%, Special: "));
        Serial.print(specialPercent, 1);
        Serial.print(F("%, Control: "));
        Serial.print(controlPercent, 1);
        Serial.println(F("%"));
        Serial.println();

        lastAnalysis = currentTime;
        lastKeyCount = stats.totalKeyPresses;
    }
}

void demonstrateKeyboardDiagnostics()
{
    static unsigned long lastDiagnostic = 0;
    unsigned long currentTime = millis();

    // Perform diagnostics every 10 seconds
    if (currentTime - lastDiagnostic > 10000)
    {
        Serial.println(F("\\n--- Keyboard Diagnostics ---"));

        // Check keyboard responsiveness
        unsigned long timeSinceLastKey = currentTime - stats.lastKeyTime;
        if (timeSinceLastKey > 30000) // 30 seconds without input
        {
            Serial.println(F("Keyboard idle for >30 seconds"));
        }
        else if (timeSinceLastKey > 5000) // 5 seconds without input
        {
            Serial.print(F("Keyboard idle for "));
            Serial.print(timeSinceLastKey / 1000);
            Serial.println(F(" seconds"));
        }
        else
        {
            Serial.println(F("Keyboard active"));
        }

        // Scan timing analysis
        unsigned long scanInterval = currentTime - keyState.lastScanTime;
        Serial.print(F("Scan interval: "));
        Serial.print(scanInterval);
        Serial.println(F("ms"));

        if (scanInterval > 50)
        {
            Serial.println(F("Warning: Scan interval >50ms - may miss rapid keypresses"));
        }

        // Memory usage check
        Serial.print(F("Currently pressed keys: "));
        Serial.println(keyState.keyCount);

        if (keyState.keyCount > 10)
        {
            Serial.println(F("Warning: Many keys pressed simultaneously"));
        }

        Serial.println();
        lastDiagnostic = currentTime;
    }
}

void displayPeriodicStatistics()
{
    static unsigned long lastStats = 0;
    unsigned long currentTime = millis();

    // Display statistics every 30 seconds
    if (currentTime - lastStats > 30000 && stats.totalKeyPresses > 0)
    {
        unsigned long sessionTime = currentTime - stats.sessionStart;

        Serial.println(F("\\n=== Session Statistics ==="));
        Serial.print(F("Session time: "));
        Serial.print(sessionTime / 1000);
        Serial.println(F(" seconds"));
        Serial.print(F("Total key presses: "));
        Serial.println(stats.totalKeyPresses);
        Serial.print(F("Alpha keys: "));
        Serial.println(stats.alphaKeys);
        Serial.print(F("Numeric keys: "));
        Serial.println(stats.numericKeys);
        Serial.print(F("Special keys: "));
        Serial.println(stats.specialKeys);
        Serial.print(F("Control keys: "));
        Serial.println(stats.controlKeys);
        Serial.println(F("========================\\n"));

        lastStats = currentTime;
    }
}
