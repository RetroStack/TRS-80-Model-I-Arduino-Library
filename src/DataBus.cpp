#include "DataBus.h"
#include "utils.h"
#include "Model1LowLevel.h"

/**
 * Configures the port used.
 */
void DataBus::_configurePort(uint8_t config)
{
  Model1LowLevel::configWriteDataBus(config);
}

/**
 * Initializes the data bus
 */
DataBus::DataBus()
{
  _logger = nullptr;
  _writable = false;
}

/**
 * Hardware initialization
 */
void DataBus::begin()
{
  setAsReadable();
}

/**
 * Puts port into a stable mode
 */
void DataBus::end()
{
  setAsReadable();
}

/**
 * Sets a logger being used.
 */
void DataBus::setLogger(ILogger &logger)
{
  _logger = &logger;
}

/**
 * Reads data from the data bus
 */
uint8_t DataBus::readData()
{
  return Model1LowLevel::readDataBus();
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
  Model1LowLevel::writeDataBus(data);
}

/**
 * Checks whether bus is readable
 */
bool DataBus::isReadable()
{
  return !_writable;
}

/**
 * Checks whether bus is writable
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
  char dataChars[9];
  uint8_t dataConfig = Model1LowLevel::configReadDataBus();
  uint8_t data = Model1LowLevel::readDataBus();
  snprintf(
      buffer,
      LEN,
      "DATA<%c-%c>(%s)",
      busStatus(dataConfig),
      _writable ? 'w' : 'r',
      uint8ToBinary(data, dataChars));
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
