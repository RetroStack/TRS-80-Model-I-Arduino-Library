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
      _logger->errF(F("DataBus: Data bus is not writable."));
    return;
  }
  Model1LowLevel::writeDataBus(data);
}

// Get the current state of the data bus
char *DataBus::getState()
{
  const int LEN = 20;
  char *buffer = (char *)malloc(LEN);
  if (!buffer)
  {
    if (_logger)
    {
      _logger->errF(F("DataBus: Failed to allocate memory for state string"));
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

