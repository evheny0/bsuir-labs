#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <iostream>
#include "slfs.h"
#include "graphics.h"

namespace slfs {


class FileManager {
  public:
    FileManager();
    ~FileManager();
    void start();
};


} // slfs
#endif // FILE_MANAGER_H
