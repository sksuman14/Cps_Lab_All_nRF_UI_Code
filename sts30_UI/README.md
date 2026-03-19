# STS3x Temperature Monitor: IoT Lab Demonstration

This project is designed for an **IoT Laboratory** setting to demonstrate high-precision thermal sensing using the **Sensirion STS3x** temperature sensor and the **Zephyr RTOS**. It features a continuous loop that reports Temperature data, along with a remote **User Interface (UI)** for dynamic system control via UART.

## Hardware Requirements

* **Microcontroller**: nRF52832 Development Kit.
* **Sensor**: Sensirion STS3x (e.g., STS30, STS31) High-Accuracy Temperature Sensor.
* **Connection Interface**:
    * **I2C**: Connects the sensor to the microcontroller for data acquisition.
    * **UART**: Connects the microcontroller to the PC for sending commands.



## Project Architecture

The application uses a modular design to separate the sensing logic from the user settings:

1 . **Hardware Layer**: Defined in the DeviceTree Overlay, enabling the `i2c0` bus and configuring the STS3x sensor node at address `0x4A`.

2 . **UI Module (`ui.c`)**: Manages serial communication. It listens for incoming text commands to change the sampling speed or restart the board.

3 . **Application Logic (`main.c`)**: Initializes the sensor, performs a soft reset, and runs the main loop to fetch and display temperature data.

## Logic & Code Flow

### 1. Setup Phase

* The **User Interface** is initialized to start listening for commands via UART.
* The system checks if the **I2C bus** is ready. If not, it halts and prints an error.
* **Soft Reset**: The system sends a specific command (`0x30A2`) to reset the sensor and waits **100ms** for it to stabilize.

### 2. Operational Loop

* **Check Speed**: At the start of every loop, the code calls `ui_get_interval_ms()` to see if the user has requested a new sampling interval.
* **Fetch Data**: The system sends a measurement command (`0x240B`) to the sensor and reads back 3 bytes of data.

* **Print Output**: The result is formatted and printed to the console (e.g., `Temp: 25.43 °C`).
* **Wait**: The system pauses for the duration of the current `delayTime` before repeating the loop.

### 3. Asynchronous Control

* **UART Interrupts**: The system uses a background process (callback) to handle commands like `SET_INTERVAL` instantly, without stopping the sensor loop.

## Features

* **High-Accuracy Sensing**: Real-time tracking of Ambient Temperature using the STS3x series.
* **Sensor Reset**: Automatically performs a soft reset on boot to ensure reliable operation.
* **Live Interval Adjustment**: Use the **User Interface** to change how often the sensor reports data.
* **Remote System Reset**: Capability to restart the microcontroller using a UI command.

## Communication Protocols

* **I2C**: Used to communicate with the sensor. The configuration uses address `0x4A` and standard bitrate.
* **UART**: Used for the **User Interface** console to receive user commands and display sensor data.

## Project Structure

* `main.c`: Contains the main loop, sensor soft reset logic, and temperature conversion formula.
* `ui.c`: Handles the text commands (`restartDevice`, `SET_INTERVAL`) and UART callback logic.
* `ui.h`: Header file that exposes the UI functions to the main application.
* `nrf52dk_nrf52832.overlay`: Configures the I2C hardware and registers the `mysensor` node at address `0x4A`.

## Setup and Build

### 1. Hardware Setup

Connect the STS3x Sensor to your nRF52 DK using I2C:

* **VCC/GND**: Connect to 3.3V and Ground.
* **SDA/SCL**: Connect to the standard I2C0 pins on the nRF52832 (typically P0.26 and P0.27).

### 2. Build and Flash

```bash
west build -b nrf52dk_nrf52832
west flash

```

## IoT Lab Usage & UI Controls

Connect to the device using the **graphical user interface (GUI)**.
Select the appropriate **Serial Port** and **baud rate (115200)** from the UI and click **Connect** to establish communication with the device.

**Changing the Sampling Interval**

To demonstrate how sampling frequency affects data density, configure the sampling interval using the UI:

* **Method (UI-Based Configuration):**
  This application is **UI-based**. Instead of sending commands manually, enter the desired value in the **Interval (seconds)** field and click **Set Interval**, as shown in the UI.

* **Example:**
  Set the interval to **5 seconds** using the UI.
  The device will now update and transmit data every **5 seconds**.


**Restarting the Device**

To demonstrate remote device management and system recovery:

* **Method (UI-Based Control):**
  Use the **Restart Device** button available in the UI.

* **Action:**
  The UI will perform a **cold system reboot** on the device and log the operation in the **UART Logs** section.


## Limitations & Assumptions

* **Sensor Address**: The code strictly assumes the sensor is at I2C address `0x4A`. If your sensor is configured for `0x4B`, the code will fail.
* **Buffer Limit**: The command text cannot be longer than 256 characters.
* **Blocking Wait**: The loop uses `k_msleep`, meaning the processor pauses completely between readings.
* **Specific Sensor Family**: This code sends commands specific to the Sensirion STS3x family (`0x240B`, `0x30A2`) and may not work with generic sensors like the LM75.

