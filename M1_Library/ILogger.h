#ifndef ILOGGER_H
#define ILOGGER_H

class ILogger
{
public:
    virtual void info(const char *fmt, ...) = 0;
    virtual void warn(const char *fmt, ...) = 0;
    virtual void err(const char *fmt, ...) = 0;
};

#endif /* ILOGGER_H */