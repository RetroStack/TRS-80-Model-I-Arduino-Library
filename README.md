# TRS-80 Model 1 - Arduino Library

This Arduino library enables interfacing a TRS-80 Model I mainboard directly with an Arduino. It allows reading and writing data, controlling bus signals, and developing tools such as ROM dumpers, RAM testers, or custom peripherals for the TRS-80.

**Note:** Currently, **only the Arduino Mega 2560** is supported. This board provides enough GPIO pins to connect to the 40-pin edge connector of the TRS-80 Model I. More boards will be added if there is interest.

## Features

- Direct connection to the TRS-80 Model I system bus via the edge connector.
- Control over address & data bus, abstracted in an easy to use interface
- Model 1 Class
  - Provides convinience functions to check address spaces
  - Enabled reading and writing to all addresses, including video RAM, DRAM, ROM (read-only), keyboard, and any other address space you want to read or modify
  - Support IO access as well
  - Additional support of interrupt, Wait, and Test signals
- Other classes
  - Cassette - Higher level access for the cassette interface
  - Keyboard - Class to read from the keyboard, including press and release events
  - ROM - ROM access to compare checksums
  - Video - Provides direct access to video writing/reading functionality, including viewports with scrolling support

## Documentation

You can find a detailed documentation of all features [here](/docs).

![Overview](/Images/Overview.png)

## Hardware Requirements

- **Arduino Mega 2560** (required)
- Connection between the Mega 2560 and the TRS-80 40-pin edge connector.
  - **Recommended:** Use the companion **TRS-80 Model I Arduino Shield** (see below).
  - Alternatively, connections can be made via jumper wires to an edge-connector, but take extra care to avoid shorts.

### TRS-80 Arduino Shield (Recommended)

A dedicated shield is available to make the connection cleaner and more reliable. It is maintained in a separate repository:

[TRS-80 Model I Arduino Shield](LINK_TO_SHIELD_DOCS)

*(Link will be updated once the repository is public.)*

## Installation

1. Clone or download this repository.
2. Copy the library folder into your Arduino libraries directory:
3. Open Arduino IDE
4. Go to File -> Examples -> TRS-80 Model 1 and choose an example
5. Compile and upload to test it

**WARNING:** When connecting it to hardware, you often need to connect everything, turn the Model 1 on and make sure it is in a good state, and then connect the Arduino to power as many of the scripts assert the TEST* signal, taking over execution. This is fine when the Model 1 started up, but will corrupt when it is already from the start.

## Main TRS-80 Model 1 Repository

For additional resources related to the TRS-80 Model 1, be sure to check out the central page for the TRS-80 Model 1 on RetroStack. You can find it [here](https://www.github.com/RetroStack/TRS-80-Model-I).

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
