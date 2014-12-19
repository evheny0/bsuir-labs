#ifndef SLFS_H
#define SLFS_H

#include <ios>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <string.h>
#include <regex>
#include "file.h"
#include "dir.h"
#include "ifilesystem.h"
#include "utils.h"

namespace slfs {

class Filesystem : public IFilesystem {
    std::fstream disk;
    std::string disk_name;
    Dir root_dir;
    char *fs_data;
    std::vector<int> data_mask;
    std::vector<int> info_mask;

    Filesystem();
    Filesystem(std::string name);

  public:
    static void init(std::string name = "slfs.fs");
    static void end();
    ~Filesystem();
    void create();
    bool is_disk_valid();
    void create_root_dir();
    void cd();
    void ls();
    char *get_file_info(int pos);
    char *get_file_data(int pos);
    Fileinfo get_file_info(std::string path);
    void add_file_to_dir(int child, int parent, int type);
    void write_dir(std::string path, char *data);
    void write_file(std::string path, char *data);
    void write_file(int pos, char *data);
    void write_data_to(int pos, char *data);
    char *read_data_from(int start);
    int create_datablock(char **data);
    int create_infoblock(char **info);

  private:
    void init_masks();
    void init_root_dir();
    int find_free_block(std::vector<int> &mask);
};

} // slfs

#endif // SLFS_H
