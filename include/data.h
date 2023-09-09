/*** includes ***/

<<<<<<< HEAD
#include <stdio.h>
=======
#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#define _GNU_SOURCE

#include <stdio.h>
#include <stdarg.h>
>>>>>>> 616d9e8ced3e1d85ca9f8837b96292d9c0abd0fe
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
<<<<<<< HEAD
#include <termios.h>

/*** data ***/
struct editorConfig
{
    int cx, cy;
    int screenrows;
    int screencols;
    struct termios orig_termios;
=======
#include <sys/types.h>
#include <time.h>
#include <termios.h>

/*** data ***/

typedef struct erow
{
  int size;
  int rsize;
  char *chars;
  char *render;
} erow;
struct editorConfig
{
  int cx, cy;
  int rx;
  int coloff;
  int rowoff;
  int screenrows;
  int screencols;
  int numrows;
  erow *row;
  char *filename;
  char statusmsg[80];
  time_t statusmsg_time;
  struct termios orig_termios;
>>>>>>> 616d9e8ced3e1d85ca9f8837b96292d9c0abd0fe
};

struct editorConfig E;

/*** defines ***/

#define KILO_VERSION "0.0.1"
#define CTRL_KEY(k) ((k) & 0x1f)
<<<<<<< HEAD

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
=======
#define KILO_TAB_STOP 8

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
>>>>>>> 616d9e8ced3e1d85ca9f8837b96292d9c0abd0fe
};