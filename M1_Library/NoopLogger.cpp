#include <arduino.h>

#include "./NoopLogger.h"

void NoopLogger::log(const char *fmt, ...)
{
    // Do nothing
}

void NoopLogger::warn(const char *fmt, ...)
{
    // Do nothing
}

void NoopLogger::err(const char *fmt, ...)
{
    // Do nothing
}