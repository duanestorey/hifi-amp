#ifndef __INA260_H__
#define __INA260_H__

#include "abstract/power-sensor.h"
#include "i2c-bus.h"
#include "config.h"

class INA260 : public PowerSensor {
public:
    enum {
        REGISTER_CONFIG = 0x00,
        REGISTER_CURRENT = 0x01,
        REGISTER_VOLTAGE = 0x02,
        REGISTER_POWER = 0x03
    };

    INA260( uint8_t addr, I2CBUSPtr bus );

    virtual float getVoltage();
    virtual float getCurrent();
    virtual float getPowerUsage();
private:
    uint8_t mAddr;
    I2CBUSPtr mBus;

    void setup();
};

#endif