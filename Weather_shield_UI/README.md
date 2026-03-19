# Weather Shield: IoT Lab Demonstration

This project is designed for an **IoT Laboratory** setting to demonstrate multi-sensor data fusion using the **Bosch BME680** (Atmospheric) and **Vishay VEML7700** (Ambient Light) sensors with the **Zephyr RTOS**. It features a synchronized loop that aggregates temperature, humidity, pressure, and light intensity data, along with a remote **User Interface (UI)** for dynamic control.

## Hardware Requirements

* **Microcontroller**: nRF52832 Development Kit.
* **Sensors**:
    * **Bosch BME680**: Measures Temperature, Humidity, and Pressure.
    * **Vishay VEML7700**: Measures High-Accuracy Ambient Light (Lux).


* **Connection Interface**:
    * **I2C**: Connects both sensors to the microcontroller on a shared bus.
    * **UART**: Connects the microcontroller to the PC for sending commands.



## Project Architecture

The application uses a modular design to separate the sensing logic from the user settings:

1 . **Hardware Layer**: Defined in the DeviceTree Overlay, enabling the `i2c0` bus. It  registers the **BME680** at address `0x76` and the **VEML7700** at address `0x10`.

2 . **UI Module (`ui.c`)**: Manages serial communication. It listens for incoming text commands to change the sampling speed or restart the board.

3 . **Application Logic (`main.c`)**: Initializes both sensors and runs the main loop to fetch, aggregate, and display the combined environmental data.

## Logic & Code Flow

### 1. Setup Phase

* The **User Interface** is initialized via `ui_init()` to start listening for UART commands.
* The system verifies the **BME680** is ready. If not, it prints "BME680 is not ready".
* The system verifies the **VEML7700** is ready. If not, it prints "VEML7700 device not ready".

### 2. Operational Loop

* **Check Speed**: At the start of every loop, the code calls `ui_get_interval_ms()` to see if the user has requested a new sampling interval.
* **Fetch BME680 Data**: The system reads Temperature, Humidity, and Pressure. Pressure is converted from kPa to hPa (multiplied by 10).
* **Fetch VEML7700 Data**: The system reads the Light Intensity (Lux).
* **Print Output**: All four values are formatted into a single CSV-style line:
  `T:25.00,H:50.00,P:1013.00,L:120.00`.
* **Wait**: The system pauses for the duration of the current `delayTime` (`k_msleep`) before repeating the loop.

### 3. Asynchronous Control

* **UART Interrupts**: The system uses a background process (callback) to handle commands like `SET_INTERVAL` instantly, without stopping the sensor loop.

## Features

* **Multi-Sensor Fusion**: Simultaneous tracking of 4 environmental variables (Temp, Hum, Press, Lux).
* **Shared I2C Bus**: Demonstrates how to manage multiple devices (Address `0x76` & `0x10`) on a single bus.
* **Live Interval Adjustment**: Use the **User Interface** to change how often the sensors report data.
* **Remote System Reset**: Capability to restart the microcontroller using a UI command.

## Communication Protocols

* **I2C**: Used to communicate with both sensors on the `i2c0` bus.
* **UART**: Used for the **User Interface** console to receive user commands and display sensor data.

## Project Structure

* `main.c`: Contains the main loop that coordinates reading both sensors and printing the combined string.
* `ui.c`: Handles the text commands (`restartDevice`, `SET_INTERVAL`) and UART callback logic.
* `ui.h`: Header file that exposes the UI functions to the main application.
* `nrf52dk_nrf52832.overlay`: Configures the I2C hardware pins (P0.26/P0.27) and defines the two sensor nodes.

## Setup and Build

### 1. Hardware Setup

Connect both sensors to your nRF52 DK using the **I2C0** bus:

* **VCC/GND**: Connect both sensors to 3.3V and Ground.
* **SDA**: Connect both sensors' SDA pins to **P0.26**.
* **SCL**: Connect both sensors' SCL pins to **P0.27**.

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

* **Sensor Addresses**: The code assumes the BME680 is at `0x76` and the VEML7700 is at `0x10`. If your hardware uses different addresses (e.g., BME680 at `0x77`), the code will fail.
* **Buffer Limit**: The command text cannot be longer than 256 characters.
* **Blocking Wait**: The loop uses `k_msleep`, meaning the processor pauses completely between readings.
* **Error Handling**: If one sensor fails, the loop prints an error for that specific sensor but attempts to continue with the next iteration.