//Team Members: Houjie Xiong, Dylan Ramdhan, Mete Gumusayak, Nick Ramondo. 09/22/2023

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "math.h"

#include "driver/adc.h"
#include "esp_adc_cal.h"


static const char *TAG = "example";


static int time=0;

static uint8_t flag=0;
static uint8_t allGood=0;
static uint8_t tempState = 0;     // 0 means ok, 1 means different from normal range 
static uint8_t lightState = 0;    // 0 means ok, 1 means different from normal range 

#define GPIO_INPUT_PIN_SEL    1ULL<<GPIO_INPUT_IO_1

#define DEFAULT_VREF    3300        //Use adc2_vref_to_gpio() to obtain a better estimate
#define NO_OF_SAMPLES   100         //Multisampling

static esp_adc_cal_characteristics_t *adc_chars;
static const adc_channel_t tempChannel = ADC_CHANNEL_3;     //GPIO34 if ADC1, GPIO14 if ADC2
static const adc_channel_t lightChannel = ADC_CHANNEL_6;
static const adc_atten_t atten = ADC_ATTEN_DB_11;
static const adc_unit_t unit = ADC_UNIT_1;

#define GPIO_INPUT_IO_1       5
#define yellowGate 18
#define greenGate 19
#define redGate 16
#define blueGate 21
#define ESP_INTR_FLAG_DEFAULT 0

static void IRAM_ATTR gpio_isr_handler(void* arg){ // hardware interrupt
  flag = !flag;
}

static void button_init() { 
  gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_ANYEDGE; // interrupt of any edge
    io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL; // bit mask of the pins, use GPIO4 here
    io_conf.mode = GPIO_MODE_INPUT;            // set as input mode
    io_conf.pull_up_en = 1;                    // enable resistor pull-up mode on pin
  gpio_config(&io_conf);                       // apply parameters
  gpio_intr_enable(GPIO_INPUT_IO_1 );          // enable interrupts on pin
  gpio_install_isr_service(ESP_INTR_FLAG_LEVEL3);   //install gpio isr service
  gpio_isr_handler_add(GPIO_INPUT_IO_1, gpio_isr_handler, (void*) GPIO_INPUT_IO_1); //hook isr handler for specific gpio pin
}

static void configure_led(void) // configure LED pins
{
    ESP_LOGI(TAG, "Example configured to GPIO LED!");

    gpio_reset_pin(yellowGate);
    gpio_reset_pin(greenGate);
    gpio_reset_pin(redGate);
    gpio_reset_pin(blueGate);

    /* Set the GPIO as a push/pull output */
    gpio_set_direction(yellowGate, GPIO_MODE_OUTPUT);
    gpio_set_direction(greenGate, GPIO_MODE_OUTPUT);
    gpio_set_direction(redGate, GPIO_MODE_OUTPUT);
    gpio_set_direction(blueGate, GPIO_MODE_OUTPUT);
}




void init() {				// Convenient way to organize initialization
    configure_led();				// Do it in this sub
    button_init();
}

// error check
static void check_efuse(void)
{
    //Check TP is burned into eFuse
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_TP) == ESP_OK) {
        printf("eFuse Two Point: Supported\n");
    } else {
        printf("eFuse Two Point: NOT supported\n");
    }

    //Check Vref is burned into eFuse
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_VREF) == ESP_OK) {
        printf("eFuse Vref: Supported\n");
    } else {
        printf("eFuse Vref: NOT supported\n");
    }
}

static void print_char_val_type(esp_adc_cal_value_t val_type)
{
    if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP) {
        printf("Characterized using Two Point Value\n");
    } else if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
        printf("Characterized using eFuse Vref\n");
    } else {
        printf("Characterized using Default Vref\n");
    }
}


