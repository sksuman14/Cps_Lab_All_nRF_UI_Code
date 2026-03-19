#include "ui.h"
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/reboot.h>
#include <zephyr/drivers/uart.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define RX_BUF_SIZE     64
#define RX_TIMEOUT      25 
#define CMD_BUF_SIZE    256

static uint8_t rx_buf[RX_BUF_SIZE];
static char cmd_buf[CMD_BUF_SIZE];
static size_t cmd_len = 0;
static bool interval_updated = false;
static int interval_ms = 1000;

static const struct device *uart_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_console));

static void process_commands(void) {
    if (cmd_len == 0) return;

    // Ensure the string is null-terminated for string functions
    cmd_buf[cmd_len] = '\0';

    // 1. Check for Restart
    if (strstr(cmd_buf, "restartDevice")) {
        printk("Rebooting...\n");
        k_msleep(50);
        sys_reboot(SYS_REBOOT_COLD);
        return;
    }

    // 2. Check for Set Interval
    char *p = strstr(cmd_buf, "SET_INTERVAL:");
    if (p) {
        p += strlen("SET_INTERVAL:");

        // Skip non-digits (spaces, colons, etc.)
        while (*p != '\0' && !isdigit((unsigned char)*p)) {
            p++;
        }

        if (isdigit((unsigned char)*p)) {
            int value = atoi(p); // atoi reads all digits until a non-digit appears
            if (value > 0) {
                interval_ms = value * 1000;
                interval_updated = true;
                printk("Interval updated to %d seconds\n", value, interval_ms);
            }
        } else {
            printk("Invalid interval value\n");
        }
    }

    // Reset buffer for the next command
    cmd_len = 0;
    memset(cmd_buf, 0, sizeof(cmd_buf));
}

// --- UART Callback Logic ---
static void uart_cb(const struct device *dev, struct uart_event *evt, void *user_data) {
    switch (evt->type) {
    
    case UART_RX_RDY: {
        const uint8_t *data = &evt->data.rx.buf[evt->data.rx.offset];
        for (size_t i = 0; i < evt->data.rx.len; i++) {
            
            // If it's a newline, process IMMEDIATELY
            if (data[i] == '\r' || data[i] == '\n') {
                if (cmd_len > 0) {
                    process_commands(); 
                }
            } 
            // Add characters to buffer
            else if (cmd_len < CMD_BUF_SIZE - 1) {
                cmd_buf[cmd_len++] = data[i];
            }
        }
        
        /* * FIX FOR THE DELAY: 
         * If the user stops typing but didn't hit enter, 
         * we can choose to process after a small amount of data is received
         * to avoid the 10-second wait.
         */
        break;
    }

    case UART_RX_DISABLED:
        // Simply re-enable the receiver. 
        // We now wait strictly for the newline (\r or \n) in UART_RX_RDY to process commands.
        uart_rx_enable(dev, rx_buf, sizeof(rx_buf), 20); 
        break;

    default:
        break;
    }
}

int ui_init(void) {
    if (!device_is_ready(uart_dev)) return -1;
    uart_callback_set(uart_dev, uart_cb, NULL);
    return uart_rx_enable(uart_dev, rx_buf, sizeof(rx_buf), RX_TIMEOUT);
}

int ui_get_interval_ms(void) { return interval_ms; }
bool ui_interval_changed(void) { return interval_updated; }
void ui_clear_change_flag(void) { interval_updated = false; }

