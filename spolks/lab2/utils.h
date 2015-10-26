#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>


template <typename T>
std::string to_string_fixed(T value)
{
    std::stringstream stream;
    stream << value;
    return stream.str();
}

std::ifstream::pos_type filesize(const char* filename);
long long int find_position(std::string string, const char *substring, int last_position = 0);
long long int stoll_fixed(char *str_raw);

#endif // UTILS_H
