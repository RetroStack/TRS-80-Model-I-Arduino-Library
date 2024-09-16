#include "Cassette.h"
#include "Model1.h"
#include "utils.h"

const uint8_t CASSETTE_PORT = 0xff;

Cassette::Cassette(Model1 *model1, ILogger *logger = nullptr)
{
    _model1 = model1;
    _logger = logger;
    _remoteOn = false;
}

void Cassette::setOutput(bool value1, bool value2)
{
    bool video64Mode = (_model1->readIO(CASSETTE_PORT) & 0b01000000) > 0;
    uint8_t value = (value1 ? 0b00000001 : 0) | (value2 ? 0b00000010 : 0) | (_remoteOn ? 0b00000100 : 0) | (video64Mode ? 0 : 0b00000100);
    _model1->writeIO(CASSETTE_PORT, value);
}

void Cassette::activateRemote()
{
    _remoteOn = true;
    _model1->writeIO(CASSETTE_PORT, 0b00000100);
}

void Cassette::deactivateRemote()
{
    _remoteOn = false;
    _model1->writeIO(CASSETTE_PORT, 0);
}