
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <stdio.h>

#include "ui.h"


// Fetch and display accelerometer data
static void fetch_and_display(const struct device *sensor)   // Function to read and print accelerometer values
{
	static unsigned int count = 0;   // Sample counter to count number of readings
	struct sensor_value accel[3];    // Array to store X, Y, and Z acceleration values
	uint32_t upTime = k_uptime_get_32(); // Get current system uptime in milliseconds

	if (sensor_sample_fetch(sensor) < 0)   // Fetch new data sample from the accelerometer
	{ // Fetch data from sensor
		printf("ERROR: Sample fetch failed\n");   // Print error if fetching fails
		return;                                   // Exit function if fetch fails
	}

	if (sensor_channel_get(sensor, SENSOR_CHAN_ACCEL_XYZ, accel) < 0)   // Retrieve XYZ acceleration values from sensor
	{ // Get XYZ acceleration data
		printf("ERROR: Channel get failed\n");     // Print error if data retrieval fails
		return;                                   // Exit function if channel read fails
	}

	// Display sample count, uptime, and XYZ acceleration values
	printf("x %.3f, y %.3f, z %.3f\n",     // Print the reading count, uptime, and acceleration values,                      // Increment sample count and show uptime
		   sensor_value_to_double(&accel[0]),    // Convert and print X-axis acceleration value
		   sensor_value_to_double(&accel[1]),    // Convert and print Y-axis acceleration value
		   sensor_value_to_double(&accel[2]));   // Convert and print Z-axis acceleration value
}

void main(void)                                   // Main program entry point
{
	ui_init();
	// Get sensor device binding.

	const struct device *sensor = DEVICE_DT_GET_ANY(st_lis2dh);   // Get device handle for LIS2DH accelerometer

	if (sensor == NULL) {                        // Check if sensor device was found
		printf("Could not get %s device\n", "st_lis3dh");   // Print error if device not found
		return;                                   // Exit if device not available
	}

	if (!device_is_ready(sensor))                 // Check if sensor device is initialized and ready
	{ // Check if sensor is ready
		printf("Device %s is not ready\n", sensor->name);   // Print error if device not ready
		return;                                   // Exit if device not ready
	}

	while (1)                                    // Infinite loop to continuously read sensor data
	{									
		int delay_ms = ui_get_interval_ms();
		fetch_and_display(sensor);                // Call function to fetch and print accelerometer data
		// k_msleep(delayTime);	
		k_msleep(delay_ms);
	               // Sleep for specified delay time (200 ms)
	}
}
