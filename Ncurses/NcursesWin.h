#ifndef NCURSES_WIN_H_
#define NCURSES_WIN_H_

#include <ncurses.h>

WINDOW *GraphicsInit(void);

void GraphicsDestroy(WINDOW **Win);

#endif //NCURSES_WIN_H_