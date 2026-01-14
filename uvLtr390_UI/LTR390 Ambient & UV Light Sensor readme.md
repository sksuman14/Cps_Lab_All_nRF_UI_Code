LTR390 Ambient & UV Light Sensor Interface – nrf52832 dev board
This repository provides firmware and instructions for interfacing the LTR390 sensor (ambient light and UV index) with the nrf52832 dev board. The solution enables real-time monitoring and logging of light and UV data, visualized via the IoT Serial Monitoring App.

Features

Measures ambient light intensity and UV index using the LTR390 sensor
Communicates over I2C with the nrf52832 dev board 
Real-time charting and log display in the IoT Serial Monitoring App
Configurable data logging intervals and UART console output

Hardware Requirements
nrf52832 dev board
LTR390 sensor (I2C interface)
Programmer (for firmware flashing and UART connection)
USB cable (for board-PC interface)

Software Requirements

IoT Serial Monitoring App (light/UV dashboard and visualization)

Prerequisites

Understanding of I2C and UART protocols
Proper hardware wiring (pin mapping below)

Pin Configuration
Board Pins	Sensor Pins
3V3	VDD
SDA	SDA
SCL	SCL
GND	GND

Setup & Configuration

Connect the LTR390 sensor to the nrf52832 dev board according to the mapping above.
Attach the board to your PC 
Start the IoT Serial Monitoring App. Enter sensor, COM port, baud rate, and logging interval.
The app will display current readings and logs for ambient and UV light.

Sample Output
Connected to COM20 at 115200 baud
LTR390 UV Index0
LTR390 UV Index2
UV 2.00 (Low)
...
Sensor Data Visualization
UV Index: 2.00
Ambient and UV light measurements are charted and logged in the IoT Serial Monitoring App.

Usage
Flash the firmware onto your nrf52832 dev board.
Real-time light and UV index values are displayed in the app and serial console.
Change logging intervals, serial settings, and sensor configuration as needed.
Troubleshooting

Sensor not detected:
Verify pin mapping, wiring, and power.
Ensure I2C addresses and baud rate are correct.
Inaccurate readings:
Confirm sensor placement and initialization.

License
Released under open-source license. See LICENSE file for details.

