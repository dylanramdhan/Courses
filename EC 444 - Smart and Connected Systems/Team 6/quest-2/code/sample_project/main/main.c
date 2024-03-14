#include <stdio.h>
#include <string.h>
#include <stdlib.h>             
#include <inttypes.h>           
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"     
#include "esp_log.h"            // for error logging
#include "esp_system.h"         
#include "driver/uart.h"
#include "driver/gptimer.h"     
#include "driver/gpio.h"        
#include "driver/ledc.h"    
#include "driver/i2c.h"
#include "driver/uart.h"
#include "esp_vfs_dev.h"	
#include "sdkconfig.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "math.h"
#include "./ADXL343.h"

#define GPIO_INPUT_IO_1       4
#define GPIO_INPUT_IO_2       36
#define GPIO_INPUT_IO_3       39

#define ESP_INTR_FLAG_DEFAULT 0
#define GPIO_INPUT_PIN_SEL    1ULL<<GPIO_INPUT_IO_1
#define GPIO_INPUT_PIN_SEL2    1ULL<<GPIO_INPUT_IO_2
#define GPIO_INPUT_PIN_SEL3    1ULL<<GPIO_INPUT_IO_3

#define DEFAULT_VREF    3300        //Use adc2_vref_to_gpio() to obtain a better estimate
#define NO_OF_SAMPLES   100          //Multisampling

#define ONBOARD   13  // onboard LED
#define GPIO_OUTPUT_PIN_SEL  (1ULL<<ONBOARD)

#define TAG "console_IO"

#define SLAVE_ADDR                         0x70 // alphanumeric address
#define OSC                                0x21 // oscillator cmd
#define HT16K33_BLINK_DISPLAYON            0x01 // Display on cmd
#define HT16K33_BLINK_OFF                  0    // Blink off cmd
#define HT16K33_BLINK_CMD                  0x80 // Blink cmd
#define HT16K33_CMD_BRIGHTNESS             0xE0 // Brightness cmd

// Master I2C
#define I2C_EXAMPLE_MASTER_SCL_IO          22   // gpio number for i2c clk
#define I2C_EXAMPLE_MASTER_SDA_IO          23   // gpio number for i2c data
#define I2C_EXAMPLE_MASTER_NUM             I2C_NUM_0  // i2c port
#define I2C_EXAMPLE_MASTER_TX_BUF_DISABLE  0    // i2c master no buffer needed
#define I2C_EXAMPLE_MASTER_RX_BUF_DISABLE  0    // i2c master no buffer needed
#define I2C_EXAMPLE_MASTER_FREQ_HZ         100000     // i2c master clock freq
#define WRITE_BIT                          I2C_MASTER_WRITE // i2c master write
#define READ_BIT                           I2C_MASTER_READ  // i2c master read
#define ACK_CHECK_EN                       true // i2c master will check ack
#define ACK_CHECK_DIS                      false// i2c master will not check ack
#define ACK_VAL                            0x00 // i2c ack value
#define NACK_VAL                           0x01 // i2c nack value (Was FF)
#define SLAVE_ADDR_ADXL                         ADXL343_ADDRESS // 0x53


static esp_adc_cal_characteristics_t *adc_chars;
static const adc_channel_t channel = ADC_CHANNEL_6;     //GPIO34 if ADC1, GPIO14 if ADC2
static const adc_atten_t atten = ADC_ATTEN_DB_11;
static const adc_unit_t unit = ADC_UNIT_1;


// Global flag
bool flag = false;
bool button_flag=false;

bool button_flag2=false;

bool button_flag3=false;

bool timer_state=false;

static int clock_timer_state=0; //0 means clock, 1 means timer
static int time_sec=0;
static int time_min=0;

static int roll_count=0;
static float rolls_data[100];
static int steps=0;

