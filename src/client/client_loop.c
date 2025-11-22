#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/Xatom.h>
#include <X11/extensions/XTest.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <pthread.h>
#include <string.h>
#include <malloc.h>
#include "client.h"
#include "../main.h"
#include "../socket/socket.h"

extern Display *display;

void* get_evn(void* src_arg)
{
    pthread_arg* arg = (pthread_arg*)src_arg;
    const int buff_len = 1024;
    char buff[buff_len];
    memset(buff, 0, buff_len);

    connect_socket(&sock_fd);
    while(1)
    {
        event e;
        receive_socket(sock_fd, buff, buff_len);
        memcpy(&e, buff, sizeof(event));
        if (e.evnt_type == KeyPress)
        {
            if (e.button == 14)
            {
                break;
            }
            XTestFakeKeyEvent(arg->Display,e.button, True, 0);
            printf("KeyPress: keycode=%d, x=%d, y=%d\n", e.button, e.x, e.y);

        }
        else if (e.evnt_type == KeyRelease)
        {
            XTestFakeKeyEvent(arg->Display,e.button, False, 0);
            printf("KeyPress: keycode=%d, x=%d, y=%d\n", e.button, e.x, e.y);
        }
        else if (e.evnt_type == ButtonPress)
        {
            XTestFakeButtonEvent(arg->Display, e.button, True, 0);
            printf("KeyPress: keycode=%d, x=%d, y=%d\n", e.button, e.x, e.y);
        }
        else if (e.evnt_type == ButtonRelease)
        {
            XTestFakeButtonEvent(arg->Display, e.button, False, 0);
            printf("KeyPress: keycode=%d, x=%d, y=%d\n", e.button, e.x, e.y);
        }
        else if (e.evnt_type == MotionNotify)
        {
            XTestFakeMotionEvent(arg->Display, DefaultScreen(arg->Display), e.x, e.y, 0);
            printf("KeyPress: keycode=%d, x=%d, y=%d\n", e.button, e.x, e.y);
        }
        XFlush(arg->Display);
        usleep(10);
    }
    return 0;
}

int main_loop(GC *gc, XEvent *evn, Window *win)
{
    Window w = target_window;
    
    pthread_t pid;
    
    pthread_arg *arg = malloc(sizeof(pthread_arg));
    arg->Display = display;
    arg->evn = evn;
    arg->win = *win;
    pthread_create(&pid, NULL, get_evn, (void*)arg);


    // Цыкл Обработки событий
    while (!XNextEvent(display, evn))
    {
        switch (evn[0].type)
        {
        case Expose:
            // События которые происходят при создании окна
            Atom opacity_atom = XInternAtom(display, "_NET_WM_WINDOW_OPACITY", False);
            unsigned long opacity = 0x00000001; // 25%
            XChangeProperty(display, *win, opacity_atom, XA_CARDINAL, 32, PropModeReplace,
                (unsigned char *)&opacity, 1);
            break;
        }
    }
}