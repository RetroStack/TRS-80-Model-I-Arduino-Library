#include <Arduino.h>
#include <Model1.h>
#include <Cassette.h>

// Include all the melody definitions from the original example
extern int imperialMelody[];
extern float imperialDurations[];
extern const size_t imperialMelodyLength;

extern int tetrisMelodyLead[];
extern float tetrisDurationsLead[];
extern const size_t tetrisMelodyLength;

extern int marioMelody[];
extern float marioDurations[];
extern const size_t marioMelodyLength;

extern int underworldMelody[];
extern float underworldDurations[];
extern const size_t underworldMelodyLength;

extern int simpsonsMelody[];
extern float simpsonsDurations[];
extern const size_t simpsonsMelodyLength;

extern int gameOfThronesMelody[];
extern float gameOfThronesDurations[];
extern const size_t gameOfThronesMelodyLength;

extern int doomMelody[];
extern float doomDurations[];
extern const size_t doomMelodyLength;

extern int pacmanMelody[];
extern float pacmanDurations[];
extern const size_t pacmanMelodyLength;

// Cassette class instance
Cassette cassette;

// Analysis structures
struct MelodyInfo
{
    const char *name;
    int *melody;
    float *durations;
    size_t length;
    int tempo;
};

struct CassetteAnalysis
{
    unsigned long totalPlayTime;
    uint32_t totalNotes;
    uint32_t songCount;
    int lowestNote;
    int highestNote;
    float averageNoteDuration;
};

CassetteAnalysis analysis = {};

// Define all available melodies
MelodyInfo melodies[] = {
    {"Imperial March", imperialMelody, imperialDurations, imperialMelodyLength, 120},
    {"Tetris", tetrisMelodyLead, tetrisDurationsLead, tetrisMelodyLength, 140},
    {"Super Mario", marioMelody, marioDurations, marioMelodyLength, 180},
    {"Mario Underworld", underworldMelody, underworldDurations, underworldMelodyLength, 180},
    {"The Simpsons", simpsonsMelody, simpsonsDurations, simpsonsMelodyLength, 160},
    {"Game of Thrones", gameOfThronesMelody, gameOfThronesDurations, gameOfThronesMelodyLength, 160},
    {"Doom E1M1", doomMelody, doomDurations, doomMelodyLength, 140},
    {"Pac-Man", pacmanMelody, pacmanDurations, pacmanMelodyLength, 120}};

const int melodyCount = 8;

void setup()
{
    // Initialize serial communication at high speed
    Serial.begin(115200);
    delay(1000);

    // Initialize the Model 1 hardware interface
    Model1.begin();

    // Take control of the TRS-80 bus
    Model1.activateTestSignal();

    // Initialize analysis data
    analysis.lowestNote = 10000;
    analysis.highestNote = 0;

    Serial.println(F("=== Comprehensive Cassette Example ==="));
    Serial.println(F("Advanced cassette control, sound generation, and analysis"));
    Serial.println();
}

void loop()
{
    // Comprehensive character mode analysis
    demonstrateAdvancedCharacterModes();
    delay(3000);

    // Advanced sound generation and analysis
    demonstrateAdvancedSoundGeneration();
    delay(4000);

    // Melody library demonstration
    demonstrateMelodyLibrary();
    delay(5000);

    // Remote control timing analysis
    demonstrateAdvancedRemoteControl();
    delay(3000);

    // Cassette system diagnostics
    demonstrateCassetteSystemDiagnostics();
    delay(4000);

    // Custom melody generation
    demonstrateCustomMelodyGeneration();
    delay(6000);
}

