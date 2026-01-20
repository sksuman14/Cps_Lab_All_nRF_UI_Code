#include <zephyr/kernel.h>                 // Includes core Zephyr kernel APIs (threads, sleep, timing)                                               // Provides device driver model and device access functions                                                // Enables use of sensor drivers and sensor APIs                                                   // Standard C library for printf()                                                   // Custom UI header for user interaction functions
#include <zephyr/device.h>                  // Provides device driver model and device access functions
#include <zephyr/drivers/sensor.h>          // Enables use of sensor drivers and sensor APIs
#include <stdio.h>                          // Standard C library for printf()
#include "ui.h"                              // Custom UI header for user interaction functions


void main(void)
{
    int delayTime= 1000; // Default delay time in milliseconds

    ui_init();            // Initialize the user interface

    const struct device *bme = DEVICE_DT_GET_ANY(bosch_bme680);

// Check if the sensor device is ready for use

    if (!device_is_ready(bme))
    {
        printf("Device %s is not ready\n", bme->name); // Print error if device is not ready
        return;                                       // Exit the program
       
    }

    struct sensor_value temp, hum, press, gas_res;

    while (1)

    {     // Get the updated delay interval from the UI
         delayTime = ui_get_interval_ms();

         // Fetch the latest sensor data from the BME680
        if (sensor_sample_fetch(bme) == 0)

        {     // Read individual sensor channels
            sensor_channel_get(bme, SENSOR_CHAN_AMBIENT_TEMP, &temp);
            sensor_channel_get(bme, SENSOR_CHAN_HUMIDITY, &hum);
            sensor_channel_get(bme, SENSOR_CHAN_PRESS, &press);


            // Print sensor values after converting them to double
            printf("Temperature: %.2f C, Humidity: %.2f %%RH, Pressure: %.2f kPa\n",
                   sensor_value_to_double(&temp),// Convert temperature to double
                   sensor_value_to_double(&hum),// Convert humidity to double
                   sensor_value_to_double(&press) * 10); // Convert pressure to kPa
        }
        else
        {
             // Print error message if sensor data fetch fails
            printf("Error: Failed to fetch BME680 data\n");
        }
          // Sleep for the specified delay time before next reading
        k_msleep(delayTime);
    }
}
