#include <zephyr/kernel.h>  // Core Zephyr kernel APIs (threads, sleep, timing)
#include <zephyr/device.h>  // Device driver model and device access APIs
#include <zephyr/devicetree.h>  // Devicetree macros and helpers
#include <zephyr/drivers/i2c.h> // I2C driver APIs
#include <zephyr/sys/printk.h>  // printk() for kernel-level logging
#include <stdio.h>

#include "ui.h"          // Custom UART-based UI interface

#define STS3X_I2C_ADDR   0x4A    // I2C slave address of the STS3x temperature sensor
#define STS3X_MEASURE_CMD 0x240B    // Command to trigger a temperature measurement

#define I2C_NODE DT_NODELABEL(mysensor)  // Devicetree node label for the I2C sensor

static int sts3x_soft_reset(const struct i2c_dt_spec *i2c_spec) {
    uint8_t reset_command[2] = {0x30, 0xA2};// Soft reset command as per datasheet
    return i2c_write_dt(i2c_spec, reset_command, sizeof(reset_command)); // Send reset command over I2C
}
// Triggers a temperature measurement and converts raw data to degrees Celsius
static int measure_temperature(const struct i2c_dt_spec *i2c_spec, float *temperature) {
    uint8_t command[2] = {STS3X_MEASURE_CMD >> 8, STS3X_MEASURE_CMD & 0xFF};
    uint8_t temp_data[3];

// Write measurement command and read back temperature data
    int ret = i2c_write_read_dt(i2c_spec, command, sizeof(command), temp_data, sizeof(temp_data));
    if (ret != 0) {
        return ret;
    }
    
    // Combine MSB and LSB to form raw temperature value
    uint16_t raw_temp = (temp_data[0] << 8) | temp_data[1];

     // Convert raw temperature to degrees Celsius (STS3x conversion formula)
    *temperature = -45.0 + 175.0 * ((float)raw_temp / 65535.0);

    return 0;
}

void main(void) {

    ui_init();   // UART UI START

        // Get I2C device specification from Devicetree
    static const struct i2c_dt_spec dev_i2c = I2C_DT_SPEC_GET(I2C_NODE);

    
    // Check if the I2C bus is ready for communication
    if (!device_is_ready(dev_i2c.bus)) {
        printk("I2C bus %s not ready!\n", dev_i2c.bus->name);
        return;
    }
   // Perform soft reset of the temperature sensor
    int ret = sts3x_soft_reset(&dev_i2c);
    if (ret != 0) {
        printk("Soft reset failed! %d\n", ret);
        return;                  // Exit if reset fails
    }

    k_msleep(100);       // Wait for sensor to stabilize after reset

    float temperature = 0.0;  // Variable to store temperature valu

    while (1) {
           // Get user-defined measurement interval from UART UI
        int delayTime = ui_get_interval_ms();

        // Read temperature from sensor
        ret = measure_temperature(&dev_i2c, &temperature);
        if (ret == 0) {
            printk("Temp: %.2f °C\n", temperature);//Print temperature value
        } else {
            printk("Error reading temperature\n");   // Print error message
        }

        k_msleep(delayTime);  // Delay before next measurement
    }
}
