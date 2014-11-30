#include <iostream>
#include <string>
#include "pointer.h"

class Lala
{
public:
    Lala() {};
    ~Lala() {};
    int value() { return 5; }
};

int main(int argc, char const *argv[])
{
    int test;
    GC::Pointer<Lala> a(new Lala());
    test = a->value();
    std::cout << test;
    return 0;
}
