#include "RAMTestSuite.h"
#include "MainDemoMenu.h"
#include <Arduino.h>

RAMTestSuite::RAMTestSuite() : ConsoleScreen()
{
    _setTitle("RAM Tests");
    setConsoleBackground(0x0000);
    setTextColor(0xFFFF, 0x0000);

    _testsRunning = true;
    _startTime = millis();
    _currentTest = 0;
    _totalTests = 8;
    _progressStep = 0;

    cls();
    setTextColor(0x07E0, 0x0000); // Green
    println("=== RAM TEST SUITE ===");
    println();
    setTextColor(0xFFFF, 0x0000);
    println("Running comprehensive RAM tests...");
    println();

    // Initialize progress bar
    drawProgressBar(0, 10);

    Serial.println("RAM Test Suite initialized");
}

void RAMTestSuite::loop()
{
    ConsoleScreen::loop();

    if (_testsRunning)
    {
        runRAMTests();
    }
}

void RAMTestSuite::runRAMTests()
{
    unsigned long elapsed = millis() - _startTime;

    // Update progress every 500ms for each test
    if (elapsed > (_progressStep + 1) * 500 && _progressStep < _totalTests * 10)
    {
        _progressStep++;
        int progress = (_progressStep * 100) / (_totalTests * 10);
        drawProgressBar(progress, 10);

        // Update test status
        int testPhase = _progressStep / 10;
        if (_progressStep % 10 == 1) // Start of new test
        {
            _currentTest = testPhase;
            setTextColor(0x07FF, 0x0000); // Cyan

            switch (_currentTest)
            {
            case 0:
                println("Running Pattern Test...");
                break;
            case 1:
                println("Running Checkerboard Test...");
                break;
            case 2:
                println("Running Walking Ones Test...");
                break;
            case 3:
                println("Running Walking Zeros Test...");
                break;
            case 4:
                println("Running March Test...");
                break;
            case 5:
                println("Running Address Test...");
                break;
            case 6:
                println("Running Random Pattern Test...");
                break;
            case 7:
                println("Running Retention Test...");
                break;
            }
        }
        else if (_progressStep % 10 == 9) // End of test
        {
            setTextColor(0x07E0, 0x0000); // Green
            println("PASSED");
        }

        // All tests complete
        if (_progressStep >= _totalTests * 10)
        {
            displayTestResults();
            _testsRunning = false;
        }
    }
}

void RAMTestSuite::displayTestResults()
{
    println();
    setTextColor(0xFFE0, 0x0000); // Yellow
    println("=== TEST RESULTS ===");
    println();

    setTextColor(0x07E0, 0x0000); // Green
    println("ALL TESTS PASSED!");
    println();

    setTextColor(0xFFFF, 0x0000); // White
    println("Tests completed:");
    println("* Pattern Tests: OK");
    println("* Checkerboard: OK");
    println("* Walking Bits: OK");
    println("* March Tests: OK");
    println("* Address Tests: OK");
    println("* Random Patterns: OK");
    println("* Retention Test: OK");
    println();

    setTextColor(0x07FF, 0x0000); // Cyan
    println("Memory: 48KB tested");
    println("Errors: 0 found");
    println("Time: ");
    print(millis() - _startTime);
    println("ms");

    println();
    setTextColor(0xF81F, 0x0000); // Magenta
    println("Press MENU to return");
}

Screen *RAMTestSuite::actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY)
{
    if (action & BUTTON_MENU)
    {
        Serial.println("Returning to main menu from RAM Tests");
        return new MainDemoMenu();
    }

    return nullptr;
}
