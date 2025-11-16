#ifndef __I2C_DEVICE_H__
#define __I2C_DEVICE_H__

#include <memory>
#include "driver/i2c_master.h"
#include <vector>

#define I2C_DEVICE_TIMEOUT -1

class I2C_Device {
public:
    I2C_Device( i2c_master_dev_handle_t handle, bool deviceExists );

    bool writeRegisterByte( uint8_t reg, uint8_t data );
    bool writeRegisterBytes( uint8_t reg, uint8_t dataSize, uint8_t *data );
    bool writeBytes( uint8_t *data, uint8_t size );
    bool readRegisterByte( uint8_t reg, uint8_t &data  );
    bool readRegisterBytes(  uint8_t reg, uint8_t dataSize, uint8_t *data  );
    bool readRegisterWord( uint8_t reg, uint8_t *data );

    bool exists() const { return mExists; }

protected:
    i2c_master_dev_handle_t mHandle;
    bool mExists;

    std::vector<uint8_t> mBuffer;

    void checkBufferSize( size_t bufferSize );
};

typedef std::shared_ptr<I2C_Device> I2C_Device_Ptr;

#endif