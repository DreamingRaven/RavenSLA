//
// George Onoufriou : 15/02/2017.
//

// !!!! Depreciated, this will be later swap its name with Packman2, where it will also be deleted

#include "Packman.h"

Packman::Packman() {

}

int Packman::init() {
    int tempBuffer = 0;
    addrListen.sin_family = AF_INET; // AF_INET = int 2 in cygwin

    //setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, "wlan1", 5);

    tempBuffer = bind(sock, (sockaddr*)&addrListen, sizeof(addrListen));
    if(tempBuffer == -1)
    {
        std::cout << "error: " << errno << std::endl;
        return 1;
    }
    tempBuffer = resolveHelper("192.168.1.104", AF_INET, "2391", &addrDest);
    if(tempBuffer != 0)
    {
        std::cout << "resolver error: " << errno << std::endl;
        return 1;
    }
    sendPacket(); // this is an opening packet, to init the sequence niceley & for quick tests
    return 0;
}

int Packman::resolveHelper(const char *hostname, int family, const char *service, sockaddr_storage *pAddr) {
    int result = 0;
    addrinfo* result_list = NULL;
    addrinfo hints = {};
    hints.ai_family = family;
    hints.ai_socktype = SOCK_DGRAM;

    result = getaddrinfo(hostname, service, &hints, &result_list);
    if(result == 0)
    {
        memcpy(pAddr, result_list->ai_addr, result_list->ai_addrlen);
        freeaddrinfo(result_list);
    }
    return result;
}

int Packman::sendPacket() {
    //std::cout << "SOCK_RAW: " << SOCK_RAW << " SOCK_DGRAM: " << SOCK_DGRAM << std::endl;
    const char* msg = "dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd";
    char reply[255] = "";
    int reply_size = 255;
    size_t msg_length = strlen(msg);
    int tempBuffer = sendto(sock, msg, msg_length, 0, (sockaddr*)&addrDest, sizeof(addrDest));
    std::cout << tempBuffer << "bytes sent" << std::endl;
    return tempBuffer;
}

int Packman::sendMsg(std::vector<char> &data){
    for(int i = 0; i < data.size(); i += 400) // this loop is the packet build & sender
    {
        char msg[200] = "";
        for(int j = i; (j < i + 400) && (j < data.size()); j+= 2) // this is the encoder and concatenator
        {

        }
    }
    return 0;
}
