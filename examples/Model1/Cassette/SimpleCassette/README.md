# SimpleCassette Example

This example demonstrates basic cassette interface operations including sound generation and video mode control.

## What It Does

- **Character Mode Control**: Switch between 32 and 64 character display modes
- **Simple Sound Generation**: Play basic tones and simple melodies
- **Remote Control**: Control cassette motor/relay operations
- **Mode Verification**: Check and verify current display mode settings

## What You'll Learn

- How to control TRS-80 display character modes
- Basic sound generation through the cassette interface
- Remote control operations for cassette motor
- Understanding the relationship between cassette and video systems
- Simple melody playback techniques

## Hardware Requirements

- Arduino Mega 2560
- TRS-80 Model I with 40-pin edge connector interface
- Serial monitor for operation status
- Optional: External cassette recorder for remote control testing
- Proper bus control via TEST signal

## Key Functions Demonstrated

- `cassette.set64CharacterMode()` / `cassette.set32CharacterMode()` - Display mode control
- `cassette.is64CharacterMode()` - Mode verification
- `cassette.playSong()` - Melody playback
- `cassette.activateRemote()` / `cassette.deactivateRemote()` - Motor control
- Simple melody data structure setup

## Character Modes

- **64-Character Mode**: Standard full-width display (80 columns equivalent)
- **32-Character Mode**: Double-width characters for accessibility

## Sound Features

- Individual tone generation
- Simple melody playback
- Frequency control (Hz)
- Duration control (beats)
- Tempo control (BPM)

## Serial Output

- Character mode status and changes
- Sound generation progress
- Remote control operation status
- Mode verification results

## Usage

1. Connect your Arduino to the TRS-80 Model I edge connector
2. Open the Serial Monitor at 9600 baud
3. Upload this sketch to your Arduino Mega 2560
4. Watch the display mode changes on the TRS-80 screen
5. Listen to the sound generation through the TRS-80 speaker
6. Observe remote control operations

This example provides a foundation for applications requiring display control, sound output, or cassette interface operations.
