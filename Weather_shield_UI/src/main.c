#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <stdio.h>
#include "ui.h"

/* Delay between samples (ms) */
// #define DELAY_TIME_MS 1000

/* VEML7700 label from DeviceTree */
#define VEML7700_LABEL "VEML7700"

void main(void)
{
    int delayTime = 1000;// Default sampling interval in milliseconds

    ui_init();           // Initialize UART-based UI for user input
    /* ---------------- BME680 DEVICE ---------------- */

    // Get BME680 sensor device instance using Devicetree compatibility
    const struct device *bme = DEVICE_DT_GET_ANY(bosch_bme680);

    // Check if BME680 device is ready for use
    if (!device_is_ready(bme))
    {
        printf("BME680  is not ready\n");
        return;
    }

    /* ---------------- VEML7700 DEVICE ---------------- */
    // Get VEML7700 sensor device using its Devicetree label
    const struct device *veml = device_get_binding(VEML7700_LABEL);

    // Check if device binding was successful
    if (!veml)
    {
        printf("Failed to get VEML7700 device binding\n");
        return;
    }
    // Check if VEML7700 device is ready for use
    if (!device_is_ready(veml))
    {
        printf("VEML7700 device not ready\n");
        return;
    }

    /* ---------------- Sensor Value Structures ---------------- */
    struct sensor_value temp, hum, press;   // Structures for BME680 sensor data
    struct sensor_value lux;                 // Structure for VEML7700 light intensity

    printf("BME680 + VEML7700 started successfully\n");

    while (1)
    {     // Get user-configured sampling interval from UART UI
        delayTime = ui_get_interval_ms();
        /* ----------- BME680 READ ----------- */
            // Trigger a new sensor measurement on BME680
        if (sensor_sample_fetch(bme) == 0)
        {
              // Read ambient temperature from BME680
            sensor_channel_get(bme, SENSOR_CHAN_AMBIENT_TEMP, &temp);
              // Read relative humidity from BME680
            sensor_channel_get(bme, SENSOR_CHAN_HUMIDITY, &hum);
              // Read atmospheric pressure from BME680

            sensor_channel_get(bme, SENSOR_CHAN_PRESS, &press);
                // Convert sensor values to human-readable format and print
            printf("BME680 -> Temp: %.2f C | Hum: %.2f %%RH | Pressure: %.2f hPa\n",
                   sensor_value_to_double(&temp),
                   sensor_value_to_double(&hum),
                   sensor_value_to_double(&press) * 10);
        }
        else
        {
            printf("Error: Failed to fetch BME680 data\n");
        }

        /* ----------- VEML7700 READ ----------- */
                // Trigger a new sensor measurement on VEML7700
        if (sensor_sample_fetch(veml) == 0)
        {
             // Read ambient light intensity (lux)
            if (sensor_channel_get(veml, SENSOR_CHAN_LIGHT, &lux) == 0)
            {
                // Print light intensity in lux
                printf("VEML7700 -> Light: %.2f lux\n",
                       sensor_value_to_double(&lux));
            }
            else
            {
                printf("Error: Failed to read VEML7700 channel\n"); // Fetch error handling
            }
        }
        else
        {    // Print separator for readability in logs
            printf("Error: Failed to fetch VEML7700 data\n");
        }

        printf("--------------------------------------------------\n");
            // Sleep for the configured interval before next sensor read
        k_msleep(delayTime);
    }
}

