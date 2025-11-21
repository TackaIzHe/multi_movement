#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "socket.h"


int connect_socket(int* fd)
{
    struct ip_mreqn mreqn;
    struct sockaddr_in multicast_sock;
    
    mreqn.imr_multiaddr.s_addr = inet_addr(ADDR);
    mreqn.imr_address.s_addr = htonl(INADDR_ANY);
    mreqn.imr_ifindex = 0;
    
    multicast_sock.sin_family = AF_INET;
    multicast_sock.sin_addr.s_addr = htonl(INADDR_ANY);
    multicast_sock.sin_port = htons(PORT);
    
    *fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (*fd == -1)
    {
        perror("Ошибка создания сокета");
        exit(EXIT_FAILURE);
    }
    
    if (setsockopt(*fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreqn, sizeof(mreqn)) == -1)
    {
        perror("Ошибка утановки опции для мультикаста");
        close(*fd);
        exit(EXIT_FAILURE);
    }
    
    if (bind(*fd, (struct sockaddr*)&multicast_sock, sizeof(multicast_sock)) == -1)
    {
        perror("Ошибка binda адреса для сокета");
        close(*fd);
        exit(EXIT_FAILURE);
    }

}