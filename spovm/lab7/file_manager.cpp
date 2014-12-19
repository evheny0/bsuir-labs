#include "file_manager.h"
namespace slfs {


FileManager::FileManager()
{
    slfs::Filesystem::init("slfs.fs");
    // gr::init_ncurses();
}

FileManager::~FileManager()
{
    gr::stop_ncurses();
    slfs::Filesystem::end();
}

void FileManager::start()
{
    gr::print_to("TEST", 2, 2);
    char a = getchar();
    // gr::print_to(&a, 2, 3);
    slfs::File f1("test/in/file.cpp");
    slfs::File f2("test/in/file.h");
    slfs::File f3("qq/in/file.cpp");
    slfs::File f4("alalal");
    slfs::Dir root(std::string("/"));
    std::vector<Dir> dirs = root.get_dirs_list();
    for(auto&& i : dirs) {
        puts(i.get_c_name());
    }
    getchar();
    // dirs = 
    // while (1) {
        
    // }
}


} // slfs
