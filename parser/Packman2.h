//
// George Onoufriou : 03/03/2017.
//

#ifndef PARSER_PACKMAN2_H
#define PARSER_PACKMAN2_H

#define REMOTE_IP           "192.168.1.104" // Replace with YOUR static esp8266 address
#define REMOTE_PORT         2391
#define PAYLOAD_MAX_SIZE    247
#define TIME_OUT_DURATION   15
#define DEBUG               false

#include <iostream>
#include <sys/socket.h>
//#include <cygwin/in.h>
#include <netdb.h>
#include <cstring>
#include <ifaddrs.h>
#include <vector>
#include <cmath>
#include <arpa/inet.h>

class Packman2 {
private:
    int thisSocket;                     // the socket number
    struct sockaddr_in thisAddress;     // address of this machine
    struct sockaddr_in remoteAddress;   // address of destination e.g RavenSLA printer
protected:
    void packetSend_achnowledgeRecieve(std::vector<char> &data);
    int dnsResolver(const char *hostname, int family, int service);
public:
    void sendData(const std::vector<char> &data);
    int initSocket();
};

#endif //PARSER_PACKMAN2_H