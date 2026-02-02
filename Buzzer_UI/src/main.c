#include <zephyr/drivers/gpio.h> // GPIO driver
#include <zephyr/device.h>		 // Device driver
#include <hal/nrf_gpio.h>		 // Nordic GPIO HAL

#define buzzer_PIN DT_GPIO_PIN(DT_NODELABEL(led0), gpios) // Define buzzer pin (LED0)
// #define SLEEP_TIME_MS 100								 // Define sleep time (1 second) 


void main(void)
{ 

	// Initialize the UI module 
	ui_init();
	
    // Print boot message with default interval value
	printk("Boot complete. Default interval = %d ms\n", ui_get_interval_ms());

	nrf_gpio_cfg_output(buzzer_PIN); // Configure buzzer pin as output

	while (1)

	{	 // Get the user-defined interval from UI module
		int delay_ms = ui_get_interval_ms(); // UI interval
		nrf_gpio_pin_set(buzzer_PIN);	// Turn the buzzer ON
		printk("Buzzer ACTIVE\n");   // Short delay for buzzer ON time
		k_sleep(K_MSEC(delay_ms)); // Delay for specified time

		nrf_gpio_pin_clear(buzzer_PIN);	
		printk("Buzzer INACTIVE\n");// Turn buzzer off
		k_sleep(K_MSEC(delay_ms)); // Delay for specified time

		 // Delay for user-defined interval before next toggle
		k_msleep(delay_ms);
	}
}