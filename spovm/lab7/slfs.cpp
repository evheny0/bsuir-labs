#include "slfs.h"
namespace slfs {

const char *FS_DECLARATION;
IFilesystem *IFilesystem::fs_manager;

void Filesystem::init(std::string name)
{
    FS_DECLARATION = "!SLFS FILESYSTEM";
    fs_manager = new Filesystem(name);
    if (!fs_manager->is_disk_valid()) {
        fs_manager->create();
    } else {
        ((Filesystem *)fs_manager)->init_root_dir();
    }
}

void Filesystem::end()
{
    delete fs_manager;
}

Filesystem::Filesystem()
{

}

Filesystem::Filesystem(std::string name)
{
    disk_name = name;
    disk.open(name.c_str(), std::fstream::in | std::fstream::out | std::fstream::binary);
    fs_data = new char[TOTAL_DISK_SIZE];
    disk.read(fs_data, TOTAL_DISK_SIZE);
    init_masks();
}

Filesystem::~Filesystem()
{
    disk.clear();
    disk.seekp(0);
    disk.write(fs_data, TOTAL_DISK_SIZE);
    disk.close();
}

void Filesystem::create()
{
    disk.clear();
    disk.seekp(0);
    write_data_to(0, (char *)FS_DECLARATION);
    create_root_dir();
}

void Filesystem::init_masks()
{
    data_mask.resize(DATA_BLOCK_COUNT);
    info_mask.resize(INFO_BLOCK_COUNT);
}

bool Filesystem::is_disk_valid()
{
    return !strcmp(read_data_from(0), FS_DECLARATION);
}

void Filesystem::create_root_dir()
{
    root_dir.create("root", "root", 0);
}

void Filesystem::init_root_dir()
{
    root_dir.open(0);
    data_mask[0] = FULL_BLOCK;
    info_mask[0] = FULL_BLOCK;
}

void Filesystem::cd()
{

}

void Filesystem::ls()
{

}

char *Filesystem::get_file_info(int pos)
{
    return fs_data + INFO_BLOCK_ARRAY_START + pos;
}

char *Filesystem::get_file_data(int pos)
{
    return fs_data + DATA_BLOCK_ARRAY_START + pos;
}

Fileinfo Filesystem::get_file_info(std::string path)
{
    if (path == "/") {
        Fileinfo r(root_dir.get_info_link(), root_dir.get_info_link(), "root");
        return r;
    }
    Dir dir;
    int link;
    auto dirs = parse_path(path);
    auto filename = dirs.back();
    dirs.pop_back();
    dir = root_dir.get_dir(dirs);
    link = dir.get_file_link(filename);
    Fileinfo r(link, dir.get_info_link(), filename);
    return r;
}

void Filesystem::add_file_to_dir(int child, int parent, int type)
{
    Dir parent_dir(parent);
    File child_file(child);
    parent_dir.add_file(child_file, type);
}

void Filesystem::write_file(std::string path, char *data)
{

}

void Filesystem::write_dir(std::string path, char *data)
{

}

void Filesystem::write_data_to(int pos, char *data)
{
    strcpy(fs_data + pos, data);
}

void Filesystem::write_file(int pos, char *data)
{

}   

char *Filesystem::read_data_from(int start)
{
    return fs_data + start;
}

int Filesystem::create_datablock(char **data)
{
    int link = find_free_block(data_mask) * DATA_BLOCK_SIZE;
    *data = fs_data + DATA_BLOCK_ARRAY_START + link;
    return link;
}


int Filesystem::create_infoblock(char **info)
{
    int link = find_free_block(info_mask) * INFO_BLOCK_SIZE;
    *info = fs_data + INFO_BLOCK_ARRAY_START + link;
    return link;
}

int Filesystem::find_free_block(std::vector<int> &mask)
{
    std::vector<int>::iterator it = find(mask.begin(), mask.end(), EMPTY_BLOCK);
    int index = std::distance(mask.begin(), it);
    if (it != mask.end()) {
        mask[index] = FULL_BLOCK;
        return index;
    } else {
        Logger::puts("Disk is full!\n");
        return -1;
    }
}


} // slfs
