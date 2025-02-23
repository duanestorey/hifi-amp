#ifndef __DSP_H__
#define __DSP_H__

#include "config.h"
#include "i2c-bus.h"
#include <memory>

class DSP {
public:
    DSP( uint8_t addr, I2CBUSPtr bus );

protected:
    uint8_t mAddr;
    I2CBUSPtr mI2C;
private:
    void setup();
};

typedef std::shared_ptr<DSP> DSPPtr;

#endif