# VL53L0X Distance Monitor: IoT Lab Demonstration

This project is designed for an **IoT Laboratory** setting to demonstrate precision distance sensing using the **VL53L0X Time-of-Flight (ToF)** sensor and the **Zephyr RTOS**. It features a continuous loop that reports distance measurements in centimeters, along with a remote **User Interface (UI)** for dynamic system control via UART.

## Hardware Requirements

* **Microcontroller**: nRF52832 Development Kit.
* **Sensor**: STMicroelectronics VL53L0X (Time-of-Flight Distance Sensor).
* **Connection Interface**:
    * **I2C**: Connects the sensor to the microcontroller for data acquisition.
    * **UART**: Connects the microcontroller to the PC for sending commands.



## Project Architecture

The application uses a modular design to separate the sensing logic from the user settings:

1 . **Hardware Layer**: Defined in the DeviceTree Overlay, enabling the `i2c0` bus and configuring the `vl53l0x` sensor node at address `0x29`.


2 . **UI Module (`ui.c`)**: Manages serial communication. It listens for incoming text commands to change the sampling speed or restart the board.

3 . **Application Logic (`main.c`)**: Initializes the sensor and runs the main loop to fetch, calculate, and display distance data.

## Logic & Code Flow

### 1. Setup Phase

* The **User Interface** is initialized to start listening for commands via UART.
* The system checks if the **VL53L0X sensor** is connected and ready. If not, it prints an error message (`sensor: device not ready`).

### 2. Operational Loop

* **Check Speed**: At the start of every loop, the code calls `ui_get_interval_ms()` to see if the user has requested a new sampling interval.
* **Fetch Data**: The system triggers a new measurement on the sensor using `sensor_sample_fetch`.
* **Get Distance**: It retrieves the distance data using the channel `SENSOR_CHAN_DISTANCE`.
* **Conversion**: The raw sensor value (typically in meters) is converted to **centimeters** by multiplying by 100.0.
* **Print Output**: The result is formatted and printed to the console (e.g., `distance is 15.420cm`).
* **Wait**: The system pauses for the duration of the current `delayTime` before repeating the loop.

### 3. Asynchronous Control

* **UART Interrupts**: The system uses a background process (callback) to handle commands like `SET_INTERVAL` instantly, without stopping the sensor loop.

## Features

* **Time-of-Flight Sensing**: Accurate distance measurement using laser-ranging technology.


* **Metric Output**: Automatically converts readings to centimeters for easy readability.
* **Live Interval Adjustment**: Use the **User Interface** to change how often the sensor reports data.
* **Remote System Reset**: Capability to restart the microcontroller using a UI command.

## Communication Protocols

* **I2C**: Used to communicate with the sensor. The configuration uses address `0x29` and standard bitrate.


* **UART**: Used for the **User Interface** console to receive user commands and display sensor data.

## Project Structure

* `main.c`: Contains the main loop that reads the sensor, performs unit conversion (m to cm), and prints the values.
* `ui.c`: Handles the text commands (`restartDevice`, `SET_INTERVAL`) and UART callback logic.
* `ui.h`: Header file that exposes the UI functions to the main application.
* `nrf52dk_nrf52832.overlay`: Configures the I2C hardware pins (P0.26/P0.27) and registers the `vl53l0x` node.



## Setup and Build

### 1. Hardware Setup

Connect the VL53L0X Sensor to your nRF52 DK using I2C:

* **VCC/GND**: Connect to 3.3V and Ground.
* **SDA**: Connect to pin **P0.26** (defined in overlay).
* **SCL**: Connect to pin **P0.27** (defined in overlay).



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


* **Sensor Address**: The code assumes the sensor is at the default I2C address `0x29`.


* **Buffer Limit**: The command text cannot be longer than 256 characters.
* **Blocking Wait**: The loop uses `k_msleep`, meaning the processor pauses completely between readings.
* **Range**: The VL53L0X typically measures up to 2 meters; readings beyond this may be inaccurate or time out.