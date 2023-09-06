/*** includes ***/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>

/*** data ***/
struct editorConfig
{
    int cx, cy;
    int screenrows;
    int screencols;
    struct termios orig_termios;
};

struct editorConfig E;

/*** defines ***/

#define KILO_VERSION "0.0.1"
#define CTRL_KEY(k) ((k) & 0x1f)

enum editorKey
{
    ARROW_LEFT = 1000,
    ARROW_RIGHT,
    ARROW_UP,
    ARROW_DOWN,
    PAGE_UP,
    PAGE_DOWN,
    HOME_KEY,
    END_KEY,
    DEL_KEY,
};