static int clock_hr=13;
static int clock_min=45;
static int clock_sec=0;
static float temp;
static const  uint16_t alphafonttable[] = {

    0b0000000000000001, 0b0000000000000010, 0b0000000000000100,
    0b0000000000001000, 0b0000000000010000, 0b0000000000100000,
    0b0000000001000000, 0b0000000010000000, 0b0000000100000000,
    0b0000001000000000, 0b0000010000000000, 0b0000100000000000,
    0b0001000000000000, 0b0010000000000000, 0b0100000000000000,
    0b1000000000000000, 0b0000000000000000, 0b0000000000000000,
    0b0000000000000000, 0b0000000000000000, 0b0000000000000000,
    0b0000000000000000, 0b0000000000000000, 0b0000000000000000,
    0b0001001011001001, 0b0001010111000000, 0b0001001011111001,
    0b0000000011100011, 0b0000010100110000, 0b0001001011001000,
    0b0011101000000000, 0b0001011100000000,
    0b0000000000000000, //
    0b0000000000000110, // !
    0b0000001000100000, // "
    0b0001001011001110, // #
    0b0001001011101101, // $
    0b0000110000100100, // %
    0b0010001101011101, // &
    0b0000010000000000, // '
    0b0010010000000000, // (
    0b0000100100000000, // )
    0b0011111111000000, // *
    0b0001001011000000, // +
    0b0000100000000000, // ,
    0b0000000011000000, // -
    0b0100000000000000, // .
    0b0000110000000000, // /
    0b0000110000111111, // 0
    0b0000000000000110, // 1
    0b0000000011011011, // 2
    0b0000000010001111, // 3
    0b0000000011100110, // 4
    0b0010000001101001, // 5
    0b0000000011111101, // 6
    0b0000000000000111, // 7
    0b0000000011111111, // 8
    0b0000000011101111, // 9
    0b0001001000000000, // :
    0b0000101000000000, // ;
    0b0010010000000000, // <
    0b0000000011001000, // =
    0b0000100100000000, // >
    0b0001000010000011, // ?
    0b0000001010111011, // @
    0b0000000011110111, // A
    0b0001001010001111, // B
    0b0000000000111001, // C
    0b0001001000001111, // D
    0b0000000011111001, // E
    0b0000000001110001, // F
    0b0000000010111101, // G
    0b0000000011110110, // H
    0b0001001000001001, // I
    0b0000000000011110, // J
    0b0010010001110000, // K
    0b0000000000111000, // L
    0b0000010100110110, // M
    0b0010000100110110, // N
    0b0000000000111111, // O
    0b0000000011110011, // P
    0b0010000000111111, // Q
    0b0010000011110011, // R
    0b0000000011101101, // S
    0b0001001000000001, // T
    0b0000000000111110, // U
    0b0000110000110000, // V
    0b0010100000110110, // W
    0b0010110100000000, // X
    0b0001010100000000, // Y
    0b0000110000001001, // Z
    0b0000000000111001, // [
    0b0010000100000000, //
    0b0000000000001111, // ]
    0b0000110000000011, // ^
    0b0000000000001000, // _
    0b0000000100000000, // `
    0b0001000001011000, // a
    0b0010000001111000, // b
    0b0000000011011000, // c
    0b0000100010001110, // d
    0b0000100001011000, // e
    0b0000000001110001, // f
    0b0000010010001110, // g
    0b0001000001110000, // h
    0b0001000000000000, // i
    0b0000000000001110, // j
    0b0011011000000000, // k
    0b0000000000110000, // l
    0b0001000011010100, // m
    0b0001000001010000, // n
    0b0000000011011100, // o
    0b0000000101110000, // p
    0b0000010010000110, // q
    0b0000000001010000, // r
    0b0010000010001000, // s
    0b0000000001111000, // t
    0b0000000000011100, // u
    0b0010000000000100, // v
    0b0010100000010100, // w
    0b0010100011000000, // x
    0b0010000000001100, // y
    0b0000100001001000, // z
    0b0000100101001001, // {
    0b0001001000000000, // |
    0b0010010010001001, // }
    0b0000010100100000, // ~
    0b0011111111111111,

};
static void i2c_example_master_init(){
    // Debug
    printf("\n>> i2c Config\n");
    int err;

    // Port configuration
    int i2c_master_port = I2C_EXAMPLE_MASTER_NUM;

    /// Define I2C configurations
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;                              // Master mode
    conf.sda_io_num = I2C_EXAMPLE_MASTER_SDA_IO;              // Default SDA pin
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;                  // Internal pullup
    conf.scl_io_num = I2C_EXAMPLE_MASTER_SCL_IO;              // Default SCL pin
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;                  // Internal pullup
    conf.master.clk_speed = I2C_EXAMPLE_MASTER_FREQ_HZ;       // CLK frequency
    conf.clk_flags = 0;                                     // <-- UNCOMMENT IF YOU GET ERRORS (see readme.md)
    err = i2c_param_config(i2c_master_port, &conf);           // Configure
    


    if (err == ESP_OK) {printf("- parameters: ok\n");}

    // Install I2C driver
    err = i2c_driver_install(i2c_master_port, conf.mode,
                       I2C_EXAMPLE_MASTER_RX_BUF_DISABLE,
                       I2C_EXAMPLE_MASTER_TX_BUF_DISABLE, 0);
    // i2c_set_data_mode(i2c_master_port,I2C_DATA_MODE_LSB_FIRST,I2C_DATA_MODE_LSB_FIRST);
    if (err == ESP_OK) {printf("- initialized: yes\n\n");}

    // Dat in MSB mode
    i2c_set_data_mode(i2c_master_port, I2C_DATA_MODE_MSB_FIRST, I2C_DATA_MODE_MSB_FIRST);
}

