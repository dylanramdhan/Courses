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
        conf.clk_flags = 0;                              // <-- UNCOMMENT IF YOU GET ERRORS (see readme.md)
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
    // displaybuffer[0] = 0b0101001000000001;  // T.
    // displaybuffer[1] = 0b0101001000001111;  // D.
    // displaybuffer[2] = 0b0100000000111001;  // C.
    // displaybuffer[3] = 0b0100000000111000;  // L.

    uint16_t displaybuffer[9];


    uint16_t numbers[9];
      numbers[0] = 0b0000000000000110;  // 1.
      numbers[1] = 0b0000000011011011;  // 2.
      numbers[2] = 0b0000000010001111;  // 3.
      numbers[3] = 0b0000000011100110;  // 4.
      numbers[4] = 0b0010000001101001;  // 5.
      numbers[5] = 0b0000000011111101;  // 6.
      numbers[6] = 0b0000000000000111;  // 7.
      numbers[7] = 0b0000000011111111;  // 8.
      numbers[8] = 0b0000000000000000;  // Space Between 1 and 8
    


    // Continually writes the same command
    while (1) {
       
        // Scrolling through the Numbers:
        for (int offset = 0; offset < 9; offset++) 
        {
            // Copy the current number to the display buffer
            for (int i = 0; i < 9; i++) 
            {
                displaybuffer[i] = numbers[(i + offset) % 9];
            }

            // Send commands characters to display over I2C
            i2c_cmd_handle_t cmd4 = i2c_cmd_link_create();
            i2c_master_start(cmd4);
            i2c_master_write_byte(cmd4, (SLAVE_ADDR << 1) | WRITE_BIT, ACK_CHECK_EN);
            i2c_master_write_byte(cmd4, (uint8_t)0x00, ACK_CHECK_EN);

            for (uint8_t i = 0; i < 8; i++) 
            {
                i2c_master_write_byte(cmd4, displaybuffer[i] & 0xFF, ACK_CHECK_EN);
                i2c_master_write_byte(cmd4, displaybuffer[i] >> 8, ACK_CHECK_EN);
            }

            i2c_master_stop(cmd4);
            ret = i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd4, 1000 / portTICK_PERIOD_MS);
            i2c_cmd_link_delete(cmd4);

            // Delay to control the scrolling speed
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }

    }


}

void app_main() 
{
    i2c_example_master_init();
    i2c_scanner();

    xTaskCreate(test_alpha_display, "test_alpha_display", 4096, NULL, 5, NULL);
}