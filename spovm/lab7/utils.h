#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <vector>
#include <string>
#include <string.h>

namespace slfs {

std::vector<char *> parse_path(std::string path);
class Logger {
  public:
    static void puts(const char *str);
    static void puts(std::string str);
};


} // slfs

#endif // UTILS_H
