#ifndef _SOCKET_H_
#define _SOCKET_H_

#define PORT 5050
#define ADDR "224.255.255.255"
extern struct sockaddr_in multicast_sock;

// Используеться для настройки мультикаста и бинда адреса
int connect_socket(int* fd);

// Используеться для создания мультикаст сокета для сервера
int create_socket(int* fd);

// Использует клиент для получения событий
int receive_socket(int fd, char *buff, int len);

// Используеться сервером для отправки события
int send_socket(int fd, char *buff, int len);

#endif