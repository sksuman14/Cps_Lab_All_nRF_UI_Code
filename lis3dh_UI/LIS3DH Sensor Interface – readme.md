LIS3DH Sensor Interface – nrf52832 dev board

This repository provides firmware and setup guidance for interfacing an LIS3DH sensor with the nrf52832 dev board. The solution enables real-time acquisition and visualization of three-axis acceleration data for motion and vibration monitoring using the IoT Serial Monitoring App.

Features

Measures acceleration on X, Y, Z axes using the LIS3DH sensor
I2C communication with the nrf52832 dev board 
Real-time visualization of sensor output in the IoT Serial Monitoring App
Configurable data logging intervals and live UART logs

Hardware Requirements

nrf52832 dev board 
LIS3DH Sensor (I2C interface)
USB cable (for PC-board interface)

Software Requirements

IoT Serial Monitoring App (for displaying and charting acceleration data)

Prerequisites

Project setup and hardware wiring ability

Pin Configuration
Board Pins	Sensor Pins
3V3	VCC
SDA	SDA
SCL	SCL
GND	GND

Setup & Configuration

Wire the LIS3DH sensor to the nrf52832 dev board as detailed above.
Open IoT Serial Monitoring App, enter correct COM port, baud rate, sensor type, and interval settings.
View and log live acceleration data in the app.

Sample Output
Connected to COM4 at 115200 baud
LIS3DH X0.000, Y0.000, Z9.807
LIS3DH X0.000, Y0.000, Z9.807
LIS3DH X0.000, Y0.000, Z9.807
...
Acceleration X 0.00 ms Y 0.00 ms Z 9.81 ms
Acceleration vector data are displayed and graphed in the IoT Serial Monitoring App.

Usage
Flash LIS3DH firmware onto the nrf52832 dev board.
Monitor live acceleration data for all axes via UART console or serial monitoring app.
Customize settings such as interval, sensor, and port in the app to match your requirements.

Troubleshooting
Sensor Not Detected:
Check wiring connections and correct sensor mapping.
Verify app COM port and baud rate settings.

Unexpected Output:
Examine sensor mounting and initialization method in code.

License
Released under an open-source license. Refer to LICENSE file for complete terms.

