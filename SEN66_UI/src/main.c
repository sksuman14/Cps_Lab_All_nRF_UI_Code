#include <zephyr/kernel.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/gpio.h>
#include "ui.h"

/* --- Configuration & Definitions --- */
#define I2C_NODE DT_NODELABEL(sen66)
static const struct i2c_dt_spec dev_i2c = I2C_DT_SPEC_GET(I2C_NODE);

#define SEN66_ADDR 0x6B
#define CMD_START_MEAS 0x0021  // Start continuous measurement
#define CMD_READ_VALUES 0x0300 // Read measured values (PM, RH, T, VOC, NOx, CO2)
#define Intial_Value 0xFF
#define CMD_RESET 0xD304 // Soft reset command

/**
 * @brief Sends a 2-byte command to the SEN66 over I2C.
 * @param cmd The 16-bit command to be split into MSB and LSB.
 */
void send_command(uint16_t cmd)
{
    uint8_t buf[2];
    buf[0] = (cmd >> 8); // Extract High Byte
    buf[1] = cmd & 0xFF; // Extract Low Byte

    i2c_write_dt(&dev_i2c, buf, sizeof(buf));
}

/**
 * @brief Reads raw data from the sensor and assembles bytes into 16-bit words.
 * @note Each 16-bit word is followed by an 8-bit CRC byte (skipped here).
 * @param output Pointer to store the resulting 16-bit values.
 * @param data_size Number of 16-bit words to extract.
 */
void read_data(uint16_t *output, uint8_t data_size)
{
    // Each measurement data point consists of 2 bytes + 1 CRC byte (3 bytes total).
    // For 9 data points, we read 27 bytes.
    uint8_t data_array[27];
    i2c_read_dt(&dev_i2c, data_array, sizeof(data_array));

    uint8_t i = 0;
    while (i < data_size)
    {
        // Assemble MSB and LSB while skipping the 3rd byte (CRC) of every triplet
        uint8_t msb = data_array[i * 3];
        uint8_t lsb = data_array[i * 3 + 1];
        output[i] = (msb << 8) | lsb;
        i++;
    }
}

/**
 * @brief Triggers the internal fan cleaning routine.
 * Measurements must be stopped before initiating cleaning.
 */
void fan_cleaning()
{
    // 1. Stop measurement mode (Cmd: 0x0104)
    send_command(0x0104);
    k_msleep(1400); // Wait for sensor to process stop command

    // 2. Start fan cleaning (Cmd: 0x5607)
    send_command(0x5607);
    k_sleep(K_SECONDS(10)); // Cleaning takes 10 seconds

    // 3. Restart measurement mode
    send_command(CMD_START_MEAS);
    k_msleep(2000); // Warm-up delay
    printk("Fan cleaning done, resumes taking reading \n");
}

int main(void)
{
    // Initialize UART/DMA UI defined in ui.c
    if (ui_init() != 0)
        return -1;

    // Check if I2C Bus is active
    if (!device_is_ready(dev_i2c.bus))
    {
        printk("I2C bus %s is not ready!\n\r", dev_i2c.bus->name);
        return -1;
    }
    printk("Sensor is working..\n");
    printk("Waiting for Fan cleaning...\n");

    // --- Initialization Sequence ---
    send_command(CMD_RESET);
    k_msleep(1200);
    send_command(CMD_START_MEAS);
    k_msleep(2000);

    // Perform initial fan cleaning on boot
    fan_cleaning();

    uint16_t output[9];
    uint32_t last_read_time = 0;

    // --- BURNER READING ---
    // Clears stale data from the sensor's internal buffer after cleaning/reboot
    send_command(CMD_READ_VALUES);
    k_msleep(20);
    read_data(output, 9);
    last_read_time = k_uptime_get_32();

    while (1)
    {
        // 24-Hour Maintenance: Trigger fan cleaning daily
        static int64_t last_time = 0;
        if ((k_uptime_get_32() - last_time >= (24 * 60 * 60 * 1000)))
        {
            fan_cleaning();
            last_time = k_uptime_get_32();
        }

        // --- HARDWARE GUARD ---
        // SEN66 should not be polled faster than once per second to prevent I2C bus congestion or sensor errors.
        uint32_t now = k_uptime_get_32();
        if (now - last_read_time < 1000)
        {
            k_msleep(1000 - (now - last_read_time));
        }

        // --- FETCH DATA ---
        send_command(CMD_READ_VALUES);
        k_msleep(20); // Small delay to let sensor prepare data
        read_data(output, 9);
        last_read_time = k_uptime_get_32();

        // --- DATA CONVERSION (Based on Sensirion Datasheet) ---
        float pm1 = output[0] / 10.0f;            // PM1.0 (Scale: 10)
        float pm25 = output[1] / 10.0f;           // PM2.5 (Scale: 10)
        float pm4 = output[2] / 10.0f;            // PM4.0 (Scale: 10)
        float pm10 = output[3] / 10.0f;           // PM10.0 (Scale: 10)
        float rh = (int16_t)output[4] / 100.0f;   // Humidity (Scale: 100)
        float temp = (int16_t)output[5] / 200.0f; // Temp (Scale: 200)
        int16_t voc = (int16_t)output[6] / 10;    // VOC Index (Scale: 10)
        int16_t nox = (int16_t)output[7] / 10;    // NOx Index (Scale: 10)
        uint16_t CO2 = output[8];                 // CO2 ppm (Scale: 1)

        // Display results to Serial Console
        printk("UPtime : %d \n", k_uptime_get_32() / 1000);
        printk("PM1.0 : %.2f µg/m³ | PM2.5 :%.2f µg/m³\n", pm1, pm25);
        printk("RH : %.2f %% | Temp : %.2f °C\n", rh, temp);
        printk("VOC Index : %d | NOx Index : %d\n", voc, nox);
        printk("CO2 : %d ppm\n\n", CO2);

        // --- RESPONSIVE SLEEP LOGIC ---
        // Instead of k_msleep(target_ms), we check for UART interval changes every 100ms.
        int target_ms = ui_get_interval_ms();
        if (target_ms < 1000)
            target_ms = 1000; // Safety floor

        int elapsed_ms = 0;
        while (elapsed_ms < target_ms)
        {
            if (ui_interval_changed())
            {
                // If a user sent "SET_INTERVAL:X" via Python UI, break sleep immediately
                ui_clear_change_flag();
                break;
            }
            k_msleep(100);
            elapsed_ms += 100;
        }
    }

    return 0;
}