// Utility  Functions //////////////////////////////////////////////////////////

// Utility function to test for I2C device address -- not used in deploy
int testConnection(uint8_t devAddr, int32_t timeout) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (devAddr << 1) | I2C_MASTER_WRITE, ACK_CHECK_EN);
    i2c_master_stop(cmd);
    int err = i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    return err;
}

// Utility function to scan for i2c device
static void i2c_scanner() {
    int32_t scanTimeout = 1000;
    printf("\n>> I2C scanning ..."  "\n");
    uint8_t count = 0;
    for (uint8_t i = 1; i < 127; i++) {
        // printf("0x%X%s",i,"\n");
        if (testConnection(i, scanTimeout) == ESP_OK) {
            printf( "- Device found at address: 0x%X%s", i, "\n");
            count++;
        }
    }
    if (count == 0)
        printf("- No I2C devices found!" "\n");
    printf("\n");
}

////////////////////////////////////////////////////////////////////////////////

// Alphanumeric Functions //////////////////////////////////////////////////////

// Turn on oscillator for alpha display

//  HX: following are for alpha display setup 
int alpha_oscillator() {
  int ret;
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, ( SLAVE_ADDR << 1 ) | WRITE_BIT, ACK_CHECK_EN);
  i2c_master_write_byte(cmd, OSC, ACK_CHECK_EN);
  i2c_master_stop(cmd);
  ret = i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);
  i2c_cmd_link_delete(cmd);
  vTaskDelay(200 / portTICK_PERIOD_MS);
  return ret;
}

// Set blink rate to off
int no_blink() {
  int ret;
  i2c_cmd_handle_t cmd2 = i2c_cmd_link_create();
  i2c_master_start(cmd2);
  i2c_master_write_byte(cmd2, ( SLAVE_ADDR << 1 ) | WRITE_BIT, ACK_CHECK_EN);
  i2c_master_write_byte(cmd2, HT16K33_BLINK_CMD | HT16K33_BLINK_DISPLAYON | (HT16K33_BLINK_OFF << 1), ACK_CHECK_EN);
  i2c_master_stop(cmd2);
  ret = i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd2, 1000 / portTICK_PERIOD_MS);
  i2c_cmd_link_delete(cmd2);
  vTaskDelay(200 / portTICK_PERIOD_MS);
  return ret;
}

// Set Brightness
int set_brightness_max(uint8_t val) {
  int ret;
  i2c_cmd_handle_t cmd3 = i2c_cmd_link_create();
  i2c_master_start(cmd3);
  i2c_master_write_byte(cmd3, ( SLAVE_ADDR << 1 ) | WRITE_BIT, ACK_CHECK_EN);
  i2c_master_write_byte(cmd3, HT16K33_CMD_BRIGHTNESS | val, ACK_CHECK_EN);
  i2c_master_stop(cmd3);
  ret = i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd3, 1000 / portTICK_PERIOD_MS);
  i2c_cmd_link_delete(cmd3);
  vTaskDelay(200 / portTICK_PERIOD_MS);

  return ret;
}

////////////////////////////////////////////////////////////////////////////////

