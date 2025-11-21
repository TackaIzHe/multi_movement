#include <X11/Xlib.h>
#include <unistd.h>
#include "main.h"

void free_proc_data(Display* display, Window win)
{
    XUnmapWindow(display, win);
    XDestroyWindow(display, win);
    XCloseDisplay(display);
    close(sock_fd);
}