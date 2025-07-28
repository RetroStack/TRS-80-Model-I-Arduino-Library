#include "Cassette.h"
#include "Model1.h"
#include "utils.h"

// Local port
#define CASSETTE_PORT 0xff

// Bits to set/read
#define CASSETTE_BIT_CASSOUT1 0    // Write-only
#define CASSETTE_BIT_CASSOUT2 1    // Write-only
#define CASSETTE_BIT_CASSREMOTE 2  // Write-only
#define CASSETTE_BIT_MODESEL_INV 3 // Write-only, inverted
#define CASSETTE_BIT_MODESEL 6     // Read-only
#define CASSETTE_BIT_CASSIN 7      // Read-only
#define CASSETTE_BIT_CHARGEN_INV 7 // Write-only, inverted

#define CASSETTE_DEFAULT_STATE 0

/**
 * Initializes the cassette interface
 */
Cassette::Cassette()
{
    _logger = nullptr;
    _state = CASSETTE_DEFAULT_STATE;
}

/**
 * Reads the value of the IO device
 */
uint8_t Cassette::_read()
{
    // Initialize the result
    uint8_t result = 0;

    // Read the cassette input I/O port
    uint8_t input = Model1.readIO(CASSETTE_PORT);

    // Set the MODE SELECT to the right value
    if (!bitRead(input, CASSETTE_BIT_MODESEL))
    {
        bitSet(result, CASSETTE_BIT_MODESEL_INV);
    }

    return result;
}

/**
 * Writes a value to the cassette I/O port
 */
void Cassette::_write(uint8_t data)
{
    Model1.writeIO(CASSETTE_PORT, data);
}

/**
 * Updates the cassette I/O state
 */
void Cassette::update()
{
    uint8_t data = _read();

    // Audio output state
    if (bitRead(_state, CASSETTE_BIT_CASSOUT1))
    {
        bitSet(data, CASSETTE_BIT_CASSOUT1);
    }
    if (bitRead(_state, CASSETTE_BIT_CASSOUT2))
    {
        bitSet(data, CASSETTE_BIT_CASSOUT2);
    }

    // Remote state
    if (bitRead(_state, CASSETTE_BIT_CASSREMOTE))
    {
        bitSet(data, CASSETTE_BIT_CASSREMOTE);
    }

    // Character generator state
    if (bitRead(_state, CASSETTE_BIT_CHARGEN_INV))
    {
        bitSet(data, CASSETTE_BIT_CHARGEN_INV);
    }

    _state = data;
}

/**
 * Writes a raw value to the cassette output
 */
void Cassette::writeRaw(bool value1, bool value2)
{
    if (value1)
    {
        bitSet(_state, CASSETTE_BIT_CASSOUT1);
    }
    else
    {
        bitClear(_state, CASSETTE_BIT_CASSOUT1);
    }

    if (value2)
    {
        bitSet(_state, CASSETTE_BIT_CASSOUT2);
    }
    else
    {
        bitClear(_state, CASSETTE_BIT_CASSOUT2);
    }

    _write(_state);
}

/**
 * Reads the raw data on cassette-in
 */
bool Cassette::readRaw()
{
    uint8_t input = Model1.readIO(CASSETTE_PORT);

    // Reset flip-flop
    _write(_state);

    if (bitRead(input, CASSETTE_BIT_CASSIN))
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * Plays a tone at a given frequency for a specified time
 */
void Cassette::play(uint16_t frequency, uint32_t durationMs)
{
    update();

    uint32_t halfPeriod = 500000UL / frequency;
    uint32_t cycles = (durationMs * 1000UL) / (halfPeriod * 2);
    for (uint32_t i = 0; i < cycles; i++)
    {
        writeRaw(true, true);
        delayMicroseconds(halfPeriod);
        writeRaw(true, false);
        delayMicroseconds(halfPeriod);
    }
}

/**
 * Plays a song by providing the melody, the durations of each tone,
 * the number of notes in both melody and duration (need to be the same),
 * and the beats per minute.
 */
void Cassette::playSong(int *melody, float *durations, size_t numNotes, int bpm)
{
    // Compute duration of a whole note in milliseconds
    float wholeNoteMs = (60000.0 * 4) / bpm;

    for (size_t i = 0; i < numNotes; i++)
    {
        int note = melody[i];
        unsigned long durationMs = wholeNoteMs * durations[i];

        if (note == REST)
        {
            delay(durationMs);
        }
        else
        {
            play(note, durationMs * 0.9);
            delay(durationMs * 0.1);
        }
    }
}

/**
 * Activates the remote motor
 */
void Cassette::activateRemote()
{
    update();
    bitSet(_state, CASSETTE_BIT_CASSREMOTE);
    _write(_state);
}

/**
 * Deactivates the remote motor
 */
void Cassette::deactivateRemote()
{
    update();
    bitClear(_state, CASSETTE_BIT_CASSREMOTE);
    _write(_state);
}

/**
 * Selects Character Generator A
 *
 * NOTE: This is only possible if the JP4 and JP5 are set C->1 and C->2 respectively.
 */
void Cassette::setCharGenA()
{
    update();
    bitClear(_state, CASSETTE_BIT_CHARGEN_INV);
    _write(_state);
}

/**
 * Selects Character Generator B
 *
 * NOTE: This is only possible if the JP4 and JP5 are set C->1 and C->2 respectively.
 */
void Cassette::setCharGenB()
{
    update();
    bitSet(_state, CASSETTE_BIT_CHARGEN_INV);
    _write(_state);
}

/**
 * Checks whether video mode is in 64 characters
 */
bool Cassette::is64CharacterMode()
{
    update();
    return !bitRead(_state, CASSETTE_BIT_MODESEL_INV);
}

/**
 * Changes the video mode to 32 characters
 */
void Cassette::set32CharacterMode()
{
    update();
    bitSet(_state, CASSETTE_BIT_MODESEL_INV);
    _write(_state);
}

/**
 * Changes the video mode to 64 characters
 */
void Cassette::set64CharacterMode()
{
    update();
    bitClear(_state, CASSETTE_BIT_MODESEL_INV);
    _write(_state);
}

/**
 * Sets a logger being used.
 */
void Cassette::setLogger(ILogger &logger)
{
  _logger = &logger;
}