void demonstrateAdvancedCharacterModes()
{
    Serial.println(F("\\n--- Advanced Character Mode Analysis ---"));

    // Detailed mode switching with timing
    Serial.println(F("Analyzing character mode switching:"));

    // Test mode switching timing
    unsigned long startTime = micros();
    cassette.set64CharacterMode();
    unsigned long mode64Time = micros() - startTime;

    startTime = micros();
    cassette.set32CharacterMode();
    unsigned long mode32Time = micros() - startTime;

    startTime = micros();
    cassette.set64CharacterMode(); // Return to 64-char mode
    unsigned long returnTime = micros() - startTime;

    Serial.print(F("64-char mode switch time: "));
    Serial.print(mode64Time);
    Serial.println(F(" μs"));

    Serial.print(F("32-char mode switch time: "));
    Serial.print(mode32Time);
    Serial.println(F(" μs"));

    Serial.print(F("Return to 64-char time: "));
    Serial.print(returnTime);
    Serial.println(F(" μs"));

    // Rapid mode switching test
    Serial.println(F("\\nRapid mode switching test (10 cycles):"));
    startTime = millis();

    for (int i = 0; i < 10; i++)
    {
        cassette.set32CharacterMode();
        delayMicroseconds(100);
        cassette.set64CharacterMode();
        delayMicroseconds(100);
    }

    unsigned long totalTime = millis() - startTime;
    Serial.print(F("10 mode switch cycles completed in "));
    Serial.print(totalTime);
    Serial.println(F("ms"));

    // Verify final state
    bool finalMode = cassette.is64CharacterMode();
    Serial.print(F("Final mode: "));
    Serial.println(finalMode ? F("64 characters") : F("32 characters"));
}

void demonstrateAdvancedSoundGeneration()
{
    Serial.println(F("\\n--- Advanced Sound Generation ---"));

    // Frequency response testing
    Serial.println(F("Testing frequency response:"));

    int testFrequencies[] = {100, 200, 440, 880, 1000, 2000, 4000};
    const char *freqNames[] = {"Low 100Hz", "Low 200Hz", "A440", "High A", "1kHz", "2kHz", "4kHz"};

    for (int i = 0; i < 7; i++)
    {
        Serial.print(F("Playing "));
        Serial.print(freqNames[i]);
        Serial.print(F(" ("));
        Serial.print(testFrequencies[i]);
        Serial.println(F("Hz)"));

        int frequency[] = {testFrequencies[i]};
        float duration[] = {0.3};

        unsigned long playStart = millis();
        cassette.playSong(frequency, duration, 1, 120);
        unsigned long playTime = millis() - playStart;

        Serial.print(F("  Actual play time: "));
        Serial.print(playTime);
        Serial.println(F("ms"));

        delay(200); // Brief pause between tones
    }

    // Chord generation test
    Serial.println(F("\\nChord simulation (rapid note sequence):"));
    int chordNotes[] = {262, 330, 392}; // C major chord
    float chordDurations[] = {0.1, 0.1, 0.1};

    cassette.playSong(chordNotes, chordDurations, 3, 200); // Fast tempo for chord effect
    delay(500);
}

void demonstrateMelodyLibrary()
{
    Serial.println(F("\\n--- Melody Library Analysis ---"));

    // Analyze all available melodies
    Serial.println(F("Available melodies:"));
    Serial.println(F("Name               | Notes | Tempo | Est. Duration"));
    Serial.println(F("-------------------|-------|-------|-------------"));

    for (int i = 0; i < melodyCount; i++)
    {
        MelodyInfo &melody = melodies[i];

        // Calculate estimated duration
        float totalDuration = 0;
        for (size_t j = 0; j < melody.length; j++)
        {
            totalDuration += melody.durations[j];
        }

        // Adjust for tempo (120 BPM = 1 beat per 0.5 seconds)
        float estimatedSeconds = totalDuration * (120.0 / melody.tempo) * 0.5;

        Serial.print(melody.name);
        // Pad name to 18 characters
        for (int pad = strlen(melody.name); pad < 18; pad++)
        {
            Serial.print(' ');
        }
        Serial.print(F(" | "));
        Serial.print(melody.length);
        if (melody.length < 100)
            Serial.print(' ');
        if (melody.length < 10)
            Serial.print(' ');
        Serial.print(F("  | "));
        Serial.print(melody.tempo);
        if (melody.tempo < 100)
            Serial.print(' ');
        Serial.print(F("   | "));
        Serial.print(estimatedSeconds, 1);
        Serial.println(F("s"));
    }

    // Play a selection of melodies with analysis
    Serial.println(F("\\nPlaying melody selection:"));

    for (int i = 0; i < 3; i++) // Play first 3 melodies
    {
        MelodyInfo &melody = melodies[i];

        Serial.print(F("Playing: "));
        Serial.println(melody.name);

        // Analyze melody before playing
        analyzeMelody(melody);

        unsigned long playStart = millis();
        cassette.playSong(melody.melody, melody.durations, melody.length, melody.tempo);
        unsigned long actualPlayTime = millis() - playStart;

        Serial.print(F("Actual play time: "));
        Serial.print(actualPlayTime);
        Serial.println(F("ms"));

        // Update global analysis
        analysis.totalPlayTime += actualPlayTime;
        analysis.totalNotes += melody.length;
        analysis.songCount++;

        delay(1000); // Pause between songs
    }
}

