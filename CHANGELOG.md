# TRS-80 Model 1 - Arduino Library - Changelog

## 23rd June 2024 - Initial Version (0.9)

This is the initial version written by Ven Reddy

## 16th September 2024 - Initial Version (0.9.1)

- General cleanup of the codebase
- Adding Arduino library pattern
- Adding cassette interface

## 27th September 2024 - Initial Version (0.9.2)

- General cleanup of the codebase

## 22th July 2025 - Initial Release (1.0.0)

- General cleanup of the codebase

## 22th July 2025 - Updated Release (1.1.0)

- Adding Print features to Video class

## 26th July 2025 - Updated Release (1.2.0)

- Adding M1Sheild and Screen implementations
  - Screen
  - ContentScreen
  - ConsoleScreen
  - MenuScreen
  - M1TerminalScreen
- Adding the ability to request TEST signal state

## 28th July 2025 - Updated Release (1.2.1)

- Added LoggerScreen with ILogger compatibility through adapter pattern
- Added CompositeLogger for multi-destination logging  
- Added SerialLogger for Serial Monitor integration
- **BREAKING CHANGE**: LoggerScreen now requires `screen->asLogger()` when used with CompositeLogger instead of passing the screen directly
