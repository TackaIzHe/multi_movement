#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <X11/Xlib.h>

// Аргументы для потока
struct pthread_arg
{
    // Ссылка дисплей
    Display* Display;
    
    // Сcылка на события
    XEvent* evn;
    // Текущее окно
    Window win;

} typedef pthread_arg;

extern Window target_window;

#endif