#include "mdns.h"
#include "mdns-net.h"
#include "debug.h"

MDNS::MDNS() {
    esp_err_t err = mdns_init();
    if ( err ) {
        AMP_DEBUG_E( "MDNS Init failed: %d\n", err );

    }
}

void
MDNS::start(  const std::string &name, const std::string &desc ) {
    //set hostname
    AMP_DEBUG_I( "Starting mDNS Service" );
    mdns_hostname_set( name.c_str() ) ;
    mdns_instance_name_set( desc.c_str() );
    mdns_service_add( NULL, "_http", "_tcp", 80, NULL, 0 );
}