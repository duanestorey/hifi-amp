#include <stdio.h>
#include "pins.h"
#include "i2c-bus.h"
#include "sdkconfig.h"
#include "debug.h"
#include "config.h"

I2CBUS::I2CBUS() : mBusHandle( 0 ) {  
    AMP_DEBUG_I( "Starting I2C bus" );

    i2c_master_bus_config_t i2c_mst_config = {};
    i2c_mst_config.clk_source = I2C_CLK_SRC_DEFAULT;
    i2c_mst_config.i2c_port = 0;
    i2c_mst_config.scl_io_num = I2C_MASTER_SCL_IO;
    i2c_mst_config.sda_io_num = I2C_MASTER_SDA_IO;
    i2c_mst_config.glitch_ignore_cnt = 7;
    i2c_mst_config.flags.enable_internal_pullup = false;
    
    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_mst_config, &mBusHandle));
}

I2C_Device_Ptr 
I2CBUS::bindAddress( uint8_t addr ) {
    i2c_device_config_t dev_cfg = {};

    dev_cfg.dev_addr_length = I2C_ADDR_BIT_LEN_7;
    dev_cfg.device_address = addr;
    dev_cfg.scl_speed_hz = I2C_MASTER_FREQ_HZ;

    i2c_master_dev_handle_t deviceHandle = 0;
    ESP_ERROR_CHECK(i2c_master_bus_add_device( mBusHandle, &dev_cfg, &deviceHandle ) );

    bool deviceExists = ( i2c_master_probe( mBusHandle, addr, I2C_PROBE_MS_TO_WAIT ) == ESP_OK );

    return I2C_Device_Ptr( new I2C_Device( deviceHandle, deviceExists ) );
}

void 
I2CBUS::scanBus() {
    int devices_found = 0;
    for( int address = 1; address < 127; address++)  {
        if ( i2c_master_probe( mBusHandle, address, I2C_PROBE_MS_TO_WAIT ) == ESP_OK ) {
            printf( "-> found device with address 0x%02x\r\n", address );
            devices_found++;
        }
    }

    if(devices_found == 0) printf("\r\n-> no devices found\r\n");
    printf("\r\n...scan completed!\r\n");
}
