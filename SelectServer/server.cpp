/*
    Video: https://stepik.org/lesson/12576/step/12?unit=3004
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

    struct sockaddr_in sockAddr;
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons(12345);
    sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(masterSocket, (struct sockaddr *)(&sockAddr), sizeof(sockAddr));

    set_nonblock(masterSocket);

    listen(masterSocket, SOMAXCONN);
    while (true) {
        fd_set set;
        FD_ZERO(&set);
        FD_SET(masterSocket, &set);
        for(auto iter = slaveSockets.begin(); iter != slaveSockets.end(); iter++) {
            FD_SET(*iter, &set);
        }

        int max = std::max(masterSocket, *std::max_element(slaveSockets.begin(), slaveSockets.end()));

        select(max + 1, &set, NULL, NULL, NULL);

        for (auto iter = slaveSockets.begin(); iter != slaveSockets.end(); iter++) {
            if (FD_ISSET(*iter, &set)) {
                static char buffer[1024];
                int recvSize = recv(*iter, buffer, 1024, MSG_NOSIGNAL);
                if (recvSize == 0) && (errno != EAGAIN) {
                    shutdown(*iter, SHUT_RDWR);
                    close(*iter);
                    slaveSockets.erase(iter);
                } else if (recvSize != 0) {
                    send(*iter, buffer, recvSize, MSG_NOSIGNAL);
                }
            }
        }

        if (FD_ISSET(masterSocket, &set)) {
            int slaveSocket = accept(masterSocket, 0, 0);
            set_nonblock(slaveSocket);
            slaveSockets.insert(slaveSocket);
        }
    }

    return 0;
}