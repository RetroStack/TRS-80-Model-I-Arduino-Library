# LoggerScreen Examples

`LoggerScreen` is an `ILogger` that draws to the panel instead of a serial
port, so anything in the library that accepts a logger can write to the screen.

## Examples Overview

### SimpleLoggerScreen

**Location:** `SimpleLoggerScreen/`
**Purpose:** The smallest useful logger screen
**Features:**

- Constructing a logger screen and handing it to `M1Shield`
- The four levels: `info`, `warn`, `err`, `debug`
- The `F()` forms, which keep message text in flash rather than SRAM

**Best for:** Seeing the whole idea in one screenful.

### ComprehensiveLoggerScreen

**Location:** `ComprehensiveLoggerScreen/`
**Purpose:** Everything the class can do
**Features:**

- Rotational log buffer, sized by `setLogBufferSize()`
- Capturing entries while the screen is inactive, then replaying them on
  activation
- Colour coding and timestamps
- Text sizing for more lines on a small panel

**Best for:** Using the logger screen as the real diagnostic surface of a
project.

## Which To Start With

Read `SimpleLoggerScreen` first. It is short enough to hold in your head, and
the comprehensive example assumes you already know what a logger screen is.
