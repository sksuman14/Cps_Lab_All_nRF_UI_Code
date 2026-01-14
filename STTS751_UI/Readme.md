STTS751 Sensor Interface – nrf52832 dev board
This repository offers firmware and setup instructions to integrate the STTS751 high-precision temperature sensor with the nrf52832 dev board. The project enables accurate, real-time acquisition and visualization of temperature data through the IoT Serial Monitoring App.

Features

Measures ambient temperature with high precision using the STTS751 sensor
Reliable I2C communication with the nrf52832 dev board 
Real-time data display and plotting in the IoT Serial Monitoring App
Supports UART logs and logging intervals for robust, flexible monitoring

Hardware Requirements
nrf52832 dev board 
STTS751 Sensor (I2C interface)
USB cable (for board-PC connection)

Software Requirements

IoT Serial Monitoring App (for live temperature output and visualization)

Prerequisites

Familiarity with I2C and UART communication protocols
Correct hardware wiring (refer to pin configuration below)

Pin Configuration
Board Pins	Sensor Pins
3V3	VCC
GND	GND
SDA	SDA
SCL	SCL

Setup & Configuration

Connect the STTS751 sensor to the nrf52832 dev board using I2C as shown above.
Use a USB cable to connect the board to your PC
Open the IoT Serial Monitoring App, enter correct COM port and baud rate, select sensor type and interval, and connect to view live temperature data.

Sample Output
Connected to COM4 at 115200 baud
STTS751 Temperature 23.25°C (73.85°F)
STTS751 Temperature 23.00°C (73.40°F)
STTS751 Temperature 23.00°C (73.40°F)
...
Temperature data is displayed and charted live in the IoT Serial Monitoring App.

Usage
Flash the STTS751 firmware onto the nrf52832 dev board.
After startup, temperature readings will be logged and visualized in the app and can be viewed via UART/serial console.
All intervals, COM port, and settings can be modified within the IoT Serial Monitoring App for tailored data logging.

Troubleshooting
Sensor Not Detected:
Confirm all wiring and physical connections.
Ensure correct baud rate, COM port, and I2C address.
Abnormal Readings:
Check sensor placement, initialization steps, and firmware configuration settings.

License
This project is released under an open-source license. Please refer to the LICENSE file for details.
