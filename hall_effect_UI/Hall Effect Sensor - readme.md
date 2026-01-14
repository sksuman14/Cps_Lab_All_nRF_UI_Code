Hall Effect Sensor Interface – nrf52832 dev board
This repository contains firmware and setup instructions to detect magnetic field changes using a Hall Effect sensor and the nrf52832 dev board. The module provides digital output for magnetic presence and supports real-time state visualization in the IoT Serial Monitoring App.

Features

Detects magnetic field changes via Hall Effect sensor (digital state monitoring)
Reliable data logging on the nrf52832 dev board 
Real-time sensor state and analog output displayed in IoT Serial Monitoring App
Customizable logging intervals and UART state logging

Hardware Requirements

nrf52832 dev board 
Hall Effect Sensor (analog or digital output)
USB cable (for board-PC interfacing)

Software Requirements

IoT Serial Monitoring App (for live state display and charting)

Prerequisites

Familiarity with analog/digital signals, UART communication
Proper sensor and board wiring (see below)

Pin Configuration
Board Pins	Sensor Pins
3V3	VCC
ADCO	DO (digital output)
GND	GND

Setup & Configuration

Wire the Hall Effect sensor to the nrf52832 dev board as shown above.
Open IoT Serial Monitoring App, enter COM port, baud rate, sensor selection, and interval configuration.
Monitor magnetic field state and analog values in real time.

Sample Output
Connected to COM4 at 115200 baud
Hall Sensor State0, AO=2735
Hall Sensor State0, AO=2734
Hall Sensor State0, AO=2731
...

Hall Sensor Low Not Detected
Sensor digital state and analog readings are displayed and visualized in the IoT Serial Monitoring App.

Usage
Flash firmware to the nrf52832 dev board.
View state changes and analog output from the Hall Effect sensor in the serial console and app.
Change data interval, COM port, and settings in the app as needed.
Troubleshooting
Sensor Not Detected:
Confirm wiring (especially VCC, DO, GND) and configuration.
Verify correct COM port and baud rate.

State/Reading Errors:
Ensure correct power and magnetic field presence for Hall Effect detection.

License
Project is released under an open-source license. See LICENSE file for full terms.

