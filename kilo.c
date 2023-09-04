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
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);
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

// обработка ошибок при вводе
char editorReadKey()
{
    int nread;
    char c;
    while ((nread = read(STDIN_FILENO, &c, 1)) != 1)
    {
        if (nread == -1 && errno != EAGAIN)
            die("read");
    };

    return c;
}

/*** output ***/

void editorDrawRows()
{
    int y;
    for (int y = 0; y < 24; y++)
    {
        write(STDOUT_FILENO, "~\r\n", 3);
    }
}

void editorRefreshScreen()
{
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);

    editorDrawRows();

    write(STDOUT_FILENO, "\x1b[H", 3);
}

/*** input ***/

// обработка ввода
void editorProcessKeypress()
{
    char c = editorReadKey();
    switch (c)
    {
    // case CTRL_KEY('q'):  -----------------  не работает ctrl
    case 'q':
        write(STDOUT_FILENO, "\x1b[2J", 4);
        write(STDOUT_FILENO, "\x1b[H", 3);
        exit(0);
        break;
    }
}

/*** init ***/

int main(int argc, char **argv)
{
    enableRawMode();
    while (1)
    {
        editorRefreshScreen();
        editorProcessKeypress();
    };
    return 0;
}