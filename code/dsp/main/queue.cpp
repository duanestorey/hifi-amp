#include "queue.h"
#include "debug.h"

Queue::Queue( uint8_t queueSize ) {
    mQueueHandle = xQueueCreate( queueSize, sizeof( Message ) );
}

Queue::~Queue() {
    AMP_DEBUG_I( "Deleting queue handle" );
    vQueueDelete( mQueueHandle );
}

void 
Queue::add( Message msg ) {
    xQueueSendToBack( mQueueHandle, (void *)&msg, 10 );
}

BaseType_t 
Queue::addFromISR( Message msg ) {
    BaseType_t taskWoken;
   // AMP_DEBUG_INT_I( "adding to isr queue" );
    xQueueSendToBackFromISR( mQueueHandle, (void *)&msg, &taskWoken );
   // AMP_DEBUG_INT_I( "finished adding to isr queue" );
    return taskWoken;
}

bool 
Queue::hasMessage() {
    return ( uxQueueMessagesWaiting( mQueueHandle ) > 0 );
}

bool 
Queue::waitForMessage( Message &msg, uint32_t maxTimeInMS ) {
    return ( xQueueReceive( mQueueHandle, (void *)&msg, maxTimeInMS / portTICK_PERIOD_MS ) == pdTRUE );
}

Message 
Queue::getMessage() {
    Message msg;

    xQueueReceive( mQueueHandle, (void *)&msg, 10 );

    return msg;
}