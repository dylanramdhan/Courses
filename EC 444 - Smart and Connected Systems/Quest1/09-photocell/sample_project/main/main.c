#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"

#define ADC_PIN ADC1_CHANNEL_4  // A4 corresponds to ADC1_CHANNEL_4

void app_main() {
    // Configure ADC to read from the specified pin
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC_PIN, ADC_ATTEN_DB_11);

    while (1) {
        uint32_t sum = 0;

        // Read and average the voltage over 1 second
        for (int i = 0; i < 10; i++) {
            uint32_t adc_reading = adc1_get_raw(ADC_PIN);
            sum += adc_reading;
            vTaskDelay(pdMS_TO_TICKS(100)); // Delay for 100 ms
        }

        // Calculate the average voltage
        uint32_t average_reading = sum / 10;
        float voltage = (average_reading / 4095.0) * 3.3; // Assuming VREF is 3.3V

        // Display the voltage on the serial monitor
        printf("%.2f V DC\n", voltage);

        vTaskDelay(pdMS_TO_TICKS(900)); // Delay for 900 ms to make it 1 second
    }
}
