#ifndef __I2CBUS_H__
#define __I2CBUS_H__

#include "mutex.h"
#include "config.h"
#include <memory>
#include "driver/i2c_master.h"

#include "i2c-device.h"

class I2CBUS {
public:
    I2CBUS();

    I2C_Device_Ptr bindAddress( uint8_t addr );

    void scanBus();
protected:
    i2c_master_bus_handle_t mBusHandle;
private:
    Mutex mMutex;
};

typedef std::shared_ptr<I2CBUS> I2CBUSPtr;

#endif