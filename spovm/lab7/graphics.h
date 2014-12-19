#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <termios.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <stdlib.h>
#include <curses.h>
#include <string>
#include <iostream>

namespace gr {

void init_ncurses();
void stop_ncurses();
void print_to(std::string str, int x = 0, int y = 0);

} // gr

#endif // GRAPHICS_H
