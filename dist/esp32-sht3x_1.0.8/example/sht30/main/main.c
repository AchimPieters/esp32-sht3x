/**
   Copyright 2024 Achim Pieters | StudioPieters®

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
   WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
   CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

   for more information visit https://www.studiopieters.nl
 **/


#include "sht3x.h"
#include <driver/i2c.h>
#include <stdio.h>

#define I2C_MASTER_SCL_IO CONFIG_I2C_MASTER_SCL  // GPIO number for I2C master clock
#define I2C_MASTER_SDA_IO CONFIG_I2C_MASTER_SDA  // GPIO number for I2C master data
#define I2C_MASTER_FREQ_HZ 100000             // I2C master clock frequency
#define I2C_MASTER_TX_BUF_DISABLE 0           // I2C master doesn't need buffer
#define I2C_MASTER_RX_BUF_DISABLE 0           // I2C master doesn't need buffer

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
