# Simple LoggerScreen

The smallest useful `LoggerScreen`: construct one, hand it to `M1Shield`, log.

## What You'll Learn

- That `LoggerScreen` is an `ILogger`, so anything that accepts a logger can
  write to the panel instead of a serial port
- The four levels -- `info`, `warn`, `err`, `debug` -- and how they are told
  apart on a colour panel and on a monochrome one
- The `F()` forms, which keep the message text in flash rather than SRAM

## What It Does

Opens a logger screen and writes one line at each level, then one through the
flash-string form. The menu button closes the screen if a back destination has
been set.

## Wiring

Nothing beyond the shield and a display. Set your panel by uncommenting one
provider at the top of the sketch.

## Next

`ComprehensiveLoggerScreen` adds the rotational buffer, colour coding,
timestamps, and replaying buffered entries when the screen is activated.