static void test_alpha_display() {
    // Debug
    int ret;
    printf(">> Test Alphanumeric Display: \n");

    // Set up routines
    // Turn on alpha oscillator
    ret = alpha_oscillator();
    if(ret == ESP_OK) {printf("- oscillator: ok \n");}
    // Set display blink off
    ret = no_blink();
    if(ret == ESP_OK) {printf("- blink: off \n");}
    ret = set_brightness_max(0xF);
    if(ret == ESP_OK) {printf("- brightness: max \n");}

    // Write to characters to buffer
      uint16_t displaybuffer[4];
      char arrayDisplayed[4]={'1','3','4','5'};
    // Continually writes the same command
    int size;
    while (1) {
        if(clock_timer_state){
            int sec_first_digit=time_sec%10;
            displaybuffer[3]=alphafonttable[sec_first_digit+48];
            int sec_second_digit=(time_sec-sec_first_digit)/10;
            displaybuffer[2]=alphafonttable[sec_second_digit+48];
            int min_first_digit=time_min%10;
            displaybuffer[1]=alphafonttable[min_first_digit+48];
            int min_second_digit=(time_min-min_first_digit)/10;
            displaybuffer[0]=alphafonttable[min_second_digit+48];
            size=0;
            for(int ii=0;ii<4;ii++){
                if (arrayDisplayed[ii]!='\0'){
                size++;
                }
            }
            // Send commands characters to display over I2C
            i2c_cmd_handle_t cmd4 = i2c_cmd_link_create();
            i2c_master_start(cmd4);
            i2c_master_write_byte(cmd4, ( SLAVE_ADDR << 1 ) | WRITE_BIT, ACK_CHECK_EN);
            i2c_master_write_byte(cmd4, (uint8_t)0x00, ACK_CHECK_EN);
            for (uint8_t i=0; i<4; i++) {
                i2c_master_write_byte(cmd4, displaybuffer[i] & 0xFF, ACK_CHECK_EN);
                i2c_master_write_byte(cmd4, displaybuffer[i] >> 8, ACK_CHECK_EN);
            }
            i2c_master_stop(cmd4);
            ret = i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd4, 1000 / portTICK_PERIOD_MS);
            i2c_cmd_link_delete(cmd4);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
        
        else{
            int sec_first_digit=clock_min%10;
            displaybuffer[3]=alphafonttable[sec_first_digit+48];
            int sec_second_digit=(clock_min-sec_first_digit)/10;
            displaybuffer[2]=alphafonttable[sec_second_digit+48];
            int min_first_digit=clock_hr%10;
            displaybuffer[1]=alphafonttable[min_first_digit+48];
            int min_second_digit=(clock_hr-min_first_digit)/10;
            displaybuffer[0]=alphafonttable[min_second_digit+48];
            size=0;
            for(int ii=0;ii<4;ii++){
                if (arrayDisplayed[ii]!='\0'){
                  size++;
                }
            }
            // Send commands characters to display over I2C
            i2c_cmd_handle_t cmd4 = i2c_cmd_link_create();
            i2c_master_start(cmd4);
            i2c_master_write_byte(cmd4, ( SLAVE_ADDR << 1 ) | WRITE_BIT, ACK_CHECK_EN);
            i2c_master_write_byte(cmd4, (uint8_t)0x00, ACK_CHECK_EN);
            for (uint8_t i=0; i<4; i++) {
                i2c_master_write_byte(cmd4, displaybuffer[i] & 0xFF, ACK_CHECK_EN);
                i2c_master_write_byte(cmd4, displaybuffer[i] >> 8, ACK_CHECK_EN);
            }
            i2c_master_stop(cmd4);
            ret = i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd4, 1000 / portTICK_PERIOD_MS);
            i2c_cmd_link_delete(cmd4);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
    }
}

static void IRAM_ATTR gpio_isr_handler(void* arg){
  button_flag = 1;
}

static void IRAM_ATTR gpio_isr_handler2(void* arg){
  button_flag2 = 1;
}

static void IRAM_ATTR gpio_isr_handler3(void* arg){
  button_flag3 = 1;
}

// Button interrupt init
static void button_init() {
  gpio_config_t io_conf;
  //interrupt of rising edge
  io_conf.intr_type = GPIO_INTR_POSEDGE;
  //bit mask of the pins, use GPIO4 here
  io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
  //set as input mode
  io_conf.mode = GPIO_MODE_INPUT;
  //enable pull-up mode
  io_conf.pull_up_en = 1;
  gpio_config(&io_conf);
  gpio_intr_enable(GPIO_INPUT_IO_1 );
  //install gpio isr service
  gpio_install_isr_service(ESP_INTR_FLAG_LEVEL3);
  //hook isr handler for specific gpio pin
  gpio_isr_handler_add(GPIO_INPUT_IO_1, gpio_isr_handler, (void*) GPIO_INPUT_IO_1);
}

