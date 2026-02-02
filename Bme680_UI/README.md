# BME680 Environmental Monitor: IoT Lab Demonstration

This project is designed for an **IoT Laboratory** setting to demonstrate real-time atmospheric sensing using the Bosch BME680 and the Zephyr RTOS. It features a dual-purpose architecture: autonomous environmental data logging and a remote **User Interface (UI)** for dynamic system control via UART.


## Hardware Requirements

* **Microcontroller**: nRF52832 Development Kit.
* **Sensor**: Bosch BME680 Environmental Sensor.
* **Connection Interface**:
    * **I2C**: For sensor-to-MCU data acquisition.
    * **UART**: For MCU-to-PC communication via the Zephyr console.


## Project Architecture

The application utilizes a modular design to separate sensing logic from the user interface:

 1.**Hardware Layer**: Defined in the DeviceTree Overlay, establishing the I2C standard bitrate and the BME680 sensor node at address `0x76`.


2.**UI Module (`ui.c`)**: Manages asynchronous serial communication. It handles a 64-byte receive buffer and parses incoming strings to trigger system changes.


3.**Application Logic (`main.c`)**: Coordinates the initialization of the UI and sensor, then enters a continuous measurement loop.

## Logic & Code Flow

1.**Setup Phase**:
* The **User Interface** is initialized, setting up UART callbacks and enabling asynchronous reception.

* The system verifies the BME680 sensor is ready for use; if not, it halts to prevent erroneous readings.

2.**Operational Loop**:
* **Dynamic Interval**: The loop calls `ui_get_interval_ms()` at the start of every cycle to check if the user has requested a new sampling rate via the UI.

* **Data Acquisition**: The BME680 fetches temperature, humidity, and atmospheric pressure data.

* **UI Output**: Results are converted to standard units (Celsius, %RH, kPa) and printed to the console.

* **Sleep**: The system enters a low-power state for the duration of the current `delayTime`.

3.**Asynchronous Control**:
* UART interrupts allow the UI module to process commands like `SET_INTERVAL` or `restartDevice` even while the main sensor loop is active.

## Features

* **Environmental Telemetry**: Real-time tracking of Temperature, Humidity, and Pressure.
* **Live Interval Adjustment**: Use the **User Interface** to change the sampling frequency on-the-fly.
* **Remote System Reset**: Capability to perform a "Cold Reboot" of the microcontroller via a UI command.


* **Robust UI Parsing**: Features an asynchronous UART callback to ensure the device remains responsive to user input.

## Communication Protocols

* **I2C**: Connects the BME680 sensor to the `i2c0` bus on the nRF52832.


* **UART**: Provides the physical layer for the **User Interface** console, utilizing an event-driven callback mechanism (`uart_cb`).

## Project Structure

* `main.c`: Core application logic, sensor data fetching, and the primary execution loop.

* `ui.c`: Implementation of the UART-based User Interface, command parsing logic, and system reset triggers.

* `ui.h`: Header file defining the public UI API, including `ui_init()` and `ui_get_interval_ms()`.

* `nrf52dk_nrf52832.overlay`: Configures the I2C hardware pins and the BME680 device node.


## Setup and Build
### 1. Hardware Setup

Connect the BME680 to your nRF52 DK:

* **VCC/GND**: 3.3V and Ground.
* **SDA/SCL**: Connect to pins P0.26 and P0.27 (standard I2C0 for nRF52832).



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

---

##  Limitations & Assumptions

* **Gas Sensor**: Although the BME680 supports gas resistance, this specific firmware focuses on Temp, Hum, and Press.


* **I2C Address**: The system assumes the sensor is at address `0x76` as defined in the overlay.


* **Command Buffer**: The UI command buffer is limited to 256 characters.
