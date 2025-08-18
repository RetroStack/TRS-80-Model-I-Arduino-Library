/*
 * AddressBus.cpp - Class for accessing the address bus
 * Authors: Ven Reddy, Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#include "AddressBus.h"
#include "utils.h"
#include "Model1LowLevel.h"

void AddressBus::_configurePort(uint16_t config)
{
    Model1LowLevel::configWriteAddressBus(config);
}

// Constructor - initialize address bus
AddressBus::AddressBus()
{
    _logger = nullptr;
    _writable = false;
}

// Initialize address bus as readable
void AddressBus::begin()
{
    setAsReadable();
}

// Reset address bus to default readable state
void AddressBus::end()
{
    setAsReadable();
}

// Set logger for debugging output
void AddressBus::setLogger(ILogger &logger)
{
    _logger = &logger;
}

// Read 16-bit memory address from bus
uint16_t AddressBus::readMemoryAddress()
{
    return Model1LowLevel::readAddressBus();
}

// Write 16-bit memory address to bus
void AddressBus::writeMemoryAddress(uint16_t address)
{
    if (!_writable)
    {
        if (_logger)
            _logger->err("Address bus is not writable.");
        return;
    }
    Model1LowLevel::writeAddressBus(address);
}

void AddressBus::writeRefreshAddress(uint8_t address)
{
    Model1LowLevel::writeAddressBus(address);
}

uint8_t AddressBus::readIOAddress()
{
    return Model1LowLevel::readAddressBus() & 0xFF;
}

void AddressBus::writeIOAddress(uint8_t address)
{
    if (!_writable)
    {
        if (_logger)
            _logger->err("IO address bus is not writable.");
        return;
    }
    Model1LowLevel::writeAddressBus(address);
}

bool AddressBus::isReadable()
{
    return !_writable;
}

bool AddressBus::isWritable()
{
    return _writable;
}

void AddressBus::setAsReadable()
{
    _setBus(false);
}

void AddressBus::setAsWritable()
{
    _setBus(true);
}

char *AddressBus::getState()
{
    const int LEN = 31;
    char *buffer = new char[LEN];
    char addrChars[17];
    uint16_t addrConfig = Model1LowLevel::configReadAddressBus();
    uint16_t addr = Model1LowLevel::readAddressBus();
    snprintf(
        buffer,
        LEN,
        "ADDR<%c-%c>(%s)",
        busStatus(addrConfig),
        _writable ? 'w' : 'r',
        uint16ToBinary(addr, addrChars));
    return buffer;
}

void AddressBus::_setBus(bool writableOption)
{
    if (_writable == writableOption)
        return;

    if (writableOption)
    { // Output
        _configurePort(0xffff);
    }
    else
    { // Input
        _configurePort(0x0000);
    }
    _writable = writableOption;
}
