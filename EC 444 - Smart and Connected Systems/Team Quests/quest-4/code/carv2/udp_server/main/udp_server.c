/* BSD Socket API Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "esp_timer.h"
#include <string.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>
#include "driver/mcpwm_prelude.h"
#include "sdkconfig.h"
#include "freertos/queue.h"
#include "driver/pulse_cnt.h"
#include "driver/gpio.h"
#include "esp_sleep.h"
#include <stdio.h>
#include "driver/gptimer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_private/esp_clk.h"
#include "driver/mcpwm_cap.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
// 14-Segment Display
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
#define NACK_VAL1                           0xFF // i2c nack value
#define NACK_VAL                           0x1 // i2c nack value


static int needReverse=0;


static int ForwardBackward=1;

static float metersPerSecond;


static int comparator_angle = 0;

static int pointFiveSpeed=170;

static const char *TAG = "example";

// Please consult the datasheet of your servo before changing the following parameters
#define SERVO_MIN_PULSEWIDTH_US 500  // Minimum pulse width in microsecond
#define SERVO_MAX_PULSEWIDTH_US 2500  // Maximum pulse width in microsecond
#define SERVO_MIN_DEGREE        -90   // Minimum angle
#define SERVO_MAX_DEGREE        90    // Maximum angle

#define MOTOR_PULSE_GPIO             26
#define SERVO_PULSE_GPIO             4        // GPIO connects to the PWM signal line
#define SERVO_TIMEBASE_RESOLUTION_HZ 1000000  // 1MHz, 1us per tick
#define SERVO_TIMEBASE_PERIOD        20000    // 20000 ticks, 20ms

#define ESC_MAX_PULSEWIDTH_US 2100
#define ESC_MIN_PULSEWIDTH_US 900

#define EXAMPLE_PCNT_HIGH_LIMIT 100
#define EXAMPLE_PCNT_LOW_LIMIT  -100

#define EXAMPLE_EC11_GPIO_A 25
#define EXAMPLE_EC11_GPIO_B 2

#include <stdio.h>
#include "driver/i2c.h"


// Master I2C
#define    LIDARLite_ADDRESS   0x62          // Default I2C Address of LIDAR-Lite.
#define    RegisterHighLowB    0x10          // Register to get both High and Low bytes in 1 call.
typedef struct {
    uint64_t event_count;
} example_queue_element_t;




#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_private/esp_clk.h"
#include "driver/mcpwm_cap.h"
#include "driver/gpio.h"

// const static char *TAG = "example";



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// Please update the following configuration according to your board spec ////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//back 
#define HC_SR04_TRIG_GPIO1  21
#define HC_SR04_ECHO_GPIO1  17
//front
#define HC_SR04_TRIG_GPIO2  16
#define HC_SR04_ECHO_GPIO2  19

float frontdist;
float backdist;
float turnAngle;
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
        int time_sec= (esp_timer_get_time()/1000000)%60;
        int time_min= ((esp_timer_get_time()/1000000)-(esp_timer_get_time()/1000000)%60)/60;
        //printf("%d:%d\n",time_min,time_sec);
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
}

static bool hc_sr04_echo_callback(mcpwm_cap_channel_handle_t cap_chan, const mcpwm_capture_event_data_t *edata, void *user_data)
{
    static uint32_t cap_val_begin_of_sample = 0;
    static uint32_t cap_val_end_of_sample = 0;
    TaskHandle_t task_to_notify = (TaskHandle_t)user_data;
    BaseType_t high_task_wakeup = pdFALSE;

    //calculate the interval in the ISR,
    //so that the interval will be always correct even when capture_queue is not handled in time and overflow.
    if (edata->cap_edge == MCPWM_CAP_EDGE_POS) {
        // store the timestamp when pos edge is detected
        cap_val_begin_of_sample = edata->cap_value;
        cap_val_end_of_sample = cap_val_begin_of_sample;
    } else {
        cap_val_end_of_sample = edata->cap_value;
        uint32_t tof_ticks = cap_val_end_of_sample - cap_val_begin_of_sample;

        // notify the task to calculate the distance
        xTaskNotifyFromISR(task_to_notify, tof_ticks, eSetValueWithOverwrite, &high_task_wakeup);
    }

    return high_task_wakeup == pdTRUE;
}


//generate single pulse on Trig pin to start a new sample
static void gen_trig_output(int trig)
{
    gpio_set_level(trig, 1); // set high
    esp_rom_delay_us(10);
    gpio_set_level(trig, 0); // set low
}
static void UltraSonic_distanceback(){
    ESP_LOGI(TAG, "Install capture timer");
    mcpwm_cap_timer_handle_t cap_timer = NULL;
    mcpwm_capture_timer_config_t cap_conf = {
        .clk_src = MCPWM_CAPTURE_CLK_SRC_DEFAULT,
        .group_id = 0,
    };
    ESP_ERROR_CHECK(mcpwm_new_capture_timer(&cap_conf, &cap_timer));

    ESP_LOGI(TAG, "Install capture channel");
    mcpwm_cap_channel_handle_t cap_chan = NULL;
    mcpwm_capture_channel_config_t cap_ch_conf = {
        .gpio_num = HC_SR04_ECHO_GPIO1,
        .prescale = 1,
        // capture on both edge
        .flags.neg_edge = true,
        .flags.pos_edge = true,
        // pull up internally
        .flags.pull_up = true,
    };
    ESP_ERROR_CHECK(mcpwm_new_capture_channel(cap_timer, &cap_ch_conf, &cap_chan));

    ESP_LOGI(TAG, "Register capture callback");
    TaskHandle_t cur_task = xTaskGetCurrentTaskHandle();
    mcpwm_capture_event_callbacks_t cbs = {
        .on_cap = hc_sr04_echo_callback,
    };
    ESP_ERROR_CHECK(mcpwm_capture_channel_register_event_callbacks(cap_chan, &cbs, cur_task));

    ESP_LOGI(TAG, "Enable capture channel");
    ESP_ERROR_CHECK(mcpwm_capture_channel_enable(cap_chan));

    ESP_LOGI(TAG, "Configure Trig pin");
    gpio_config_t io_conf = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << HC_SR04_TRIG_GPIO1,
    };
    ESP_ERROR_CHECK(gpio_config(&io_conf));
    // drive low by default
    ESP_ERROR_CHECK(gpio_set_level(HC_SR04_TRIG_GPIO1, 0));

    ESP_LOGI(TAG, "Enable and start capture timer");
    ESP_ERROR_CHECK(mcpwm_capture_timer_enable(cap_timer));
    ESP_ERROR_CHECK(mcpwm_capture_timer_start(cap_timer));

    uint32_t tof_ticks;
    while (1) {
        // trigger the sensor to start a new sample
        gen_trig_output(HC_SR04_TRIG_GPIO1);
        // wait for echo done signal
        if (xTaskNotifyWait(0x00, ULONG_MAX, &tof_ticks, pdMS_TO_TICKS(1000)) == pdTRUE) {
            float pulse_width_us = tof_ticks * (1000000.0 / esp_clk_apb_freq());
            if (pulse_width_us > 35000) {
                // out of range
                continue;
            }
            // convert the pulse width into measure distance
            backdist = (float) pulse_width_us / 58;
            // ESP_LOGI(TAG, "Measured distance: %.2fcm", distance);
            // printf("distance back %f\n",backdist);
        }
        vTaskDelay(pdMS_TO_TICKS(153));
    }
}
static void UltraSonic_distancefront(){
    ESP_LOGI(TAG, "Install capture timer");
    mcpwm_cap_timer_handle_t cap_timer = NULL;
    mcpwm_capture_timer_config_t cap_conf = {
        .clk_src = MCPWM_CAPTURE_CLK_SRC_DEFAULT,
        .group_id = 1,
    };
    ESP_ERROR_CHECK(mcpwm_new_capture_timer(&cap_conf, &cap_timer));

    ESP_LOGI(TAG, "Install capture channel");
    mcpwm_cap_channel_handle_t cap_chan = NULL;
    mcpwm_capture_channel_config_t cap_ch_conf = {
        .gpio_num = HC_SR04_ECHO_GPIO2,
        .prescale = 1,
        // capture on both edge
        .flags.neg_edge = true,
        .flags.pos_edge = true,
        // pull up internally
        .flags.pull_up = true,
    };
    ESP_ERROR_CHECK(mcpwm_new_capture_channel(cap_timer, &cap_ch_conf, &cap_chan));

    ESP_LOGI(TAG, "Register capture callback");
    TaskHandle_t cur_task = xTaskGetCurrentTaskHandle();
    mcpwm_capture_event_callbacks_t cbs = {
        .on_cap = hc_sr04_echo_callback,
    };
    ESP_ERROR_CHECK(mcpwm_capture_channel_register_event_callbacks(cap_chan, &cbs, cur_task));

    ESP_LOGI(TAG, "Enable capture channel");
    ESP_ERROR_CHECK(mcpwm_capture_channel_enable(cap_chan));

    ESP_LOGI(TAG, "Configure Trig pin");
    gpio_config_t io_conf = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << HC_SR04_TRIG_GPIO2,
    };
    ESP_ERROR_CHECK(gpio_config(&io_conf));
    // drive low by default
    ESP_ERROR_CHECK(gpio_set_level(HC_SR04_TRIG_GPIO2, 0));

    ESP_LOGI(TAG, "Enable and start capture timer");
    ESP_ERROR_CHECK(mcpwm_capture_timer_enable(cap_timer));
    ESP_ERROR_CHECK(mcpwm_capture_timer_start(cap_timer));

    uint32_t tof_ticks;
    while (1) {
        // trigger the sensor to start a new sample
        gen_trig_output(HC_SR04_TRIG_GPIO2);
        // wait for echo done signal
        if (xTaskNotifyWait(0x00, ULONG_MAX, &tof_ticks, pdMS_TO_TICKS(1000)) == pdTRUE) {
            float pulse_width_us = tof_ticks * (1000000.0 / esp_clk_apb_freq());
            if (pulse_width_us > 35000) {
                // out of range
                continue;
            }
            // convert the pulse width into measure distance
            frontdist = (float) pulse_width_us / 58;
            // ESP_LOGI(TAG, "Measured distance: %.2fcm", distance);
            // printf("distance front %f\n",frontdist);
        }
        vTaskDelay(pdMS_TO_TICKS(173));
    }
}

// Function to initiate i2c -- note the MSB declaration!
//i2c init from Master init
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
  conf.clk_flags = 0;
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
        if (testConnection(i, scanTimeout) == ESP_OK) {
            printf( "- Device found at address: 0x%X%s", i, "\n");
            count++;
        }
    }
    if (count == 0)
        printf("- No I2C devices found!" "\n");
    printf("\n");
}


#define PORT CONFIG_EXAMPLE_PORT


static char rx_buffer[128];
static int pwr = 0;

static void udp_server_task(void *pvParameters)
{
    
    char addr_str[128];
    int addr_family = (int)pvParameters;
    int ip_protocol = 0;
    struct sockaddr_in6 dest_addr;

    while (1) {

        if (addr_family == AF_INET) {
            struct sockaddr_in *dest_addr_ip4 = (struct sockaddr_in *)&dest_addr;
            dest_addr_ip4->sin_addr.s_addr = htonl(INADDR_ANY);
            dest_addr_ip4->sin_family = AF_INET;
            dest_addr_ip4->sin_port = htons(PORT);
            ip_protocol = IPPROTO_IP;
        } else if (addr_family == AF_INET6) {
            bzero(&dest_addr.sin6_addr.un, sizeof(dest_addr.sin6_addr.un));
            dest_addr.sin6_family = AF_INET6;
            dest_addr.sin6_port = htons(PORT);
            ip_protocol = IPPROTO_IPV6;
        }

        int sock = socket(addr_family, SOCK_DGRAM, ip_protocol);
        if (sock < 0) {
            ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
            break;
        }
        ESP_LOGI(TAG, "Socket created");

#if defined(CONFIG_LWIP_NETBUF_RECVINFO) && !defined(CONFIG_EXAMPLE_IPV6)
        int enable = 1;
        lwip_setsockopt(sock, IPPROTO_IP, IP_PKTINFO, &enable, sizeof(enable));
#endif

#if defined(CONFIG_EXAMPLE_IPV4) && defined(CONFIG_EXAMPLE_IPV6)
        if (addr_family == AF_INET6) {
            // Note that by default IPV6 binds to both protocols, it is must be disabled
            // if both protocols used at the same time (used in CI)
            int opt = 1;
            setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
            setsockopt(sock, IPPROTO_IPV6, IPV6_V6ONLY, &opt, sizeof(opt));
        }
#endif
        // Set timeout
        struct timeval timeout;
        timeout.tv_sec = 10;
        timeout.tv_usec = 0;
        setsockopt (sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof timeout);

        int err = bind(sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
        if (err < 0) {
            ESP_LOGE(TAG, "Socket unable to bind: errno %d", errno);
        }
        ESP_LOGI(TAG, "Socket bound, port %d", PORT);

        struct sockaddr_storage source_addr; // Large enough for both IPv4 or IPv6
        socklen_t socklen = sizeof(source_addr);

#if defined(CONFIG_LWIP_NETBUF_RECVINFO) && !defined(CONFIG_EXAMPLE_IPV6)
        struct iovec iov;
        struct msghdr msg;
        struct cmsghdr *cmsgtmp;
        u8_t cmsg_buf[CMSG_SPACE(sizeof(struct in_pktinfo))];

        iov.iov_base = rx_buffer;
        iov.iov_len = sizeof(rx_buffer);
        msg.msg_control = cmsg_buf;
        msg.msg_controllen = sizeof(cmsg_buf);
        msg.msg_flags = 0;
        msg.msg_iov = &iov;
        msg.msg_iovlen = 1;
        msg.msg_name = (struct sockaddr *)&source_addr;
        msg.msg_namelen = socklen;
#endif

        while (1) {
            ESP_LOGI(TAG, "Waiting for data");
#if defined(CONFIG_LWIP_NETBUF_RECVINFO) && !defined(CONFIG_EXAMPLE_IPV6)
            int len = recvmsg(sock, &msg, 0);
#else
            int len = recvfrom(sock, rx_buffer, sizeof(rx_buffer) - 1, 0, (struct sockaddr *)&source_addr, &socklen);
#endif
            // Error occurred during receiving
            if (len < 0) {
                ESP_LOGE(TAG, "recvfrom failed: errno %d", errno);
                break;
            }
            // Data received
            else {
                // Get the sender's ip address as string
                if (source_addr.ss_family == PF_INET) {
                    inet_ntoa_r(((struct sockaddr_in *)&source_addr)->sin_addr, addr_str, sizeof(addr_str) - 1);
#if defined(CONFIG_LWIP_NETBUF_RECVINFO) && !defined(CONFIG_EXAMPLE_IPV6)
                    for ( cmsgtmp = CMSG_FIRSTHDR(&msg); cmsgtmp != NULL; cmsgtmp = CMSG_NXTHDR(&msg, cmsgtmp) ) {
                        if ( cmsgtmp->cmsg_level == IPPROTO_IP && cmsgtmp->cmsg_type == IP_PKTINFO ) {
                            struct in_pktinfo *pktinfo;
                            pktinfo = (struct in_pktinfo*)CMSG_DATA(cmsgtmp);
                            ESP_LOGI(TAG, "dest ip: %s\n", inet_ntoa(pktinfo->ipi_addr));
                        }
                    }
#endif
                } else if (source_addr.ss_family == PF_INET6) {
                    inet6_ntoa_r(((struct sockaddr_in6 *)&source_addr)->sin6_addr, addr_str, sizeof(addr_str) - 1);
                }

                rx_buffer[len] = 0; // Null-terminate whatever we received and treat like a string...
                ESP_LOGI(TAG, "Received %d bytes from %s:", len, addr_str);
                ESP_LOGI(TAG, "%s", rx_buffer);

                int err = sendto(sock, rx_buffer, len, 0, (struct sockaddr *)&source_addr, sizeof(source_addr));
                if (err < 0) {
                    ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
                    break;
                }
            }
        }

        if (sock != -1) {
            ESP_LOGE(TAG, "Shutting down socket and restarting...");
            shutdown(sock, 0);
            close(sock);
        }
    }
    vTaskDelete(NULL);
}

void ManualForward(){

}





static bool example_pcnt_on_reach(pcnt_unit_handle_t unit, const pcnt_watch_event_data_t *edata, void *user_ctx)
{
    BaseType_t high_task_wakeup;
    QueueHandle_t queue = (QueueHandle_t)user_ctx;
    // send event data to queue, from this interrupt callback
    xQueueSendFromISR(queue, &(edata->watch_point_value), &high_task_wakeup);
    return (high_task_wakeup == pdTRUE);
}

static inline uint32_t example_angle_to_compare(int angle)
{
    if( angle > SERVO_MAX_DEGREE) angle = SERVO_MAX_DEGREE;
    if( angle < SERVO_MIN_DEGREE) angle = SERVO_MIN_DEGREE;
    return (angle - SERVO_MIN_DEGREE) * (SERVO_MAX_PULSEWIDTH_US - SERVO_MIN_PULSEWIDTH_US) / (SERVO_MAX_DEGREE - SERVO_MIN_DEGREE) + SERVO_MIN_PULSEWIDTH_US;
}

static inline float ESC_SPEED(float speed) // speed -1 to 1
{
    if( speed > 1) speed = 1;
    if( speed < -1) speed = -1;
    return (speed+1) * (ESC_MAX_PULSEWIDTH_US - ESC_MIN_PULSEWIDTH_US) / 2 + ESC_MIN_PULSEWIDTH_US;
}
static void speed_task()
{
    ///counter
    example_queue_element_t ele;
    QueueHandle_t queue = xQueueCreate(10, sizeof(example_queue_element_t));
    if (!queue) {
        ESP_LOGE(TAG, "Creating queue failed");
        return;
    }

    
    ESP_LOGI(TAG, "Create timer handle");
    gptimer_handle_t gptimer = NULL;
    gptimer_config_t timer_config1 = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_COUNT_UP,
        .resolution_hz = 1000000, // 1MHz, 1 tick=1us
    };
    ESP_ERROR_CHECK(gptimer_new_timer(&timer_config1, &gptimer));

    ESP_LOGI(TAG, "Enable timer");
    ESP_ERROR_CHECK(gptimer_enable(gptimer));

    ESP_LOGI(TAG, "install pcnt unit");
    pcnt_unit_config_t unit_config = {
        .high_limit = EXAMPLE_PCNT_HIGH_LIMIT,
        .low_limit = EXAMPLE_PCNT_LOW_LIMIT,
    };
    
    pcnt_unit_handle_t pcnt_unit = NULL;
    ESP_ERROR_CHECK(pcnt_new_unit(&unit_config, &pcnt_unit));

    ESP_LOGI(TAG, "set glitch filter");
    pcnt_glitch_filter_config_t filter_config = {
        .max_glitch_ns = 1000,
    };
    ESP_ERROR_CHECK(pcnt_unit_set_glitch_filter(pcnt_unit, &filter_config));

    ESP_LOGI(TAG, "install pcnt channels");
    pcnt_chan_config_t chan_a_config = {
        .edge_gpio_num = EXAMPLE_EC11_GPIO_A,
        .level_gpio_num = EXAMPLE_EC11_GPIO_B,
    };
    pcnt_channel_handle_t pcnt_chan_a = NULL;
    ESP_ERROR_CHECK(pcnt_new_channel(pcnt_unit, &chan_a_config, &pcnt_chan_a));
    pcnt_chan_config_t chan_b_config = {
        .edge_gpio_num = EXAMPLE_EC11_GPIO_B,
        .level_gpio_num = EXAMPLE_EC11_GPIO_A,
    };
    pcnt_channel_handle_t pcnt_chan_b = NULL;
    ESP_ERROR_CHECK(pcnt_new_channel(pcnt_unit, &chan_b_config, &pcnt_chan_b));

    ESP_LOGI(TAG, "set edge and level actions for pcnt channels");
    ESP_ERROR_CHECK(pcnt_channel_set_edge_action(pcnt_chan_a, PCNT_CHANNEL_EDGE_ACTION_DECREASE, PCNT_CHANNEL_EDGE_ACTION_INCREASE));
    ESP_ERROR_CHECK(pcnt_channel_set_level_action(pcnt_chan_a, PCNT_CHANNEL_LEVEL_ACTION_KEEP, PCNT_CHANNEL_LEVEL_ACTION_INVERSE));
    ESP_ERROR_CHECK(pcnt_channel_set_edge_action(pcnt_chan_b, PCNT_CHANNEL_EDGE_ACTION_INCREASE, PCNT_CHANNEL_EDGE_ACTION_DECREASE));
    ESP_ERROR_CHECK(pcnt_channel_set_level_action(pcnt_chan_b, PCNT_CHANNEL_LEVEL_ACTION_KEEP, PCNT_CHANNEL_LEVEL_ACTION_INVERSE));

    ESP_LOGI(TAG, "add watch points and register callbacks");
    int watch_points[] = {EXAMPLE_PCNT_LOW_LIMIT, -50, 0, 50, EXAMPLE_PCNT_HIGH_LIMIT};
    for (size_t i = 0; i < sizeof(watch_points) / sizeof(watch_points[0]); i++) {
        ESP_ERROR_CHECK(pcnt_unit_add_watch_point(pcnt_unit, watch_points[i]));
    }
    pcnt_event_callbacks_t cbs = {
        .on_reach = example_pcnt_on_reach,
    };
    QueueHandle_t queue1 = xQueueCreate(10, sizeof(int));
    ESP_ERROR_CHECK(pcnt_unit_register_event_callbacks(pcnt_unit, &cbs, queue1));

    ESP_LOGI(TAG, "enable pcnt unit");
    ESP_ERROR_CHECK(pcnt_unit_enable(pcnt_unit));
    ESP_LOGI(TAG, "clear pcnt unit");
    ESP_ERROR_CHECK(pcnt_unit_clear_count(pcnt_unit));
    ESP_LOGI(TAG, "start pcnt unit");
    ESP_ERROR_CHECK(pcnt_unit_start(pcnt_unit));

    #if CONFIG_EXAMPLE_WAKE_UP_LIGHT_SLEEP
        // EC11 channel output high level in normal state, so we set "low level" to wake up the chip
        ESP_ERROR_CHECK(gpio_wakeup_enable(EXAMPLE_EC11_GPIO_A, GPIO_INTR_LOW_LEVEL));
        ESP_ERROR_CHECK(esp_sleep_enable_gpio_wakeup());
        ESP_ERROR_CHECK(esp_light_sleep_start());
    #endif

        ESP_ERROR_CHECK(gptimer_start(gptimer));
        uint64_t cnt;
        uint64_t prevcnt=0;
    
    int pulse_count = 0;
    int event_count = 0;
    int count = 0;
    int counter=0;
    while (1)
    {
        int countNum=5;
         if (xQueueReceive(queue1, &event_count, pdMS_TO_TICKS(200))) {
            counter++;

            // printf("detect \n");
            if(counter>=countNum){

                counter=0;
                ESP_ERROR_CHECK(gptimer_get_raw_count(gptimer, &cnt));
                // ESP_LOGI(TAG, "Watch point event, count: %d", event_count);
                uint64_t diff = (cnt-prevcnt)/countNum;
                float RPS = (1000000.0/12.0)/diff;
                metersPerSecond = .05*2*3.14*RPS;
                //printf("total %d time since last %lldus %fm/s \n",count++,diff,metersPerSecond);
                prevcnt = cnt;
            }
        } else {
            // ESP_LOGI(TAG, "Pulse count: %d", pulse_count);
            ESP_ERROR_CHECK(pcnt_unit_get_count(pcnt_unit, &pulse_count));
            metersPerSecond=0;
        }
    }
}

void PID(float speed){
    pointFiveSpeed=(speed-metersPerSecond)*550;
    if(pointFiveSpeed < 0)
        pointFiveSpeed = 158;
    pwr=pointFiveSpeed;

}

// Write one byte to register (single byte write)
void writeRegister(uint8_t reg, uint8_t data, int addr) {
  // create i2c communication init
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  i2c_master_start(cmd);	// 1. Start (Master write start)
  i2c_master_write_byte(cmd, ( addr << 1 ) | WRITE_BIT, I2C_MASTER_ACK); // (Master write slave add + write bit)
  // wait for salve to ack
  i2c_master_write_byte(cmd, reg, I2C_MASTER_ACK); // (Master write register address)
  // wait for slave to ack
  i2c_master_write_byte(cmd, data, I2C_MASTER_ACK);// master write data 
  // wait for slave to ack
  i2c_master_stop(cmd); // 11. Stop
  // i2c communication done and delete
  i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);
  i2c_cmd_link_delete(cmd);
  // no return here since data is written by master onto slave
}



uint8_t readRegister(uint8_t reg, int addr) {

    uint8_t data;

    i2c_cmd_handle_t cmd = i2c_cmd_link_create(); // Create an I2C command

    // Start the I2C communication, write the register address, and read data

    i2c_master_start(cmd);

    i2c_master_write_byte(cmd, (addr << 1) | WRITE_BIT, ACK_CHECK_EN);

    i2c_master_write_byte(cmd, reg, ACK_CHECK_EN);

    i2c_master_start(cmd);

    i2c_master_write_byte(cmd, (addr << 1) | READ_BIT, ACK_CHECK_EN);

    i2c_master_read_byte(cmd, &data, ACK_CHECK_DIS); // Read the data

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


int16_t read16(uint8_t reg,int addr) {

uint8_t data_low, data_high;

i2c_cmd_handle_t cmd = i2c_cmd_link_create(); // Create an I2C command

// Start the I2C communication, write the register address, and read data

i2c_master_start(cmd);

i2c_master_write_byte(cmd, (addr << 1) | WRITE_BIT, ACK_CHECK_EN);

i2c_master_write_byte(cmd, reg, ACK_CHECK_EN);

i2c_master_start(cmd);

i2c_master_write_byte(cmd, (addr << 1) | READ_BIT, ACK_CHECK_EN);

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

float distance_m=10;
static void LIDAR(){
    printf("\n>> Polling Lidar\n");
    while (1) {
        //write to register 0x00 the value 0x04
        writeRegister(0x00, 0x04,LIDARLite_ADDRESS);
        //READ REGISTER 0X01 UNTIL LSB GOES LOW 
        //if LSB goes low then set flag to true
        int flag = 1;
        while(flag){
            uint16_t data = read16(0x01,LIDARLite_ADDRESS);
            // printf("DATA: %X\n", data);
            flag = data & 1;
            vTaskDelay(5);
        }

        uint16_t distance = read16(RegisterHighLowB,LIDARLite_ADDRESS);
        // printf("%X\n",);
        distance_m = (float)distance/100.0;

        
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
/*void reverse(){
    printf("Start reversing");
    pwr = 0;
    ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(comparator2, ESC_SPEED(pwr/1000.0)));

    pwr = -100;
    vTaskDelay(100 / portTICK_PERIOD_MS);
    ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(comparator2, ESC_SPEED(pwr/1000.0)));

    vTaskDelay(100 / portTICK_PERIOD_MS);
    pwr = 0;
    ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(comparator2, ESC_SPEED(pwr/1000.0)));
    
    vTaskDelay(100 / portTICK_PERIOD_MS);
    pwr = -100;
    ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(comparator2, ESC_SPEED(pwr/1000.0)));
}*/
// Adding WASD control to buggy (thanks to Nick Hardy et al.)
//
// 1. Use ESP32 UDP Client (ESP32 requests input from HTML interface
//    hosted by node app, interface sends key input) [UDP_client.c]
//
// OR
//
// 2. Use ESP32 UDB Server (ESP32 opens a socket and listens for input
//    from the HTML interface hosted by the node app) [UDP_server.c]
// 
// In either case, data is put into rx_buffer
static int drivestate = 1;
static void input_task(){
    drivestate = 1;

    char wasd = ' ';       // Get wasd from socket or datagram 
    wasd = rx_buffer[0];   // Get wasd from socket or datagram
    while(1)
    {
        wasd=rx_buffer[0];
        //Set the state manually
        if (wasd == 'g'){
            // Set drivestate to auto
            drivestate = 0;
            printf("Auto\n");
            rx_buffer[0] = ' ';
        }
        if (wasd == 'm'){
            // Set drivestate to manual
            drivestate = 1;
            printf("Manual\n");
            rx_buffer[0] = ' ';
        }
        if (wasd == 'q'){
            //Stop the car no matter what
            pwr = -100;
            printf("Stop\n");
            rx_buffer[0] = ' ';
            drivestate = 1;
        }
        
        //Manual Mode
        if (drivestate == 1)
            {
            if (wasd == 'w')
            {
                pwr=pointFiveSpeed;
                PID(0.4);
                vTaskDelay(500 / portTICK_PERIOD_MS);
                wasd=' ';
                printf("Forward\n");
                rx_buffer[0] = ' ';
            }
            else if (wasd == 'a')
            {
                if (comparator_angle <= 80){
                comparator_angle+=20;
                vTaskDelay(20 / portTICK_PERIOD_MS);
                printf("Left\n");
                }
                rx_buffer[0] = ' ';
	         }
            else if (wasd == 'd')
            {
                if (comparator_angle >= -80){
                    comparator_angle-=20;
                    vTaskDelay(20 / portTICK_PERIOD_MS);
                    printf("Right\n");
                }
                rx_buffer[0] =' ';
            }
            else if (wasd == 's')
            { 
                if(ForwardBackward){
                    needReverse=1;
                    ForwardBackward=0;//0 means backward
                }
                pwr=-225;
                printf("Backward\n");
                rx_buffer[0] = ' ';
            }
            else if (wasd == ' '){
            //Stop the car no matter what
                pwr=0;
                printf("Stop\n");
            }
        }
    //Auto Mode
        else if (drivestate == 0)
        {
        //    pwr = 160;
            PID(0.4);

            comparator_angle = turnAngle;
        }
        vTaskDelay(300 / portTICK_PERIOD_MS);
    }
    
}


