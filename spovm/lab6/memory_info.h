#ifndef MEMORY_INFO_H
#define MEMORY_INFO_H

namespace GC {


class MemoryInfo {
  public:
    void *pointer;
    int id;
    int num_of_links;
    MemoryInfo();
    ~MemoryInfo();
};

} // GC namespace
#endif // MEMORY_INFO_H
