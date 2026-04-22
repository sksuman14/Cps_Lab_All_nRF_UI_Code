#include "ui.h"
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/reboot.h>
#include <zephyr/drivers/uart.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/* --- UART RX Configuration --- */
#define RX_BUF_SIZE     128 // DMA transfer block size
#define RX_TIMEOUT      25  // Hardware timeout in ms to trigger RX_RDY if buffer isn't filled
#define CMD_BUF_SIZE    256 // Application-level buffer for payload aggregation

/* --- Double-Buffering (Ping-Pong) Architecture --- */
/* Utilizes two RX buffers to maintain continuous asynchronous reception. 
 * While the application processes buffer N, the UART peripheral/DMA 
 * writes into buffer N+1, mitigating hardware overruns during buffer swap. */
static uint8_t rx_buf[2][RX_BUF_SIZE];
static int rx_buf_idx = 0; // Active buffer index for peripheral RX

/* Application-level buffer for accumulating fragmented UART frames */
static char cmd_buf[CMD_BUF_SIZE];
static size_t cmd_len = 0;

/* --- Thread Synchronization Variables --- */
/* Volatile qualifiers suppress compiler optimization (e.g., register caching), 
 * ensuring that modifications made inside the UART callback context are 
 * immediately visible to the main application thread. */
static volatile bool interval_updated = false;
static volatile int interval_ms = 1000; 

/* Device tree binding for the console UART peripheral */
static const struct device *uart_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_console));

/**
 * @brief Parses the accumulated string in cmd_buf and executes system commands.
 * @note Invoked from the UART callback context upon CR/LF detection.
 */
static void process_commands(void) {
    if (cmd_len == 0) return;

    // Null-terminate the accumulated payload for safe string operations
    cmd_buf[cmd_len] = '\0';

    // 1. Handle system reboot vector
    if (strstr(cmd_buf, "restartDevice")) {
        printk("Rebooting...\n");
        k_msleep(50); // Flush TX buffer delay
        sys_reboot(SYS_REBOOT_COLD); // Trigger hardware reset
        return;
    }

    // 2. Handle interval configuration command
    char *p = strstr(cmd_buf, "SET_INTERVAL:");
    if (p) {
        p += strlen("SET_INTERVAL:"); // Advance pointer past the command header

        // Traverse pointer past any non-numeric characters (e.g., whitespace)
        while (*p != '\0' && !isdigit((unsigned char)*p)) {
            p++;
        }

        // Validate and convert ASCII numeric payload to integer
        if (isdigit((unsigned char)*p)) {
            int value = atoi(p); 
            
            if (value > 0) {
                interval_ms = value * 1000; // Convert sec to ms
                interval_updated = true;    // Set synchronization flag for main thread
                printk("\n Interval updated to %d seconds \n", value);
            }
        } else {
            printk("\nInvalid interval value. Please use format: SET_INTERVAL:10\n");
        }
    }

    // Buffer sanitization: Reset length index and clear memory for the next frame
    cmd_len = 0;
    memset(cmd_buf, 0, sizeof(cmd_buf));
}

/**
 * @brief Asynchronous UART event callback handler.
 * @param dev UART device instance.
 * @param evt Pointer to the UART event structure.
 * @param user_data Optional user context data (unused).
 */
static void uart_cb(const struct device *dev, struct uart_event *evt, void *user_data) {
    switch (evt->type) {
    
    case UART_RX_RDY: {
        /* Data chunk is available in the current RX buffer. 
         * Offset and length delineate the valid data boundaries. */
        const uint8_t *data = &evt->data.rx.buf[evt->data.rx.offset];
        
        for (size_t i = 0; i < evt->data.rx.len; i++) {
            
            // CR/LF detection indicates End-Of-Frame (EOF)
            if (data[i] == '\r' || data[i] == '\n') {
                if (cmd_len > 0) {
                    process_commands(); // Dispatch payload to parser
                }
            } 
            // Accumulate bytes, guarding against application buffer overrun
            else if (cmd_len < CMD_BUF_SIZE - 1) {
                cmd_buf[cmd_len++] = data[i];
            }
        }
        break;
    }

    case UART_RX_DISABLED:
        /* The peripheral has ceased reception (buffer full or manually disabled).
         * Execute the ping-pong buffer swap and re-arm the UART RX DMA. */
        rx_buf_idx = (rx_buf_idx + 1) % 2; // Toggle index (0 -> 1 -> 0)
        uart_rx_enable(dev, rx_buf[rx_buf_idx], RX_BUF_SIZE, RX_TIMEOUT); 
        break;

    default:
        break;
    }
}

/**
 * @brief Initializes the UART peripheral for asynchronous reception.
 * @retval 0 on success, negative error code on failure.
 */
int ui_init(void) {
    // Validate device tree binding initialization
    if (!device_is_ready(uart_dev)) return -1;
    
    // Register the asynchronous event handler
    uart_callback_set(uart_dev, uart_cb, NULL);
    
    // Zero-initialize buffers to prevent processing uninitialized memory artifacts
    memset(rx_buf, 0, sizeof(rx_buf));
    memset(cmd_buf, 0, sizeof(cmd_buf));
    
    // Prime the UART peripheral to begin reception into the primary buffer
    return uart_rx_enable(uart_dev, rx_buf[rx_buf_idx], RX_BUF_SIZE, RX_TIMEOUT);
}

/* --- Accessor Methods for Thread Synchronization --- */

/** @brief Retrieves the current polling interval limit. */
int ui_get_interval_ms(void) { return interval_ms; }

/** @brief Polling function to evaluate if an asynchronous event altered the interval. */
bool ui_interval_changed(void) { return interval_updated; }

/** @brief Clears the interval update flag post-processing by the main thread. */
void ui_clear_change_flag(void) { interval_updated = false; }