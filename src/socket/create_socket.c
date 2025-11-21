#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>

#include "socket.h"
struct sockaddr_in multicast_sock;


int create_socket(int* fd)
{
    
    multicast_sock.sin_addr.s_addr = inet_addr(ADDR);
    multicast_sock.sin_family = AF_INET;
    multicast_sock.sin_port = htons(PORT); 
    
    *fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (*fd == -1)
    {
        perror("Ошибка создания мультикаст сокета");
        exit(EXIT_FAILURE);
    }
    return 0;
}