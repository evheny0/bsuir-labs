#include <iostream>
#include "slfs.h"

int main(int argc, char const *argv[])
{
    slfs::Filesystem::init("slfs.fs");

    slfs::File lala("folder/two/file");
    // lala.write("test");
    puts(lala.read());
    slfs::Filesystem::end();
    return 0;
}
