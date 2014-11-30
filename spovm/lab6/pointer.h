#ifndef POINTER_H
#define POINTER_H

#include <iostream>
#include "collector.h"


namespace GC {

template<typename T>
class Pointer {
    int id;
    T *object;
  public:
    Pointer();
    Pointer(T &value);
    Pointer(const Pointer& pointer);
    ~Pointer();
    T &operator*();
    T *operator->();
    Pointer &operator=(Pointer<T> &new_value);
    Pointer &operator=(T &value);
  private:
    void create_new_object();
};



template<typename T>
Pointer<T>::Pointer()
{
    create_new_object();
}

template<typename T>
Pointer<T>::Pointer(T &value)
{
    create_new_object();
    *object = value;
}

template<typename T>
Pointer<T>::Pointer(const Pointer& pointer)
{
    object = pointer.object;
    id = pointer.id;
    Memory::add_link(id);
}

template<typename T>
Pointer<T>::~Pointer()
{
    Memory::delete_link(id);
}

template<typename T>
void Pointer<T>::create_new_object()
{
    MemoryInfo info = Memory::get_new(sizeof(T));
    object = reinterpret_cast<T *>(info.pointer);
    id = info.id;
}

template<typename T>
T &Pointer<T>::operator*() 
{
    return *object;
}

template<typename T>
T *Pointer<T>::operator->() 
{ 
    return object; 
}

template<typename T>
Pointer<T> &Pointer<T>::operator=(Pointer<T> &new_value) 
{
    Memory::delete_link(id);
    object = new_value.object;
    id = new_value.id;
    Memory::add_link(id);
    return *this;
}

template<typename T>
Pointer<T> &Pointer<T>::operator=(T &value)
{
    Memory::delete_link(id);
    create_new_object();
    *object = value;
    return *this;
}


} // GC namespace
#endif // POINTER_H
