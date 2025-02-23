#ifndef __I2CBUS_H__
#define __I2CBUS_H__

#include "config.h"
#include "driver/i2c_slave.h"
#include "esp_attr.h"
#include "queue.h"
#include "i2c-listener.h"
#include <vector>

#define I2C_BUF_LEN     128

typedef std::vector<I2C_Listener *> Listeners;

class I2CBUS {
public:
    I2CBUS( uint8_t slaveAddr, QueuePtr queue  );

    bool writeBytes( uint8_t *data, uint32_t bytesToWrite );
    void processData( uint8_t *buffer, uint32_t length );
    bool readData( uint8_t *&buffer, uint8_t &length, uint16_t maxLength = 1, uint16_t timeout = 1000 );
    void addEventListener( I2C_Listener *listener ) { mListeners.push_back( listener ); }
protected:
    uint8_t mReceiveBuffer[ I2C_BUF_LEN ];
    uint8_t mSendBuffer[ I2C_BUF_LEN ];

    QueuePtr mQueue;
    uint8_t mSlaveAddr;
    i2c_slave_dev_handle_t mBusHandle;
    Listeners mListeners;
private:

};

#endif