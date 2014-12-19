#include "graphics.h"

namespace gr {


void init_ncurses()
{
    initscr();
    cbreak();
    noecho();
    curs_set(0);
}

void stop_ncurses()
{
    endwin();
}

void set_color(int color)
{
    attrset(color);
}

void print_to(std::string str, int x, int y)
{
    move(y, x);
    printw(str.c_str());
    refresh();
}

} // gr
