#include <zephyr/kernel.h> / //Zephyr kernel APIs (sleep, timing, threads)
#include <zephyr/device.h>   // Device driver model APIs
#include <zephyr/drivers/sensor.h>   // Zephyr sensor framework APIs
#include <stdio.h>                  // Standard C library for printf()
#include "ui.h"                        // Custom UART-based user interface functions

void main(void)
{
  int delayTime = 1000; // Default delay between sensor readings (in milliseconds)
  ui_init();             // Initialize UART UI for user interaction
// Get the SHT4x sensor device using Zephyr's device tree matching
  const struct device *sht = DEVICE_DT_GET_ANY(sensirion_sht4x);
    // Check if the sensor device is ready for use
  if (!device_is_ready(sht))
  {
    printf("Device %s is not ready\n", sht->name);// Print error if sensor is not ready
  }

  struct sensor_value temp, hum; // Structures to store temperature and humidity values

  while (1)
  {
    
    // Get the sampling interval dynamically from the UART UI
    delayTime = ui_get_interval_ms();
    
    // Trigger a new measurement on the SHT40 sensor
    if (sensor_sample_fetch(sht) == 0)
    {
        // Read ambient temperature data from the sensor
      sensor_channel_get(sht, SENSOR_CHAN_AMBIENT_TEMP, &temp);
       // Read relative humidity data from the sensor
      sensor_channel_get(sht, SENSOR_CHAN_HUMIDITY, &hum);

      // Convert sensor values to double and print formatted output
      printf("SHT40:Temperature:%.2f°C, Humidity:%.2f%%\n",
             sensor_value_to_double(&temp),
             sensor_value_to_double(&hum));
    }
    else
    { 
        // Print error message if sensor data fetch fails
      printf("Error: Failed to fetch sensor data\n");
    }

    k_msleep(delayTime);  // Sleep for the specified interval before next reading
  }
}
