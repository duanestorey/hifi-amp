#include "tmp100.h"
#include "debug.h"

TMP100::TMP100( uint8_t address, I2CBUSPtr bus ) : mAddress( address ), mI2C( bus ) {
    mI2C->writeRegisterByte( mAddress, 1, 0x60 );
}

float 
TMP100::readTemperature() {
	uint8_t data[2];

	mI2C->readRegisterBytes( mAddress, 0, 2, data );
	uint16_t ss = ( ( data[0] << 8 ) +  (data[1] & 0xf0 ) ) / 16;
	float f = ss * 0.065f;

//	AMP_DEBUG_I( "Temperature is %0.2f", f );

    return f;
}