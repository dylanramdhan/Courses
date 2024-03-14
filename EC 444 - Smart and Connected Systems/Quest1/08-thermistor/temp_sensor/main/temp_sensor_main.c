#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"

#define ADC_CHANNEL ADC1_CHANNEL_3 // Analog pin A3
#define ADC_ATTEN ADC_ATTEN_DB_11  // 0-0.8V attenuation

void app_main() {
    // Configure ADC
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC_CHANNEL, ADC_ATTEN);

    while (1) {
        float sumVoltage = 0.0;

        // Read the voltage on A3 and accumulate it over 1 second
        for (int i = 0; i < 10; i++) {
            int rawValue = adc1_get_raw(ADC_CHANNEL);
            float voltage_mV = (float)rawValue * 1100.0 / 4095.0; // Convert raw value to mV

            // Debugging: Print raw voltage value
            printf("Raw Voltage: %.2f mV\n", voltage_mV);

            sumVoltage += voltage_mV;
            vTaskDelay(pdMS_TO_TICKS(100)); // Wait for 100 ms
        }

        // Calculate the average voltage over 1 second
        float averageVoltage_mV = sumVoltage / 10.0;

        // Calculate temperature using the new linear relationship
        float temperature = (averageVoltage_mV - 133.0) / ((146.0 - 133.0) / (24.5 - 23.0)) + 23.0;

        // Ensure the temperature is within the specified range
        if (temperature < 15.0) {
            temperature = 15.0;
        } else if (temperature > 30.0) {
            temperature = 30.0;
        }

        // Display the temperature on the monitor
        printf("Temperature: %.2f degrees C\n", temperature);

        vTaskDelay(pdMS_TO_TICKS(1000)); // Wait for 1 second before the next reading
    }
}



// /*
//  * SPDX-FileCopyrightText: 2021-2022 Espressif Systems (Shanghai) CO LTD
//  *
//  * SPDX-License-Identifier: Apache-2.0
//  */

// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "esp_log.h"
// #include "driver/temperature_sensor.h"

// static const char *TAG = "example";

// void app_main(void)
// {
//     ESP_LOGI(TAG, "Install temperature sensor, expected temp ranger range: 10~50 ℃");
//     temperature_sensor_handle_t temp_sensor = NULL;
//     temperature_sensor_config_t temp_sensor_config = TEMPERATURE_SENSOR_CONFIG_DEFAULT(10, 50);
//     ESP_ERROR_CHECK(temperature_sensor_install(&temp_sensor_config, &temp_sensor));

//     ESP_LOGI(TAG, "Enable temperature sensor");
//     ESP_ERROR_CHECK(temperature_sensor_enable(temp_sensor));

//     ESP_LOGI(TAG, "Read temperature");
//     int cnt = 20;
//     float tsens_value;
//     while (cnt--) {
//         ESP_ERROR_CHECK(temperature_sensor_get_celsius(temp_sensor, &tsens_value));
//         ESP_LOGI(TAG, "Temperature value %.02f ℃", tsens_value);
//         vTaskDelay(pdMS_TO_TICKS(1000));
//     }
// }
