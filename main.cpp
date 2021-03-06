#define Max_Print_Vector_Size (8)

#include "vectorOperators.h"
#include "print.h"
#include "global.h"
#include "defines.h"

#include <list>
#include <array>

int main()
{
    std::cout << std::boolalpha;
    std::cout << is_container<std::vector<int>>::value << std::endl; ///true
    std::cout << is_container<std::list<int>>::value << std::endl; ///true
    std::cout << is_container<int>::value << std::endl; ///false
    vectCompFloat a{{1.f, 2.f}, {3.f, 4.f}, {5.f, 6.f}};
    std::cout << a << std::endl;
    int b[]{1, 2, 3};
    std::array<int, 10> arr{1};
    repr("arr", arr);
    int* pb{b};
    repr(b);
    std::cout << "b " << b << std::endl;
    repr(pb);

    std::vector<vectCompFloat> vecA(2, a);
    vectFloat v(100);
    repr(np::toStr(true), true, np::toStr(false), false);
    repr(np::toStr(a));
    repr(np::toStr(a).c_str());
    repr(a);
    print(np::toStr(a));
    std::generate(v.begin(), v.end(), [n = 0.f]() mutable { return float(++n); });
    show(v);
    repr(std::list<vectDouble>());
    repr(std::list<vectDouble>({{1.5, 2.5, 3.5}, {4.5, 5.5, 7.5}}));
    repr(std::list<vectDouble>({{1.5}}));
    print(np::getShape(5));
    print("Hello World!", 5, 2.5, true, 3.14f, 25l);
    repr("Hello World!", 5, 2.5, true, 3.14f, 25l);
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
