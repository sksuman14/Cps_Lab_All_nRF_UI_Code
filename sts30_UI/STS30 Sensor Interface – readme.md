STS30 Sensor Interface – nrf52832 dev board

This repository provides firmware and setup instructions to integrate the STS30 temperature sensor with the nrf52832 dev board. The project supports real-time acquisition and visualization of temperature data using the IoT Serial Monitoring App.

Features

Measures ambient temperature using the STS30 sensor
Connects via I2C interface for stable sensor communication
Live display and charting of data in the IoT Serial Monitoring App
Supports logging intervals and UART logs for flexible monitoring

Hardware Requirements

nrf52832 dev board 
STS30 Sensor (I2C)
USB cable (for board-PC interface)

Software Requirements

IoT Serial Monitoring App (for live data output)

Prerequisites

Understanding of I2C and UART communication protocols
Basic project wiring skills

Pin Configuration

Board Pins	Sensor Pins
3V3	VCC
GND	GND
SDA	SDA
SCL	SCL

Setup & Configuration

Connect the STS30 sensor to the nrf52832 dev board via I2C using the pin mapping above.
Use a USB cable to connect your board to the PC
Start the IoT Serial Monitoring App, enter COM port, baud rate, sensor selection, and interval settings.
Monitor and visualize live temperature data from the sensor.

Sample Output
Connected to COM4 at 115200 baud
STS30 Temperature 23.03°C
STS30 Temperature 23.05°C
STS30 Temperature 23.06°C
...
Temperature readings are charted and logged in the IoT Serial Monitoring App.

Usage

Flash the STS30 firmware to your nrf52832 dev board.
Start the system and sensor readings will appear in the serial terminal or IoT Serial Monitoring App.
Adjust COM port, baud rate, sensor selection, and interval in the app as needed.

Troubleshooting
Sensor Not Detected:
Inspect wiring and connections; confirm correct I2C address and baud rate.


License
This project is released under an open-source license. See LICENSE file for details.

