#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <string.h>
#include <stdio.h>

Window printf_children_window(Display **display, Window *root)
{
    Window root_out, parent;
    Window *children = NULL;
    unsigned int nchildren;
    Window target_window;

    Status status = XQueryTree(*display, *root, &root_out, &parent, &children, &nchildren);
    if (!status)
    {
        fprintf(stderr, "Ошибка XQueryTree!\n");
        XCloseDisplay(*display);
        return 1;
    }
    if (nchildren > 0)
    {
        for (unsigned int i = 0; i < nchildren; ++i)
        {
            XWindowAttributes attrs;
            XGetWindowAttributes(*display, children[i], &attrs);

            XTextProperty text_prop;
            if (XGetWMName(*display, children[i], &text_prop) == 1)
            {
                printf("XGetWMName succeeded: %s\n", text_prop.value);
                // Конвертация как в предыдущем примере...
                // (вставь код из моего предыдущего ответа для обработки text_prop)
            }
            else
            {
                continue;
            }
            printf("Status: %d\n", status);
            printf("visual: %lx\n", attrs.visual->visualid);
            printf("ID: %lx\n", children[i]);
            printf("H: %d; W: %d\n", attrs.height, attrs.width);

            if (attrs.width == 1440)
            {
                printf("%lx\n", attrs.visual->visualid);
                printf("H: %d; W: %d\n", attrs.height, attrs.width);
                XSelectInput(*display, children[i], ExposureMask | KeyPressMask);
                target_window = children[i];
            }
        }
    }
    // Освобождаем память, выделенную XQueryTree
    if (children)
    {
        XFree(children);
    }
    return target_window;
}