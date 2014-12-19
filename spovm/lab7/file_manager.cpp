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
    slfs::File f1("aa/file");
    slfs::File f2("bbb/file");
    slfs::File f4("ccc/file");
    slfs::File f5("ddd/file");
    slfs::File f6("eee/ccc/ggg/aaa");
    slfs::File f7("hhh/alal");
    bool is_run = true;
    int i;
    slfs::Dir root(std::string("/")), parent_dir(std::string("/"));
    std::vector<File *> dirs = root.get_all_files_list();
    std::vector<File *>::iterator it, current = dirs.begin();

    while(is_run) {
        i = 0;
        for(it = dirs.begin(); it != dirs.end(); it++, i++) {
            if (it == current) {
                gr::set_color(A_UNDERLINE);
            } else {
                gr::set_color(A_NORMAL);
            }
            gr::print_to((**it).get_c_name(), 2, i);
            print_type(*it, i);
        }
        // char sym = getchar();
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
                parent_dir = *((Dir *) *current);
                dirs = parent_dir.get_all_files_list();
                current = dirs.begin();
                clear();
            }
            break;
        case 'a':
            parent_dir.open(parent_dir.get_parent_link());
            dirs = parent_dir.get_all_files_list();
            current = dirs.begin();
            clear();
            break;
        case 27: 
            is_run = false; 
            break;
        }
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


} // slfs
