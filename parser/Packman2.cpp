//
// George Onoufriou : 03/03/2017.
//

#include "Packman2.h"


// Recursive
/// This function will do everything in its power to make sure that packet is sent and confirmed
void Packman2::packetSend_achnowledgeRecieve(std::vector<char> &data) {

    size_t data_length = data.size();
    socklen_t sizeAdress = sizeof(remoteAddress);
    unsigned char buffer[PAYLOAD_MAX_SIZE];
    buffer[0] = 0;
    if(0 < sendto(thisSocket, data.data(), data_length, 0, (sockaddr*)&remoteAddress, sizeof(remoteAddress)))
    {
        time_t start = time(NULL); // continually ticking up counter, of current time! It does not stop, beware!
        time_t duration = TIME_OUT_DURATION; // E.G. TIME_OUT_DURATION = 15 (seconds)
        time_t end = start + duration;
        std::cout << "- Packet sent: " << data.data() << std::endl;
        while( start < end && // timeout condition
                !recvfrom(thisSocket, buffer, PAYLOAD_MAX_SIZE, 0, (struct sockaddr*)&remoteAddress, &sizeAdress))
        {
            // do nothing && hog CPU. Point of further functionality, like dynamic user controls
        }
        // this if else, just makes sure, that if the message has timed out, that it will try again
        if(buffer[0] == 0){std::cout << " - - Timed out, resending" << std::endl; packetSend_achnowledgeRecieve(data);}
        else              {std::cout << " - - Response: " << buffer << std::endl; }
    }
    else{
        std::cout << "*** Could not send data: " << data.data() << "\n  Likely socket issue." << std::endl;
    }
}



void Packman2::sendData(const std::vector<char> &data) {
    // data is now a character vector that needs to be sent to a receiver
    std::vector<char> packetData(PAYLOAD_MAX_SIZE); // vector of compressed data to send of size max eg 247 bytes

    int requiredPackets = ceil((double)data.size() / packetData.size() ); // calc needed packets
    std::cout << "Total required packets: " << requiredPackets << " = "
              << data.size() << "/" << packetData.size()<< std::endl;

    for(int c = 0; c < requiredPackets; ++c) // packet handler
    {
        int startPos = c * packetData.size();
        for(int i = 0; i < packetData.size(); ++i) // packet filler
        {
            // this is statement overwrites any spare slots with filler values, if packets cant fit exactly
            // preventing instructions from the previous packet passing over. Philosophy = speed.
            if( (i+startPos) < data.size()) {packetData[i] = data[i+startPos];}
            else                            {packetData[i] = ' ';}
        }

        if(DEBUG == true)
        {
            int flag = 1;
            while(flag)
            {
                char inp = getchar();
                if(inp == 'n')
                    flag = 0;
                if(inp == 'q')
                    exit(0);
            }
        }
        std::cout << " - " << c << "/ " << requiredPackets << std::endl;
        packetSend_achnowledgeRecieve(packetData); // pass data to send-reciever
    }
    //std::vector<char> test;
    //for(int i = 0; i < 20; ++i)
    //{
    //    test.push_back('i');
    //}
    //packetSend_achnowledgeRecieve(test);
    //packetSend_achnowledgeRecieve(packetData); // test function call for debug
}



int Packman2::initSocket() {
    // try and assign socket
    if( (thisSocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
    {
        // cry me a river :P
        std::cout << "Could not assign socket. RIP. " << std::endl;
        return 0; // zero is evil, Swiper no swiping.
    }

    // reserving memory for struct
    memset( (char*)&thisAddress, 0, sizeof(thisAddress) );
    // assigning members of struct manually:
    thisAddress.sin_addr.s_addr = htonl(INADDR_ANY); // any ip address it likes
    thisAddress.sin_family = AF_INET; // IPV4
    thisAddress.sin_port = htons(0); // any port it likes

    // try to name / bind this socket now
    if( bind(thisSocket, (struct sockaddr*)&thisAddress, sizeof(thisAddress)) < 0 )
    {
        std::cout << "Could not bind socket." << std::endl;
        return 0;
    }

    // DNS resolver ... need to change this a lil doesnt ever return true
    if( dnsResolver(REMOTE_IP, AF_INET, REMOTE_PORT) )
    {
        std::cout << "could not resolve: " << REMOTE_IP << " : " << REMOTE_PORT << std::endl;
        return 0;
    }

    std::cout << "Socket created, bound, and DNS resolved, huzzah!" << std::endl;
    return 1; // Yay
}



// just manualy changes variables instead of DNS atm, DNS is future expansion
int Packman2::dnsResolver(const char *hostname, int family, int service) {

    memset((char *)&remoteAddress, 0, sizeof(remoteAddress));
    remoteAddress.sin_family = AF_INET;
    remoteAddress.sin_addr.s_addr = inet_addr(hostname); // sankuu arpa
    remoteAddress.sin_port = htons(service);
    return 0; // 0 = success, 1 = failure, currently never fails
}
