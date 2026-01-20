## BME680 Sensor Interface – nrf52832 dev board

This repository provides firmware and setup instructions to integrate the BME680 environmental sensor with the nrf52832 dev board. The solution supports real-time acquisition and visualization of temperature, humidity, and pressure readings via the IoT Serial Monitoring App.

# Features

Acquires data from BME680
Communicates over I2C interface for reliable sensor data gathering
Designed for use with nrf52832 dev board
Visualizes data in real time using IoT Serial Monitoring App
Configurable logging intervals via the serial interface
Supports soft reboots and error handling for robust operation

# Hardware Requirements

nrf52832 dev board 
BME680 Sensor (I2C interface)
USB cable (for board-PC connection)

# Software Requirements

IoT Serial Monitoring App

Prerequisites

Familiarity with sensor communication protocols 
Correct hardware wiring as described below

# Pin Configuration

Board Pins	Sensor Pins
3V3	VCC
GND	GND
SDA	SDA
SCL	SCL

# Setup & Configuration

Connect the nrf52832 dev board to your PC using the USB cable.
Wire the BME680 sensor to the board via I2C as shown above.

Launch the IoT Serial Monitoring App and select the appropriate COM port and baud rate.
Set the desired interval for data updates.

Sample Output
Connected to COM61 at 115200 baud
BME680 Temperature 24.96°C, Pressure 983.53 hPa, Humidity 65.65%
BME680 Temperature 25.02°C, Pressure 983.69 hPa, Humidity 65.71%
BME680 Temperature 26.37°C, Pressure 983.77 hPa, Humidity 67.62%
Data is refreshed at user-defined intervals, and live charts visualize sensor values.

# Usage
Flash the firmware onto your nrf52832 dev board.
The application starts reading BME680 sensor data and logs it to the serial console.
Use the IoT Serial Monitoring App to view and analyze results.
Adjust configuration settings (port, baud rate, interval) as needed in the app.

# Troubleshooting

Sensor Not Detected:
Verify physical connections and I2C address (default 0x76/0x77).
Confirm interface and address settings in configuration.

# Incorrect Readings:
Ensure sensor wiring and environmental conditions are appropriate.
Confirm sensor initialization in firmware.
License
Released under an open-source license. See LICENSE file for details.

