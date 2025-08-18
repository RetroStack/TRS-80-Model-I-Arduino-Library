/*
 * DataBus.cpp - Class for accessing the data bus
 * Authors: Ven Reddy, Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#include "DataBus.h"
#include "utils.h"
#include "Model1LowLevel.h"

// Configure data bus port direction
void DataBus::_configurePort(uint8_t config)
{
  Model1LowLevel::configWriteDataBus(config);
}

// Constructor - initialize data bus
DataBus::DataBus()
{
  _logger = nullptr;
  _writable = false;
}

// Initialize data bus as readable
void DataBus::begin()
{
  setAsReadable();
}

// Reset data bus to default readable state
void DataBus::end()
{
  setAsReadable();
}

// Set logger for debugging output
void DataBus::setLogger(ILogger &logger)
{
  _logger = &logger;
}

// Read 8-bit data value from bus
uint8_t DataBus::readData()
{
  return Model1LowLevel::readDataBus();
}

// Write 8-bit data value to bus
void DataBus::writeData(uint8_t data)
{
  if (!_writable)
  {
    if (_logger)
      _logger->err(F("Data bus is not writable."));
    return;
  }
  Model1LowLevel::writeDataBus(data);
}

bool DataBus::isReadable()
{
  return !_writable;
}

bool DataBus::isWritable()
{
  return _writable;
}

void DataBus::setAsReadable()
{
  _setBus(false);
}

void DataBus::setAsWritable()
{
  _setBus(true);
}

char *DataBus::getState()
{
  const int LEN = 20;
  char *buffer = (char *)malloc(LEN);
  if (!buffer)
  {
    if (_logger)
    {
      _logger->err(F("DataBus: Failed to allocate memory for state string"));
    }
    return nullptr;
  }
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
