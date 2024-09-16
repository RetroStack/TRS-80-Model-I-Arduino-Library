/*
 * Cassette.h - Class for accessing the cassette interface
 * Authors: Ven Reddy, Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef CASSETTE_H
#define CASSETTE_H

#include <Arduino.h>
#include "ILogger.h"
#include "Model1.h"

class Cassette
{
private:
    Model1 *_model1;
    ILogger *_logger;

    bool _remoteOn;

public:
    Cassette(Model1 *model1, ILogger *logger = nullptr);

    void writeRaw(bool value1, bool value2);
    uint8_t read();

    void activateRemote();
    void deactivateRemote();
};

#endif /* CASSETTE_H */