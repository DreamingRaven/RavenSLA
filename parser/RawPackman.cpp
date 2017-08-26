//
// George Onoufriou : 26/02/2017.
//

#include "RawPackman.h"
#include <iostream>
#include <sys/socket.h>
#include <cygwin/in.h> // maybe not correct
#include <cstring>
#include <sys/ioctl.h>


void RawPackman::init() {
    if( (socketRaw = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) == -1 )
    {std::cout << "dunFKedup" << std::endl;}

    struct ifreq if_idx;
    memset(&if_idx, 0, sizeof(struct ifreq));
    strncpy(if_idx.ifr_name, "eth0", IFNAMSIZ-1);
    if (ioctl(socketRaw, SIOCGIFINDEX, &if_idx) < 0)
    {
        perror("SIOCGIFINDEX");
    }
    std::cout << if_idx.ifr_ifindex << std::endl;
}
