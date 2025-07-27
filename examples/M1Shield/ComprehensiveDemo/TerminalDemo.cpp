#include "TerminalDemo.h"
#include "MainDemoMenu.h"
#include <Arduino.h>

TerminalDemo::TerminalDemo() : M1TerminalScreen()
{
    _setTitle("Terminal Demo");

    _lastDemoUpdate = millis();
    _demoStep = 0;
    _demoRunning = true;

    showDemoInstructions();

    Serial.println("Terminal Demo initialized");
}

void TerminalDemo::loop()
{
    M1TerminalScreen::loop();

    if (_demoRunning)
    {
        runTerminalDemo();
    }
}

void TerminalDemo::showDemoInstructions()
{
    cls();
    setTextColor(0x07E0); // Green
    print("M1TERMINAL DEMONSTRATION");
    setTextColor(0xFFFF); // White
    newline();
    newline();
    print("This demonstrates the full");
    newline();
    print("TRS-80 Model I terminal");
    newline();
    print("emulation capabilities.");
    newline();
    newline();
    setTextColor(0xFFE0); // Yellow
    print("Watch the automated demo...");
    newline();
    setTextColor(0x07FF); // Cyan
    print("Press MENU to return");
    newline();
    newline();
}

void TerminalDemo::runTerminalDemo()
{
    unsigned long elapsed = millis() - _lastDemoUpdate;

    switch (_demoStep)
    {
    case 0:
        if (elapsed > 3000)
        {
            setTextColor(0xF81F); // Magenta
            print("READY");
            newline();
            print(">");
            _lastDemoUpdate = millis();
            _demoStep++;
        }
        break;

    case 1:
        if (elapsed > 1000)
        {
            setTextColor(0xFFFF); // White
            print("PRINT \"HELLO WORLD\"");
            newline();
            _lastDemoUpdate = millis();
            _demoStep++;
        }
        break;

    case 2:
        if (elapsed > 1500)
        {
            setTextColor(0x07E0); // Green
            print("HELLO WORLD");
            newline();
            setTextColor(0xF81F); // Magenta
            print(">");
            _lastDemoUpdate = millis();
            _demoStep++;
        }
        break;

    case 3:
        if (elapsed > 2000)
        {
            setTextColor(0xFFFF); // White
            print("FOR I=1 TO 5");
            newline();
            print("PRINT I,\"DEMO LINE\"");
            newline();
            print("NEXT I");
            newline();
            _lastDemoUpdate = millis();
            _demoStep++;
        }
        break;

    case 4:
        if (elapsed > 2000)
        {
            setTextColor(0x07FF); // Cyan
            for (int i = 1; i <= 5; i++)
            {
                print(String(i) + " DEMO LINE");
                newline();
            }
            setTextColor(0xF81F); // Magenta
            print(">");
            _lastDemoUpdate = millis();
            _demoStep++;
        }
        break;

    case 5:
        if (elapsed > 3000)
        {
            setTextColor(0xFFE0); // Yellow
            print("TERMINAL DEMO COMPLETE!");
            newline();
            print("Features demonstrated:");
            newline();
            setTextColor(0xFFFF); // White
            print("* Color text output");
            newline();
            print("* Cursor positioning");
            newline();
            print("* Line wrapping");
            newline();
            print("* TRS-80 character set");
            newline();
            newline();
            setTextColor(0x07FF); // Cyan
            print("Press MENU to return");
            newline();
            _demoRunning = false;
        }
        break;
    }
}

Screen *TerminalDemo::actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY)
{
    if (action & BUTTON_MENU)
    {
        Serial.println("Returning to main menu from Terminal Demo");
        return new MainDemoMenu();
    }

    // Let parent handle other actions
    return M1TerminalScreen::actionTaken(action, offsetX, offsetY);
}
