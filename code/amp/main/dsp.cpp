#include "dsp.h"
#include "debug.h"

DSP::DSP( uint8_t addr, I2CBUSPtr bus ) : mAddr( addr ), mI2C( bus ) {
    setup();
}

void 
DSP::setup() {
    AMP_DEBUG_I( "Setting up DSP" );

    uint8_t version;
    mI2C->readRegisterByte( mAddr, 100, version );

    AMP_DEBUG_I( "Reported version from DSP is 0x%x", version );
}