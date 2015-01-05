#include "file_manager.h"
namespace slfs {


FileManager::FileManager()
{
    slfs::Filesystem::init("slfs.fs");
    gr::init_ncurses();
}

FileManager::~FileManager()
{
    gr::stop_ncurses();
    slfs::Filesystem::end();
}

void FileManager::start()
{
    // slfs::File f1("aa/file");
    // slfs::File f2("bbb/file");
    // slfs::File f4("ccc/file");
    // slfs::File f5("ddd/file");
    // slfs::File f6("eee/ccc/ggg/aaa");
    // slfs::File f7("hhh/alal");
    bool is_run = true;
    slfs::Dir root(std::string("/")), parent_dir(std::string("/"));
    std::vector<File *> dirs = root.get_all_files_list();
    std::vector<File *>::iterator it, current = dirs.begin();

    while(is_run) {
        print_file_list(dirs, current);
        switch (getchar()) {
        case 's': 
            if (++current == dirs.end()) {
                current--;
            }
            break;
        case 'w':
            if (--current + 1 == dirs.begin()) {
                current++;
            }
            break;
        case 'd':
            if ((**current).get_type() == DIR) {
                goto_new_folder(current, parent_dir, dirs);
            } else {
                show_file(current);
            }
            break;
        case 'a':
            goto_parent_folder(current, parent_dir, dirs);
            break;
        case 'o':
            create_new_dir(parent_dir);
            dirs = parent_dir.get_all_files_list();
            current = dirs.begin();
            break;
        case 'i':
            create_new_file(parent_dir);
            dirs = parent_dir.get_all_files_list();
            current = dirs.begin();
            break;
        case 27: 
            is_run = false; 
            break;
        }
    }
}
void FileManager::goto_new_folder(std::vector<File *>::iterator &current, Dir &parent_dir, std::vector<File *> &dirs)
{
    if ((**current).get_type() == DIR) {
        parent_dir = *((Dir *) *current);
        dirs = parent_dir.get_all_files_list();
        current = dirs.begin();
        clear();
    }
}

void FileManager::show_file(std::vector<File *>::iterator &current)
{
    clear();
    gr::print_to((**current).read(), 1, 1);
    getchar();
    clear();
}

void FileManager::goto_parent_folder(std::vector<File *>::iterator &current, Dir &parent_dir, std::vector<File *> &dirs)
{
    parent_dir.open(parent_dir.get_parent_link());
    dirs = parent_dir.get_all_files_list();
    current = dirs.begin();
    clear();
}

void FileManager::print_file_list(std::vector<File *> &dirs, std::vector<File *>::iterator &current)
{
    int i = 1;
    std::vector<File *>::iterator it;
    for(it = dirs.begin(); it != dirs.end(); it++, i++) {
        if (it == current) {
            gr::set_color(A_UNDERLINE);
        } else {
            gr::set_color(A_NORMAL);
        }
        gr::print_to((**it).get_c_name(), 2, i);
        print_type(*it, i);
    }
}


void FileManager::print_type(File *file, int pos)
{
    if (file->get_type() == DIR) {
        gr::print_to("Dir", 15, pos);
    } else {
        gr::print_to("File", 15, pos);
    }
}

void FileManager::create_new_dir(Dir &parent_dir)
{
    slfs::Dir new_dir;
    // new_dir.create(std::string("i.filename"), parent_dir.get_info_link(), DIR);
}

void FileManager::create_new_file(Dir &parent_dir)
{
    slfs::File new_file;
    std::string str;
    std::cin >> str;
    new_file.create(str, parent_dir.get_info_link(), FILE);
    IFilesystem::fs_manager->add_file_to_dir(new_file.get_info_link(), parent_dir.get_info_link(), FILE);
    std::cin >> str;
    new_file.write((char *)str.c_str());
}



} // slfs
