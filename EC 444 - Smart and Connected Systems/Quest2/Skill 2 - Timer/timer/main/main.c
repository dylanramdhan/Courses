// RUN THIS INSIDE -- (Document/Visual Studio Code/EC 444/Quest X/SKILL/main): 
//  1 -- " . /Users/dylanramdhan/esp/esp-idf/export.sh "
//      1.a -- idf.py fullclean  <-- do this when running a new project code
//  2 -- idf.py build
//  3 -- idf.py flash monitor
//  4 -- Program Running...
//  5 -- To Stop Program: ^ AND } button to STOP running program



/*
  Adapted I2C example code to work with the Adafruit 14-segment Alphanumeric Display. Key notes: MSB!!
  
    Emily Lam, Sept 2018, Updated Aug 2019

*/
#include <stdio.h>
#include "driver/i2c.h"
#include <string.h>
#include "esp_task_wdt.h"
#include <stdio.h>
#include "driver/i2c.h"
#include "driver/gpio.h"
#include "esp_task_wdt.h"
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
#define NACK_VAL                           0xFF // i2c nack value

// Function to initiate i2c -- note the MSB declaration!
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
static const uint16_t alphafonttable[] = {

   
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

uint16_t char_to_segment(char c) 
{
    if (c < ' ' || c > '~') { // If character is out of range in the table
        return 0; // Return all segments off
    }
    return alphafonttable[c - ' ']; // Return the segment representation from the table
}

void send_to_display(uint16_t *displaybuffer) 
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (SLAVE_ADDR << 1) | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, (uint8_t)0x00, ACK_CHECK_EN);
    for (uint8_t i = 0; i < 8; i++) {
        i2c_master_write_byte(cmd, displaybuffer[i] & 0xFF, ACK_CHECK_EN);
        i2c_master_write_byte(cmd, displaybuffer[i] >> 8, ACK_CHECK_EN);
    }
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
}

int stdin_available()
{
    struct timeval tv;
    fd_set fds;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    FD_ZERO(&fds);
    FD_SET(0, &fds); // Use 0 directly instead of STDIN_FILENO
    select(1, &fds, NULL, NULL, &tv); // Change STDIN_FILENO+1 to 1
    return FD_ISSET(0, &fds); // Use 0 directly instead of STDIN_FILENO
}


void write_to_display(char *str) 
{
    uint16_t displaybuffer[8] = {0};
    int len = strlen(str);
    int position = 0;

    while (1) {
        // Clear the display buffer
        for (int i = 0; i < 4; i++) {
            displaybuffer[i] = 0;
        }

        // Fill the display buffer with up to 4 characters from the string
        for (int i = 0; i < 4; i++) {
            if (i + position < len) {
                displaybuffer[i] = char_to_segment(str[i + position]);
            }
        }

        // Send commands characters to display over I2C
        send_to_display(displaybuffer);

        // If the string is 4 characters or less, break out of the loop
        if (len <= 4) {
            break;
        }

        // Increment the position and wrap around if necessary
        position = (position + 1) % len;

        vTaskDelay(1000 / portTICK_PERIOD_MS); // Delay for 1 second before shifting

        // Check for new input after shifting
        if (stdin_available()) {
            int ch = getchar(); // Read the character to clear the buffer
            if (ch == '\n') { // If 'enter' is pressed, break out of the loop
                break;
            }
        }
    }
}



static void test_alpha_display() 
{
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

    char inputStr[9] = {0}; // 8 characters + null terminator
    int index = 0;

    printf("Enter a string (up to 8 characters) and press 'enter': ");
    while (1) {
        int ch = getchar();  // Non-blocking read of character
        if (ch != EOF) {  // Check if a character is available
            if (ch == '\n' && index > 0) {  // If 'enter' is pressed
                inputStr[index] = '\0';  // Null terminate the string
                write_to_display(inputStr);
                index = 0;  // Reset index for next input
                printf("Enter a string (up to 8 characters) and press 'enter': ");
            } else if (index < 8) {
                inputStr[index++] = ch;
            }
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);  // Small delay to prevent CPU hogging
    }
}
// Global variable for the timer
volatile int seconds = 0;
volatile int minutes = 0;



// ISR for GPIO 15 negative edge detection

// Initialize GPIO 15 with pull-up and attach interrupt for negative edge

void init_gpio_15_polling()
{
    gpio_set_direction(GPIO_NUM_15, GPIO_MODE_INPUT);
    gpio_set_pull_mode(GPIO_NUM_15, GPIO_PULLUP_ONLY);
}
void button_polling_task(void* arg) {
    while (1) {
        // Check if button is pressed (GPIO 15 is LOW)
        if (gpio_get_level(GPIO_NUM_15) == 0) {
            // Reset the timer
            minutes = 0;
            seconds = 0;

            // Debounce: Wait for a short period to avoid multiple detections
            vTaskDelay(200 / portTICK_PERIOD_MS);
        }

        // Check every 10ms
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}


void update_display() 
{
    char displayStr[5];
    snprintf(displayStr, sizeof(displayStr), "%d:%02d", minutes, seconds);
    write_to_display(displayStr);
}


void timer_task(void* arg) 
{
    while (1) {
        update_display();
        vTaskDelay(1000 / portTICK_PERIOD_MS);  // Delay for 1 second
        seconds++;
        if (seconds >= 60) {
            seconds = 0;
            minutes++;
        }
    }
}

void app_main() 
{
    freopen("/dev/uart/0", "r", stdin); // Set stdin to blocking mode

    i2c_example_master_init();
    i2c_scanner();
    init_gpio_15_polling();  // Use the polling initialization

    // Create tasks
    xTaskCreate(timer_task, "timer_task", 4096, NULL, 5, NULL);
    xTaskCreate(button_polling_task, "button_polling_task", 2048, NULL, 5, NULL);
}

