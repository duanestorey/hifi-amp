#ifndef __ADC_PCM1863_H__
#define __ADC_PCM1863_H__

#include "config.h"
#include "i2c-bus.h"
#include <memory>

class PCM1863 {
    public:
        enum {
            SAMPLING_RATE_48K = 0,
            SAMPLING_RATE_96K = 1
        };
        
        PCM1863( uint8_t addr, I2CBUSPtr bus, uint8_t samplingRate = SAMPLING_RATE_96K );
        virtual void init();
    protected:
        uint8_t mAddr;
        I2CBUSPtr mI2C;
        uint8_t mSamplingRate;
};

typedef std::shared_ptr<PCM1863> PCM1863Ptr;

#endif 