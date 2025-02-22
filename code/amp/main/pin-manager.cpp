#include "pin-manager.h"
#include "pin-esp32.h"
#include "pin-mcp.h"
#include "debug.h"
#include "config.h"
#include "driver/gpio.h"

void 
ESP32_GPIO_ISR( void *arg ) {
    ESP32_IntData *data = (ESP32_IntData *)arg;
    data->mPinManager->handleInterrupt( data->mPin );
}

void 
ESP32_GPIO_ISR_PORTA( void *arg ) {
    AMP_DEBUG_INT_W( "In PORTA Interrupt" );
    ((PinManager *)arg)->_handlePortA();
}

void 
ESP32_GPIO_ISR_PORTB( void *arg ) {
    AMP_DEBUG_INT_W( "In PORTB Interrupt" );
    ((PinManager *)arg)->_handlePortB();
}

PinManager::PinManager( I2CBUSPtr i2c, QueuePtr interruptQueue ) : mInterruptQueue( interruptQueue ), mI2C( i2c ) {
    // set up global interrupts
    gpio_install_isr_service( ESP_INTR_FLAG_LEVEL3 );

    mPinManagerMCP1 = PinMcpManagerPtr( new PinMcpManager( i2c, AMP_I2C_ADDR_MCP_1, interruptQueue ) );
    mPinManagerMCP2 = PinMcpManagerPtr( new PinMcpManager( i2c, AMP_I2C_ADDR_MCP_2, interruptQueue ) );

    configureMCPInterrupts();

    reset();

}

void 
PinManager::reset() {
    // disable reset, enable MCP
    gpio_set_direction( (gpio_num_t)AMP_PIN_MCP_RESET, GPIO_MODE_OUTPUT );
    gpio_set_level( (gpio_num_t)AMP_PIN_MCP_RESET, 0 );
    gpio_set_pull_mode( (gpio_num_t)AMP_PIN_MCP_RESET, GPIO_FLOATING );

    vTaskDelay( 1 / portTICK_PERIOD_MS );
    gpio_set_level( (gpio_num_t)AMP_PIN_MCP_RESET, 1 );
    vTaskDelay( 1 / portTICK_PERIOD_MS );

    mPinManagerMCP1->reset();
    mPinManagerMCP2->reset();
}

PinPtr 
PinManager::createPin( uint8_t pinType, uint8_t pinReference, uint8_t direction, uint8_t pulldown, uint8_t pullup ) {
    PinPtr pin;

    switch( pinType ) {
        case PIN_TYPE_ESP32:
            pin = PinPtr( new PinESP32( this, (gpio_num_t)pinReference, direction, pulldown, pullup ) ); 
            break;
        case PIN_TYPE_MCP1:
            pin = mPinManagerMCP1->createPin( pinReference, direction, pulldown, pullup ); 
            break;
        case PIN_TYPE_MCP2:
            pin = mPinManagerMCP2->createPin( pinReference, direction, pulldown, pullup ); 
            break;    
        default:
            AMP_DEBUG_E( "Trying to create unknown pin type" );
            break;
    }

    return pin;
}

void 
PinManager::enableInterrupt( uint8_t pinReference, uint8_t interruptType ) {
    ESP32_IntData *pinData = new ESP32_IntData( pinReference, this );

    mInterrupts[ pinReference ] = pinData;

    switch( interruptType ) {
        case Pin::PIN_INT_LEADING:
            gpio_set_intr_type( (gpio_num_t)pinReference, GPIO_INTR_NEGEDGE );
            break;
        case Pin::PIN_INT_TRAILING:
            gpio_set_intr_type( (gpio_num_t)pinReference, GPIO_INTR_POSEDGE );
            break;
        case Pin::PIN_INT_BOTH:
            gpio_set_intr_type( (gpio_num_t)pinReference, GPIO_INTR_ANYEDGE );
            break;
    }
    
    gpio_isr_handler_add( (gpio_num_t)pinReference, ESP32_GPIO_ISR, (void *)pinData );
}

void 
PinManager::handleInterrupt( uint8_t pin ) {
    mInterruptQueue->addFromISR( Message::MSG_GPIO_INTERRUPT, (int32_t)PinManager::PIN_TYPE_ESP32, (int32_t)pin );
}

void 
PinManager::configureMCPInterrupts() {
    gpio_config_t io_conf;

    io_conf.intr_type = GPIO_INTR_NEGEDGE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;

    // set up port a interrupt handler
    io_conf.pin_bit_mask = ( ((uint64_t)1) << ( uint64_t ) AMP_PIN_MCP_PORTA );
	gpio_config( &io_conf ); 
    gpio_isr_handler_add( (gpio_num_t)AMP_PIN_MCP_PORTA, ESP32_GPIO_ISR_PORTA, (void *)this );

    // set up port b interrupt handler
    io_conf.pin_bit_mask = ( ((uint64_t)1) << ( uint64_t ) AMP_PIN_MCP_PORTB );
	gpio_config( &io_conf );   
    gpio_isr_handler_add( (gpio_num_t)AMP_PIN_MCP_PORTB, ESP32_GPIO_ISR_PORTB, (void *)this );

    // set up reset button
    io_conf.pin_bit_mask = ( ((uint64_t)1) << ( uint64_t ) AMP_PIN_MCP_RESET );
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    gpio_config( &io_conf );  
}

void 
PinManager::handleMcpPortA() {
    mPinManagerMCP1->processPortAInterrupt();
}

void 
PinManager::handleMcpPortB() {
    mPinManagerMCP2->processPortBInterrupt();
}

void 
PinManager::_handlePortA() {    
    mInterruptQueue->addFromISR( Message::MSG_GPIO_MCP_PORTA );
}

void 
PinManager::_handlePortB() {
    mInterruptQueue->addFromISR( Message::MSG_GPIO_MCP_PORTB ); 
}

