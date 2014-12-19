#ifndef IFILESYSTEM_H
#define IFILESYSTEM_H

#include <string.h>
#include <cstdint>
#include "utils.h"

namespace slfs {

enum FILETYPE {
    EMPTY,
    FILE,
    DIR
};

// enum BLOCK_STATE {
//     EMPTY_BLOCK,
//     FULL_BLOCK
// };
#define EMPTY_BLOCK false
#define FULL_BLOCK true

union Link_t {
    char str_link[4];
    int int_link;
};

struct Fileinfo {
    Fileinfo() {};
    Fileinfo(int info, int parent, std::string name) 
    { 
        link_to_info = info; 
        link_to_parent = parent; 
        filename = name;
    };
    
    int link_to_info;
    char filetype;
    std::string filename;
    int link_to_data;
    int link_to_parent;

};

/*
    DIR:
        :filetype
        :filename
        :link_to_data
        :link_to_parent_infoblock

    FILE:
        :filetype
        :filename
        :link_to_data
        :link_to_folder
*/

const int SUPER_BLOCK_SIZE = 1000;
const int INFO_BLOCK_ARRAY_SIZE = 10000;
const int DATA_BLOCK_ARRAY_SIZE = 100000;
const int TOTAL_DISK_SIZE = SUPER_BLOCK_SIZE + INFO_BLOCK_ARRAY_SIZE + DATA_BLOCK_ARRAY_SIZE;

const int SUPER_BLOCK_START = 0;
const int INFO_BLOCK_ARRAY_START = SUPER_BLOCK_SIZE;
const int DATA_BLOCK_ARRAY_START = INFO_BLOCK_ARRAY_START + INFO_BLOCK_ARRAY_SIZE;

const int INFO_MASK_START = 50;
const int INFO_MASK_SIZE = 150;
const int DATA_MASK_START = 200;
const int DATA_MASK_SIZE = 150;
const int CELL_SIZE = 8;

const int FILETYPE_SIZE = 4;
const int FILENAME_SIZE = 30;
const int LINK_SIZE = 4;
const int LINK_TO_PARENT_SIZE = 4;

const int INFO_BLOCK_SIZE = 100;
const int DATA_BLOCK_SIZE = 1000;
const int INFO_BLOCK_COUNT = INFO_BLOCK_ARRAY_SIZE / INFO_BLOCK_SIZE;
const int DATA_BLOCK_COUNT = DATA_BLOCK_ARRAY_SIZE / DATA_BLOCK_SIZE;

const int FILE_SIZE_IN_FOLDER = FILENAME_SIZE + FILETYPE_SIZE + LINK_SIZE;
const int FILETYPE_IN_FOLDER_START = FILENAME_SIZE;
const int LINK_IN_FOLDER_START = FILENAME_SIZE + FILETYPE_SIZE;
const int MAX_FOLDER_COUNT = DATA_BLOCK_SIZE / FILE_SIZE_IN_FOLDER - 1;

extern const char *FS_DECLARATION;

class IFilesystem {
  public:
    static IFilesystem *fs_manager;

    IFilesystem() {};
    virtual ~IFilesystem() {};
    virtual bool is_disk_valid() { return false; };
    virtual void create() {};
    virtual Fileinfo get_file_info(std::string path) { return Fileinfo(); };
    virtual void write_dir(std::string path, char *data) {};
    virtual void write_file(std::string path, char *data) {};
    virtual char *get_file_info(int pos) { return NULL; };
    virtual char *get_file_data(int pos) { return NULL; };
    virtual int create_datablock(char **data) { return 0; };
    virtual int create_infoblock(char **info) { return 0; };
    virtual void add_file_to_dir(int child, int parent, int type) {};
};

} // slfs
#endif // IFILESYSTEM_H