static void button_init2() {
  gpio_config_t io_conf;
  //interrupt of rising edge
  io_conf.intr_type = GPIO_INTR_POSEDGE;
  //bit mask of the pins, use GPIO4 here
  io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL2;
  //set as input mode
  io_conf.mode = GPIO_MODE_INPUT;
  //enable pull-up mode
  io_conf.pull_up_en = 1;
  gpio_config(&io_conf);
  gpio_intr_enable(GPIO_INPUT_IO_2 );
  //install gpio isr service
  //hook isr handler for specific gpio pin
  gpio_isr_handler_add(GPIO_INPUT_IO_2, gpio_isr_handler2, (void*) GPIO_INPUT_IO_2);
}

static void button_init3() {
  gpio_config_t io_conf;
  //interrupt of rising edge
  io_conf.intr_type = GPIO_INTR_POSEDGE;
  //bit mask of the pins, use GPIO4 here
  io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL3;
  //set as input mode
  io_conf.mode = GPIO_MODE_INPUT;
  //enable pull-up mode
  io_conf.pull_up_en = 1;
  gpio_config(&io_conf);
  gpio_intr_enable(GPIO_INPUT_IO_3 );
  //install gpio isr service
  //hook isr handler for specific gpio pin
  gpio_isr_handler_add(GPIO_INPUT_IO_3, gpio_isr_handler3, (void*) GPIO_INPUT_IO_3);
}

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

void thermal_task(void)
{
    //Check if Two Point or Vref are burned into eFuse
    check_efuse();
    float Vo,R2,logR2,T;
    float R1=10000;
    float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

    //Configure ADC
    if (unit == ADC_UNIT_1) {
        adc1_config_width(ADC_WIDTH_BIT_12);
        adc1_config_channel_atten(channel, atten);
    } else {
        adc2_config_channel_atten((adc2_channel_t)channel, atten);
    }

    //Characterize ADC
    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(unit, atten, ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars);
    print_char_val_type(val_type);

    //Continuously sample ADC1
    while (1) {
        uint32_t adc_reading = 0;
        //Multisampling
        for (int i = 0; i < NO_OF_SAMPLES; i++) {
            if (unit == ADC_UNIT_1) {
                adc_reading += adc1_get_raw((adc1_channel_t)channel);
            } else {
                int raw;
                adc2_get_raw((adc2_channel_t)channel, ADC_WIDTH_BIT_12, &raw);
                adc_reading += raw;
            }
        }
        adc_reading /= NO_OF_SAMPLES;
        //Convert adc_reading to voltage in mV

        Vo = adc_reading;
        R2 = R1 * (4095.0 / (float)Vo - 1.0);
        logR2 = log(R2);
        T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
        T = T - 273.15;
        temp=T;
        vTaskDelay(100);
    }
}
void button_task(){
  while(1) {
    if(button_flag) {
      timer_state=!timer_state;
      button_flag = 0;
      
    }
    if(button_flag2){
      printf("timer reset.\n");
      timer_state=0;
      time_sec=0;
      time_min=0;
      button_flag2=0;
    }
    if(button_flag3){
        button_flag3=0;
        clock_timer_state=!clock_timer_state;
    }
    vTaskDelay(100/portTICK_PERIOD_MS);
  }
}

// Initialize GPIO for LED signals
static void led_init() {
    gpio_config_t io_conf = {};        // zero-initialize the config structure
    io_conf.mode = GPIO_MODE_OUTPUT;   // set as output mode
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL; // bit mask of the pins that you want to set
    io_conf.pull_down_en = 0;          // disable pull-down mode
    io_conf.pull_up_en = 0;            // disable pull-up mode
    gpio_config(&io_conf);             // configure GPIO with the given settings
}

// A simple structure for queue elements
typedef struct {
    uint64_t event_count;
} example_queue_element_t;

// Create a FIFO queue for timer-based events
example_queue_element_t ele;
QueueHandle_t timer_queue;

