#include "package.h"

Package::Package()
{
    data = new char[BUFFER_MESSAGE_SIZE]();
    size = 0;
}

Package::Package(const char *package_data, int new_size)
{
    data = new char[BUFFER_MESSAGE_SIZE]();
    memcpy(data, package_data, new_size);
    size = new_size;
}

Package::~Package()
{
    delete data;
}

void Package::free()
{
    // delete data;
}

void Package::clear_data()
{
    memset(data, 0, BUFFER_MESSAGE_SIZE);
}

void Package::set_data(const char *data_in, int new_size)
{
    memcpy(data, data_in, new_size);
    size = new_size;
}
