#include "NcursesWin.h"

#include <ncurses.h>
#include <assert.h>

int Height = 144;
int Width = 192 * 2;

WINDOW *GraphicsInit(void) {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE); 
    curs_set(0);
    refresh();

    WINDOW *Win = newwin(Height, Width, (LINES - Height) / 2, (COLS - Width) / 2);
    WINDOW *Board = newwin(Height + 2, Width + 2, (LINES - Height) / 2 - 1, (COLS - Width) / 2 - 1);

    if(Win == NULL || Board == NULL)
        return NULL;

    box(Board, 0, 0);
    
    //wrefresh(Win);
    wrefresh(Board);
    //refresh();


    delwin(Board);

    Board = NULL;

    return Win;
}

void GraphicsDestroy(WINDOW **Win) {
    assert(Win);
    if (*Win == NULL)
        return;

    delwin(*Win);

    // clear();
    // refresh();

    endwin();

    *Win = NULL;
}

// bool Draw(long long int *VRAM, WINDOW *Win)
// {
//     if(VRAM == NULL || Win == NULL)
//         return true;
    
//     werase(Win);

//     for (int i = 0; i < Width * Height / 2; i++)
//     {
//         //fprintf(stderr, "i=%d ", i);

//         //assert(VRAM[i] <= 70);
//         waddch(Win, (unsigned int)[ram[i]]);

//         waddch(Win, ' ');
//     }

//     wrefresh(Win);
//     for (volatile size_t i = 0; i < (size_t)(7 * 1e7); i++)
//         ;
//     // refresh();

//     return false;
// }