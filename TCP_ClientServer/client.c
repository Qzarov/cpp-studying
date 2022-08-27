
/*
    Video: https://stepik.org/lesson/12576/step/12?unit=3004
*/
#include <stdio.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char **argv)
{
    int MasterSocket = socket(
        AF_INET,        // IPv4
        SOCK_STREAM     // TCP
        IPPROTO_TCP
        );

    struct sockaddr_in SockAddr;
    SockAddr.sin_family = AF_INET;
    SockAddr.sin_port = htons(12345);
    SockAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // 127.0.0.1
        
    connect(Socket, (struct sockaddr *)(&SockAddr), sizeof(SockAddr));

    char Buffer[] = "PING";
    send(Socket, Buffer, 4, MSG_NOSIGNAL);
    recv(Socket, Buffer, 4, MSG_NOSIGNAL);

    shutdown(Socket, SHUT_RDWR);
    close(Socket);

    printf("%s\n", Buffer);

    return 0;
}