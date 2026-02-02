#include <zephyr/kernel.h> // Include the Zephyr kernel API
#include <hal/nrf_gpio.h>  // Include the Nordic HAL (Hardware Abstraction Layer) for direct GPIO control

void main(void) // Main function
{
    int delayTime = 1000;
    ui_init();

    nrf_gpio_cfg_output(DT_GPIO_PIN(DT_NODELABEL(led0), gpios)); // Configure the GPIO pin connected to the LED as an output
    while (1)                                                    // Infinite loop to continuously run the code
    {
        delayTime = ui_get_interval_ms();
        nrf_gpio_pin_set(DT_GPIO_PIN(DT_NODELABEL(led0), gpios));   // Set the GPIO pin high, turning the LED on
        printk("LED: OFF\n");
        k_msleep(delayTime);                                        // Sleep for 100 milliseconds (LED stays on)
        nrf_gpio_pin_clear(DT_GPIO_PIN(DT_NODELABEL(led0), gpios)); // Clear the GPIO pin (set it low), turning the LED off
        printk("LED: ON\n");
        k_msleep(delayTime);                                        // Sleep for 100 milliseconds (LED stays off)
    }
}
