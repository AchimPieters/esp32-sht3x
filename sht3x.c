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

#define WRITE_BIT I2C_MASTER_WRITE         // I2C master write
#define READ_BIT I2C_MASTER_READ           // I2C master read
#define ACK_CHECK_EN 0x1                   // I2C master will check ack from slave
#define ACK_VAL I2C_MASTER_ACK             // I2C ack value
#define NACK_VAL I2C_MASTER_NACK           // I2C nack value

static esp_err_t sht3x_send_command(uint8_t address, uint16_t command)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (address << 1) | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, (command >> 8) & 0xFF, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, command & 0xFF, ACK_CHECK_EN);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(SHT3X_I2C_PORT, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}

static esp_err_t sht3x_read_data(uint8_t address, uint8_t *data, size_t len)
{
    if (len == 0) {
        return ESP_OK;
    }
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (address << 1) | READ_BIT, ACK_CHECK_EN);
    if (len > 1) {
        i2c_master_read(cmd, data, len - 1, ACK_VAL);
    }
    i2c_master_read_byte(cmd, data + len - 1, NACK_VAL);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(SHT3X_I2C_PORT, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}

esp_err_t sht3x_init(uint8_t address)
{
    return sht3x_send_command(address, SHT3X_CMD_SOFT_RESET);
}

esp_err_t sht3x_read_temperature_humidity(uint8_t address, float *temperature, float *humidity)
{
    esp_err_t ret = sht3x_send_command(address, SHT3X_CMD_MEASURE_HIGHREP);
    if (ret != ESP_OK) {
        return ret;
    }
    vTaskDelay(50 / portTICK_PERIOD_MS);

    uint8_t data[6];
    ret = sht3x_read_data(address, data, sizeof(data));
    if (ret != ESP_OK) {
        return ret;
    }

    uint16_t temp_raw = (data[0] << 8) | data[1];
    uint16_t hum_raw = (data[3] << 8) | data[4];

    *temperature = -45.0f + 175.0f * ((float)temp_raw / 65535.0f);
    *humidity = 100.0f * ((float)hum_raw / 65535.0f);

    return ESP_OK;
}
