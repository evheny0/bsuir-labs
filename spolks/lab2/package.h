#ifndef PACKAGE_H
#define PACKAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <cstring>


static int BUFFER_MESSAGE_SIZE = 1000000;


struct Package {
    char *data;
    long size;

    Package();
    Package(const char *package_data, int new_size);
    ~Package();
    void free();
    void clear_data();
};


#endif // PACKAGE_H
