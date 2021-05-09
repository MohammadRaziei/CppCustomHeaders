#define Max_Print_Vector_Size (8)

#include "vectorOperators.h"
#include "print.h"
#include "global.h"
#include "defines.h"

int main()
{
    vectCompFloat a{{1.f, 2.f}, {3.f, 4.f}, {5.f, 6.f}};
    std::vector<vectCompFloat> vecA(2, a);
    vectFloat v(100);
    std::generate(v.begin(), v.end(), [n = 0.f]() mutable { return float(++n); });
    show(v);
    print(getShape(std::vector<vectDouble>()));
    print("Hello World!", 5, 2.5, true);
    print("hi");
    print();
    print(3, vectDouble(), 2);
    repr(3, vectDouble(), 2);
    repr(vecA);

    show(os::listDir("."));

    show(a);
    io::writeToFile("__test__output.bin", a);
    a = io::readFromFile<compFloat>("__test__output.bin");
    show(a);
}
