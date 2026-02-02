# STTS751 Temperature Monitor: IoT Lab Demonstration

This project is designed for an **IoT Laboratory** setting to demonstrate digital thermal sensing using the **STTS751** temperature sensor and the **Zephyr RTOS**. It features a continuous loop that reports Temperature in both Celsius and Fahrenheit, along with a remote **User Interface (UI)** for dynamic system control via UART.

## Hardware Requirements

* **Microcontroller**: nRF52832 Development Kit.
* **Sensor**: STTS751 Digital Temperature Sensor.
* **Connection Interface**:
  * **I2C**: Connects the sensor to the microcontroller for data acquisition.
  * **UART**: Connects the microcontroller to the PC for sending commands.





## Project Architecture

The application uses a modular design to separate the sensing logic from the user settings:

1 . **Hardware Layer**: Defined in the DeviceTree Overlay, enabling the `i2c0` bus and configuring the `mysensor` node at address `0x4A`.


2 . **UI Module (`ui.c`)**: Manages serial communication. It listens for incoming text commands to change the sampling speed or restart the board.

3 . **Application Logic (`main.c`)**: Initializes the sensor configuration and runs the main loop to fetch, calculate, and display temperature data.

## Logic & Code Flow

### 1. Setup Phase

* The **User Interface** is initialized to start listening for commands via UART.
* **Sensor Configuration**: The system writes the value `0x8C` to the Configuration Register (`0x03`) of the STTS751 to set the resolution and operation mode.

### 2. Operational Loop

* **Check Speed**: At the start of every loop, the code calls `ui_get_interval_ms()` to see if the user has requested a new sampling interval.
* **Fetch Data**: The system reads two separate registers:
    * `stts751TempHighReg` (`0x00`): The integer part of the temperature.
    * `stts751TempLowReg` (`0x02`): The fractional part of the temperature.



* **Print Output**: The data is printed to the console (e.g., `STTS751: Temperature: 25.50 °C | 77.90 °F`).
* **Wait**: The system pauses for the duration of the current `delayTime`.

### 3. Asynchronous Control

* **UART Interrupts**: The system uses a background process (callback) to handle commands like `SET_INTERVAL` instantly, without stopping the sensor loop.

## Features

* **Dual Unit Display**: Real-time tracking of temperature in both **Celsius (°C)** and **Fahrenheit (°F)**.
* **Live Interval Adjustment**: Use the **User Interface** to change how often the sensor reports data.
* **Remote System Reset**: Capability to restart the microcontroller using a UI command.
* **Configuration Write**: Automatically configures the sensor register on boot for optimal performance.

## Communication Protocols

* **I2C**: Used to communicate with the sensor. The configuration uses address `0x4A`.


* **UART**: Used for the **User Interface** console to receive user commands and display sensor data.

## Project Structure

* `main.c`: Contains the main loop, sensor configuration (`0x03`), and temperature conversion logic.
* `ui.c`: Handles the text commands (`restartDevice`, `SET_INTERVAL`) and UART callback logic.
* `ui.h`: Header file that exposes the UI functions to the main application.
* `nrf52dk_nrf52832.overlay`: Configures the I2C hardware and registers the `mysensor` node with label `STTS751`.



## Setup and Build

### 1. Hardware Setup

Connect the STTS751 Sensor to your nRF52 DK using I2C:

* **VCC/GND**: Connect to 3.3V and Ground.
* **SDA/SCL**: Connect to the I2C pins defined for your board (The overlay comments suggest P0.26 for SDA and P0.25 for SCL, but standard nRF52 DK pinout is often P0.26/P0.27. Check your specific pinout).



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


**Sensor Address**: The code assumes the sensor is at I2C address `0x4A`.


* **Buffer Limit**: The command text cannot be longer than 256 characters.
* **Blocking Wait**: The loop uses `k_msleep`, meaning the processor pauses completely between readings.
* **Register Map**: The code relies on specific registers (`0x00`, `0x02`, `0x03`) which are specific to the STTS751 and compatible sensors.