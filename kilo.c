/*** includes ***/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <unistd.h>
#include <termios.h>

/*** data ***/

struct termios orig_termios;

/*** defines ***/

#define CTRL_KEY(k) ((k) & 0x1f)

/*** terminal ***/

void die(const char *s)
{
    perror(s);
    exit(1);
}

// возвращаем все как было
void disableRawMode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

// отключаем канонический режим терминалы, выключаем отображение ввода в терминале? отключаем работу сочетаний cntrl + Z/C/S/Q/V
void enableRawMode()
{
    // ECHO = отключить Эхо
    // IXON = отключить cntrl + S/Q
    // ICANON = отключить канонический режим терминала
    // IEXTEN = отключить cntrl + V
    // ICRNL = отключить cntrl + M
    // ISIG = отключить cntrl + C/Z
    // OPOST = отключить всю обработку вывода
    // Когда BRKINTэтот параметр включен, условие останова приведет к SIGINTотправке сигнала в программу, например, при нажатии Ctrl-C.
    // INPCK включает проверку четности, которая, похоже, не применима к современным эмуляторам терминала.
    // ISTRIP приводит к удалению 8-го бита каждого входного байта, то есть ему присваивается значение 0. Вероятно, это уже отключено.
    // CS8 это не флаг, это битовая маска из нескольких битов, которую мы устанавливаем с помощью оператора побитового ИЛИ ( |), в отличие от всех флагов, которые мы отключаем. Он устанавливает размер символа (CS) равным 8 бит на байт. В моей системе уже так настроено.
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
        die("tcgetattr");
    atexit(disableRawMode);

    struct termios raw = orig_termios;

    raw.c_lflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_cflag &= ~(OPOST);
    raw.c_cflag &= ~(CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        die("tcsetattr");
}

/*** init ***/

int main(int argc, char **argv)
{
    enableRawMode();
    while (1)
    {
        char c;
        if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN)
            die("read");
        // Проверка на управляющие символы и вывод нажатия
        if (iscntrl(c))
        {
            printf("%d\r\n", c);
        }
        else
        {
            printf("%d (%c)\r\n", c, c);
        }
        // кнопка q это выход из программы
        if (c == CTRL_KEY('q')) break;
    };
    return 0;
}