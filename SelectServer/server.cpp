/*
    Video: https://stepik.org/lesson/12579/step/2?unit=3005
*/
#include <iostream>
#include <algorithm>
#include <set>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>

int set_nonblock(int fd) 
{
    int flags;
#if defined(O_NONBLOCK)
    if (-1 == (flags = fcntl(fd, F_GETFL, 0))) {
        flags = 0;
    }
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
#else
    flags = 1;
    return ioctl(fd, FIOBIO, &flags);
#endif
}


int main(int argc, char **argv)
{
    int masterSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    std::set<int> slaveSockets;

    // Аналогично server.c в "../TCP_ClientServer/server.c"
    struct sockaddr_in sockAddr;
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons(12345);
    sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(masterSocket, (struct sockaddr *)(&sockAddr), sizeof(sockAddr)); 

    // Set masterSocket non-block
    // without that won't work with "select"
    set_nonblock(masterSocket);

    listen(masterSocket, SOMAXCONN);
    while (true) {
        fd_set set;
        FD_ZERO(&set);
        FD_SET(masterSocket, &set); // adding Master Socket
        for(auto iter = slaveSockets.begin(); iter != slaveSockets.end(); iter++) {
            FD_SET(*iter, &set);
        } // adding Slave Sockets

        // Finding max file descriptor (fd) for setting in "select"
        int max = std::max(masterSocket, *std::max_element(slaveSockets.begin(), slaveSockets.end()));

        // configure for reading, go to sleep until event happens
        select(max + 1, &set, NULL, NULL, NULL);

        for (auto iter = slaveSockets.begin(); iter != slaveSockets.end(); iter++) {
            if (FD_ISSET(*iter, &set)) { // check if socket in "set"
                static char buffer[1024];
                int recvSize = recv(*iter, buffer, 1024, MSG_NOSIGNAL); // read info until we can
                if ((recvSize == 0) && (errno != EAGAIN)) { // check if connection was interrupted
                    shutdown(*iter, SHUT_RDWR);
                    close(*iter);
                    slaveSockets.erase(iter);
                } else if (recvSize != 0) {
                    send(*iter, buffer, recvSize, MSG_NOSIGNAL); // send data back
                }
            }
        }

        if (FD_ISSET(masterSocket, &set)) { // in case select was triggered by Master Socket
            int slaveSocket = accept(masterSocket, 0, 0);
            set_nonblock(slaveSocket);
            slaveSockets.insert(slaveSocket);
        }
    }

    return 0;
}