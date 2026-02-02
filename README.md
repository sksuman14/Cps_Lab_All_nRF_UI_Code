# IOT Lab: Interactive Control & Visualization

This repository contains various experiments and projects for embedded systems using **Zephyr RTOS** with the **nRF52832 Development Kit**. The projects utilize different sensors, actuators, and communication interfaces to demonstrate their functionality and integration.

A key feature of this repository is the unified **User Interface (UI)** module, which allows for dynamic, real-time control and visualization of the hardware via UART without recompiling code.

## Key Feature: Interactive User Interface (UI)

All projects in this repository utilize a shared UI module (`ui.c` / `ui.h`) that enables asynchronous control over UART. This interface allows for dynamic control and data visualization using either a standard serial terminal or a dedicated graphical application.

### Graphical User Interface (GUI) and Visualization

A companion GUI application provides a user-friendly way to interact with the firmware. The GUI allows users to easily configure settings, view raw UART logs, and visualize sensor data in real-time using gauges and charts.

### UI Capabilities

Using the GUI or terminal commands, users can perform the following actions:

* **Dynamic Interval Adjustment**: Change the sensor sampling rate on-the-fly. In the GUI, simply enter the desired seconds in the "Interval Configuration" section and click the **Set Interval** button.
    * *Terminal Command*: `SET_INTERVAL: <seconds>`
    * *Logic*: The system parses the input and updates the global delay variable used by the main loop.
* **Remote System Reset**: Perform a "Cold Reboot" of the microcontroller by clicking the **Restart Device** button in the GUI.
    * *Terminal Command*: `restartDevice`
    * *Logic*: Triggers `sys_reboot(SYS_REBOOT_COLD)` after a brief safety delay.
* **Data Visualization**: The GUI parses incoming sensor data strings and renders them visually, such as the Celsius and Fahrenheit thermometers shown in the image above.
* **Asynchronous Processing**: The firmware uses UART interrupts and a callback mechanism (`uart_cb`) to handle incoming user input into a buffer (`rx_buf`), ensuring the main sensor loop is never blocked.

---

## Table of Contents

* [Weather Shield (BME680 + VEML7700)](#weather-shield-bme680--veml7700)
* [BME680 Sensor](#bme680-sensor)
* [Accelerometer (LIS3DH)](#accelerometer-lis2dh)
* [Buzzer Control](#buzzer-control)
* [Hall Effect Sensor](#hall-effect-sensor)
* [LED Blink Remote](#led-blink-remote)
* [LTR390 UV Sensor](#ltr390-uv-sensor)
* [Relay Control](#relay-control)
* [SHT4x Environment](#sht4x-environment)
* [STS3x Temperature](#sts3x-temperature)
* [STTS751 Temperature](#stts751-temperature)
* [VCNL4040 Lux](#vcnl4040-lux)
* [VL53L0X Distance](#vl53l0x-distance)

---

## Project Descriptions

### Weather Shield (BME680 + VEML7700)
This sample application demonstrates **sensor fusion** by combining the **Bosch BME680** and **Vishay VEML7700** on a single I2C bus.
* **Functionality**: It periodically measures four environmental parameters: Temperature, Humidity, Pressure (converted to hPa), and Ambient Light (Lux).
* **Output**: Data is aggregated into a single CSV-style string: `T:25.00,H:50.00,P:1013.00,L:120.00`.
* **Hardware**: Uses I2C addresses `0x76` (BME680) and `0x10` (VEML7700).

### BME680 Sensor
This sample application focuses specifically on the **Bosch BME680** Environmental Sensor.
* **Functionality**: Measures Temperature, Humidity, Atmospheric Pressure, and Gas Resistance (Air Quality).
* **Use Case**: Ideal for indoor air quality monitoring projects where light sensing is not required.

### Accelerometer (LIS3DH)
This project interfaces the **LIS3DH accelerometer** to measure motion data.
* **Functionality**: Tracks acceleration across the X, Y, and Z axes.
* **Output**: Displays raw or converted G-force coordinates on the console.

### Buzzer Control
This project implements an audio signaling interface using a piezoelectric buzzer.
* **Functionality**: Allows the user to remotely trigger the buzzer and adjust the ON/OFF frequency via UART commands.
* **Use Case**: Alarm systems or notification alerts.

### Hall Effect Sensor
This sample application utilizes a **Digital Hall Effect sensor** to detect magnetic fields.
* **Functionality**: Uses hardware interrupts to trigger immediate status updates.
* **Output**: Reports "Magnet Detected" or "Not Detected" instantly.

### LED Blink 
A dynamic "blinky" experiment where the LED toggling interval is controlled remotely.
* **Functionality**: Users can change the blinking speed on-the-fly using the `SET_INTERVAL` command in the UI.

### LTR390 UV Sensor
This project integrates the **LTR390 sensor** to measure Ultraviolet (UV) radiation.
* **Functionality**: Configures the sensor in UVS mode to read and display raw UV data or calculated UV Index.

### Relay Control
This application demonstrates the control of high-power peripherals using a **Relay module**.
* **Functionality**: Toggles a GPIO pin to switch the relay state at user-defined intervals.
* **Hardware**: Maps the relay control to a specific GPIO pin defined in the device tree.

### SHT4x Environment
This project interfaces the **Sensirion SHT4x** (e.g., SHT40) to read high-precision environmental data.
* **Functionality**: Displays Ambient Temperature and Relative Humidity readings with high accuracy.

### STS3x Temperature
This sample application interfaces the high-accuracy **STS3x temperature sensor**.
* **Functionality**: Includes logic to perform a soft reset on the sensor at boot and provides stable temperature readings.

### STTS751 Temperature
This project reads data from the **STTS751 digital temperature sensor**.
* **Functionality**: Processes high and low-byte registers to output temperature values in both Celsius and Fahrenheit. The data can be visualized using thermometers in the GUI.

### VCNL4040 Lux
This repository contains code to interface the **Vishay VCNL4040 sensor**.
* **Functionality**: Focuses on the ambient light sensor (ALS) capabilities, reading and displaying Lux values.

### VL53L0X Distance
This sample measures the distance between the **VL53L0X Time-of-Flight sensor** and a target object.
* **Functionality**: The raw measurement is converted and displayed in centimeters for easy readability.

---

## Getting Started

### Prerequisites
1 .  **Zephyr RTOS**: Ensure the Zephyr toolchain is installed and configured.

2 .  **nRF52832 Development Board**: The hardware platform for these experiments.

3 .  **Terminal Emulator or GUI App**: nrf Connect or the provided GUI application  (Settings: **115200 baud**).

### Setup & Build
1 .  **Hardware Connection**: Connect your sensors via I2C (Standard Pinout: `SDA=P0.26`, `SCL=P0.27`).

2 .  **Build**: Navigate to the specific project folder and run:
    ```bash
    west build -b nrf52dk_nrf52832
    ```
    
3 .  **Flash**: Connect the board via USB and run:
    ```bash
    west flash
    ```

4 .  **Connect UI**: Open your serial terminal or the GUI application. You can now interact with the device, change intervals, and visualize data.
