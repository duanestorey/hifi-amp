#include "analog-channel-selector.h"
#include "debug.h"

AnalogChannelSelector::AnalogChannelSelector( I2CBUSPtr i2c, PinManagerPtr pinManager ) : mI2C( i2c ), mEnabled( false ), mCurrentChannel( 0 ) {
    mEnable1 = pinManager->createPin( PinManager::PIN_TYPE_ESP32, GPIO_NUM_36, Pin::PIN_TYPE_OUTPUT, Pin::PIN_PULLDOWN_DISABLE, Pin::PIN_PULLUP_DISABLE );
    mEnable2 = pinManager->createPin( PinManager::PIN_TYPE_ESP32, GPIO_NUM_37, Pin::PIN_TYPE_OUTPUT, Pin::PIN_PULLDOWN_DISABLE, Pin::PIN_PULLUP_DISABLE );
    mEnable3 = pinManager->createPin( PinManager::PIN_TYPE_ESP32, GPIO_NUM_38, Pin::PIN_TYPE_OUTPUT, Pin::PIN_PULLDOWN_DISABLE, Pin::PIN_PULLUP_DISABLE );
}

void 
AnalogChannelSelector::selectChannel( uint8_t channel ) {
    disableAll();

    if ( channel <= 2 ) {
        mCurrentChannel = channel;
    }

    maybeActivateChannel();
}

void 
AnalogChannelSelector::maybeActivateChannel() {
    if ( mEnabled ) {
        AMP_DEBUG_I( "Activing output relay on channel %d", mCurrentChannel );
        switch( mCurrentChannel ) {
            case 0:
                mEnable1->enable();
                break;
            case 1:
                mEnable2->enable();
                break;
            case 2:
                mEnable3->enable();
                break;
        }
    }
}

void 
AnalogChannelSelector::enable( bool enable ) {
    if ( mEnabled ) {
        if ( !enable ) {
            disableAll();
        }

        mEnabled = enable;
    } else {
        mEnabled = enable;
        
        if ( enable ) {
            maybeActivateChannel();
        }
    }

   
}

void 
AnalogChannelSelector::disableAll() {
    mEnable1->disable();
    mEnable2->disable();
    mEnable3->disable();
}