#include <stdio.h>
#include "i2c-bus.h"
#include "sdkconfig.h"
#include "config.h"
#include "debug.h"
#include "driver/i2c.h"
#include <stdlib.h>
#include <memory>
#include <cstring>

I2CBUS::I2CBUS( uint8_t slaveAddr, QueuePtr queue ) : mQueue( queue ), mSlaveAddr( slaveAddr ), mBusHandle( 0 ) {  
    AMP_DEBUG_I( "Starting I2C bus on addr %x", slaveAddr );
    i2c_config_t conf = {};

    conf.sda_io_num = DSP_PIN_SDA;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = DSP_PIN_SCL;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.mode = I2C_MODE_SLAVE;

    conf.slave.slave_addr = slaveAddr;
    conf.slave.addr_10bit_en = 0;

    i2c_param_config( I2C_NUM_0, &conf );
    ESP_ERROR_CHECK( i2c_driver_install( I2C_NUM_0, conf.mode, I2C_BUF_LEN, I2C_BUF_LEN, 0 ) );
}

bool 
I2CBUS::readData( uint8_t *&buffer, uint8_t &length, uint16_t maxLength, uint16_t timeout ) {
    size_t size = i2c_slave_read_buffer( I2C_NUM_0, mReceiveBuffer, maxLength, timeout / portTICK_PERIOD_MS );
    if ( size ) {
        buffer = mReceiveBuffer;
        length = (uint8_t)size;

        return true;
    } else {
        return false;
    }
}

void 
I2CBUS::processData( uint8_t *buffer, uint32_t length ) {
    AMP_DEBUG_I( "Processing data %d", (uint8_t)length );
    if ( length >= 1 ) {
        uint8_t reg = buffer[ 0 ];

        for ( Listeners::iterator i = mListeners.begin(); i != mListeners.end(); i++ ) {
            AMP_DEBUG_I( "Executing callback for register %d", reg );
            (*i)->onNewI2CData( reg, buffer, length );
        }
    }
}

bool 
I2CBUS::writeBytes( uint8_t *data, uint32_t bytesToWrite ) {
    AMP_DEBUG_I( "Writing bytes" );
    int ret = i2c_reset_tx_fifo( I2C_NUM_0 );
    if (ret != ESP_OK) {
        AMP_DEBUG_W( "failed to reset fifo" );
    }
    
    ret = i2c_slave_write_buffer( I2C_NUM_0, data, bytesToWrite, 1000 / portTICK_PERIOD_MS );
    if (ret <= 0) {
        AMP_DEBUG_W( "failed to write data" );
    }
 //  vTaskDelay( 100 / portTICK_PERIOD_MS );

    return true;
}