// System log tags -- get logged when things happen, for debugging 
static const char *TAG_TIMER = "ec444: timer";       

// Timer interrupt handler -- callback timer function -- from GPTimer guide example
static bool IRAM_ATTR timer_on_alarm_cb(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_data) {
    BaseType_t high_task_awoken = pdFALSE;
    QueueHandle_t timer_queue1 = (QueueHandle_t)user_data;    // represents state info passed to callback, if needed
    example_queue_element_t ele = {
          .event_count = edata->count_value                   // Retrieve count value and send to queue
      };
    xQueueSendFromISR(timer_queue1, &ele, &high_task_awoken); // Puts data into queue and alerts other recipients
    return (high_task_awoken == pdTRUE);  		      // pdTRUE indicates data posted successfully
}

// Timer configuration -- from GPTimer guide example
static void alarm_init() {
    gptimer_handle_t gptimer = NULL;
    gptimer_config_t timer_config = {
      .clk_src = GPTIMER_CLK_SRC_DEFAULT,
      .direction = GPTIMER_COUNT_UP,
      .resolution_hz = 1000000, // 1MHz, 1 tick=1us
    };
    ESP_ERROR_CHECK(gptimer_new_timer(&timer_config, &gptimer)); // instantiates timer
  
    gptimer_event_callbacks_t cbs = { // Set alarm callback
      .on_alarm = timer_on_alarm_cb,  // This is a specific supported callback from callbacks list
    };
    ESP_ERROR_CHECK(gptimer_register_event_callbacks(gptimer, &cbs, timer_queue)); // This registers the callback
    ESP_ERROR_CHECK(gptimer_enable(gptimer));                                      // Enables timer interrupt ISR

    ESP_LOGI(TAG_TIMER, "Start timer, update alarm value dynamically and auto reload"); 
    gptimer_alarm_config_t alarm_config = { // Configure the alarm 
      .reload_count = 0,                    // counter will reload with 0 on alarm event
      .alarm_count = 1*1000000,            // period = 1*1s = 1s
      .flags.auto_reload_on_alarm = true,   // enable auto-reload
    };
    ESP_ERROR_CHECK(gptimer_set_alarm_action(gptimer, &alarm_config));  // this enacts the alarm config
    ESP_ERROR_CHECK(gptimer_start(gptimer));                            // this starts the timer
}
static bool temp_state=0;
// LED task to light LED based on alarm flag
void led_task(){
  while(1) {
    if (temp_state) {                             // Uses flag set by Timer task; could be moved to there
        gpio_set_level(ONBOARD, 1);
	      flag = !flag;
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    gpio_set_level(ONBOARD, 0);
  }
}

// Timer task -- what to do when the timer alarm triggers 
static void timer_evt_task(void *arg) {
  while (1) {
    // Transfer from queue and do something if triggered
    if (xQueueReceive(timer_queue, &ele, pdMS_TO_TICKS(2000))) { 
        clock_sec++;
        if(clock_sec==60){
            clock_sec=0;
            clock_min++;
            if (clock_min==60)  {
                clock_min=0;
                clock_hr++;
            }
        
        }

      if(timer_state){
        time_sec++;
        if(time_sec==60){
          time_sec=0;
          time_min++;
        }
        if (temp>28){
          temp_state=1;
        }
        else{
          temp_state=0;
        }
        printf("%d : %d  ,  %f  ,  %d \n",time_min ,time_sec,temp,steps);
        sprintf(payload, "%d : %d  ,  %f  ,  %d \n",time_min ,time_sec,temp,steps);
      }

      flag = true;           // Set a flag to be used elsewhere 
    }
  }
}








static void i2c_master_init(){
  // Debug
  printf("\n>> i2c Config\n");
  int err;

  // Port configuration
  int i2c_master_port = I2C_EXAMPLE_MASTER_NUM;

  /// Define I2C configurations
  i2c_config_t conf;
  conf.mode = I2C_MODE_MASTER;                              // Master mode
  conf.sda_io_num = I2C_EXAMPLE_MASTER_SDA_IO;              // Default SDA pin
  conf.sda_pullup_en = GPIO_PULLUP_ENABLE;                  // Internal pullup
  conf.scl_io_num = I2C_EXAMPLE_MASTER_SCL_IO;              // Default SCL pin
  conf.scl_pullup_en = GPIO_PULLUP_ENABLE;                  // Internal pullup
  conf.master.clk_speed = I2C_EXAMPLE_MASTER_FREQ_HZ;       // CLK frequency
  conf.clk_flags=0;
  err = i2c_param_config(i2c_master_port, &conf);           // Configure
  if (err == ESP_OK) {printf("- parameters: ok\n");}

  // Install I2C driver
  err = i2c_driver_install(i2c_master_port, conf.mode,
                     I2C_EXAMPLE_MASTER_RX_BUF_DISABLE,
                     I2C_EXAMPLE_MASTER_TX_BUF_DISABLE, 0);
  if (err == ESP_OK) {printf("- initialized: yes\n");}

  // Data in MSB mode
  i2c_set_data_mode(i2c_master_port, I2C_DATA_MODE_MSB_FIRST, I2C_DATA_MODE_MSB_FIRST);
}

// Utility  Functions //////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////

// ADXL343 Functions ///////////////////////////////////////////////////////////

// Get Device ID
int getDeviceID(uint8_t *data) {
  int ret;
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, ( SLAVE_ADDR_ADXL << 1 ) | WRITE_BIT, ACK_CHECK_EN);
  i2c_master_write_byte(cmd, ADXL343_REG_DEVID, ACK_CHECK_EN);
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, ( SLAVE_ADDR_ADXL << 1 ) | READ_BIT, ACK_CHECK_EN);
  i2c_master_read_byte(cmd, data, ACK_CHECK_DIS);
  i2c_master_stop(cmd);
  ret = i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);
  i2c_cmd_link_delete(cmd);
  return ret;
}

