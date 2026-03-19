#include <zephyr/kernel.h>       // Core kernel APIs (k_msleep)
#include <zephyr/drivers/gpio.h> // GPIO driver
#include <zephyr/device.h>       // Device driver
#include <zephyr/devicetree.h>   // Devicetree macros
#include <hal/nrf_gpio.h>        // Nordic GPIO HAL
#include <zephyr/sys/printk.h>   // Kernel logging (printk)
#include "ui.h"                  // Custom UI module

#define buzzer_PIN DT_GPIO_PIN(DT_NODELABEL(led0), gpios) // Define buzzer pin (LED0)

int main(void) // Updated to Zephyr standard int main(void)
{ 
    // Initialize the UI module 
    if (ui_init() != 0) return -1;
    
    // Print boot message with default interval value
    printk("Boot complete. Default interval = %d ms\n", ui_get_interval_ms());

    nrf_gpio_cfg_output(buzzer_PIN); // Configure buzzer pin as output

    while (1)
    {
        // Get the user-defined interval from UI module
        int target_ms = ui_get_interval_ms(); 
        int elapsed_ms = 0;
        bool ui_interrupted = false;

        /* --- BUZZER ON PHASE --- */
        nrf_gpio_pin_set(buzzer_PIN);    // Turn the buzzer ON
        printk("Buzzer ACTIVE\n");       
        
        // 100ms polling loop for the ON phase
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

        /* --- BUZZER OFF PHASE --- */
        nrf_gpio_pin_clear(buzzer_PIN);  // Turn buzzer off
        printk("Buzzer INACTIVE\n");
        
        elapsed_ms = 0; // Reset counter for the OFF phase
        
        // 100ms polling loop for the OFF phase
        while (elapsed_ms < target_ms) {
            if (ui_interval_changed()) {
                ui_clear_change_flag();
                ui_interrupted = true;
                break; // We don't need `continue` here because the loop naturally restarts
            }
            k_msleep(100);
            elapsed_ms += 100;
        }
    }
    
    return 0;
}