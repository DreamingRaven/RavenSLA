//
// George Onoufriou : 15/02/2017.
//
//#include <winsock2.h> // issue with windows being stupid
#include <iostream>
#include <sys/socket.h>
#include <cygwin/in.h> // maybe not correct
#include <netdb.h>
#include <cstring>
#include <ifaddrs.h>
#include <vector>

#ifndef PARSER_PACKMAN_H
#define PARSER_PACKMAN_H

class Packman {
    // packet list
private:
    //int result = 0;
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    //char szIP[100];
    sockaddr_in addrListen = {};
    sockaddr_storage addrDest = {};
protected:
    int resolveHelper(const char* hostname, int family, const char* service, sockaddr_storage* pAddr);
    int sendPacket();
    //int sendPacket();
public:
    Packman();
    int init();
    int sendMsg(std::vector<char> &data);
};


#endif //PARSER_PACKMAN_H
