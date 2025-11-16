#ifndef __MDNS_H__
#define __MDNS_H__

#include <memory>
#include <string>

class MDNS {
    public:
        MDNS();
        void start( const std::string &name, const std::string &desc );
};

typedef std::shared_ptr<MDNS> MDNSPtr;

#endif