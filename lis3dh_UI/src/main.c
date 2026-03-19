#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/sys/printk.h>       // Replaced stdio.h with native Zephyr logging

#include "ui.h"

// Fetch and display accelerometer data
static void fetch_and_display(const struct device *sensor)   // Function to read and print accelerometer values
{
    static unsigned int count = 0;   // Sample counter to count number of readings
    struct sensor_value accel[3];    // Array to store X, Y, and Z acceleration values
    uint32_t upTime = k_uptime_get_32(); // Get current system uptime in milliseconds

    if (sensor_sample_fetch(sensor) < 0)   // Fetch new data sample from the accelerometer
    { 
        printk("ERROR: Sample fetch failed\n");   // Print error if fetching fails
        return;                                   // Exit function if fetch fails
    }

    if (sensor_channel_get(sensor, SENSOR_CHAN_ACCEL_XYZ, accel) < 0)   // Retrieve XYZ acceleration values from sensor
    { 
        printk("ERROR: Channel get failed\n");     // Print error if data retrieval fails
        return;                                    // Exit function if channel read fails
    }

    // Display sample count, uptime, and XYZ acceleration values
    printk("x %.3f, y %.3f, z %.3f\n",     
           sensor_value_to_double(&accel[0]),    
           sensor_value_to_double(&accel[1]),    
           sensor_value_to_double(&accel[2]));   
}

int main(void)                                    
{
    if (ui_init() != 0) return -1;

    // Get sensor device binding.
    const struct device *sensor = DEVICE_DT_GET_ANY(st_lis2dh);   

    if (sensor == NULL) {                        
        printk("Could not get %s device\n", "st_lis3dh");   
        return -1;                                  
    }

    if (!device_is_ready(sensor))                 
    { 
        printk("Device %s is not ready\n", sensor->name);   
        return -1;                                  
    }

    while (1)                                    
    {                                   
        // Call function to fetch and print accelerometer data
        fetch_and_display(sensor);                
        
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