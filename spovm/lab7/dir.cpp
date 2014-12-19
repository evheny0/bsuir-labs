#include "dir.h"

namespace slfs {

Dir::Dir() : File()
{

}

Dir::Dir(std::string path) : File(path)
{

}

Dir::Dir(int _link) : File(_link)
{

}

Dir::~Dir()
{

}

void Dir::set_type()
{
    type = DIR;
}

void Dir::create(std::string _name, std::string _parent_path, int _parent_link)
{
    name = _name;
    patent_path = _parent_path;
    parent_link = _parent_link;
    data_link = IFilesystem::fs_manager->create_datablock(&data);
    info_link = IFilesystem::fs_manager->create_infoblock(&info);
    fill_infoblock(DIR);
}

void Dir::write_to_source()
{
    IFilesystem::fs_manager->write_dir(name, data);
}

Dir Dir::get_dir(std::vector<char *> dirs_path)
{
    if (!dirs_path.size()) {
        return *this;
    }
    Dir dir = find_child_dir(dirs_path.front());
    dirs_path.erase(dirs_path.begin());
    return dir.get_dir(dirs_path);
}

Dir Dir::find_child_dir(std::string _name)
{
    Dir result;
    int link_to_file;
    for (int i = 0; i < MAX_FOLDER_COUNT; ++i) {
        if (!strcmp(_name.c_str(), data + (i * FILE_SIZE_IN_FOLDER))) {
            memcpy(&link_to_file, data + (i * FILE_SIZE_IN_FOLDER) + LINK_IN_FOLDER_START, LINK_SIZE);
            result.open(link_to_file);
            return result;
        }
    }
    result.create(_name, path, info_link);
    add_file(result, DIR);
    return result;
}

void Dir::add_file(File &child, int type)
{
    int link = find_free_block();
    int child_info = child.get_info_link();
    memcpy(data + link, child.get_c_name(), FILENAME_SIZE);
    memcpy(data + link + FILETYPE_IN_FOLDER_START, &type, FILETYPE_SIZE);
    memcpy(data + link + LINK_IN_FOLDER_START, &child_info, LINK_SIZE);
}

int Dir::find_free_block()
{
    for (int i = 0; i < DATA_BLOCK_SIZE; i += FILE_SIZE_IN_FOLDER) {
        if (!data[i]) {
            return i;
        }
    }
    Logger::puts("Dir is full");
    return -1;
}

int Dir::get_file_link(std::string filename)
{
    File result;
    int link_to_file;
    for (int i = 0; i < MAX_FOLDER_COUNT; ++i) {
        if (!strcmp(filename.c_str(), data + (i * FILE_SIZE_IN_FOLDER))) {
            memcpy(&link_to_file, data + (i * FILE_SIZE_IN_FOLDER) + LINK_IN_FOLDER_START, LINK_SIZE);
            return link_to_file;
        }
    }
    return -1;
}

std::vector<Dir> Dir::get_dirs_list()
{
    std::vector<Dir> result;
    std::vector<int> dirs = get_list(DIR);
    for(auto i : dirs) {
        Dir dir(i);
        result.push_back(dir);
    }
    return result;
}

std::vector<int> Dir::get_list(int type)
{
    std::vector<int> result;
    int link_to_file;
    for (int i = 0; i < DATA_BLOCK_SIZE; i += FILE_SIZE_IN_FOLDER) {
        if (data[i] && !memcmp(&type, data + i + FILENAME_SIZE, FILETYPE_SIZE)) {
            memcpy(&link_to_file, data + i + LINK_IN_FOLDER_START, LINK_SIZE);
            result.push_back(link_to_file);
        }
    }
    return result;
}

} // slfs