void analyzeMelody(MelodyInfo &melody)
{
    int localLowest = 10000;
    int localHighest = 0;
    float totalDuration = 0;

    for (size_t i = 0; i < melody.length; i++)
    {
        if (melody.melody[i] < localLowest && melody.melody[i] > 0)
        {
            localLowest = melody.melody[i];
        }
        if (melody.melody[i] > localHighest)
        {
            localHighest = melody.melody[i];
        }
        totalDuration += melody.durations[i];
    }

    Serial.print(F("  Range: "));
    Serial.print(localLowest);
    Serial.print(F("Hz - "));
    Serial.print(localHighest);
    Serial.print(F("Hz ("));
    Serial.print((float)localHighest / localLowest, 2);
    Serial.println(F(" ratio)"));

    Serial.print(F("  Avg note duration: "));
    Serial.print(totalDuration / melody.length, 2);
    Serial.println(F(" beats"));

    // Update global statistics
    if (localLowest < analysis.lowestNote)
        analysis.lowestNote = localLowest;
    if (localHighest > analysis.highestNote)
        analysis.highestNote = localHighest;
}

void demonstrateAdvancedRemoteControl()
{
    Serial.println(F("\\n--- Advanced Remote Control ---"));

    // Remote control timing analysis
    Serial.println(F("Remote control timing analysis:"));

    unsigned long activateTime = micros();
    cassette.activateRemote();
    activateTime = micros() - activateTime;

    delay(100); // Keep active for measurement

    unsigned long deactivateTime = micros();
    cassette.deactivateRemote();
    deactivateTime = micros() - deactivateTime;

    Serial.print(F("Remote activate time: "));
    Serial.print(activateTime);
    Serial.println(F(" μs"));

    Serial.print(F("Remote deactivate time: "));
    Serial.print(deactivateTime);
    Serial.println(F(" μs"));

    // Pulse width modulation simulation
    Serial.println(F("\\nPWM simulation (motor speed control):"));

    for (int dutyCycle = 10; dutyCycle <= 90; dutyCycle += 20)
    {
        Serial.print(F("Duty cycle: "));
        Serial.print(dutyCycle);
        Serial.println(F("%"));

        for (int pulse = 0; pulse < 20; pulse++) // 20 pulses
        {
            cassette.activateRemote();
            delay(dutyCycle / 10); // On time proportional to duty cycle

            cassette.deactivateRemote();
            delay((100 - dutyCycle) / 10); // Off time
        }

        delay(500); // Pause between duty cycles
    }

    Serial.println(F("PWM simulation complete"));
}

