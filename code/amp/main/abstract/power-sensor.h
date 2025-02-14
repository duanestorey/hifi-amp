#ifndef __POWER_SENSOR_H__
#define __POWER_SENSOR_H__

#include <memory>

class PowerSensor {
public:
    virtual float getVoltage() = 0;
    virtual float getCurrent() = 0;
    virtual float getPowerUsage() = 0;
};

typedef std::shared_ptr<PowerSensor> PowerSensorPtr;

#endif