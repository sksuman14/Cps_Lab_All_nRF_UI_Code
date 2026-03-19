#include <zephyr/kernel.h>                 // Includes core Zephyr kernel APIs (threads, sleep, timing)
#include <zephyr/device.h>                 // Provides device driver model and device access functions
#include <zephyr/drivers/sensor.h>         // Enables use of sensor drivers and sensor APIs
#include <zephyr/sys/printk.h>             // Native Zephyr logging (replaced stdio.h)
#include "ui.h"                            // Custom UI header for user interaction functions

int main(void)
{
    if (ui_init() != 0) return -1;        // Initialize the user interface 

    const struct device *bme = DEVICE_DT_GET_ANY(bosch_bme680);

    // Check if the sensor device is ready for use
    if (!device_is_ready(bme))
    {
        printk("Device %s is not ready\n", bme->name); // Print error if device is not ready
        return -1;                                      // Exit the program
    }

    struct sensor_value temp, hum, press; // Removed gas_res as it was unused in your printk

    while (1)
    {
        // Fetch the latest sensor data from the BME680
        if (sensor_sample_fetch(bme) == 0)
        {
            // Read individual sensor channels
            sensor_channel_get(bme, SENSOR_CHAN_AMBIENT_TEMP, &temp);
            sensor_channel_get(bme, SENSOR_CHAN_HUMIDITY, &hum);
            sensor_channel_get(bme, SENSOR_CHAN_PRESS, &press);

            // Print sensor values after converting them to double
            printk("Temperature: %.2f C, Humidity: %.2f %%RH, Pressure: %.2f kPa\n",
       sensor_value_to_double(&temp),
       sensor_value_to_double(&hum),
       sensor_value_to_double(&press) * 10);
        }
        else
        {
             // Print error message if sensor data fetch fails
            printk("Error: Failed to fetch BME680 data\n");
        }
        
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
    
    return 0;
}