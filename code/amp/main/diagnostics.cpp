#include "diagnostics.h"
#include "debug.h"

Diagnostics::Diagnostics() {

}

void 
Diagnostics::addPowerSensor( const std::string &str, PowerSensorPtr sensor ) {
    mPowerSensors[ str ] = sensor;
}

void 
Diagnostics::addTemperatureSensor(  const std::string &str, TempSensorPtr sensor ) {
    mTempSensors[ str ] = sensor;
}

void 
Diagnostics::dumpAllTemperatures() {
    for ( std::map<std::string, TempSensorPtr>::iterator i = mTempSensors.begin(); i != mTempSensors.end(); i++ ) {
        AMP_DEBUG_I( "Temperature of [%s] is [%0.2f]", i->first.c_str(), i->second->readTemperature() );
    }
}

void 
Diagnostics::dumpAllPower() {
    for ( std::map<std::string, PowerSensorPtr>::iterator i = mPowerSensors.begin(); i != mPowerSensors.end(); i++ ) {
        AMP_DEBUG_I( "Power of [%s] is [%0.2f]", i->first.c_str(), i->second->getPowerUsage() );
    }
}

float 
Diagnostics::getVoltage( const std::string &str ) {
    float voltage = 0;

    std::map<std::string, PowerSensorPtr>::iterator i = mPowerSensors.find( str );
    if ( i != mPowerSensors.end() ) {
        voltage = i->second->getVoltage();
    }

    return voltage;
}

float 
Diagnostics::getCurrent( const std::string &str ) {
    float current = 0;

    std::map<std::string, PowerSensorPtr>::iterator i = mPowerSensors.find( str );
    if ( i != mPowerSensors.end() ) {
        current = i->second->getCurrent();
    }

    return current;
}

float 
Diagnostics::getPower( const std::string &str ) {
    float power = 0;

    std::map<std::string, PowerSensorPtr>::iterator i = mPowerSensors.find( str );
    if ( i != mPowerSensors.end() ) {
        power = i->second->getPowerUsage();
    }

    return power;
}

float 
Diagnostics::getTemperature( const std::string &str ) {
    float temp = 0;

    std::map<std::string, TempSensorPtr>::iterator i = mTempSensors.find( str );
    if ( i != mTempSensors.end() ) {
        temp = i->second->readTemperature();
    }

    return temp;
}