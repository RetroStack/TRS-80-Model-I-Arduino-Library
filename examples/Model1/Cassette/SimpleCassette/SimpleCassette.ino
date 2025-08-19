#include <Arduino.h>
#include <Model1.h>
#include <Cassette.h>

// Cassette class instance for TRS-80 cassette/video control
Cassette cassette;

// Simple melody data for demonstration
int simpleMelody[] = {
    262, 294, 330, 349, 392, 440, 494, 523 // C4 to C5 scale
};

float simpleDurations[] = {
    0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 1.0};

const size_t simpleMelodyLength = 8;

void setup()
{
    // Initialize serial communication
    Serial.begin(9600);
    delay(1000);

    // Initialize the Model 1 hardware interface
    Model1.begin();

    // Take control of the TRS-80 bus
    Model1.activateTestSignal();

    Serial.println(F("=== Simple Cassette Example ==="));
    Serial.println(F("Basic cassette and video control operations"));
    Serial.println();
}

void loop()
{
    // Character mode demonstrations
    demonstrateCharacterModes();
    delay(3000);

    // Simple sound demonstration
    demonstrateSimpleSound();
    delay(3000);

    // Remote control demonstration
    demonstrateRemoteControl();
    delay(3000);
}

void demonstrateCharacterModes()
{
    Serial.println(F("--- Character Mode Control ---"));

    // Check current mode
    bool is64Mode = cassette.is64CharacterMode();
    Serial.print(F("Current mode: "));
    Serial.println(is64Mode ? F("64 characters") : F("32 characters"));

    // Switch to 64-character mode
    Serial.println(F("Setting 64-character mode..."));
    cassette.set64CharacterMode();
    delay(1000);

    // Verify the change
    is64Mode = cassette.is64CharacterMode();
    Serial.print(F("Mode is now: "));
    Serial.println(is64Mode ? F("64 characters") : F("32 characters"));
    delay(2000);

    // Switch to 32-character mode
    Serial.println(F("Setting 32-character mode..."));
    cassette.set32CharacterMode();
    delay(1000);

    // Verify the change
    is64Mode = cassette.is64CharacterMode();
    Serial.print(F("Mode is now: "));
    Serial.println(is64Mode ? F("64 characters") : F("32 characters"));
    delay(2000);

    // Return to 64-character mode for normal operation
    Serial.println(F("Returning to 64-character mode..."));
    cassette.set64CharacterMode();

    Serial.println();
}

void demonstrateSimpleSound()
{
    Serial.println(F("--- Simple Sound Generation ---"));

    // Play a simple ascending scale
    Serial.println(F("Playing simple scale..."));
    cassette.playSong(simpleMelody, simpleDurations, simpleMelodyLength, 120);

    Serial.println(F("Scale complete"));
    delay(1000);

    // Play some individual tones
    Serial.println(F("Playing individual tones:"));

    // Play middle C
    Serial.println(F("Middle C (262 Hz)"));
    int middleC[] = {262};
    float halfSecond[] = {0.5};
    cassette.playSong(middleC, halfSecond, 1, 120);
    delay(750);

    // Play A440
    Serial.println(F("A440 (440 Hz)"));
    int a440[] = {440};
    cassette.playSong(a440, halfSecond, 1, 120);
    delay(750);

    // Play high C
    Serial.println(F("High C (523 Hz)"));
    int highC[] = {523};
    cassette.playSong(highC, halfSecond, 1, 120);
    delay(750);

    Serial.println();
}

void demonstrateRemoteControl()
{
    Serial.println(F("--- Remote Control Operations ---"));

    // The remote control typically controls cassette motor/relay
    Serial.println(F("Activating remote control..."));
    cassette.activateRemote();

    Serial.println(F("Remote is now active"));
    Serial.println(F("(This would typically start a cassette motor)"));
    delay(2000);

    Serial.println(F("Deactivating remote control..."));
    cassette.deactivateRemote();

    Serial.println(F("Remote is now inactive"));
    Serial.println(F("(This would typically stop a cassette motor)"));
    delay(1000);

    Serial.println(F("Remote control demonstration complete"));
    Serial.println();
}
