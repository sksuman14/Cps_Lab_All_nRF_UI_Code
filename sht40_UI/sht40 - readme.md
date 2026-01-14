SHT40 Sensor Interface – nrf52832 dev board
This repository provides firmware and setup instructions to integrate the SHT40 temperature and humidity sensor with the nrf52832 dev 
board . The project enables real-time acquisition and visualization of environmental data using the IoT Serial Monitoring App.

Features

Measures temperature and humidity using the SHT40 sensor
Communicates via I2C for reliable and precise sensor data acquisition
Designed for compatibility with nrf52832 dev board hardware
Live data display and charting in the IoT Serial Monitoring App
Supports user-configurable logging intervals and UART logs



Hardware Requirements
nrf52832 dev Board 
SHT40 Sensor (I2C interface)

USB cable (PC to board connection)

Software Requirements

IoT Serial Monitoring App

Prerequisites

Understanding of I2C and etc. communication protocols
Basic project setup and wiring skills

Pin Configuration
Board Pins	Sensor Pins
3V3	VCC
GND	GND
SDA	SDA
SCL	SCL


Setup & Configuration

Connect the SHT40 sensor to the nrf52832 dev board via I2C as per the pin mapping above.
Connect your board to the PC using a USB cable.
Launch the IoT Serial Monitoring App, enter COM port, baud rate, sensor type, and interval settings.
View live temperature and humidity data in the app.

Sample Output
Connected to COM4 at 115200 baud
SHT40 Temperature 30.18°C, Humidity 52.09%
SHT40 Temperature 30.08°C, Humidity 51.25%
SHT40 Temperature 29.96°C, Humidity 50.78%
SHT40 Temperature 29.87°C, Humidity 50.37%
SHT40 Temperature 29.76°C, Humidity 50.15%
Live visualization and logs are available in the IoT Serial Monitoring App.

Usage
Flash the firmware onto your nrf52832 dev Board.
The firmware starts reading temperature and humidity data from the SHT40 sensor.
Data is streamed to your PC and visualized via the IoT Serial Monitoring App.
Log intervals, COM port, and settings can be configured in the app.

Troubleshooting
Sensor Not Detected:
Check all wiring and physical connections.
Ensure the I2C address and baud rate are set correctly in the app.

Inconsistent Readings:
Confirm correct sensor placement.
Verify I2C interface selections and initialization in firmware.

License
Released under an open-source license. See LICENSE file for details.