// Write one byte to register
int writeRegister(uint8_t reg, uint8_t data) {
  i2c_cmd_handle_t cmd = i2c_cmd_link_create(); // Create an I2C command

  // Start the I2C communication, write the register address, and data
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, (SLAVE_ADDR_ADXL << 1) | WRITE_BIT, ACK_CHECK_EN);
  i2c_master_write_byte(cmd, reg, ACK_CHECK_EN);
  i2c_master_write_byte(cmd, data, ACK_CHECK_EN);
  i2c_master_stop(cmd);

  // Execute the I2C command
  int err = i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);
  i2c_cmd_link_delete(cmd);

  return err; // Return success code or an error code
}

// Read register
uint8_t readRegister(uint8_t reg) {
  uint8_t data;

  i2c_cmd_handle_t cmd = i2c_cmd_link_create(); // Create an I2C command

  // Start the I2C communication, write the register address
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, (SLAVE_ADDR_ADXL << 1) | WRITE_BIT, ACK_CHECK_EN);
  i2c_master_write_byte(cmd, reg, ACK_CHECK_EN);
  
  // Restart and read data
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, (SLAVE_ADDR_ADXL << 1) | READ_BIT, ACK_CHECK_EN);
  i2c_master_read_byte(cmd, &data, I2C_MASTER_ACK); // Read the data with ACK
  i2c_master_stop(cmd);

  // Execute the I2C command
  int err = i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);
  i2c_cmd_link_delete(cmd);

  if (err == ESP_OK) {
    return data; // Return the read data
  } else {
    return 0; // Return an error value or handle the error as needed
  }
}


// read 16 bits (2 bytes)
int16_t read16(uint8_t reg) {
  uint8_t data_low, data_high;

  i2c_cmd_handle_t cmd = i2c_cmd_link_create(); // Create an I2C command

  // Start the I2C communication, write the register address, and read data
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, (SLAVE_ADDR_ADXL << 1) | WRITE_BIT, ACK_CHECK_EN);
  i2c_master_write_byte(cmd, reg, ACK_CHECK_EN);
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, (SLAVE_ADDR_ADXL << 1) | READ_BIT, ACK_CHECK_EN);
  i2c_master_read_byte(cmd, &data_low, ACK_VAL);
  i2c_master_read_byte(cmd, &data_high, NACK_VAL); // NACK the last byte
  i2c_master_stop(cmd);
 
  // Execute the I2C command
  int err = i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);
  i2c_cmd_link_delete(cmd);

  if (err == ESP_OK) {
    int16_t value = (int16_t)((data_high << 8) | data_low);
    return value; // Return the 16-bit value
  } else {
    return 0; // Return an error value or handle the error as needed
  }
}


