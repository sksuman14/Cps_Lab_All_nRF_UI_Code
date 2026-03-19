#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/devicetree.h>
#include <hal/nrf_gpio.h>
#include "ui.h"

// Defined a macro to make your GPIO calls a bit cleaner to read
#define RELAY_PIN DT_GPIO_PIN(DT_NODELABEL(led0), gpios)

int main(void)
{
    int delayTime = 1000;

    if (ui_init() != 0) return -1; 
    
    // Print boot message along with the default interval obtained from UI
    printk("Boot complete. Default interval = %d ms\n", ui_get_interval_ms());

    /* Configure LED pin once */
    nrf_gpio_cfg_output(RELAY_PIN); 

    while (1)
    {
        // Fetch the current delay value set via UART UI
        delayTime = ui_get_interval_ms();
        int elapsed_ms = 0;
        bool ui_interrupted = false;

        /* Relay ON */
        nrf_gpio_pin_set(RELAY_PIN);
        printk("Relay ON  (delay = %d ms)\n", delayTime); 
        
        // Polling loop for the ON phase
        while (elapsed_ms < delayTime) {
            if (ui_interval_changed()) {
                ui_clear_change_flag();
                ui_interrupted = true;
                break;
            }
            k_msleep(100);
            elapsed_ms += 100;
        }

        // Instantly restart the cycle if the user changes the speed via UART
        if (ui_interrupted) continue;

        /* Relay OFF */
        nrf_gpio_pin_clear(RELAY_PIN);
        printk("Relay OFF (delay = %d ms)\n", delayTime);  
        
        elapsed_ms = 0; // Reset counter for the OFF phase

        // Polling loop for the OFF phase
        while (elapsed_ms < delayTime) {
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