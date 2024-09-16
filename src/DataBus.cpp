#include "DataBus.h"
#include "port_config.h"
#include "port_macros.h"
#include "utils.h"

/**
 * Configures the port used.
 */
void DataBus::_configurePort(uint8_t config)
{
  busConfigWrite(DATA, config);
}

/**
 * Initializes the data bus
 */
DataBus::DataBus(ILogger *logger = nullptr)
{
  _logger = logger;
  _writable = false;
}

/**
 * Hardware initialization
 */
void DataBus::begin()
{
  _configurePort(0x00);
}

/**
 * Reads data from the data bus
 */
uint8_t DataBus::readData()
{
  return busRead(DATA);
}

/**
 * Writes data to the data bus
 *
 * Requires the bus to be writable.
 */
void DataBus::writeData(uint8_t data)
{
  if (!_writable)
  {
    if (_logger)
      _logger->err("Data bus is not writable.");
    return;
  }
  busWrite(DATA, data);
}

/**
 * Checks wether bus is readable
 */
bool DataBus::isReadable()
{
  return !_writable;
}

/**
 * Checks wether bus is writable
 */
bool DataBus::isWritable()
{
  return _writable;
}

/**
 * Set bus as readable
 */
void DataBus::setAsReadable()
{
  _setBus(false);
}

/**
 * Set bus as writable
 */
void DataBus::setAsWritable()
{
  _setBus(true);
}

/**
 * Returns the current state as string
 */
char *DataBus::getState()
{
  const int LEN = 20;
  char *buffer = new char[LEN];
  char data[9];
  snprintf(
      buffer,
      LEN,
      "DATA<%c-%c>(%s)",
      busStatus(busConfigRead(DATA)),
      _writable ? 'w' : 'r',
      uint8ToBinary(busRead(DATA), data));
  return buffer;
}

/**
 * Sets the configuration of the bus
 */
void DataBus::_setBus(bool writableOption)
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
