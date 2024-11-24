// sht3x.h
#ifndef SHT3X_H_
#define SHT3X_H_

#include "esp_err.h"

#define SHT3X_I2C_PORT I2C_NUM_0
#define SHT3X_CMD_SOFT_RESET 0x30A2
#define SHT3X_CMD_MEASURE_HIGHREP 0x2400

esp_err_t sht3x_init(uint8_t address);
esp_err_t sht3x_read_temperature_humidity(uint8_t address, float *temperature, float *humidity);

#endif // SHT3X_H_
