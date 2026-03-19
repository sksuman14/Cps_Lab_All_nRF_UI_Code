
# LED Blinker: IoT Lab Demonstration

This project is designed for an **IoT Laboratory** setting to demonstrate how to control an **LED** using the **Zephyr RTOS**. It features a simple loop that turns a light on and off, with a remote **User Interface (UI)** to change the speed via UART.

## Hardware Requirements

* **Microcontroller**: nRF52832 Development Kit.
* **Component**: Onboard LED (Green LED 0).
* **Connection Interface**:
    * **GPIO**: For turning the LED pin High or Low.
    * **UART**: For sending commands from the PC to the device.



## Project Architecture

The application uses a modular design to separate the blinking code from the user settings:

1 . **Hardware Layer**: Defined in the DeviceTree Overlay, mapping the `led0` label to pin `17` (P0.17).


2 . **UI Module (`ui.c`)**: Manages serial communication. It reads incoming text commands to update the blinking speed or restart the board.

3 . **Application Logic (`main.c`)**: Sets up the LED and runs the continuous loop to toggle the light.



## Logic & Code Flow

### 1. Setup Phase

* The **User Interface** is initialized to start listening for commands.


* The system configures the LED pin as an **output** so it can be controlled by the code.


### 2. Operational Loop

* **Check Speed**: At the start of every loop, the code calls `ui_get_interval_ms()` to see if the user changed the delay time.


* **Turn LED Off**: The system sets the pin "Low". This turns the light **OFF**.


* **Wait**: The system pauses for the duration of `delayTime`.


* **Turn LED On**: The system set the pin (sets it "High"), which turns the light **ON**.


* **Wait**: The system pauses again before repeating the loop.



### 3. Asynchronous Control

* **UART Interrupts**: The system uses a "callback" to process commands like `SET_INTERVAL` instantly, even while the main loop is waiting.

## Features

* **Blinking LED**: Continuously toggles the onboard light on and off.
* **Live Speed Adjustment**: Use the **User Interface** to change how fast the LED blinks.
* **Remote System Reset**: Capability to restart the microcontroller using a UI command.
* **Status Logging**: Prints "LED: ON" and "LED: OFF" to the screen so you can verify it is working.



## Communication Protocols

* **GPIO**: Directly controls the voltage on pin P0.17.


* **UART**: Used for the **User Interface** console to send and receive text.

## Project Structure

* `main.c`: Contains the main loop that turns the LED on and off.

* `ui.c`: Handles the text commands from the computer.
* `ui.h`: Header file that allows the main code to talk to the UI code.
* `nrf52dk_nrf52832.overlay`: Tells the system that the LED is connected to pin 17.



## Setup and Build

### 1. Hardware Setup

The code is configured for the **nRF52 DK** onboard LED:

* **LED Pin**: P0.17 (Internal Green LED 0).


* **Wiring**: No extra wires are needed; the LED is built into the board.

### 2. Build and Flash

```bash
west build -b nrf52dk_nrf52832
west flash

```

## IoT Lab Usage & UI Controls

Connect to the device using the **graphical user interface (GUI)**.
Select the appropriate **Serial Port** and **baud rate (115200)** from the UI and click **Connect** to establish communication with the device.

**Changing the Sampling Interval**

To demonstrate how the blinking speed changes, configure the sampling interval using the UI:

* **Method (UI-Based Configuration):**
  This application is **UI-based**. Instead of sending commands manually, enter the desired value in the **Interval (seconds)** field and click **Set Interval**, as shown in the UI.

* **Example:**
  Set the interval to **5 seconds** using the UI.
  The device will now update and transmit data every **5 seconds**.


**Restarting the Device**

To demonstrate a remote reset::

* **Method (UI-Based Control):**
  Use the **Restart Device** button available in the UI.

* **Action:**
  The UI will perform a **cold system reboot** on the device and log the operation in the **UART Logs** section.
---

## Limitations & Assumptions

* **Buffer Limit**: The command text cannot be longer than 256 characters.
* **Blocking Wait**: The loop uses `k_msleep`, so the processor stops doing other work while waiting for the LED to toggle.