#include "ina260.h"
#include "debug.h"

INA260::INA260( uint8_t addr, I2CBUSPtr bus ) : mAddr( addr ), mBus( bus ) {
    setup();
}

float 
INA260::getVoltage() {
    uint8_t data[2];

    mBus->readRegisterBytes( mAddr, INA260::REGISTER_VOLTAGE, 2, data );
    uint16_t voltage = data[1] | ( data[0] << 8 );
   // AMP_DEBUG_I( "Reading voltage %d, value is %u", mAddr, voltage );
    return ((float)voltage)*0.00125;
}

float 
INA260::getCurrent() {
    uint8_t data[2];

    mBus->readRegisterWord( mAddr, INA260::REGISTER_CURRENT, data );
    uint16_t current = data[1] | ( data[0] << 8 );
   // AMP_DEBUG_I( "Reading current %d, value is %u", mAddr, current );
    return ((float)current)*1.25;
}

float 
INA260::getPowerUsage() {
    uint8_t data[2];

    mBus->readRegisterWord( mAddr, INA260::REGISTER_POWER, data );
    uint16_t power = data[1] | (data[0] << 8 );
   // AMP_DEBUG_I( "Reading power %d, value is %u", mAddr, power );
    return ((float)power)*0.010;
}

void
INA260::setup() {
    uint8_t data[2];
    data[0] = 0b01101001;
    data[1] = 0b00100111;

    mBus->writeRegisterBytes( mAddr, INA260::REGISTER_CONFIG, 2, data );
}