# TRS-80 Model I – Arduino Library

This Arduino library enables interfacing a TRS-80 Model I mainboard directly with an Arduino. It allows reading and writing data, controlling bus signals, and developing tools such as ROM dumpers, RAM testers, or custom peripherals for the TRS-80.

**Note:** Currently, **only the Arduino Mega 2560** is supported. This board provides enough GPIO pins to connect to the 40-pin edge connector of the TRS-80 Model I. More boards may be added in the future if there is interest.

## Features

- Direct connection to the TRS-80 Model I system bus via the edge connector.
- Control over the address and data bus, abstracted in an easy-to-use interface.
- **Model1 Class**
  - Provides convenience functions to check address spaces.
  - Enables reading and writing to all addresses, including video RAM, DRAM, ROM (read-only), keyboard, and any other address space you want to inspect or modify.
  - Supports I/O access as well.
  - Additional support for interrupt, WAIT, and TEST signals.
- **Other Classes**
  - **Cassette** – Higher-level access for the cassette interface.
  - **Keyboard** – Class to read keyboard state, including press and release events.
  - **ROM** – Access to ROM content and checksum comparison.
  - **Video** – Provides direct access to video reading/writing, including viewports with scrolling support.
  - **ILogger** - Standard interface for all logging events.
  - **SerialLogger** - Specific implementation of an ILogger for Serial logging.

## Documentation

You can find detailed documentation of all features [here](/docs).

![Overview](/Images/Overview.png)

## Hardware Requirements

- **Arduino Mega 2560** (required)
- Connection between the Mega 2560 and the TRS-80 40-pin edge connector.
  - **Recommended:** Use the companion **TRS-80 Model I Arduino Shield** (see below).
  - Alternatively, you can connect via jumper wires to an edge connector, but take extra care to avoid shorts.

### TRS-80 Arduino Shield (Recommended)

A dedicated shield is available to make the connection cleaner and more reliable. It is maintained in a separate repository:

[TRS-80 Model I Arduino Shield](LINK_TO_SHIELD_DOCS)

*(Link will be updated once the repository is public.)*

## Installation

1. Clone or download this repository.
2. Copy the library folder into your Arduino libraries directory.
3. Open the Arduino IDE.
4. Go to **File → Examples → TRS-80 Model I** and choose an example.
5. Compile and upload to test it.

**⚠️ WARNING:**  
When connecting to hardware, you often need to connect everything, turn the Model I on, and ensure it is in a good state **before** powering the Arduino. Many scripts assert the `TEST*` signal to take over execution. This is safe if the Model I has fully booted but can corrupt memory or cause instability if asserted immediately on startup.

## Main TRS-80 Model I Repository

For additional resources related to the TRS-80 Model I, be sure to check out the central project page on RetroStack:

[RetroStack – TRS-80 Model I](https://www.github.com/RetroStack/TRS-80-Model-I)

## License

This project is licensed under the MIT License – see the [LICENSE](LICENSE) file for details.
