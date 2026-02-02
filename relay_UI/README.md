# Relay Control : IoT Lab Demonstration

This project is designed for an **IoT Laboratory** setting to demonstrate how to control a high-power actuator, such as a **Relay**, using the **Zephyr RTOS**. It features an automated loop that switches the relay on and off, along with a remote **User Interface (UI)** to change the timing.

## Hardware Requirements

* **Microcontroller**: nRF52832 Development Kit.
* **Actuator**: Relay Module (connected to a GPIO pin).
* **Connection Interface**:
    * **GPIO**: For triggering the relay switch.
    * **UART**: For sending commands from the PC to the device.



## Project Architecture

The application uses a modular design to separate the switching logic from the user settings:

1 . **Hardware Layer**: Defined in the DeviceTree Overlay, mapping the `led0` label to pin **27** (P0.27).

2 . **UI Module (`ui.c`)**: Manages serial communication. It reads incoming text commands to update the switching speed or restart the board.

3 . **Application Logic (`main.c`)**: Configures the GPIO pin and runs the continuous loop to toggle the relay.

## Logic & Code Flow

### 1. Setup Phase

* The **User Interface** is initialized to start listening for commands.
* The system configures the control pin (connected to the Relay) as an **output**.
* A boot message prints the default interval to the screen.

### 2. Operational Loop

* **Check Speed**: At the start of every loop, the code calls `ui_get_interval_ms()` to see if the user has requested a new delay time.
* **Relay ON**: The system sets the pin "High". The console prints "Relay ON".
* **Wait**: The system pauses for the duration of the current `delayTime`.
* **Relay OFF**: The system clears the pin (sets it "Low"). The console prints "Relay OFF".
* **Wait**: The system pauses again before repeating the loop.

### 3. Asynchronous Control

* **UART Interrupts**: The system uses a background process (callback) to handle commands like `SET_INTERVAL` instantly, even while the main loop is waiting.

## Features

* **Relay Switching**: Continuously toggles a GPIO pin to control an external relay module.
* **Live Interval Adjustment**: Use the **User Interface** to change how long the relay stays ON or OFF.
* **Remote System Reset**: Capability to restart the microcontroller using a UI command.
* **Status Logging**: Prints "Relay ON" and "Relay OFF" to the screen so you can verify the timing.

## Communication Protocols

* **GPIO**: Directly controls the voltage on pin P0.27 (Logic High/Low).
* **UART**: Used for the **User Interface** console to receive user commands.

## Project Structure

* `main.c`: Contains the main loop that toggles the relay pin.
* `ui.c`: Handles the text commands (`restartDevice`, `SET_INTERVAL`) from the computer.
* `ui.h`: Header file that allows the main code to talk to the UI code.
* `nrf52dk_nrf52832.overlay`: Configures the hardware pin mapping (Pin 27).

## Setup and Build

### 1. Hardware Setup

Connect your Relay Module to the nRF52 DK:

* **VCC/GND**: Connect to board power (usually 5V or 3.3V depending on your relay) and Ground.
* **Signal Pin**: Connect to pin **P0.27** (Defined as `led0` in the overlay).

### 2. Build and Flash

```bash
west build -b nrf52dk_nrf52832
west flash

```

## IoT Lab Usage & UI Controls

Connect to the device using the **graphical user interface (GUI)**.
Select the appropriate **Serial Port** and **baud rate (115200)** from the UI and click **Connect** to establish communication with the device.

**Changing the Switching Speed**

To demonstrate how the relay timing changes:

* **Method (UI-Based Configuration):**
  This application is **UI-based**. Instead of sending commands manually, enter the desired value in the **Interval (seconds)** field and click **Set Interval**, as shown in the UI.

* **Example:**
  Set the interval to **5 seconds** using the UI.
  The relay will now switch state every **5 seconds**.


**Restarting the Device**

To demonstrate remote device management and system recovery:

* **Method (UI-Based Control):**
  Use the **Restart Device** button available in the UI.

* **Action:**
  The UI will perform a **cold system reboot** on the device and log the operation in the **UART Logs** section.


## Limitations & Assumptions

* **Pin Usage**: The code uses pin 27 (`&gpio0 27`), which is typically an LED on the nRF52 DK. If connecting an external relay, ensure you use the correct physical pin.
* **Buffer Limit**: The command text cannot be longer than 256 characters.
* **Blocking Wait**: The loop uses `k_msleep`, so the processor stops doing other work while waiting for the relay to toggle.