#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <sstream>
#include <fstream>

template <typename T>
std::string to_string_fixed(T value)
{
    std::stringstream stream;
    stream << value;
    return stream.str();
}

std::ifstream::pos_type filesize(const char* filename);
int find_position(std::string string, const char *substring, int last_position = 0);

#endif // UTILS_H
