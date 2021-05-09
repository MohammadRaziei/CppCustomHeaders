#ifndef PRINT_H
#define PRINT_H

#include <iostream>
#include <iterator>
#include <vector>
#include <sstream>
#include <regex>

#define eprintf(format, ...) fprintf(stderr, format __VA_OPT__(, ) __VA_ARGS__)
#define show(x)              \
    std::cout << #x << ": "; \
    repr(x);

#ifndef Max_Print_Vector_Size
#define Max_Print_Vector_Size (10)
#endif // Max_Print_Vector_Size

template <class STREAM, typename T>
STREAM& operator<<(STREAM& out, const std::vector<T>& v)
{
    typedef typename std::vector<T>::const_iterator const_iterator;
    if (v.size() == 0)
    {
        out << "[ ]";
    }
    else
    {
        const_iterator end{v.end() - 1};
        bool isContinue{false};
        if (v.size() > Max_Print_Vector_Size)
        {
            end = v.begin() + Max_Print_Vector_Size - 1;
            isContinue = true;
        }
        out << "[";
        const_iterator it = v.begin();
        for (; it != end; ++it)
            out << *it << ", ";
        out << *it << (isContinue ? ", ...]" : "]");
    }
    return out;
}

inline void print()
{
    std::cout << std::endl;
}
template <typename T>
inline void printn(T a)
{
    std::cout << a;
}
template <typename T, class... ARG>
inline void printn(T a, ARG... arg)
{
    printn(a);
    std::cout << ", ";
    printn(arg...);
}
template <class... ARG>
inline void print(ARG... arg)
{
    printn(arg...);
    std::cout << std::endl;
}

///
/// REPR
///

template <typename T>
inline void reprn(const T& value)
{
    std::ostringstream out;
    out << value;
    std::cout << typeName(value) << "(" << out.str() << ")";
}
template <typename T>
void reprn(const std::vector<T>& v)
{
    std::ostringstream out;
    out << v;
    std::cout << typeName(v) << "(size:" << v.size() << ", " << out.str() << ")";
}
template <typename T, class... ARG>
inline void reprn(T a, ARG... arg)
{
    reprn(a);
    std::cout << ", ";
    reprn(arg...);
}
template <class... ARG>
inline void repr(ARG... arg)
{
    reprn(arg...);
    std::cout << std::endl;
}
template <typename T>
std::string typeName(const T& v)
{
    std::string typeStr = typeid(v).name();
    typeStr = std::regex_replace(typeStr, std::regex(",class std::allocator<(?:[^><]+|<(?:[^><]+|<[^><]*>)*>)*> "), "");
    typeStr = std::regex_replace(typeStr, std::regex("std::basic_string<char,struct std::char_traits<char>"), "std::string");
    typeStr = std::regex_replace(typeStr, std::regex("class "), "");
    return typeStr;
}

#endif // PRINT_H
