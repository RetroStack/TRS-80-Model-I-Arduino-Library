#include "AddressBus.h"
#include "port_config.h"
#include "port_macros.h"
#include "utils.h"

/**
 * Configures the port used
 */
void AddressBus::_configurePort(uint8_t config)
{
    busConfigWrite(ADDR_LOW, config);
    busConfigWrite(ADDR_HIGH, config);
}

/**
 * Initializes the Bus Access
 */
AddressBus::AddressBus(ILogger *logger)
{
    _logger = logger;
    _writable = false;
}

/**
 * Hardware initialization
 */
void AddressBus::begin()
{
    _configurePort(0x00);
}

/**
 * Reads from the data bus
 */
uint16_t AddressBus::readMemoryAddress()
{
    return (busRead(ADDR_HIGH) << 8) | busRead(ADDR_LOW);
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
    busWrite(ADDR_LOW, address & 0xff);
    busWrite(ADDR_HIGH, (address & 0xff00) >> 8);
}

/**
 * Writes a refresh address to the bus
 *
 * NOTE: Careful when using this directly as it doesn't make any checks to improve performance.
 */
void AddressBus::writeRefreshAddress(uint8_t address)
{
    // Direct call to ports to save time
    busWrite(ADDR_LOW, address);
    busWrite(ADDR_HIGH, 0);
}

/**
 * Reads from the IO address bus
 */
uint8_t AddressBus::readIOAddress()
{
    return busRead(ADDR_LOW);
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
    busWrite(ADDR_LOW, address);
    busWrite(ADDR_HIGH, 0);
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
    char addrLow[9];
    char addrHigh[9];
    snprintf(
        buffer,
        LEN,
        "ADDR<%c %c-%c>(%s %s)",
        busStatus(busConfigRead(ADDR_HIGH)),
        busStatus(busConfigRead(ADDR_LOW)),
        _writable ? 'w' : 'r',
        uint8ToBinary(busRead(ADDR_HIGH), addrHigh),
        uint8ToBinary(busRead(ADDR_LOW), addrLow));
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
        _configurePort(0xff);
    }
    else
    { // Input
        _configurePort(0x00);
    }
    _writable = writableOption;
}