void setRange(range_t range) {
  /* Red the data format register to preserve bits */
  uint8_t format = readRegister(ADXL343_REG_DATA_FORMAT);

  /* Update the data rate */
  format &= ~0x0F;
  format |= range;

  /* Make sure that the FULL-RES bit is enabled for range scaling */
  format |= 0x08;

  /* Write the register back to the IC */
  writeRegister(ADXL343_REG_DATA_FORMAT, format);

}

range_t getRange(void) {
  /* Red the data format register to preserve bits */
  return (range_t)(readRegister(ADXL343_REG_DATA_FORMAT) & 0x03);
}

dataRate_t getDataRate(void) {
  return (dataRate_t)(readRegister(ADXL343_REG_BW_RATE) & 0x0F);
}

////////////////////////////////////////////////////////////////////////////////

// function to get acceleration
void getAccel(float * xp, float *yp, float *zp) {
  *xp = read16(ADXL343_REG_DATAX0) * ADXL343_MG2G_MULTIPLIER * SENSORS_GRAVITY_STANDARD;
  *yp = read16(ADXL343_REG_DATAY0) * ADXL343_MG2G_MULTIPLIER * SENSORS_GRAVITY_STANDARD;
  *zp = read16(ADXL343_REG_DATAZ0) * ADXL343_MG2G_MULTIPLIER * SENSORS_GRAVITY_STANDARD;
    //printf("X: %.2f \t Y: %.2f \t Z: %.2f\n", *xp, *yp, *zp);
}

  int x, y, z;                        //three axis acceleration data
double roll = 0.00, pitch = 0.00;       //Roll & Pitch are the angles which rotate by the axis X and y

// function to print roll and pitch
void calcRP(float x, float y, float z){
  // YOUR CODE HERE
  roll = atan2(y , z) * 57.3;

  if(timer_state){
    rolls_data[roll_count]=roll;
    roll_count++;
    if(roll_count==99){
        roll_count=0;
        steps=0;
        for (int ii=1;ii<100;ii++){
            if(rolls_data[ii-1]*rolls_data[ii]<0){
                steps++;
            }
        }
    }
  }
}

// Task to continuously poll acceleration and calculate roll and pitch
static void steps_task() {
    uint8_t deviceID;
  getDeviceID(&deviceID);
  if (deviceID == 0xE5) {
    printf("\n>> Found ADAXL343\n");
  }
  // Disable interrupts
  writeRegister(ADXL343_REG_INT_ENABLE, 0);

  // Set range
  setRange(ADXL343_RANGE_16_G);

  // Enable measurements
  writeRegister(ADXL343_REG_POWER_CTL, 0x08);

  printf("\n>> Polling ADAXL343\n");
  while (1) {
    float xVal, yVal, zVal;
    getAccel(&xVal, &yVal, &zVal);
    calcRP(xVal, yVal, zVal);
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}









void app_main() {

  // Timer queue initialize 
  timer_queue = xQueueCreate(10, sizeof(example_queue_element_t));
  if (!timer_queue) {
    ESP_LOGE(TAG_TIMER, "Creating queue failed");
    return;
  }
  ESP_ERROR_CHECK( uart_driver_install(UART_NUM_0,256, 0, 0, NULL, 0) );

  /* Tell VFS to use UART driver */
  esp_vfs_dev_uart_use_driver(UART_NUM_0);

  i2c_example_master_init();
  i2c_scanner();

  xTaskCreate(test_alpha_display,"test_alpha_display", 4096, NULL, 5, NULL);
  // Create task to handle timer-based events 
  xTaskCreate(button_task, "button_task", 2048, NULL, 5, NULL);
  xTaskCreate(timer_evt_task, "timer_evt_task", 2048, NULL, 5, NULL);
  // Create task to use LED based on flag
  xTaskCreate(led_task, "led_task", 2048, NULL, 5, NULL);
  
  xTaskCreate(thermal_task, "thermal_task", 2048, NULL, 5, NULL);
  
  xTaskCreate(steps_task,"steps_task", 4096, NULL, 5, NULL);
  

  // Initialize all the things
  led_init();
  alarm_init();
  button_init(); 
  button_init2(); 
  button_init3(); 
  //i2c_master_init();
}