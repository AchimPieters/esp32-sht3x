# Example for `sht3x` driver

## What it does

This example demonstrates how to configure and interact with an `SHT3x` sensor using the ESP-IDF framework.

* High-Level Mode `(CONFIG_EXAMPLE_SHT3X_DEMO_HL)`:
When this mode is enabled, the example creates a task that triggers a temperature and humidity measurement every 5 seconds. For power efficiency, the sensor operates in single-shot mode. In this mode, the high-level function sht3x_read_temperature_humidity() is used to perform a complete measurement in a single step during each cycle.
* Low-Level Mode `(CONFIG_EXAMPLE_SHT3X_DEMO_LL)`:
When this mode is enabled, the example creates a task that also triggers a measurement every 5 seconds using single-shot mode for power efficiency. However, in this mode, the process is split into lower-level operations: the measurement is initiated, a delay is applied for the sensor to collect data, and the results are fetched using separate functions.

To choose the desired mode, navigate to Example Configuration in `menuconfig` and select either `CONFIG_EXAMPLE_SHT3X_DEMO_HL` or `CONFIG_EXAMPLE_SHT3X_DEMO_LL`. By default, `CONFIG_EXAMPLE_SHT3X_DEMO_HL` is enabled.

## Wiring


Connect `SCL` and `SDA` pins to the following pins with appropriate pull-up
resistors.

| Name | Description | Defaults |
|------|-------------|----------|
| `CONFIG_EXAMPLE_I2C_MASTER_SCL` | GPIO number for `SCL` | "5" for `esp8266`, "6" for `esp32c3`, "19" for `esp32`, `esp32s2`, and `esp32s3` |
| `CONFIG_EXAMPLE_I2C_MASTER_SDA` | GPIO number for `SDA` | "4" for `esp8266`, "5" for `esp32c3`, "18" for `esp32`, `esp32s2`, and `esp32s3` |
