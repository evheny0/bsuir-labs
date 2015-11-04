#include "utils.h"


std::ifstream::pos_type filesize(const char* filename)
{
    std::ifstream file(filename, std::ifstream::ate | std::ifstream::binary);
    std::ifstream::pos_type result = file.tellg();
    file.close();
    return result; 
}

long long int find_position(std::string string, const char *substring, int last_position)
{
    long long int i;
    for(i = last_position; i < string.size() - 1; ++i) {
        if (string[i] == substring[0] && string[i + 1] == substring[1]) {
            return i;
        }
    }
    return std::string::npos;
}

long long int stoll_fixed(char *str_raw)
{
    long long int result = 0;
    std::stringstream str(str_raw);
    str >> result;
    return result;
}
