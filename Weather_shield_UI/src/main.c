#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/sys/printk.h>      // Native Zephyr logging
#include "ui.h"

double t = 0, h = 0, p = 0, l = 0;

int main(void)
{
    if (ui_init() != 0) return -1;  // Initialize UART-based UI for user input

    /* ---------------- BME680 DEVICE ---------------- */
    // Get BME680 sensor device instance using Devicetree compatibility
    const struct device *bme = DEVICE_DT_GET_ANY(bosch_bme680);

    // Check if BME680 device is ready for use
    if (!device_is_ready(bme))
    {
        printk("BME680 is not ready\n");
        return -1;
    }

    /* ---------------- VEML7700 DEVICE ---------------- */
    // Get VEML7700 sensor device instance using Devicetree compatibility
    const struct device *veml = DEVICE_DT_GET_ANY(vishay_veml7700);

    // Check if VEML7700 device is ready for use
    if (!device_is_ready(veml))
    {
        printk("VEML7700 device not ready\n");
        return -1;
    }

    /* ---------------- Sensor Value Structures ---------------- */
    struct sensor_value temp, hum, press;   // Structures for BME680 sensor data
    struct sensor_value lux;                 // Structure for VEML7700 light intensity

    printk("BME680 + VEML7700 started successfully\n");

    while (1)
    {
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
            printk("Error: BME680 fetch failed\n");
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
            printk("Error: VEML7700 fetch failed\n");
            continue;
        }

        // Exact print format preserved
        printk("T:%.2f,H:%.2f,P:%.2f,L:%.2f\n", t, h, p, l);

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