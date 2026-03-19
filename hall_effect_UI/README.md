
# Hall Effect Sensor Monitor: IoT Lab Demonstration

This project is designed for an **IoT Laboratory** setting to demonstrate digital sensing using a **Hall Effect Sensor** and the **Zephyr RTOS**. It features a dual-purpose architecture: an interrupt-driven magnetic field detector and a remote **User Interface (UI)** for dynamic system control via UART.

## Hardware Requirements

* **Microcontroller**: nRF52832 Development Kit.
* **Sensor**: Digital Hall Effect Sensor (Magnetic Sensor).
* **Indicator**: Onboard LED (or external LED).
* **Connection Interface**:
    * **GPIO**: For reading the sensor and controlling the LED.
    * **UART**: For MCU-to-PC communication via the Zephyr console.



## Project Architecture

The application utilizes a modular design to separate sensing logic from the user interface:

 1 . **Hardware Layer**: Defined in the DeviceTree Overlay, establishing the `hall_sensor` node at GPIO pin `27`.

 2 . **UI Module (`ui.c`)**: Manages asynchronous serial communication. It handles a receive buffer and parses incoming strings to trigger system changes.

3 . **Application Logic (`main.c`)**: Coordinates the initialization of the UI and sensor interrupts, then enters a continuous status reporting loop.



## Logic & Code Flow

### 1. Setup Phase

* The **User Interface** is initialized, setting up UART callbacks and enabling reception.


* The system configures the Hall Sensor pin as an input and sets up an **interrupt** to detect changes on both edges (rising and falling).


* The LED is configured as an output to serve as a visual indicator.

### 2. Event-Driven Sensing (Interrupts)

* Unlike a standard loop, the sensor reading happens automatically. When a magnet approaches or leaves, the `hall_sensor_triggered` function runs immediately.
* **Action**: The system updates the global `magnet_detected` variable and toggles the LED status based on the sensor state.

### 3. Operational Loop

* **Dynamic Interval**: The loop calls `ui_get_interval_ms()` at the start of every cycle to check how often the user wants status updates.
* **Status Output**: The current state of the sensor (0 or 1) is printed to the console.
* **Sleep**: The system waits for the duration of the current interval before printing again.

### 4. Asynchronous Control

* UART interrupts allow the UI module to process commands like `SET_INTERVAL` or `restartDevice` in the background, without stopping the sensor loop.

## Features

* **Magnetic Detection**: Real-time detection of magnetic fields using hardware interrupts.
* **Visual Feedback**: The LED turns on automatically when a magnet is detected.
* **Live Interval Adjustment**: Use the **User Interface** to change how often the device reports its status to the PC.
* **Remote System Reset**: Capability to perform a "Cold Reboot" of the microcontroller via a UI command.

## Communication Protocols

* **GPIO**: Direct connection to the Hall Sensor and LED for low-latency control.


* **UART**: Provides the physical layer for the **User Interface** console, utilizing an event-driven callback mechanism (`uart_cb`).

## Project Structure

* `main.c`: Core application logic, interrupt setup, and the primary reporting loop.


* `ui.c`: Implementation of the UART-based User Interface, command parsing logic, and system reset triggers.
* `ui.h`: Header file defining the public UI functions like `ui_init()` and `ui_get_interval_ms()`.
* `nrf52dk_nrf52832.overlay`: Configures the GPIO hardware pin (P0.27) for the sensor.



## Setup and Build

### 1. Hardware Setup

Connect the Hall Effect Sensor to your nRF52 DK:

* **VCC/GND**: Connect to 3.3V and Ground.
* **Signal**: Connect the sensor output to pin **P0.27** (defined in the overlay).



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

## Limitations & Assumptions

* **Pin Conflict**: The code uses Pin 27 for the sensor. On some nRF52 boards, this pin might be connected to an onboard button or LED, which could cause interference if not isolated.


* **Active Low**: The overlay defines the sensor as `GPIO_ACTIVE_LOW`. The code logic assumes the signal goes LOW when a magnet is present.


* **Command Buffer**: The UI command buffer is limited to 256 characters.