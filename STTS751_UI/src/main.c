#include <zephyr/kernel.h>        // Required for k_msleep
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/sys/printk.h>    // Native Zephyr logging
#include "ui.h"                   // Custom UI module

#define stts751TempHighReg 0x00 // Define the register address for temperature high byte
#define stts751TempLowReg 0x02  // Define the register address for temperature low byte
#define stts751ConfigReg 0x03   // Define the register address for configuration

#define I2C_NODE DT_NODELABEL(mysensor) // Define the I2C node label as 'mysensor'

int main(void) // Updated to Zephyr standard int main(void)
{
    if (ui_init() != 0) return -1;
    int ret; 

    static const struct i2c_dt_spec devI2c = I2C_DT_SPEC_GET(I2C_NODE); 
    
    // Check if the I2C bus associated with the device is ready
    if (!device_is_ready(devI2c.bus))
    {                                                           
        printk("I2C bus %s is not ready!\n\r", devI2c.bus->name); 
        return -1;                                                     
    }

    uint8_t buffer[2] = {stts751ConfigReg, 0x8C};        // Define a buffer containing data to be written
    ret = i2c_write_dt(&devI2c, buffer, sizeof(buffer)); // Write data to the I2C device
    if (ret != 0)
    {                                                                                           
        printk("Failed to write to I2C device address %x at Reg. %x \n", devI2c.addr, buffer[0]); 
        return -1;                                                                                     
    }

    while (1)
    { 
        uint8_t temp_reading[2] = {0};                                             // Array to store temperature readings
        uint8_t sensor_regs[2] = {stts751TempLowReg, stts751TempHighReg};          // Array containing the sensor register addresses
        
        // Read low byte
        ret = i2c_write_read_dt(&devI2c, &sensor_regs[0], 1, &temp_reading[0], 1); 
        if (ret != 0)
        {                                                                                                   
            printk("Failed to write/read I2C device address %x at Reg. %x \r\n", devI2c.addr, sensor_regs[0]); 
        }
        
        // Read high byte
        ret = i2c_write_read_dt(&devI2c, &sensor_regs[1], 1, &temp_reading[1], 1); 
        if (ret != 0)
        {                                                                                                   
            printk("Failed to write/read I2C device address %x at Reg. %x \r\n", devI2c.addr, sensor_regs[1]); 
        }

        // Calculate the temperature value from the raw temperature readings
        int temp = ((int)temp_reading[1] * 256 + ((int)temp_reading[0] & 0xF0)) / 16; 
        if (temp > 2047)                                                            
        {
            temp -= 4096; // Adjust the temperature value for negative temperatures
        }

        double cTemp = temp * 0.0625; // Convert the temperature value to Celsius
        double fTemp = cTemp * 1.8 + 32; // Convert to Fahrenheit

        // Print the temperature in Celsius and Fahrenheit
        printk("STTS751: Temperature: %.2f °C | %.2f °F\n", cTemp, fTemp); 
        
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