# SEN66 Environmental Sensor: IoT Lab Demonstration

This project is designed for an **IoT Laboratory** setting to demonstrate how to interface with and read data from the **Sensirion SEN66** environmental sensor node using the **Zephyr RTOS**. It features a continuous loop that fetches air quality metrics (Particulate Matter, CO₂, VOCs, NOx, humidity, and temperature) over I2C, with a remote **User Interface (UI)** to change the sampling speed via UART.

## Hardware Requirements

* **Microcontroller:** nRF52832 Development Kit.
* **Component:** Sensirion SEN66 Environmental Sensor Node.
* **Connection Interface:**
    * **I2C:** For communicating data between the SEN66 sensor and the microcontroller.
    * **UART:** For sending commands from the PC to the device.

## Project Architecture

The application uses a modular design to separate the sensor polling code from the user settings:

1. **Hardware Layer:** Defined in the DeviceTree Overlay, mapping the I2C bus (SDA/SCL pins) and establishing the `sen66` sensor node at its specific I2C address.
2. **UI Module (`ui.c`):** Manages serial communication. It reads incoming text commands to update the data sampling interval or restart the board.
3. **Application Logic (`main.c`):** Sets up the I2C interface, initializes the SEN66 to start continuous measurement, and runs the loop to fetch and print air quality data.

## Logic & Code Flow

### 1. Setup Phase
* The **User Interface** is initialized to start listening for commands.
* The system configures the I2C bus and sends an initialization command to the SEN66 to begin its internal measurement engine.

### 2. Operational Loop
* **Check Speed:** At the start of every loop, the code calls `ui_get_interval_ms()` to see if the user changed the sampling delay time.
* **Fetch Data:** The system sends an I2C read request to the SEN66 to retrieve the latest environmental metrics.
* **Process and Output:** The system parses the raw I2C payload into human-readable values and prints them to the console.
* **Wait:** The system pauses for the duration of `delayTime` before repeating the loop.

### 3. Asynchronous Control
* **UART Interrupts:** The system uses a "callback" to process commands like `SET_INTERVAL` instantly, even while the main loop is waiting between sensor readings.

## Features

* **Multi-Parameter Sensing:** Continuously reads PM1.0/2.5/4.0/10, CO₂, VOC Index, NOx Index, relative humidity, and temperature.
* **Live Speed Adjustment:** Use the **User Interface** to change how frequently the microcontroller polls the SEN66 for new data.
* **Remote System Reset:** Capability to restart the microcontroller and re-initialize the sensor using a UI command.
* **Status Logging:** Prints real-time formatted sensor data streams to the screen so you can easily verify air quality metrics.

## Communication Protocols

* **I2C:** Operates as the digital interface to read multi-signal registers from the SEN66 sensor at 3.3V.
* **UART:** Used for the **User Interface** console to send and receive text commands.

## Project Structure

* `main.c`: Contains the main loop that initializes the sensor and fetches data.
* `ui.c`: Handles the text commands from the computer.
* `ui.h`: Header file that allows the main code to talk to the UI code.
* `nrf52dk_nrf52832.overlay`: Tells the system which pins are used for the I2C bus communicating with the SEN66.

## Setup and Build

### 1. Hardware Setup
The code is configured for the **nRF52 DK** wired to the SEN66:
* **Power:** The SEN66 requires a 3.3V supply (VDD) and Ground (GND) from the DK.
* **Wiring:** Connect the SEN66 SDA and SCL pins to the designated I2C pins on the nRF52 DK using jumper wires.

### 2. Build and Flash
```bash
west build -b nrf52dk_nrf52832
west flash
