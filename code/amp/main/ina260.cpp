#include "ina260.h"
#include "debug.h"

INA260::INA260( uint8_t addr, I2CBUSPtr bus ) : mAddr( addr ) {
    mBus = bus->bindAddress( addr );

    setup();
    uint8_t data[2];

    if ( mBus->readRegisterWord( INA260::REGISTER_ID, data ) ) {
        AMP_DEBUG_I( "Starting INA260 code for addr %x, ID is %x %x", addr, data[0], data[1] );
    }
}

float 
INA260::getVoltage() {
    uint8_t data[2];

    if ( mBus->readRegisterBytes( INA260::REGISTER_VOLTAGE, 2, data ) ) {
        AMP_DEBUG_I( "Voltage Values %d, %d", data[1], data[0] );
         uint16_t voltage = (uint16_t)data[1] | ( (uint16_t)data[0] << 8 );
        return ((float)voltage)*0.00125;
    } else {
        return 0;
    }
}

float 
INA260::getCurrent() {
    uint8_t data[2];

    if ( mBus->readRegisterWord( INA260::REGISTER_CURRENT, data ) ) {
        AMP_DEBUG_I( "Current Values %d, %d", data[1], data[0] );
        int16_t current = (int16_t)data[1] | ( ((int16_t)data[0]) << 8 );
        return ((float)current)*1.25;
    } else {
        return 0;
    }
}

float 
INA260::getPowerUsage() {
    uint8_t data[2];

    if ( mBus->readRegisterWord( INA260::REGISTER_POWER, data ) ) {
        uint16_t power = data[1] | (data[0] << 8 );
        return ((float)power)*0.010;
    } else {
        return 0;
    }
}

void
INA260::setup() {
    uint8_t data[2];
    data[0] = 0b01101101;
    data[1] = 0b11100111;

    mBus->writeRegisterBytes( INA260::REGISTER_CONFIG, 2, data );
}