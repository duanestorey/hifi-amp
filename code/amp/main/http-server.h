#ifndef __HTTP_SERVER_H__
#define __HTTP_SERVER_H__

#include "queue.h"
#include <esp_http_server.h>
#include "diagnostics.h"
#include <string>
#include <memory>

class HTTPServer {
public:
    HTTPServer( QueuePtr queue, DiagnosticsPtr diag );

    enum {
        SERVER_MAIN,
        SERVER_POWEROFF,
        SERVER_POWERON,
        SERVER_VOLUME_UP,
        SERVER_VOLUME_DOWN,
        SERVER_VOLUME_UP_5,
        SERVER_VOLUME_DOWN_5,
        SERVER_INPUT_HDMI,
        SERVER_INPUT_BLUERAY,
        SERVER_INPUT_STREAMER,
        SERVER_INPUT_TV,
        SERVER_INPUT_VINYL,
        SERVER_INPUT_GAME,
        SERVER_NOT_FOUND
    };

    void start();
    void stop();

    esp_err_t handleResponse( uint8_t requestType, httpd_req_t *req );
    
protected:
    QueuePtr mQueue;
    httpd_handle_t mServerHandle;
    std::string mMainPage;
    DiagnosticsPtr mDiag;
private:   
    void replaceWithFloat( std::string &str, const char *find, float replace, uint8_t digits = 1 );

    std::string replaceAll( std::string str, const std::string& from, const std::string& to );
};

typedef std::shared_ptr<HTTPServer> HTTPServerPtr;

#endif