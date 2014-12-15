#include "utils.h"
namespace slfs {


std::vector<char *> parse_path(std::string path)
{
    std::vector<char *> dirs;
    char *dir = strtok((char *)path.c_str(), "/");
    while (dir) {
        dirs.push_back(dir);
        dir = strtok(NULL, "/");
    }
    return dirs;
}

void Logger::puts(const char *str)
{
    std::cout << str << "\n";
}

void Logger::puts(std::string str)
{
    std::cout << str << "\n";
}


} // slfs
