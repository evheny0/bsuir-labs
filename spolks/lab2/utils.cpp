#include "utils.h"


std::ifstream::pos_type filesize(const char* filename)
{
    std::ifstream file(filename, std::ifstream::ate | std::ifstream::binary);
    std::ifstream::pos_type result = file.tellg();
    file.close();
    return result; 
}
