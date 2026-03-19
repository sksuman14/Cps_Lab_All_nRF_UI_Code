#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/sys/printk.h>
#include "ui.h"

// Grab the raw I2C node from the overlay
#define I2C0_NODE DT_NODELABEL(mysensor)

int main(void)
{
    if (ui_init() != 0) return -1;
    
    static const struct i2c_dt_spec dev_i2c = I2C_DT_SPEC_GET(I2C0_NODE);
    
    if (!device_is_ready(dev_i2c.bus)) 
    {
        printk("I2C bus is not ready.\n"); 
        return -1;                             
    }

    printk("Bypassing Zephyr Driver - Starting VCNL4040 Raw I2C\n");

    // Register 0x00 is ALS_CONF. 
    // Writing 0x00 (LSB) and 0x00 (MSB) turns on the Ambient Light Sensor
    uint8_t als_init[3] = {0x00, 0x00, 0x00}; 
    int ret = i2c_write_dt(&dev_i2c, als_init, sizeof(als_init));
    if (ret != 0) {
        printk("Failed to power on ALS. Error: %d\n", ret);
        return -1;
    }

    // Proximity initialization removed!

    while (1) 
    {
        // --- Read Light (Register 0x09) ---
        uint8_t light_cmd = 0x09;
        uint8_t light_data[2] = {0};
        i2c_write_read_dt(&dev_i2c, &light_cmd, 1, light_data, 2);
        
        uint16_t raw_lux = (light_data[1] << 8) | light_data[0];
        
        // VCNL4040 datasheet math: 0.1 lux per step. 
        // Cast to an integer so your %d printk statement works perfectly!
        int lux = (int)(raw_lux * 0.1); 

        // Print flawlessly
        printk(" VCNL4040: Lux=%d\n", lux); 

        // Get the updated delay interval from the UI
        int target_ms = ui_get_interval_ms();
        if (target_ms < 100) target_ms = 1000; // Safety Net

        int elapsed_ms = 0;
        while (elapsed_ms < target_ms) {
            if (ui_interval_changed()) {
                ui_clear_change_flag();
                break; 
            }
            k_msleep(100);
            elapsed_ms += 100;
        }
    }
    
    return 0;
}