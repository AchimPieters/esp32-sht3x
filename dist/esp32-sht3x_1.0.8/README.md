# Example for `sht3x` Driver

## What it Does

This example demonstrates how to configure and interact with an `SHT3x` sensor using the ESP-IDF framework in high-level mode (`CONFIG_EXAMPLE_SHT3X_DEMO_HL`). The sensor operates in single-shot mode for power efficiency. The high-level function `sht3x_read_temperature_humidity()` is used to perform a complete measurement every 5 seconds.

To enable this mode, ensure that `CONFIG_EXAMPLE_SHT3X_DEMO_HL` is selected in **Example Configuration** under `menuconfig`. This mode is enabled by default.

## Wiring

Connect `SCL` and `SDA` pins to the following pins with appropriate pull-up resistors.

| Name                            | Description           | Defaults                                                                         |
| ------------------------------- | --------------------- | -------------------------------------------------------------------------------- |
| `CONFIG_EXAMPLE_I2C_MASTER_SCL` | GPIO number for `SCL` | "5" for `esp8266`, "6" for `esp32c3`, "19" for `esp32`, `esp32s2`, and `esp32s3` |
| `CONFIG_EXAMPLE_I2C_MASTER_SDA` | GPIO number for `SDA` | "4" for `esp8266`, "5" for `esp32c3`, "18" for `esp32`, `esp32s2`, and `esp32s3` |

<img src="https://github.com/AchimPieters/ESP32-SmartPlug/blob/main/images/MIT%7C%20SOFTWARE%20WHITE.svg" alt="MIT Licence Logo" width="150">

## Include the Library in Your Code

In your project's source code, include the SHT3x library headers:


```c
#include "sht3x.h"
#include <driver/i2c.h>
#include <stdio.h>

#define I2C_MASTER_SCL_IO CONFIG_I2C_MASTER_SCL  // GPIO number for I2C master clock
#define I2C_MASTER_SDA_IO CONFIG_I2C_MASTER_SDA  // GPIO number for I2C master data
#define I2C_MASTER_FREQ_HZ 100000               // I2C master clock frequency
#define I2C_MASTER_TX_BUF_DISABLE 0             // I2C master doesn't need buffer
#define I2C_MASTER_RX_BUF_DISABLE 0             // I2C master doesn't need buffer

void i2c_master_init(void)
{
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };
    i2c_param_config(SHT3X_I2C_PORT, &conf);
    i2c_driver_install(SHT3X_I2C_PORT, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
}

void app_main(void)
{
    uint8_t sht3x_address = CONFIG_SHT3X_I2C_ADDRESS;
    float temperature = 0.0f, humidity = 0.0f;

    // Initialize I2C
    i2c_master_init();

    // Initialize SHT3X sensor
    if (sht3x_init(sht3x_address) == ESP_OK) {
        printf("SHT3X sensor initialized successfully\n");
    } else {
        printf("Failed to initialize SHT3X sensor\n");
        return;
    }

    // Read temperature and humidity
    while (1) {
        if (sht3x_read_temperature_humidity(sht3x_address, &temperature, &humidity) == ESP_OK) {
            printf("Temperature: %.2f C, Humidity: %.2f %%RH\n", temperature, humidity);
        } else {
            printf("Failed to read data from SHT3X sensor\n");
        }
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}
```
StudioPieters® | Innovation and Learning Labs | https://www.studiopieters.nl
