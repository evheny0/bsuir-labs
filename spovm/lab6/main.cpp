#include <iostream>
#include <string>
#include <ncurses.h>
#include "pointer.h"

class Lala
{
  public:
    Lala() {};
    ~Lala() {};
    int very_complex_calculation() { return 5 + 5; }
};

void fun2(GC::Pointer<Lala> l)
{
    attron(COLOR_PAIR(1));
    printw("fun2 is: %d\n", l->very_complex_calculation());
}

void fun()
{
    GC::Pointer<Lala> a;
    int test = a->very_complex_calculation();
    attron(COLOR_PAIR(1));
    printw("fun is: %d\n", test);
}

void init_ncurses()
{
    initscr();
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_CYAN);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
}



int main(int argc, char const *argv[])
{
    init_ncurses();

    int test;
    GC::Pointer<Lala> a;
    test = a->very_complex_calculation();
    std::cout << test << std::endl;

    fun();
    fun2(a);
    refresh();
    getchar();
    endwin();
    return 0;
}
