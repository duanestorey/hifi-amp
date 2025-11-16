#include "pin-esp32.h"
#include "pin-manager.h"
#include "debug.h"

PinESP32::PinESP32( PinManager *pinManager, gpio_num_t pin, uint8_t direction, uint8_t pulldown, uint8_t pullup ) : Pin( direction, pulldown, pullup ), mPinManager( pinManager ), mPin( pin ) {
    config( direction, pulldown, pullup );
}

void 
PinESP32::enableInterrupt( uint8_t interruptType ) {
    Pin::enableInterrupt( interruptType );

    gpio_int_type_t intType = GPIO_INTR_DISABLE;
    switch( interruptType ) {
        case Pin::PIN_INT_LEADING:
            intType = GPIO_INTR_POSEDGE;
            break;
        case Pin::PIN_INT_TRAILING:
            intType = GPIO_INTR_NEGEDGE;
            break;
        case Pin::PIN_INT_BOTH:
            intType = GPIO_INTR_ANYEDGE;
            break;
        default:
            break;

    }

    gpio_set_intr_type( mPin, intType );

    mPinManager->enableInterrupt( mPin, interruptType );

    gpio_intr_enable( mPin );
}

void 
PinESP32::config( uint8_t direction, uint8_t pulldown, uint8_t pullup ) {
    AMP_DEBUG_I( "...setting up pin %d", mPin );


    switch( direction ) {
        case Pin::PIN_TYPE_OUTPUT:
            gpio_set_direction( mPin, GPIO_MODE_OUTPUT );
            break;
        case Pin::PIN_TYPE_INPUT:
            gpio_set_direction( mPin, GPIO_MODE_INPUT );
            break;   
    }

    switch( pulldown ) {
        case Pin::PIN_PULLDOWN_ENABLE:
            gpio_pulldown_en( mPin );
            break;
        case Pin::PIN_PULLDOWN_DISABLE:
            gpio_pulldown_dis( mPin );
            break;   
    }

    switch( pullup ) {
        case Pin::PIN_PULLUP_ENABLE:
            gpio_pullup_en( mPin );
            break;
        case Pin::PIN_PULLUP_DISABLE:
            gpio_pullup_dis( mPin );
            break;   
    }
}

void 
PinESP32::setState( uint8_t state ) {
    AMP_DEBUG_I( "Setting PIN %d to %d", mPin, state );
    if ( state == Pin::PIN_STATE_LOW ) {
        gpio_set_level( mPin, 0 );
    } else if ( state == Pin::PIN_STATE_HIGH ) {
        gpio_set_level( mPin, 1 );
    }
}

uint8_t 
PinESP32::getState() const {
    return gpio_get_level( mPin );
}

std::string 
PinESP32::getClass() const {
    return "ESP32";
}

uint8_t 
PinESP32::getPinID() const {
    return (uint8_t)mPin;
}