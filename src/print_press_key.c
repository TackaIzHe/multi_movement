#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <stdlib.h>
#include <stdio.h>

char print_press_key(XEvent evn) {
    char cur_key = 0;
    XKeyEvent xkey = evn.xkey;

    // Конвертируем в keysym с учётом модификаторов (Shift и т.д.)
    KeySym keysym = XLookupKeysym(&xkey, 0);  // 0 - без модификаторов; для учёта Shift используйте цикл или XKeycodeToKeysym

    // Для учёта Shift: проверим state (модификаторы)
    if (xkey.state & ShiftMask) {
        keysym = XLookupKeysym(&xkey, 1);  // 1 - с Shift
    }

    // Получаем строку из keysym
    const char *key_str = XKeysymToString(keysym);
    if (!key_str) {
        printf("Unknown key (keysym: %lu)\n", keysym);
        return 0;
    }

    // Проверяем, является ли это printable символом (буквы, цифры)
    if (keysym >= XK_a && keysym <= XK_z) {
        cur_key = (char)(keysym - XK_a + 'a');
        printf("'%c'\n", cur_key);
        return cur_key;
    } else if (keysym >= XK_A && keysym <= XK_Z) {
        cur_key = (char)(keysym - XK_A + 'A');
        printf("'%c'\n", cur_key);
        return cur_key;
    } else if (keysym >= XK_0 && keysym <= XK_9) {
        cur_key = (char)(keysym - XK_0 + '0');
        printf("'%c'\n", cur_key);
        return cur_key;
    } else {
        // Для специальных клавиш (F1, Enter и т.д.) печатаем имя
        printf("'%s'\n", key_str);
        // Возвращаем 0 для не-printable; или можно маппить на ASCII-коды
        return 0;
    }
}