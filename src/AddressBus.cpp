/*
 * AddressBus.cpp - Class for accessing the address bus
 * Authors: Ven Reddy, Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#include "AddressBus.h"
#include "utils.h"
#include "Model1LowLevel.h"

/**
 * Configures the port used
 */
void AddressBus::_configurePort(uint16_t config)
{
    Model1LowLevel::configWriteAddressBus(config);
}

/**
 * Initializes the Bus Access
 */
AddressBus::AddressBus()
{
    _logger = nullptr;
    _writable = false;
}

/**
 * Hardware initialization
 */
void AddressBus::begin()
{
    setAsReadable();
}

/**
 * Puts ports into a stable mode
 */
void AddressBus::end()
{
    setAsReadable();
}

/**
 * Sets a logger being used.
 */
void AddressBus::setLogger(ILogger &logger)
{
    _logger = &logger;
}

/**
 * Reads from the data bus
 */
uint16_t AddressBus::readMemoryAddress()
{
    return Model1LowLevel::readAddressBus();
}

/**
 * Writes to the bus
 */
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

/**
 * Writes a refresh address to the bus
 *
 * NOTE: Careful when using this directly as it doesn't make any checks to improve performance.
 */
void AddressBus::writeRefreshAddress(uint8_t address)
{
    Model1LowLevel::writeAddressBus(address);
}

/**
 * Reads from the IO address bus
 */
uint8_t AddressBus::readIOAddress()
{
    return Model1LowLevel::readAddressBus() & 0xFF;
}

/**
 * Writes to the IO address bus
 */
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

/**
 * Checks whether the bus is readable
 */
bool AddressBus::isReadable()
{
    return !_writable;
}

/**
 * Checks whether the bus is writable
 */
bool AddressBus::isWritable()
{
    return _writable;
}

/**
 * Sets the address bus to be readable
 */
void AddressBus::setAsReadable()
{
    _setBus(false);
}

/**
 * Sets the address bus to be writable
 */
void AddressBus::setAsWritable()
{
    _setBus(true);
}

/**
 * Returns the current state as string
 */
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

/**
 * Sets the data bus direction
 */
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
