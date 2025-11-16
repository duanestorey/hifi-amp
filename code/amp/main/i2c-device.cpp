#include "i2c-device.h"
#include "debug.h"

I2C_Device::I2C_Device( i2c_master_dev_handle_t handle, bool deviceExists ) : mHandle( handle ), mExists( deviceExists ) {
    mBuffer.reserve( 64 );
}

void 
I2C_Device::checkBufferSize( size_t bufferSize ) {
    // Make sure our std::vector has enough room
    if ( mBuffer.size() < bufferSize ) {
        mBuffer.resize( bufferSize );
    } 
}

bool 
I2C_Device::writeRegisterByte( uint8_t reg, uint8_t data ) {
    uint8_t toWrite[2] = { reg, data };

    esp_err_t err = i2c_master_transmit( mHandle, toWrite, sizeof( toWrite ), I2C_DEVICE_TIMEOUT );
    if ( err != ESP_OK ) {
        AMP_DEBUG_E( "Failed to write to register 0x%x", reg );     
    }

    return ( err == ESP_OK );
}

bool 
I2C_Device::writeRegisterBytes( uint8_t reg, uint8_t dataSize, uint8_t *data ) {
    size_t totalSize = 1 + dataSize;

    checkBufferSize( totalSize );

    mBuffer[ 0 ] = reg;
    memcpy( &mBuffer[ 1 ], data, dataSize );

    esp_err_t err = i2c_master_transmit( mHandle, &mBuffer[0], totalSize, I2C_DEVICE_TIMEOUT );
    if ( err != ESP_OK ) {
        AMP_DEBUG_E( "Failed to write to register 0x%x", reg );     
    }

    return ( err == ESP_OK );  
}

bool 
I2C_Device::writeBytes( uint8_t *data, uint8_t size ) {
    esp_err_t err = i2c_master_transmit( mHandle, data, size, I2C_DEVICE_TIMEOUT );
    if ( err != ESP_OK ) {
        AMP_DEBUG_E( "Failed to write data of size %d", size );;     
    }

    return ( err == ESP_OK ); 
}

bool 
I2C_Device::readRegisterByte( uint8_t reg, uint8_t &data  ) {
    return readRegisterBytes( reg, 1, &data );   
}

bool 
I2C_Device::readRegisterBytes( uint8_t reg, uint8_t dataSize, uint8_t *data  ) {
    esp_err_t err = i2c_master_transmit_receive( mHandle, &reg, 1, data, dataSize, I2C_DEVICE_TIMEOUT );
    if ( err != ESP_OK ) {
        AMP_DEBUG_E( "Failed to read from register 0x%x", reg );     
    }

    return ( err == ESP_OK );   
}

bool 
I2C_Device::readRegisterWord( uint8_t reg, uint8_t *data ) {
    return readRegisterBytes( reg, 2, data ); 
}