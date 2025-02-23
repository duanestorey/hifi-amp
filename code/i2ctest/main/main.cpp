#include <stdio.h>
#include "driver/i2c.h"

#include "esp_log.h"
#include <sstream>

#define AMP_DEBUG_I(FORMAT, ...) ESP_LOGI( __PRETTY_FUNCTION__, FORMAT, ##__VA_ARGS__ )
#define AMP_DEBUG_INT_I(FORMAT, ...) ESP_DRAM_LOGI( __PRETTY_FUNCTION__, FORMAT, ##__VA_ARGS__ )
#define AMP_DEBUG_W(FORMAT, ...) ESP_LOGW( __PRETTY_FUNCTION__, FORMAT, ##__VA_ARGS__ )
#define AMP_DEBUG_E(FORMAT, ...) ESP_LOGE( __PRETTY_FUNCTION__, FORMAT, ##__VA_ARGS__ )

bool 
readRegisterByte( uint8_t address, uint8_t reg, uint8_t &data  ) {
    esp_err_t err = ESP_OK;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();

    i2c_master_start( cmd );
    i2c_master_write_byte( cmd, (address << 1), true );
    i2c_master_write_byte( cmd, reg, true );
    i2c_master_start( cmd );
    i2c_master_write_byte( cmd, (address << 1) | I2C_MASTER_READ, true );
    i2c_master_read_byte( cmd, &data, I2C_MASTER_LAST_NACK );
    i2c_master_stop( cmd );

    if ( ( err = i2c_master_cmd_begin( I2C_NUM_0, cmd, 1000 / portTICK_PERIOD_MS ) ) != ESP_OK ) {
        AMP_DEBUG_W( "Issue while reading I2C data at address %d",(int)address );
    }

    i2c_cmd_link_delete(cmd);

    return ( err == ESP_OK );
}

bool
writeRegisterBytes( uint8_t address, uint8_t reg, uint8_t dataSize, uint8_t *data ) {
    esp_err_t err = ESP_OK;

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start( cmd );
    i2c_master_write_byte( cmd, (address << 1) | I2C_MASTER_WRITE, true );
    i2c_master_write_byte( cmd, reg, true );
    i2c_master_write( cmd, data, dataSize, true );
    i2c_master_stop( cmd );
    if ( ( err = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_PERIOD_MS ) ) != ESP_OK ) {
        AMP_DEBUG_W( "Issue while sending I2C data to address %d",(int)address );
    }
    
    i2c_cmd_link_delete(cmd);

    return ( err == ESP_OK );
}


void doTest() {
    i2c_config_t conf = {};

    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = 7;
    conf.scl_io_num = 6;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = 5000;

    esp_err_t err = i2c_param_config( I2C_NUM_0, &conf );
    ESP_ERROR_CHECK( i2c_driver_install( I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0) );

    AMP_DEBUG_I( "Starting I2C bus" );

    vTaskDelay( 1000 / portTICK_PERIOD_MS );

    int devices_found = 0;
    for(int address = 1; address < 127; address++) {
        // create and execute the command link
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (address << 1) | I2C_MASTER_WRITE, true);
        i2c_master_stop(cmd);
        if( i2c_master_cmd_begin(I2C_NUM_0, cmd, 100 / portTICK_PERIOD_MS) == ESP_OK) {
            printf("-> found device with address 0x%02x\r\n", address);
            devices_found++;
        }
        i2c_cmd_link_delete(cmd);
    }
    if(devices_found == 0) printf("\r\n-> no devices found\r\n");
    printf("\r\n...scan completed!\r\n");

    AMP_DEBUG_I( "Trying to read version info" );
    uint8_t version;

    uint8_t c;
    while ( true ) {
        if ( readRegisterByte( 0x58, 100, version ) ) {
            AMP_DEBUG_I( "Version info is 0x%x", version );
        }

        c = 0;
        writeRegisterBytes( 0x58, 10, 1, &c );

        vTaskDelay( 1000 / portTICK_PERIOD_MS );
    }
    
}

extern "C" void app_main(void) {
    doTest();
}
