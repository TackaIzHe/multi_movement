#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include "main.h"

enum hex {
    a = 10,
    b = 11,
    c = 12,
    d = 13,
    e = 14,
    f = 15
};

static void xwininfo(int fd)
{
    char comand_name[] = "xwininfo";
    pid_t pid = fork();
    if (pid == 0)
    {
        char* windows[] = {comand_name, "-root", "-children", "-tree", NULL};
        dup2(fd, 1);
        execvp(comand_name, windows);
    }
    wait(&pid);
    printf("Работа с %s завершена\n", comand_name);
    if (pid != 0)
    {
        perror("Ошибка в процесе получения окон");
        exit(EXIT_FAILURE);
    }
}   

static void grep(int fd_2)
{
    char comand_name[] = "grep";
    pid_t pid = fork();
    if (pid == 0)
    {
        char* windows[] = {comand_name, WINDOW_NAME, BUFFER_FILE_1, NULL};
        dup2(fd_2, 1);
        execvp(comand_name, windows);
    }
    wait(&pid);
    printf("Работа с %s завершена\n", comand_name);
    if (pid != 0)
    {
        perror("Ошибка в процесе получения окон");
        exit(EXIT_FAILURE);
    }
}

static void awk(int *fd_1)
{
    close(*fd_1);
    int fd_2 = open(BUFFER_FILE_1, O_CREAT | O_RDWR | O_TRUNC, 0666);
    if (fd_2 == -1)
    {
        perror("Ошибка открытия буфер файла");
        exit(EXIT_FAILURE);
    }
    *fd_1 = fd_2;
    char comand_name[] = "awk";
    pid_t pid = fork();
    if (pid == 0)
    {
        char* windows[] = {comand_name, "{print $1}", BUFFER_FILE_2, NULL};
        dup2(*fd_1, 1);
        execvp(comand_name, windows);
    }
    wait(&pid);
    printf("Работа с %s завершена\n", comand_name);
    if (pid != 0)
    {
        perror("Ошибка в процесе получения окон");
        exit(EXIT_FAILURE);
    }
}

static unsigned long convert_string_ulong(int fd_1)
{
    unsigned long id = 0;
    // Используеться для цыкла
    int i = 0;
    char zero = '0';
    char buff[100] = {0};
    lseek(fd_1, 2, SEEK_SET);
    if (read(fd_1, buff, 100) == -1)
    {
        perror("Ошибка чтения файла");
        exit(EXIT_FAILURE);
    }
    while(1)
    {
        if (buff[i] == '\n')
        {
            buff[i] = 0;
            break;
        }
        i++;
    }
    printf("%s\n", buff);
    int j = 0;
    for (i = strlen(buff) - 1; i >= 0; i--)
    {
        unsigned long stepen = 1;
        for (int x = 0; x <= j; x++)
        {
            if (x == 0)
            {
                stepen = 1;
                continue;
            }
            stepen = stepen * 16;
        }
        switch (buff[i])
        {
            case 'a':
                id += stepen * a;
                break;
            
            case 'b':
                id += stepen * b;
                break;

            case 'c':
                id += stepen * c;
                break;

            case 'd':
                id += stepen * d;
                break;

            case 'e':
                id += stepen * e;
                break;

            case 'f':
                id += stepen * f;
                break;

            default:
                id += stepen * (buff[i] - zero);
                break;
        }
        j++;
    }
    
    return id;
}

int get_window_id_for_send_evn(unsigned long *window_id)
{
    int fd_1 = open(BUFFER_FILE_1, O_CREAT | O_RDWR | O_TRUNC, 0666);
    if (fd_1 == -1)
    {
        perror("Ошибка открытия буфер файла");
        exit(EXIT_FAILURE);
    }

    int fd_2 = open(BUFFER_FILE_2, O_CREAT | O_RDWR | O_TRUNC, 0666);
    if (fd_2 == -1)
    {
        perror("Ошибка открытия буфер файла");
        exit(EXIT_FAILURE);
    }

    xwininfo(fd_1);
    grep(fd_2);
    awk(&fd_1);
    *window_id = convert_string_ulong(fd_1);

    close(fd_1);
    close(fd_2);
    unlink(BUFFER_FILE_1);
    unlink(BUFFER_FILE_2);
    return 0;
}