void demonstrateCassetteSystemDiagnostics()
{
    Serial.println(F("\\n--- Cassette System Diagnostics ---"));

    // System state verification
    Serial.println(F("System state verification:"));

    bool currentMode = cassette.is64CharacterMode();
    Serial.print(F("Character mode: "));
    Serial.println(currentMode ? F("64-char") : F("32-char"));

    // Test mode switching reliability
    Serial.println(F("\\nMode switching reliability test:"));

    int successfulSwitches = 0;
    const int testCycles = 50;

    for (int i = 0; i < testCycles; i++)
    {
        // Switch to 32-char mode
        cassette.set32CharacterMode();
        if (!cassette.is64CharacterMode()) // Should be false (32-char mode)
        {
            // Switch back to 64-char mode
            cassette.set64CharacterMode();
            if (cassette.is64CharacterMode()) // Should be true (64-char mode)
            {
                successfulSwitches++;
            }
        }
    }

    Serial.print(F("Successful mode switches: "));
    Serial.print(successfulSwitches);
    Serial.print(F("/"));
    Serial.print(testCycles);
    Serial.print(F(" ("));
    Serial.print((float)successfulSwitches / testCycles * 100, 1);
    Serial.println(F("%)"));

    if (successfulSwitches == testCycles)
    {
        Serial.println(F("Mode switching: RELIABLE"));
    }
    else if (successfulSwitches > testCycles * 0.9)
    {
        Serial.println(F("Mode switching: MOSTLY RELIABLE"));
    }
    else
    {
        Serial.println(F("Mode switching: UNRELIABLE"));
    }

    // Performance summary
    Serial.println(F("\\nSession Performance Summary:"));
    Serial.print(F("Total songs played: "));
    Serial.println(analysis.songCount);
    Serial.print(F("Total notes played: "));
    Serial.println(analysis.totalNotes);
    Serial.print(F("Total play time: "));
    Serial.print(analysis.totalPlayTime);
    Serial.println(F("ms"));

    if (analysis.totalNotes > 0)
    {
        Serial.print(F("Average note time: "));
        Serial.print((float)analysis.totalPlayTime / analysis.totalNotes, 2);
        Serial.println(F("ms"));
    }

    if (analysis.highestNote > 0)
    {
        Serial.print(F("Frequency range: "));
        Serial.print(analysis.lowestNote);
        Serial.print(F("Hz - "));
        Serial.print(analysis.highestNote);
        Serial.println(F("Hz"));
    }
}

void demonstrateCustomMelodyGeneration()
{
    Serial.println(F("\\n--- Custom Melody Generation ---"));

    // Generate algorithmic melodies
    Serial.println(F("Generating algorithmic melodies:"));

    // Pentatonic scale melody
    Serial.println(F("1. Pentatonic scale pattern"));
    int pentatonic[] = {262, 294, 330, 392, 440, 392, 330, 294};
    float pentatonicDur[] = {0.25, 0.25, 0.25, 0.25, 0.5, 0.25, 0.25, 0.5};

    cassette.playSong(pentatonic, pentatonicDur, 8, 140);
    delay(1000);

    // Arpeggio pattern
    Serial.println(F("2. Major arpeggio pattern"));
    int arpeggio[] = {262, 330, 392, 523, 392, 330, 262, 262};
    float arpeggioDir[] = {0.3, 0.3, 0.3, 0.6, 0.3, 0.3, 0.3, 0.6};

    cassette.playSong(arpeggio, arpeggioDir, 8, 120);
    delay(1000);

    // Random melody generation
    Serial.println(F("3. Pseudo-random melody"));

    int randomMelody[16];
    float randomDurations[16];
    int baseFreq = 262; // Middle C

    // Generate pseudo-random melody using simple algorithm
    for (int i = 0; i < 16; i++)
    {
        // Use a simple LFSR-like algorithm for pseudo-randomness
        int variation = ((i * 7 + 13) * 17) % 8;       // 0-7 range
        randomMelody[i] = baseFreq + (variation * 31); // Create scale steps

        // Vary durations pseudo-randomly
        int durVariation = ((i * 5 + 7) * 11) % 4;
        randomDurations[i] = 0.2 + (durVariation * 0.1); // 0.2 to 0.5 duration
    }

    cassette.playSong(randomMelody, randomDurations, 16, 160);
    delay(2000);

    Serial.println(F("Custom melody generation complete"));
}
