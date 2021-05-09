#ifndef PRINT_H
#define PRINT_H

#include <iostream>
#include <iterator>
#include <vector>

#define eprintf(format, ...) fprintf(stderr, format __VA_OPT__(, ) __VA_ARGS__)
#define show(x) std::cout << #x << ": " << x << std::endl

#ifndef Max_Print_Vector_Size
#define Max_Print_Vector_Size (10)
#endif // Max_Print_Vector_Size

template <class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
{
    if (v.size() == 0)
    {
        os << "[ ]" << std::endl;
        return os;
    }
    os << "[";
    for (typename std::vector<T>::const_iterator it = v.begin(); it != v.end() - 1; ++it)
        os << *it << ", ";
    os << *v.rbegin() << "]";
    return os;
}

inline void print()
{
    std::cout << std::endl;
}
template <typename T>
inline void __print(T a)
{
    std::cout << a;
}
template <typename T, class... ARG>
inline void __print(T a, ARG... arg)
{
    __print(a);
    std::cout << ", ";
    __print(arg...);
}
template <class... ARG>
inline void print(ARG... arg)
{
    __print(arg...);
    std::cout << std::endl;
}
template <typename T>
void __print(const std::vector<T>& v)
{
    typedef typename std::vector<T>::const_iterator const_iterator;
    if (v.size() == 0)
    {
        std::cout << "[ ]";
        return;
    }
    const_iterator end{v.end() - 1};
    bool isContinue{false};
    if (v.size() > Max_Print_Vector_Size)
    {
        end = v.begin() + Max_Print_Vector_Size - 1;
        isContinue = true;
    }
    std::cout << "[";
    const_iterator it = v.begin();
    for (; it != end; ++it)
        std::cout << *it << ", ";
    std::cout << *it << (isContinue ? ", ...]" : "]");
}

#endif // PRINT_H