//Photocell
static void task_1()			// Define your first task here
{   
    check_efuse();
    //Configure ADC
    if (unit == ADC_UNIT_1) {
        adc1_config_width(ADC_WIDTH_BIT_12);
        adc1_config_channel_atten(lightChannel, atten);
    } else {
        adc2_config_channel_atten((adc2_channel_t)lightChannel, atten);
    }
    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(unit, atten, ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars);
        print_char_val_type(val_type);


    while(1){			
        uint32_t adc_reading = 0;
        //Multisampling
        for (int i = 0; i < NO_OF_SAMPLES; i++) {
            if (unit == ADC_UNIT_1) {
                adc_reading += adc1_get_raw((adc1_channel_t)lightChannel);
            } else {
                int raw;
                adc2_get_raw((adc2_channel_t)lightChannel, ADC_WIDTH_BIT_12, &raw);
                adc_reading += raw;
            }
            vTaskDelay(2);
        }
        adc_reading /= NO_OF_SAMPLES;
        //Convert adc_reading to voltage in mV
        uint32_t voltage = esp_adc_cal_raw_to_voltage(adc_reading, adc_chars);
        if(voltage <800|| adc_reading>1000){
            printf("Alarm! Light change\n");
            gpio_set_level(yellowGate,1);
            lightState = 1;
        }
        else{
            lightState = 0;
                gpio_set_level(yellowGate,0);
        }
        printf("Light \tVoltage: %ld mV \n",voltage);
        printf("Time: %d s\n",time++);
        time++;
    }
}


//Thermistor
static void task_2()			// Define your second task here
{
    check_efuse();
    //Configure ADC
    float Vo,R2,logR2,T;
    float R1=10000;
    float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
    if (unit == ADC_UNIT_1) {
        adc1_config_width(ADC_WIDTH_BIT_12);
        adc1_config_channel_atten(tempChannel, atten);
    } else {
        adc2_config_channel_atten((adc2_channel_t)tempChannel, atten);
    }
    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(unit, atten, ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars);
        print_char_val_type(val_type);

    while(1){			// Or for( ;; )
        uint32_t adc_reading = 0;
        //Multisampling
        for (int i = 0; i < NO_OF_SAMPLES; i++) {
            if (unit == ADC_UNIT_1) {
                adc_reading += adc1_get_raw((adc1_channel_t)tempChannel);
            } else {
                int raw;
                adc2_get_raw((adc2_channel_t)tempChannel, ADC_WIDTH_BIT_12, &raw);
                adc_reading += raw;
            }
            vTaskDelay(2);
        }

        adc_reading /= NO_OF_SAMPLES;
        //Convert adc_reading to voltage in mV
        Vo = adc_reading;
        R2 = R1 * ((4095.0 / (float)Vo) - 1.0);
        logR2 = log(R2);
        T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
        T = T - 273.15;
        uint32_t voltage = esp_adc_cal_raw_to_voltage(adc_reading, adc_chars);
        if(T>23.5 || T<20){
            printf("Alarm! Temp change\n");
            gpio_set_level(redGate,1);
            tempState = 1;
        }
        else{
            gpio_set_level(redGate,0);
            tempState = 0;
        }

        printf("Temp \tTemperature: %f C \n", T);
    }
}

//pressure sensor
static void task_3()			// Define your second task here
{
  while(1) {                               // loop forever in this task
    if(flag) {
      printf("Button pressed.\n");
    }
    gpio_set_level(blueGate,flag);
    vTaskDelay(20 / portTICK_PERIOD_MS);  // wait a bit
  }
}
//all good
static void task_4(){
    while(1){
        allGood = !(tempState | lightState | flag);
        gpio_set_level(greenGate,allGood);
        vTaskDelay(100 / portTICK_PERIOD_MS);  
    }
}
void app_main(void)
{

    init();				// Initialize stuff
	xTaskCreate(task_1, "task_1",1024*2, NULL, configMAX_PRIORITIES, NULL);
	xTaskCreate(task_2, "task_2",1024*2, NULL, configMAX_PRIORITIES-1, NULL);
	xTaskCreate(task_3, "task_3",1024*2, NULL, configMAX_PRIORITIES-2, NULL);
	xTaskCreate(task_4, "task_4",1024*2, NULL, configMAX_PRIORITIES-2, NULL);
    /* Configure the peripheral according to the LED type */
}

