#include <zephyr/kernel.h>        // Include the Zephyr kernel API
#include <zephyr/devicetree.h>    // Required for DT_NODELABEL and DT_GPIO_PIN macros
#include <zephyr/sys/printk.h>    // Native Zephyr logging
#include <hal/nrf_gpio.h>         // Include the Nordic HAL for direct GPIO control
#include "ui.h"                   // Custom UI module

// Define the pin macro for cleaner code
#define LED_PIN DT_GPIO_PIN(DT_NODELABEL(led0), gpios)

int main(void) // Updated to Zephyr standard int main(void)
{
    if (ui_init() != 0) return -1;

    // Configure the GPIO pin connected to the LED as an output
    nrf_gpio_cfg_output(LED_PIN); 
    
    while (1)                                                     
    {
        int target_ms = ui_get_interval_ms();
        int elapsed_ms = 0;
        bool ui_interrupted = false;

        /* --- PHASE 1: LED OFF --- */
        nrf_gpio_pin_set(LED_PIN);   // Set the GPIO pin high, turning the LED OFF
        printk("LED: OFF\n");
        
        // Polling loop for the OFF phase
        while (elapsed_ms < target_ms) {
            if (ui_interval_changed()) {
                ui_clear_change_flag();
                ui_interrupted = true;
                break;
            }
            k_msleep(100);
            elapsed_ms += 100;
        }

        // If user sent a UART command, instantly restart the cycle with the new speed
        if (ui_interrupted) continue; 

        /* --- PHASE 2: LED ON --- */
        nrf_gpio_pin_clear(LED_PIN); // Clear the GPIO pin (set it low), turning the LED ON
        printk("LED: ON\n");
        
        elapsed_ms = 0; // Reset counter for the ON phase
        
        // Polling loop for the ON phase
        while (elapsed_ms < target_ms) {
            if (ui_interval_changed()) {
                ui_clear_change_flag();
                break; // Restart loop with new interval immediately
            }
            k_msleep(100);
            elapsed_ms += 100;
        }
    }
    
    return 0;
}