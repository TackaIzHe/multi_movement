#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/Xatom.h>
#include <X11/extensions/record.h>
#include <X11/extensions/XTest.h>
#include <X11/extensions/shape.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/signal.h>
#include <pthread.h>
#include "server.h"
#include "../main.h"
#include "../socket/socket.h"

XRecordRange *range;
XRecordContext context;
Window window;
Display *dis;

static void event_callback(XPointer closure, XRecordInterceptData *recorded_data);

static void sigev(int i)
{
    printf("Сканер выключен\n");
    XRecordDisableContext(dis, context);
    XRecordFreeContext(dis, context);
    XCloseDisplay(dis);
    return;
}

static void *startXRecordContext(void*)
{
    signal(15, sigev);
    // Подключаемся в серверу X11
    if ((dis = XOpenDisplay(NULL)) == NULL)
    {
        perror("Неполучилось открыть дисплей");
    }
    printf("%s\n", dis == 0 ? "Ошибка дисплея" : "Всё нормально");

    // Включаем контекст с callback
    if (!XRecordEnableContext(dis, context, event_callback, NULL))
    {
        fprintf(stderr, "Cannot enable context\n");
        XRecordFreeContext(display, context);
        XFree(range);
        return 0;
    }
    return 0;
}

static void setEvn(uint8_t *data, event *evn)
{
    short *x_root = (short *)(data + 20);
    short *y_root = (short *)(data + 22);
    evn->evnt_type = data[0];
    evn->button = data[1];
    evn->is_press = 0;
    evn->x = *x_root;
    evn->y = *y_root;
}
static int counter = 0;
// Callback-функция для обработки записанных событий (вызывается для каждого события)
static void event_callback(XPointer closure, XRecordInterceptData *recorded_data)
{
    char buff[1024] = {0};
    event evn;
    uint8_t *data = recorded_data->data;
    if (recorded_data->category == XRecordFromServer)
    {
        if (data[0] == KeyPress) // Нажатие клавиши клавиатуры
        {
            setEvn(data, &evn);
            memcpy(buff, &evn, sizeof(evn));
            send_socket(sock_fd, buff, sizeof(evn));
            printf("KeyPress: keyCode=%d; root-x=%d, root-y=%d\n", evn.button, evn.x, evn.y);
        }
        else if (data[0] == KeyRelease) // Отжатие клавиши клавиатуры
        {
            setEvn(data, &evn);
            memcpy(buff, &evn, sizeof(evn));
            send_socket(sock_fd, buff, sizeof(evn));
            printf("KeyRelease: %d\n", data[1]);
        }
        else if (data[0] == MotionNotify) // Движение курсора
        {
            if (counter == 5)
            {
                counter = 0;
                setEvn(data, &evn);
                memcpy(buff, &evn, sizeof(evn));
                send_socket(sock_fd, buff, sizeof(evn));
                printf("MotionNotify: %d x=%d; y=%d;\n", evn.button, evn.x, evn.y);
            }
            counter++;
        }
        else if (data[0] == ButtonPress) // Нажатие клавиши мыши
        {
            setEvn(data, &evn);
            memcpy(buff, &evn, sizeof(evn));
            send_socket(sock_fd, buff, sizeof(evn));
            printf("ButtonPress: %d x=%d; y=%d;\n", evn.button, evn.x, evn.y);
        }
        else if (data[0] == ButtonRelease) // Отжатие клавиши мыши
        {
            setEvn(data, &evn);
            memcpy(buff, &evn, sizeof(evn));
            send_socket(sock_fd, buff, sizeof(evn));
            printf("ButtonRelease: %d x=%d; y=%d;\n", evn.button, evn.x, evn.y);
        }
    }
    XRecordFreeData(recorded_data);
    return;
}

int main_loop(GC *gc, XEvent *evn, Window *win)
{
    Window w = target_window;
    XRectangle rect = {0, 0, WIDTH, HEIGHT};
    pthread_t pid;
    window = *win;

    XSelectInput(display, w, KeyPressMask);

    // Создаём диапазон событий: только MotionNotify (можно расширить до ButtonPress/Release)
    range = XRecordAllocRange();
    if (!range)
    {
        fprintf(stderr, "Cannot allocate range\n");
        return 1;
    }
    range->device_events.first = 2; // Начало диапазона

    range->device_events.last = 6; // Конец (для одного типа)

    // Создаём контекст для всех клиентов (глобальный захват)
    XRecordClientSpec clients = XRecordAllClients;
    context = XRecordCreateContext(display, 0, &clients, 1, &range, 1);
    if (!context)
    {
        fprintf(stderr, "Cannot create context\n");
        XFree(range);
        return 1;
    }

    // Цыкл Обработки событий
    while (!XNextEvent(display, evn))
    {
        switch (evn[0].type)
        {
        case Expose:
            // События которые происходят при создании окна
            // Устанавливает прозрачность
            Atom opacity_atom = XInternAtom(display, "_NET_WM_WINDOW_OPACITY", False);
            unsigned long opacity = 0x00000001; // 25%
            XChangeProperty(display, *win, opacity_atom, XA_CARDINAL, 32, PropModeReplace,
                (unsigned char *)&opacity, 1);
            pthread_create(&pid, 0, startXRecordContext, 0);
            break;
        case KeyPress:
            // Сдесь прописываються события которые нужно обробатывать при нажатии
            // print_press_key(*evn);
            // XSendEvent(*display, w, False, 0, evn);
            // printf("%d\n",evn->xkey.keycode);
            // memcpy(buff, evn, sizeof(*evn));
            // send_socket(sock_fd, buff, sizeof(*evn));

            KeySym keysym = XLookupKeysym(&(evn[0]).xkey, 0);
            if (XK_5 == keysym)
            {
                pthread_kill(pid, 15);
                return 0;
            }
            break;
        }
    }
    return 0;
}