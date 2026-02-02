#include "ui.h"                 // Header file for UI-related function declarations
#include <zephyr/sys/printk.h>   // Provides printk() for kernel-level logging
#include <zephyr/sys/reboot.h>    // Enables system reboot functionality   
#include <zephyr/drivers/uart.h>    // UART driver APIs
#include <zephyr/device.h>            // Device driver model support
#include <zephyr/devicetree.h>         // Access to device tree macros  
#include <string.h>                      // String handling functions
#include <stdlib.h>                       // Standard library (atoi)  
#include <ctype.h>                         // Character handling (isdigit) 

#define RX_BUF_SIZE     64                  // Size of UART receive buffer
#define RX_TIMEOUT      100                    // UART receive timeout in milliseconds
#define CMD_BUF_SIZE    256                 // Buffer size for command processing

static uint8_t rx_buf[RX_BUF_SIZE];         // Buffer to store incoming UART bytes 
static char cmd_buf[CMD_BUF_SIZE];              // UART receive timeout in milliseconds
static size_t cmd_len = 0;                    // Buffer size for command processing  

static const struct device *uart_dev =
    DEVICE_DT_GET(DT_CHOSEN(zephyr_console));

static int interval_ms = 1000; /* default 1 second */   // Default sensor read interval (1 second)

/* -------------------------------------------------- */

static void process_commands(void)
{
    /* 1) Restart Device */
    char *p = strstr(cmd_buf, "restartDevice");    // Search for restart command 
    if (p) {
        printk("Restart command received\n");         // Log command reception
        printk("Rebooting...\n");                     // Notify reboot action  
        k_msleep(50);
        sys_reboot(SYS_REBOOT_COLD);
        return;                                      // Exit function  
    }

    /* 2) SET_INTERVAL */
    p = strstr(cmd_buf, "SET_INTERVAL:");     // Search for interval command
    if (p) {
        p += strlen("SET_INTERVAL:");         // Move pointer past command text

        /* skip spaces */
        while (*p == ' ')
            p++;

        /* wait until number is complete */
        if (!isdigit((unsigned char)*p))
            return;

        int value = atoi(p);         // Convert string to integer
        if (value > 0) {
            interval_ms = value * 1000;      // Convert seconds to milliseconds
            printk("Interval updated to %d seconds\n", value);
        } else {
            printk("Invalid interval value\n"); // Handle invalid input
        }

        /* remove processed command from buffer */
        cmd_len = 0;
        memset(cmd_buf, 0, sizeof(cmd_buf));
    }
}

/* -------------------------------------------------- */
// Function to return the current interval in milliseconds
int ui_get_interval_ms(void)
{
    return interval_ms;
}

/* -------------------------------------------------- */
// UART callback function triggered on UART events
static void uart_cb(const struct device *dev,
                    struct uart_event *evt,
                    void *user_data)
{
    ARG_UNUSED(user_data);   // Mark unused parameter

    if (evt->type == UART_RX_RDY) {   // Data received event
        const uint8_t *data =
            &evt->data.rx.buf[evt->data.rx.offset];// Pointer to received data
        size_t len = evt->data.rx.len; // Length of received data

        for (size_t i = 0; i < len; i++) {
            if (cmd_len < CMD_BUF_SIZE - 1) {
                cmd_buf[cmd_len++] = data[i];// Store byte in command buffer
                cmd_buf[cmd_len] = '\0'; // Maintain null-terminated string
            }
        }

        process_commands();                      // Process received command

    }
    else if (evt->type == UART_RX_DISABLED) {
        // Re-enable UART reception if RX is disabled
        uart_rx_enable(dev, rx_buf, sizeof(rx_buf), RX_TIMEOUT);
    }
}

/* -------------------------------------------------- */
// UI initialization function
int ui_init(void)
{   
        // Check if UART device is ready
    if (!device_is_ready(uart_dev)) {
        printk("UART device not ready\n");
        return -1;
    }
    // Register UART callback function
    int ret = uart_callback_set(uart_dev, uart_cb, NULL);
    if (ret) {
        printk("Failed to set UART callback\n");
        return ret;
    }
     // Enable UART receive with buffer and timeout
    ret = uart_rx_enable(uart_dev, rx_buf,
                         sizeof(rx_buf), RX_TIMEOUT);
    if (ret) {
        printk("Failed to enable UART RX\n");
        return ret;
    }
    return 0;   // UI initialization successful
}