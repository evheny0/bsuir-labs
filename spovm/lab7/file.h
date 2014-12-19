#ifndef FILE_H
#define FILE_H

#include <string>
#include <vector>
#include <iostream>
#include "ifilesystem.h"

namespace slfs {

class File {
  protected:
    int type;
    std::string path;
    std::string name;
    char *data;
    char *info;

    std::string patent_path;
    int parent_link;
    int info_link;
    int data_link;
  public:
    File();
    File(std::string path);
    File(int _link);
    ~File();
    virtual void set_type();
    int get_info_link();
    char *get_c_name();
    void open(std::string path);
    void open(int _info_link);
    void create(std::string _name, int _parent_link, int _type);
    void write(char *str);
    virtual void write_to_source();
    char *read();
    void close();
    void get_data_from_infoblock();
    void fill_infoblock(int type);
    int get_parent_link();
    int get_type();
};

} // slfs
#endif // FILE_H
