#include <iostream>
#include <string>
#include "pointer.h"

class Lala
{
public:
    Lala() {};
    ~Lala() {};
    int very_complex_calculation() { return 5 + 5; }
};

void fun2(GC::Pointer<Lala> l)
{
    std::cout << "fun2 is: " << l->very_complex_calculation() << std::endl;
}

void fun()
{
    GC::Pointer<Lala> a;
    int test = a->very_complex_calculation();
    std::cout << "fun is: " << test << std::endl;
}



int main(int argc, char const *argv[])
{
    int test;
    GC::Pointer<Lala> a;
    test = a->very_complex_calculation();
    std::cout << test << std::endl;

    fun();
    fun2(a);
    return 0;
}
