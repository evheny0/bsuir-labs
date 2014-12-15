#include "file.h"
namespace slfs {

File::File()
{

}

File::File(std::string path)
{
    open(path);
}

File::~File()
{

}

void File::open(std::string path)
{
    this->path = path;
    Fileinfo i = IFilesystem::fs_manager->get_file_info(path);
    if (i.link_to_info == -1) {
        create(i.filename, i.link_to_parent, FILE);
    } else {
        open(i.link_to_info);
    }
}

void File::open(int _info_link)
{
    info_link = _info_link;
    info = IFilesystem::fs_manager->get_file_info(info_link);
    get_data_from_infoblock();
}

void File::create(std::string _name, int _parent_link, int _type)
{
    name = _name;
    parent_link = _parent_link;
    data_link = IFilesystem::fs_manager->create_datablock(&data);
    info_link = IFilesystem::fs_manager->create_infoblock(&info);
    fill_infoblock(FILE);
}

int File::get_info_link()
{
    return info_link;
}

void File::write(char *str)
{
    strcpy(data, str);
}

char *File::read()
{
    return data;
}

void File::write_to_source()
{
    IFilesystem::fs_manager->write_file(path, data);
}

void File::get_data_from_infoblock()
{
    char buffer[64];
    strcpy(buffer, info + FILETYPE_SIZE);
    name = buffer;
    memcpy(&data_link, info + FILETYPE_SIZE + FILENAME_SIZE, LINK_SIZE);
    memcpy(&parent_link, info + FILETYPE_SIZE + FILENAME_SIZE + LINK_SIZE, LINK_SIZE);
    data = IFilesystem::fs_manager->get_file_data(data_link);
}

void File::fill_infoblock(int type)
{
    /*
    :filetype
    :filename
    :link_to_data
    :link_to_parent_infoblock
    */
    memcpy(info, &type, sizeof(type));
    memcpy(info + FILETYPE_SIZE, name.c_str(), name.size());
    memcpy(info + FILETYPE_SIZE + FILENAME_SIZE, &data_link, LINK_SIZE);
    memcpy(info + FILETYPE_SIZE + FILENAME_SIZE + LINK_SIZE, &parent_link, LINK_SIZE);
}

void File::close()
{
    // IFilesystem::fs_manager->write();
}



} // slfs
