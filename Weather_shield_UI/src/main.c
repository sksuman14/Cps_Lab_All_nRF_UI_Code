#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <stdio.h>
#include "ui.h"

/* Delay between samples (ms) */
// #define DELAY_TIME_MS 1000

/* VEML7700 label from DeviceTree */
#define VEML7700_LABEL "VEML7700"
double t = 0, h = 0, p = 0, l = 0;

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
{
    delayTime = ui_get_interval_ms();

    /* ----------- BME680 READ ----------- */
    if (sensor_sample_fetch(bme) == 0)
    {
        sensor_channel_get(bme, SENSOR_CHAN_AMBIENT_TEMP, &temp);
        sensor_channel_get(bme, SENSOR_CHAN_HUMIDITY, &hum);
        sensor_channel_get(bme, SENSOR_CHAN_PRESS, &press);

        t = sensor_value_to_double(&temp);
        h = sensor_value_to_double(&hum);
        p = sensor_value_to_double(&press) * 10; // hPa
    }
    else
    {
        printf("Error: BME680 fetch failed\n");
        continue;
    }

    /* ----------- VEML7700 READ ----------- */
    if (sensor_sample_fetch(veml) == 0 &&
        sensor_channel_get(veml, SENSOR_CHAN_LIGHT, &lux) == 0)
    {
        l = sensor_value_to_double(&lux);
    }
    else
    {
        printf("Error: VEML7700 fetch failed\n");
        continue;
    }

    
    printf("T:%.2f,H:%.2f,P:%.2f,L:%.2f\n", t, h, p, l);

    k_msleep(delayTime);
}
}