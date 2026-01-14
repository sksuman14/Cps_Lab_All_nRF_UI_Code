Weather Shield Sensor Interface – nrf52832 dev board
This repository provides firmware and instructions for interfacing a Weather Shield sensor module with the nrf52832 dev board. It collects, logs, and visualizes environmental parameters (temperature, humidity, pressure, and light intensity) in real time using the IoT Serial Monitoring App.

Features

Measures temperature, humidity, atmospheric pressure, and ambient light (lux)
I2C communication for multi-sensor support on nrf52832 dev board 
Live data charting and output in the IoT Serial Monitoring App
Supports interval-based data logging and UART console logging

Hardware Requirements

nrf52832 dev board 
Weather Shield sensor module (I2C)
USB cable (for board to PC interface)

Software Requirements
IoT Serial Monitoring App (live dashboard/charting)

Prerequisites

Understanding of I2C/UART and environmental sensor protocols
Correct wiring for all modules (see below)

Pin Configuration
Board Pins	Sensor Pins
3V3	VCC
SCL	SCL
SDA	SDA
GND	GND

Setup & Configuration

Connect the Weather Shield sensor module to the nrf52832 dev board using the mapping above.
Attach the board to your PC via USB 
Launch the IoT Serial Monitoring App, configure COM port, baud rate, sensor selection, and logging interval.
View all live readings and sensor logs in the dashboard and UART console.

Sample Output
Connected to COM4 at 115200 baud
Temperature 22.90°C, Humidity 57.58%, Pressure 986.81 hPa, Light 662.40 Lux
Temperature 22.90°C, Humidity 57.55%, Pressure 986.81 hPa, Light 656.00 Lux
Temperature 22.90°C, Humidity 57.56%, Pressure 986.81 hPa, Light 684.80 Lux
...
Sensor Data Visualization
Temperature: 22.90°C
Humidity: 57.49%
Pressure: 986.80 hPa
Light Intensity: 665.6 Lux
All sensor values are continuously graphed and logged in the app.

Usage

Flash the Weather Shield firmware to your nrf52832 dev board.
Monitor temperature, humidity, pressure, and light data in real time via UART console and app dashboard.
Adjust intervals, sensor settings, and COM port as needed for your application.

Troubleshooting
Sensor not detected:
Check wiring, I2C addresses, and configuration settings.
Ensure all pins and modules are securely connected.

Incorrect readings:
Confirm sensor calibration and environmental exposure.

License
Released under open-source license. See LICENSE for terms.
