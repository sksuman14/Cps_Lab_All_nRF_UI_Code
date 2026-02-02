#include <zephyr/kernel.h>       // Zephyr kernel APIs (sleep, threads, etc.)
#include <zephyr/device.h>            // Device driver definitions
#include <zephyr/drivers/gpio.h>     // GPIO driver APIs
#include <zephyr/sys/printk.h>      // printk() for debug messages

#include "ui.h"                      // UI module for interval configuration

// #define SLEEP_TIME_MS 1000  // Adjust as needed

/* Get Hall Effect Sensor configuration from devicetree */
#define HALL_SENSOR_NODE DT_NODELABEL(hall_sensor_0) 

#if !DT_NODE_HAS_STATUS(HALL_SENSOR_NODE, okay)
#error "Unsupported board: hall_sensor devicetree alias is not defined"
#endif
// Get GPIO spec for Hall effect sensor (or default {0} if not defined)
static const struct gpio_dt_spec hall_sensor = GPIO_DT_SPEC_GET_OR(HALL_SENSOR_NODE, gpios, {0});
// GPIO callback structure for Hall effect sensor interrupts
static struct gpio_callback hall_sensor_cb;

/* Optional LED configuration */
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET_OR(DT_ALIAS(led0), gpios, {0});
static int magnet_detected = 0; // 0 = No magnet, 1 = Magnet detected

/* Interrupt Handler for Hall Sensor */
void hall_sensor_triggered(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    magnet_detected = gpio_pin_get_dt(&hall_sensor); // Read sensor state
    printk("Magnet %s detected\n", magnet_detected ? "NOT" : "IS");

    if (led.port) {
        gpio_pin_set_dt(&led, !magnet_detected); // Turn LED on when magnet is detected
    }
}

int main(void)
{
    ui_init();
    int ret;

    /* Check if Hall effect sensor is available */
    if (!gpio_is_ready_dt(&hall_sensor)) {
        printk("Error: Hall sensor device %s is not ready\n", hall_sensor.port->name);
        return 0;
    }

    /* Configure Hall effect sensor pin as input */
    ret = gpio_pin_configure_dt(&hall_sensor, GPIO_INPUT);
    if (ret != 0) {
        printk("Error %d: failed to configure %s pin %d\n", ret, hall_sensor.port->name, hall_sensor.pin);
        return 0;
    }

    /* Enable interrupt for Hall effect sensor */
    ret = gpio_pin_interrupt_configure_dt(&hall_sensor, GPIO_INT_EDGE_BOTH);
    if (ret != 0) {
        printk("Error %d: failed to configure interrupt on %s pin %d\n", ret, hall_sensor.port->name, hall_sensor.pin);
        return 0;
    }
    // GPIO callback structure for Hall effect sensor interrupts
    gpio_init_callback(&hall_sensor_cb, hall_sensor_triggered, BIT(hall_sensor.pin));
    gpio_add_callback(hall_sensor.port, &hall_sensor_cb);
    printk("Set up Hall effect sensor at %s pin %d\n", hall_sensor.port->name, hall_sensor.pin);

    /* Check if LED is available */
        // Check LED readiness
    if (led.port && !gpio_is_ready_dt(&led)) {
        printk("Error: LED device %s is not ready; ignoring it\n", led.port->name);
        return 0;
    }

    /* Configure LED as output if available */
    if (led.port) {
        ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT);
        if (ret != 0) {
            printk("Error %d: failed to configure LED at %s pin %d\n", ret, led.port->name, led.pin);
        }
    }

    while (1) {
           // Get interval from UI module
        int SLEEP_TIME_MS = ui_get_interval_ms();
        /* Print output every cycle */
        printk("Hall Sensor Output: %d\n", magnet_detected);
        k_msleep(SLEEP_TIME_MS);
    }

    return 0; 
}
