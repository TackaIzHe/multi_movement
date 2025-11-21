#include <X11/Xlib.h>
#include <stdlib.h>
#include <stdio.h>
#include "../main.h"
#include "../socket/socket.h"

Display *display;
static int screen;
static Window root_window;
int sock_fd;

Window target_window;

int main()
{
    const char title[50] = "Мой новый заголовок окна";
    Window win;
    XEvent evn;
    GC gc;
    
    get_window_id_for_send_evn(&target_window);
    printf("Выбраное окно для взаимодействия 0x%x\n", target_window);
    
    create_window(&display, &screen, &root_window, &win);
    XStoreName(display, win, title);

    // printf_children_window(&display, &root_window);

    create_socket(&sock_fd);

    main_loop(&gc, &evn, &win);
    
    free_proc_data(display,win);
    exit(EXIT_SUCCESS);
}