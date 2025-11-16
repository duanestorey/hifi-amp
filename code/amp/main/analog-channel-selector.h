#ifndef __ANALOG_CHANNEL_SELECTOR_H__
#define __ANALOG_CHANNEL_SELECTOR_H__

#include "button.h"
#include "pin-manager.h"
#include "i2c-bus.h"
#include "config.h"
#include <memory>

class AnalogChannelSelector {
    public:
        AnalogChannelSelector( I2CBUSPtr i2c, PinManagerPtr pinManager );
        void selectChannel( uint8_t channel );
        void enable( bool enable );

    protected:
        PinPtr mEnable1;
        PinPtr mEnable2;
        PinPtr mEnable3;

        I2CBUSPtr mI2C;

        bool mEnabled;
        uint8_t mCurrentChannel;
    private:
        void disableAll();
        void maybeActivateChannel();
};

typedef std::shared_ptr<AnalogChannelSelector> AnalogChannelSelectorPtr;

#endif