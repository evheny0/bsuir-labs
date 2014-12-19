#ifndef DIR_H
#define DIR_H

#include "file.h"
#include <cstdio>
#include <iostream>

namespace slfs {


class Dir : public File {
  public:
    Dir();
    Dir(std::string path);
    Dir(int _link);
    ~Dir();
    void set_type();
    void write_to_source();
    void create(std::string _name, std::string _parent_path, int _parent_link);
    Dir get_dir(std::vector<char *> dirs_path);
    Dir find_child_dir(std::string name);
    void add_file(File &child, int type);
    int find_free_block();
    int get_file_link(std::string filename);
    std::vector<int> get_list(int type);
    std::vector<Dir> get_dirs_list();
};

} // slfs
#endif // DIR_H
