# Accelerometer Monitor: IoT Lab Demonstration

This project is designed for an **IoT Laboratory** setting to demonstrate how to track motion using an **Accelerometer** (LIS2DH) and the **Zephyr RTOS**. It features a continuous loop that reports motion data (X, Y, Z coordinates), along with a remote **User Interface (UI)** to control the system.

## Hardware Requirements

* **Microcontroller**: nRF52832 Development Kit.
* **Sensor**: LIS2DH Accelerometer (measures motion/tilt).
* **Connection Interface**:
    * **I2C**: Connects the sensor to the microcontroller for data reading.
    * **UART**: Connects the microcontroller to the PC for sending commands.



## Project Architecture

The application uses a modular design to separate the sensor reading from the user settings:

1 . **Hardware Layer**: Defined in the DeviceTree Overlay, enabling the `i2c0` bus and configuring the LIS2DH sensor at address `0x19`.


2 . **UI Module (`ui.c`)**: Manages serial communication. It listens for incoming text commands to change the reporting speed or restart the board.

3 . **Application Logic (`main.c`)**: Initializes the sensor and runs the main loop to fetch and display acceleration data.

## Logic & Code Flow

### 1. Setup Phase

* The **User Interface** is initialized to start listening for commands.
* The system checks if the **LIS2DH sensor** is connected and ready. If not, it prints an error message.

### 2. Operational Loop

* **Check Speed**: At the start of every loop, the code calls `ui_get_interval_ms()` to see if the user has requested a new update speed.
* **Fetch Data**: The system asks the sensor for the current X, Y, and Z acceleration values.
* **Print Output**: The data is formatted and printed to the console (e.g., "x 0.010, y 0.200, z 9.800").
* **Wait**: The system pauses for the duration of the current interval before repeating the loop.

### 3. Asynchronous Control

* **UART Interrupts**: The system uses a background process (callback) to handle commands like `SET_INTERVAL` instantly, without stopping the sensor loop.

## Features

* **Motion Tracking**: Real-time monitoring of acceleration on three axes (X, Y, Z).
* **Live Interval Adjustment**: Use the **User Interface** to change how often the sensor reports data.
* **Remote System Reset**: Capability to restart the microcontroller using a UI command.
* **Error Handling**: Detects if the sensor is missing or failing to provide data.

## Communication Protocols

* **I2C**: Used to talk to the sensor. The code uses the standard speed and address `0x19`.


* **UART**: Used for the **User Interface** console to receive user commands and display sensor data.

## Project Structure

* `main.c`: Contains the main loop that reads the sensor and prints the values.
* `ui.c`: Handles the text commands (`restartDevice`, `SET_INTERVAL`) from the computer.
* `ui.h`: Header file that allows the main code to talk to the UI code.
* `nrf52dk_nrf52832.overlay`: Configures the I2C hardware and registers the LIS2DH sensor node.



## Setup and Build

### 1. Hardware Setup

Connect the LIS2DH Accelerometer to your nRF52 DK using I2C:

* **VCC/GND**: 3.3V and Ground.
* **SDA/SCL**: Connect to the standard I2C pins on the nRF52832 (usually P0.26 and P0.27).

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

* **Sensor Address**: The code assumes the sensor is at I2C address `0x19`.


* **Buffer Limit**: The command text cannot be longer than 256 characters.
* **Blocking Wait**: The loop uses `k_msleep`, meaning the processor pauses completely between readings.