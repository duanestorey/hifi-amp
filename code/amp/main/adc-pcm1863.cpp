#include "adc-pcm1863.h"
#include "debug.h"

PCM1863::PCM1863( uint8_t addr, I2CBUSPtr bus, uint8_t samplingRate ) : mAddr( addr ), mI2C( bus ), mSamplingRate( samplingRate ) {
    init();
}

void 
PCM1863::init() {
    AMP_DEBUG_I( "Initializing ADC into master mode" );
    // select page 0
    mI2C->writeRegisterByte( mAddr, 0, 0 );

    // set clock master
    mI2C->writeRegisterByte( mAddr, 32, 16 );

    // set GPIO0 to output SCLK
    mI2C->writeRegisterByte( mAddr, 16, 3 );

    if ( mSamplingRate == SAMPLING_RATE_48K ) {
        // set SCLK to BLCK ratio, should select 512Fs @ 48kHz
        mI2C->writeRegisterByte( mAddr, 38, 7 );
    } else if ( mSamplingRate == SAMPLING_RATE_96K ) {
        // set SCLK to BLCK ratio, should select 256Fs @ 96kHz
        mI2C->writeRegisterByte( mAddr, 38, 3 );
    }
}