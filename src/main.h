#ifndef _MAIN_H_
#define _MAIN_H_

#include <X11/Xlib.h>

#define POSx 0
#define POSy 0
#define HEIGHT 1080
#define WIDTH 1920
#define BORDER 0
#define BUFFER_FILE_1 "buffer_file_for_window_list_1"
#define BUFFER_FILE_2 "buffer_file_for_window_list_2"
#define WINDOW_NAME "Albion Online Client"//"tackaizhe@archlinux:~"//"1442x931+0+0"

struct event
{
    int evnt_type;
    int button;
    int is_press;
    int x;
    int y;
} typedef event;


char print_press_key(XEvent evn);
int create_window(Display **display, int *screen, Window *root, Window *win);
int main_loop(GC *gc, XEvent *evn, Window *win);
Window printf_children_window(Display **display, Window *root);
int print_string_to_window(GC *gc, Display **display, Window *win, char *buff);
void free_proc_data(Display* display, Window win);
int get_window_id_for_send_evn(unsigned long *window_id);

extern int sock_fd;

#endif