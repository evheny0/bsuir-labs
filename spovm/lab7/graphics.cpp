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

void print_to(std::string str, int x, int y)
{
    // attron(A_BOLD);
    // move(y, x);
    // printw(str.c_str());
    // refresh();
    std::cout << str;
}

} // gr
