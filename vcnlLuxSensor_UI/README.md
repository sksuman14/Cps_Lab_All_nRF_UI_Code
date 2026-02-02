# VCNL4040 Light Sensor: IoT Lab Demonstration

This project is designed for an **IoT Laboratory** setting to demonstrate how to measure **Ambient Light** (Lux) using the **Vishay VCNL4040** sensor and the **Zephyr RTOS**. It features a continuous loop that reports light intensity data, along with a remote **User Interface (UI)** to control the system.

## Hardware Requirements

* **Microcontroller**: nRF52832 Development Kit.
* **Sensor**: Vishay VCNL4040 (Proximity and Ambient Light Sensor).
* **Connection Interface**:
    * **I2C**: Connects the sensor to the microcontroller for data acquisition.
    * **UART**: Connects the microcontroller to the PC for sending commands.



## Project Architecture

The application uses a modular design to separate the sensing logic from the user settings:

1 . **Hardware Layer**: Defined in the DeviceTree Overlay, enabling the `i2c0` bus and configuring the `vcnl4040` sensor node at address `0x60`.

2 . **UI Module (`ui.c`)**: Manages serial communication. It listens for incoming text commands to change the sampling speed or restart the board.

3 . **Application Logic (`main.c`)**: Initializes the sensor and runs the main loop to fetch and display light intensity values.

## Logic & Code Flow

### 1. Setup Phase

* The **User Interface** is initialized via `ui_init()` to start listening for UART commands.
* The system checks if the **VCNL4040 sensor** is connected and ready. If not, it prints an error message (`Sensor: device not ready`).

### 2. Operational Loop

* **Check Speed**: At the start of every loop, the code calls `ui_get_interval_ms()` to see if the user has requested a new sampling interval.
* **Fetch Data**: The system triggers a new measurement on the sensor using `sensor_sample_fetch`.
* **Get Light Value**: It retrieves the light intensity data using the channel `SENSOR_CHAN_LIGHT`.
* **Print Output**: The result is printed to the console (e.g., `VCNL4040: Lux=120`).
* **Wait**: The system pauses for the duration of the current `delayTime` (`k_msleep`) before repeating the loop.

### 3. Asynchronous Control

* **UART Interrupts**: The system uses a background process (callback) to handle commands like `SET_INTERVAL` instantly, without stopping the sensor loop.

## Features

* **Light Sensing**: Real-time monitoring of ambient light intensity in Lux.
* **Live Interval Adjustment**: Use the **User Interface** to change how often the sensor reports data.
* **Remote System Reset**: Capability to restart the microcontroller using a UI command.
* **Robust Input Handling**: The UI ignores spaces and validates numbers to prevent crashes.

## Communication Protocols

* **I2C**: Used to communicate with the sensor. The configuration uses address `0x60` and "Fast" bitrate.
* **UART**: Used for the **User Interface** console to receive user commands and display sensor data.

## Project Structure

* `main.c`: Contains the main loop that reads the light channel and prints the values.
* `ui.c`: Handles the text commands (`restartDevice`, `SET_INTERVAL`) and UART callback logic.
* `ui.h`: Header file that exposes the UI functions to the main application.
* `nrf52dk_nrf52832.overlay`: Configures the I2C hardware and registers the `vishay,vcnl4040` sensor node.

## Setup and Build

### 1. Hardware Setup

Connect the VCNL4040 Sensor to your nRF52 DK using I2C:

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

* **Sensor Address**: The code assumes the sensor is at I2C address `0x60`.
* **Buffer Limit**: The command text cannot be longer than 256 characters.
* **Blocking Wait**: The loop uses `k_msleep`, meaning the processor pauses completely between readings.
* **Single Channel**: Although the VCNL4040 is also a proximity sensor, this code only reads and reports the **Light (Lux)** channel.