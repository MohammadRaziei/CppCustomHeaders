#include "vectorOperators.h"
#include "print.h"
#include "global.h"
#include "defines.h"

int main()
{
    print("Hello World!", 5, 2.5, true);
    print("hi");
    print();
    print(3, vectDouble(), 2);
    repr(3, vectDouble(), 2);

    show(os::listDir("."));

    vectCompFloat a{{1.f, 2.f}, {3.f, 4.f}, {5.f, 6.f}};
    show(a);
    io::writeToFile("__test__output.bin", a);
    a = io::readFromFile<compFloat>("__test__output.bin");
    show(a);
}
