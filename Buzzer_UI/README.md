# IoT Lab Demonstration: Buzzer Control with Zephyr RTOS on nRF Board

This project is designed for an **IoT Laboratory** setting to demonstrate Buzzer using the **Zephyr RTOS** on an nRF52832 microcontroller. It features a dual-purpose architecture: Buzzer and a remote **User Interface (UI)** for dynamic system control via UART communication.

## Hardware Requirements

* **Microcontroller**: nRF52832 Development Kit (nRF52 DK).
* **Actuator**: Piezoelectric Buzzer.
* **Connection Interface**:
    * **GPIO**: For actuating the buzzer.
    * **UART**: For MCU-to-PC communication via the Zephyr console.

## Project Architecture

The application utilizes a modular design to separate the actuation logic from the user interface:

1 . **Hardware Layer**: Defined in the DeviceTree Overlay (`nrf52dk_nrf52832.overlay`), mapping the `led0` alias to a specific GPIO pin (P0.27) to drive the output.


2 . **UI Module (`ui.c`)**: Manages asynchronous serial communication. It buffers incoming UART data, parses commands, and updates global configuration variables.

3 . **Application Logic (`main.c`)**: Coordinates the initialization of the system and executes the main actuation loop based on user-defined intervals.


## Logic & Code Flow

### 1. Setup Phase

* The **User Interface** is initialized via `ui_init()`, setting up UART callbacks and enabling the receive buffer with a timeout.


* The system configures the `buzzer_PIN` (mapped from `led0`) as an output pin.


* A boot message is printed to the console indicating the default interval.

### 2. Operational Loop

The `main()` function enters an infinite loop that performs the following cycle:

* **Fetch Configuration**: Retrieves the current `delay_ms` value from the UI module using `ui_get_interval_ms()`.


* **Actuation (ON)**: Sets the buzzer pin high, prints "Buzzer ACTIVE", and sleeps for `delay_ms`.


* **Actuation (OFF)**: Clears the buzzer pin, prints "Buzzer INACTIVE", and sleeps for `delay_ms`.


* **Interval Wait**: The system performs an additional wait (`k_msleep`) for the duration of `delay_ms` before restarting the cycle.



### 3. Asynchronous Control

* **UART Interrupts**: The `uart_cb` function listens for incoming data. When valid commands are detected in the buffer (e.g., `SET_INTERVAL:`), the system updates the `interval_ms` variable immediately, affecting the next cycle of the main loop.

## Features

* **Remote Actuation Control**: Toggles a GPIO pin (Buzzer) with a configurable duty cycle.
* **Live Interval Adjustment**: Use the **User Interface** to change the WAIT duration on-the-fly via UART.
* **Remote System Reset**: Capability to perform a "Cold Reboot" of the microcontroller via the `restartDevice` command.
* **Status Logging**: Real-time printouts of the buzzer state ("ACTIVE" / "INACTIVE") to the console.



## Communication Protocols

* **GPIO**: Controls the physical pin state (High/Low) via the Nordic GPIO HAL and Zephyr drivers.


* **UART**: Provides the physical layer for the **User Interface** console, utilizing an event-driven callback mechanism (`uart_cb`) to handle RX events without blocking the main loop.

## Project Structure

* `main.c`: Contains the main execution loop, GPIO configuration, and actuation logic.


* `ui.c`: Implements the UART-based User Interface, command parsing (using `strstr`), and system reboot logic.
* `ui.h`: Header file exposing the public API (`ui_init` and `ui_get_interval_ms`).
* `nrf52dk_nrf52832.overlay`: Configures the hardware pin mapping, assigning `gpio0 27` to the `led0` node.



## Setup and Build

### 1. Hardware Setup

Connect your Buzzer (or observe the onboard LED) on the nRF52 DK:

* **Signal Pin**: Connect to **P0.27** (Defined as `led0` in the overlay).


* **Ground**: Connect to a GND pin on the board.
* *Note*: Ensure you check the GPIO pin assignments suitable for your specific board version.



### 2. Build and Flash

Use the standard Zephyr `west` toolchain to build and flash the firmware:

```bash
west build -b nrf52dk_nrf52832
west flash

```

## IoT Lab Usage & UI Controls

Connect to the device using the **graphical user interface (GUI)** or a serial terminal (e.g., nrf Connect). Select the appropriate **Serial Port** and **baud rate (115200)**.

### Changing the Actuation Interval

To demonstrate how variable timing affects the actuator rhythm:

* **Method (UI-Based Configuration):** Send the command string `SET_INTERVAL: <seconds>` (or enter interval time and then  click the **Set Interval** button in the GUI).

* **Example:** Sending `SET_INTERVAL: 2` updates the cycle to 2 seconds ON, 2 seconds OFF, and 2 seconds Wait.

### Restarting the Device

To demonstrate remote device management:

* **Method (UI-Based Control):** Send the command `restartDevice` (or click the **Restart Device** button in the GUI).

* **Action:** The system logs "Rebooting...", waits 50ms, and triggers a `sys_reboot(SYS_REBOOT_COLD)`.

---

## Limitations & Assumptions

* **Pin Usage**: The code re-purposes the `led0` node. If using the onboard LED, it corresponds to "Green LED 0" (P0.27).

* **Buffer Size**: The UART command buffer (`cmd_buf`) is limited to 256 bytes; commands longer than this may be truncated or ignored.
* **Blocking Sleep**: The `main.c` loop uses `k_sleep`. While this yields the CPU to other threads, the buzzer timing is strictly sequential and relies on the loop completing.

