#include "Cassette.h"
#include "Model1.h"
#include "utils.h"

// Local port
const uint8_t CASSETTE_PORT = 0xff;

/**
 * Initializes the cassette interface
 */
Cassette::Cassette(Model1 *model1, ILogger *logger)
{
    _model1 = model1;
    _logger = logger;
    _remoteOn = false;
}

/**
 * Writes a raw value to the cassette output
 */
void Cassette::writeRaw(bool value1, bool value2)
{
    bool video64Mode = (_model1->readIO(CASSETTE_PORT) & 0b01000000) > 0;
    uint8_t value = (value1 ? 0b00000001 : 0) | (value2 ? 0b00000010 : 0) | (_remoteOn ? 0b00000100 : 0) | (video64Mode ? 0 : 0b00000100);
    _model1->writeIO(CASSETTE_PORT, value);
}

/**
 * Reads the value on the cassette input
 */
uint8_t Cassette::read()
{
    return (_model1->readIO(CASSETTE_PORT) & 0b10000000) > 0 ? HIGH : LOW;
}

/**
 * Activates the remote motor
 */
void Cassette::activateRemote()
{
    _remoteOn = true;
    _model1->writeIO(CASSETTE_PORT, 0b00000100);
}

/**
 * Deactivates the remote motor
 */
void Cassette::deactivateRemote()
{
    _remoteOn = false;
    _model1->writeIO(CASSETTE_PORT, 0x00);
}