void app_main(void)
{
    i2c_master_init();
    i2c_scanner();
    ////////////////////////////////////////////////
    // Servo
    ////////////////////////////////////////////////
    ESP_LOGI(TAG, "Create timer and operator");
    mcpwm_timer_handle_t timer = NULL;
    mcpwm_timer_config_t timer_config = {
        .group_id = 0,
        .clk_src = MCPWM_TIMER_CLK_SRC_DEFAULT,
        .resolution_hz = SERVO_TIMEBASE_RESOLUTION_HZ,
        .period_ticks = SERVO_TIMEBASE_PERIOD,
        .count_mode = MCPWM_TIMER_COUNT_MODE_UP,
    };
    ESP_ERROR_CHECK(mcpwm_new_timer(&timer_config, &timer));

    mcpwm_oper_handle_t oper = NULL;
    mcpwm_operator_config_t operator_config = {
        .group_id = 0, // operator must be in the same group to the timer
    };
    ESP_ERROR_CHECK(mcpwm_new_operator(&operator_config, &oper));

    ESP_LOGI(TAG, "Connect timer and operator");
    ESP_ERROR_CHECK(mcpwm_operator_connect_timer(oper, timer));

    ESP_LOGI(TAG, "Create comparator and generator from the operator");
    mcpwm_cmpr_handle_t comparator = NULL;
    mcpwm_comparator_config_t comparator_config = {
        .flags.update_cmp_on_tez = true,
    };
ESP_ERROR_CHECK(mcpwm_new_comparator(oper, &comparator_config, &comparator));

    mcpwm_gen_handle_t generator = NULL;
    mcpwm_generator_config_t generator_config = {
        .gen_gpio_num = SERVO_PULSE_GPIO,
    };
    ESP_ERROR_CHECK(mcpwm_new_generator(oper, &generator_config, &generator));
    
    // set the initial compare value, so that the servo will spin to the center position
    ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(comparator, example_angle_to_compare(0)));

    ESP_LOGI(TAG, "Set generator action on timer and compare event");
    // go high on counter empty
    ESP_ERROR_CHECK(mcpwm_generator_set_action_on_timer_event(generator,
                    MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_HIGH)));
    // go low on compare threshold
    ESP_ERROR_CHECK(mcpwm_generator_set_action_on_compare_event(generator,
                    MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, comparator, MCPWM_GEN_ACTION_LOW)));

    ESP_LOGI(TAG, "Enable and start timer");
    ESP_ERROR_CHECK(mcpwm_timer_enable(timer));
    ESP_ERROR_CHECK(mcpwm_timer_start_stop(timer, MCPWM_TIMER_START_NO_STOP));

    ////////////////////////////////////////////////
    // Motor
    ////////////////////////////////////////////////
    ESP_LOGI(TAG, "Create timer and operator Motor");
    mcpwm_timer_handle_t timer2 = NULL;
    mcpwm_timer_config_t timer_config2 = {
        .group_id = 0,
        .clk_src = MCPWM_TIMER_CLK_SRC_DEFAULT,
        .resolution_hz = SERVO_TIMEBASE_RESOLUTION_HZ,
        .period_ticks = SERVO_TIMEBASE_PERIOD,
        .count_mode = MCPWM_TIMER_COUNT_MODE_UP,
    };
    ESP_ERROR_CHECK(mcpwm_new_timer(&timer_config2, &timer2));

    mcpwm_oper_handle_t oper2 = NULL;
    mcpwm_operator_config_t operator_config2 = {
        .group_id = 0, // operator must be in the same group to the timer
    };
    ESP_ERROR_CHECK(mcpwm_new_operator(&operator_config2, &oper2));

    ESP_LOGI(TAG, "Connect timer and operator motor");
    ESP_ERROR_CHECK(mcpwm_operator_connect_timer(oper2, timer2));

    ESP_LOGI(TAG, "Create comparator and generator from the operator motor");
    mcpwm_cmpr_handle_t comparator2 = NULL;
    mcpwm_comparator_config_t comparator_config2 = {
        .flags.update_cmp_on_tez = true,
    };
    ESP_ERROR_CHECK(mcpwm_new_comparator(oper2, &comparator_config2, &comparator2));

    mcpwm_gen_handle_t generator2 = NULL;
    mcpwm_generator_config_t generator_config2 = {
        .gen_gpio_num = MOTOR_PULSE_GPIO,
    };
    ESP_ERROR_CHECK(mcpwm_new_generator(oper2, &generator_config2, &generator2));

    // set the initial compare value, so that the motor will not move
    // ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(comparator2, ESC_SPEED(0)));

    ESP_LOGI(TAG, "Set generator action on timer and compare event motor");
    // go high on counter empty
    ESP_ERROR_CHECK(mcpwm_generator_set_action_on_timer_event(generator2,
                    MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_HIGH)));
    // go low on compare threshold
    ESP_ERROR_CHECK(mcpwm_generator_set_action_on_compare_event(generator2,
                    MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, comparator, MCPWM_GEN_ACTION_LOW)));

    ESP_LOGI(TAG, "Enable and start timer motor");
    ESP_ERROR_CHECK(mcpwm_timer_enable(timer2));
    ESP_ERROR_CHECK(mcpwm_timer_start_stop(timer2, MCPWM_TIMER_START_NO_STOP));

    ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(comparator2, 1500));
    vTaskDelay(3100 / portTICK_PERIOD_MS); // Do for at least 3s, and leave in neutral state

    

    // Report counter value
    
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    /* This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
     * Read "Establishing Wi-Fi or Ethernet Connection" section in
     * examples/protocols/README.md for more information about this function.
     */
    ESP_ERROR_CHECK(example_connect());

    #ifdef CONFIG_EXAMPLE_IPV4
        xTaskCreate(udp_server_task, "udp_server", 4096, (void*)AF_INET, 5, NULL);
    #endif
    #ifdef CONFIG_EXAMPLE_IPV6
        xTaskCreate(udp_server_task, "udp_server", 4096, (void*)AF_INET6, 5, NULL);
    #endif
    xTaskCreate(test_alpha_display,"test_alpha_display", 4096, NULL, 5, NULL);

    xTaskCreate(speed_task, "speed_task", 4096, (void*)AF_INET, 5, NULL);

    xTaskCreate(input_task, "input_task", 4096, (void*)AF_INET, 5, NULL);

    xTaskCreate(UltraSonic_distanceback,"UltraSonic_distanceback", 4096, NULL, 5, NULL);
    xTaskCreate(UltraSonic_distancefront,"UltraSonic_distancefront", 4096, NULL, 5, NULL);

    xTaskCreate(LIDAR,"LIDAR", 4096, NULL, 5, NULL);

    while(1){
        if(needReverse){
            printf("Start reversing");
            pwr = 0;
            ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(comparator2, ESC_SPEED(pwr/1000.0)));

            pwr = -100;
            vTaskDelay(100 / portTICK_PERIOD_MS);
            ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(comparator2, ESC_SPEED(pwr/1000.0)));

            vTaskDelay(100 / portTICK_PERIOD_MS);
            pwr = 0;
            ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(comparator2, ESC_SPEED(pwr/1000.0)));
            
            vTaskDelay(100 / portTICK_PERIOD_MS);
            pwr = -100;
            ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(comparator2, ESC_SPEED(pwr/1000.0)));
            
            ForwardBackward=0;
            needReverse=0;
        }
        if (drivestate == 0)
        {
        //    pwr = 160;
            PID(0.4);

            // comparator_angle = turnAngle;
        }
        if(distance_m < 0.4&& pwr >0){
            ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(comparator2, ESC_SPEED(-1000/1000.0)));
            pwr= -100;
            drivestate = 1;
        }
        // auto steering
        if(frontdist <100 && backdist<100){ // noise filter
        
            float steerError = frontdist-backdist;
            float scale = 2;
            turnAngle = steerError*scale >30 ? 30 :  steerError*scale <-30 ? -30:steerError*scale ;
            float avgdist = (frontdist+backdist)/2;
            if(avgdist > 50 || avgdist < 30){
                float turn = avgdist-30;
                turnAngle = turn>30?30: turn < -30 ? -30 : turn;
            }
            turnAngle*=-1;
        }

        ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(comparator2, ESC_SPEED(pwr/1000.0)));
        ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(comparator, example_angle_to_compare(comparator_angle)));
        vTaskDelay(20 / portTICK_PERIOD_MS);
        
        printf("pwr: %d\n",pwr);
        printf("comparator_angle: %d\n",comparator_angle);
        printf("metersPerSecond: %f\n",metersPerSecond);
        printf("turn angle %f front : %f back %f\n",turnAngle,frontdist,backdist);
        printf("Distance LIDAR: %.2f m\n", distance_m);
        
        //vTaskDelay(200 / portTICK_PERIOD_MS);
        
        
    }
    
}

