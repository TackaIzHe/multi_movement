#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/extensions/shape.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include "main.h"

int create_window(Display **display, int *screen, Window *root, Window *win)
{
    // Подключаемся в серверу X11
    if ((*display = XOpenDisplay(NULL)) == NULL)
    {
        perror("Неполучилось открыть дисплей");
    }
    printf("%s\n", *display == 0 ? "Ошибка дисплея" : "Всё нормально");

    // Получаем экран
    *screen = DefaultScreen(*display);

    // Получаем родительское окно
    *root = RootWindow(*display, *screen);

    int depth = (int)DefaultDepth(*display, *screen);

    Visual* visual = DefaultVisual(*display, *screen);

    XSetWindowAttributes attr;
    attr.override_redirect = True;  // Оверлей, WM не трогает
    attr.event_mask = ExposureMask | StructureNotifyMask;  // Только для рисования/структуры, без ввода!


    *win = XCreateWindow(*display, *root, POSx, POSy, WIDTH, HEIGHT, BORDER, 
                                depth,
                                InputOutput,  // Видимое окно
                                visual,
                                CWOverrideRedirect | CWEventMask,
                                &attr);
    
    // Делаем окно input-прозрачным (события проходят сквозь)
    // XShapeCombineMask(*display, *win, ShapeInput, 0, 0, None, ShapeSet);
        XShapeCombineRectangles(*display, *win, ShapeInput, 0, 0, NULL, 0, ShapeSet, Unsorted);

    // Создание окна
    // *win = XCreateSimpleWindow(*display, *root, POSx, POSy, WIDTH, HEIGHT, BORDER, BlackPixel(*display, *screen), WhitePixel(*display, *screen));

    // Для работы событий
    XSelectInput(*display, *win, ExposureMask | KeyPressMask); //ExposureMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | 

    // Отрисовка окна
    XMapWindow(*display, *win);

    // XLowerWindow(*display, *win);
    XRaiseWindow(*display, *win);  // Поднимаем поверх других
    XFlush(*display);

    
    return 0;
}