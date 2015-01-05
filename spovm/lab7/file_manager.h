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
    void print_type(File *file, int pos);
    void goto_new_folder(std::vector<File *>::iterator &current, Dir &parent_dir, std::vector<File *> &dirs);
    void goto_parent_folder(std::vector<File *>::iterator &current, Dir &parent_dir, std::vector<File *> &dirs);
    void print_file_list(std::vector<File *> &dirs, std::vector<File *>::iterator &current);
    void create_new_dir(Dir &parent_dir);
    void create_new_file(Dir &parent_dir);
    std::string enter_string();
    void show_file(std::vector<File *>::iterator &current);
};


} // slfs
#endif // FILE_MANAGER_H
