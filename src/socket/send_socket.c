#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "socket.h"

int send_socket(int fd, char *buff, int len)
{
    int sock_len = sizeof(multicast_sock);
    if (sendto(fd, buff, len, 0, (struct sockaddr*)&multicast_sock, sock_len) == -1)
    {
        perror("Ошибка отпраки сообщения в очередь");
        close(fd);
        return -1;
    }
    return 0;
}