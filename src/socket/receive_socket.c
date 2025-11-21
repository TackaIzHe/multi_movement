#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include "socket.h"

int receive_socket(int fd, char *buff, int len)
{
    if (recvfrom(fd, buff, len, 0, 0, 0) == -1)
    {
        perror("Ошибка получения сообщения от multicast адреса");
        close(fd);
        return -1;
    }
    return 0;
}