#include <X11/Xlib.h>
#include <string.h>

int print_string_to_window(GC *gc, Display **display, Window *win, char *buff)
{
    // Функция создаёт и возвращает новый контекст вывода графики
    *gc = XCreateGC(*display, *win, 0, NULL);

    // Груба говоря здесь цвет текста
    XSetForeground(*display, *gc, /*BlackPixel ( display, 0) Чёрный цвет Текста*/ 0xff0000);

    // Отрисовка строки
    XDrawString(*display, *win, *gc, 20, 50, buff, strlen(buff));

    // проталкивание буфера
    XFlush(*display);

    // Пологаю что высвобождение контекста графики
    XFreeGC(*display, *gc);
}