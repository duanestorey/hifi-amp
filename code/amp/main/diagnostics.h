#ifndef __DIAGNOSTICS_H__
#define __DIAGNOSTICS_H__

#include <map>
#include <string>
#include <memory>
#include "abstract/temp-sensor.h"
#include "abstract/power-sensor.h"

class Diagnostics {
    public:
        Diagnostics();
        void addTemperatureSensor( const std::string &str, TempSensorPtr sensor );
        void addPowerSensor( const std::string &str, PowerSensorPtr sensor );

        void dumpAllTemperatures();
        void dumpAllPower();

        float getTemperature( const std::string &str );
        float getPower( const std::string &str );
        float getVoltage( const std::string &str );
        float getCurrent( const std::string &str );
    protected:
        std::map<std::string, TempSensorPtr> mTempSensors;
        std::map<std::string, PowerSensorPtr> mPowerSensors;
};

typedef std::shared_ptr<Diagnostics> DiagnosticsPtr;

#endif