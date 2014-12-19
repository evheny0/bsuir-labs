#include <iostream>
#include "file_manager.h"

int main(int argc, char const *argv[])
{
    slfs::FileManager file_manager;
    file_manager.start();
    return 0;
}
