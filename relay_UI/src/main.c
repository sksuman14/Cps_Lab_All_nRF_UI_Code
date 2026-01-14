
#include <zephyr/drivers/gpio.h>
#include <zephyr/device.h>
#include <hal/nrf_gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include "ui.h"

void main(void)
{
    int delayTime = 1000;

    ui_init();// Default LED toggle delay in milliseconds
    
 // Print boot message along with the default interval obtained from UI
    printk("Boot complete. Default interval = %d ms\n", ui_get_interval_ms());

    /* Configure LED pin once */
    nrf_gpio_cfg_output(DT_GPIO_PIN(DT_NODELABEL(led0), gpios)); // Set LED GPIO pin HIGH

    while (1)
    {
        // Fetch the current delay value set via UART UI
        delayTime = ui_get_interval_ms();

        /* LED ON */
        nrf_gpio_pin_set(DT_GPIO_PIN(DT_NODELABEL(led0), gpios));
        printk("LED ON  (delay = %d ms)\n", delayTime); // Keep LED ON for the specified delay
        k_msleep(delayTime);

        /* LED OFF */
        nrf_gpio_pin_clear(DT_GPIO_PIN(DT_NODELABEL(led0), gpios));// Set LED GPIO pin LOW
        printk("LED OFF (delay = %d ms)\n", delayTime);  // Log LED OFF state
        k_msleep(delayTime);    // Keep LED OFF for the specified delay
    }
}
