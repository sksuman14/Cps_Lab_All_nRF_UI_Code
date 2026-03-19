#include <zephyr/kernel.h>  
#include <zephyr/device.h>  
#include <zephyr/drivers/sensor.h>  
#include <zephyr/sys/printk.h>      // Exclusive use of native Zephyr logging
#include <zephyr/drivers/i2c.h>  
#include "ui.h"

int main(void)
{
    // Initialize the user interface
    if (ui_init() != 0) return -1;

    // Declare and initialize a variable to hold the I2C device
    const struct device *const dev = DEVICE_DT_GET_ANY(st_vl53l0x);

    // Declare variables for sensor readings and return values
    struct sensor_value value;
    int ret;

    // Check if the device is ready
    if (!device_is_ready(dev)) {
        // Print a message if the device is not ready and return
        printk("sensor: device not ready.\n");
        return 0;
    }

    // Infinite loop for continuous sensor reading
    while (1) {
        // Fetch sensor samples from the device
        ret = sensor_sample_fetch(dev);
        
        // Check if fetching sensor samples failed
        if (ret) {
            // Print an error message if fetching samples failed and return
            printk("sensor_sample_fetch failed ret %d\n", ret);
            return 0;
        }

        // Get distance sensor data from the device
        ret = sensor_channel_get(dev, SENSOR_CHAN_DISTANCE, &value);

        // Convert meters to centimeters
        double distance_cm = sensor_value_to_double(&value) * 100.0;
        
        // Print the distance sensor value in centimeters
        printk("distance is %.3fcm\n", distance_cm);

        // Get the updated delay interval from the UI
        int target_ms = ui_get_interval_ms();
        int elapsed_ms = 0;

        // Check for updates every 100ms while waiting for next reading
        while (elapsed_ms < target_ms) {
            if (ui_interval_changed()) {
                ui_clear_change_flag();
                break; // Restart loop with new interval immediately
            }
            k_msleep(100);
            elapsed_ms += 100;
        }
    }

    // End of main function
    return 0;
}