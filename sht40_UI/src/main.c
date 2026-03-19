#include <zephyr/kernel.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/sys/printk.h>
#include "ui.h"

void main(void) {
    const struct device *const dev = DEVICE_DT_GET_ANY(sensirion_sht4x);
    
    if (ui_init() != 0) return;

   

    while (1) {
        if (dev != NULL && device_is_ready(dev)) {
            struct sensor_value temp, hum;
            if (sensor_sample_fetch(dev) == 0) {
                sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, &temp);
                sensor_channel_get(dev, SENSOR_CHAN_HUMIDITY, &hum);
                printk("SHT40:Temperature:%.2f°C, Humidity:%.2f%%\n",
                       sensor_value_to_float(&temp), sensor_value_to_float(&hum));
            }
        }

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
}
