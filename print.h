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

template <typename T>
std::string vectorToStr(const std::vector<T>& v,
     const size_t maxPrintSize = size_t(-1),
     const std::string& sep = ", ",
     const std::string& opening = "[",
     const std::string& closing = "]",
     bool removeEndSep = true)
{
    std::ostringstream out;
    typedef typename std::vector<T>::const_iterator const_iterator;
    if (v.size() == 0)
    {
        out << opening << " " << closing;
    }
    else
    {
        const_iterator end{v.end() - 1};
        bool isContinue{false};
        if (v.size() > maxPrintSize)
        {
            end = v.begin() + Max_Print_Vector_Size - 1;
            isContinue = true;
        }
        out << opening;
        const_iterator it = v.begin();
        for (; it != end; ++it)
            out << *it << sep;
        out << *it << (isContinue ? sep + "..." : (removeEndSep ? "" : sep)) << closing;
    }
    return out.str();
}

template <class STREAM, typename T>
STREAM& operator<<(STREAM& out, const std::vector<T>& v)
{
    out << vectorToStr(v, Max_Print_Vector_Size);
    return out;
}

inline void print()
{
    std::cout << std::endl;
}
template <typename T>
inline void printn(const T& a)
{
    std::cout << a;
}
template <typename T, class... ARG>
inline void printn(const T& a, const ARG&... arg)
{
    printn(a);
    std::cout << ", ";
    printn(arg...);
}
template <class... ARG>
inline void print(const ARG&... arg)
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
    std::cout << typeNameSimplifed(v) << "(shape:" << vectorToStr(getShape(v), 5, ",", "(", ")", false)
              << ", " << out.str() << ")";
}
template <typename T, class... ARG>
inline void reprn(const T& a, const ARG&... arg)
{
    reprn(a);
    std::cout << ", ";
    reprn(arg...);
}
template <class... ARG>
inline void repr(const ARG&... arg)
{
    reprn(arg...);
    std::cout << std::endl;
}
template <typename T>
std::string typeName(const T& v, bool showStd = true)
{
    std::string typeStr = typeid(v).name();
    //    typeStr = std::regex_replace(typeStr, std::regex(",class std::allocator<(?:[^><]+|<(?:[^><]+|<[^><]*>)*>)*> "), "");
    std::string classAllocator = ",class std::allocator";
    const size_t found = typeStr.find(classAllocator);
    if (found != std::string::npos)
    {
        typeStr = typeStr.substr(0, found);
        const size_t numFound = countSubString(typeStr, "vector");
        typeStr += std::string(numFound, '>');
    }
    typeStr = std::regex_replace(typeStr, std::regex("std::basic_string<char,struct std::char_traits<char>"), "std::string");
    typeStr = std::regex_replace(typeStr, std::regex("class "), "");
    if (!showStd) typeStr = std::regex_replace(typeStr, std::regex("std::"), "");
    return typeStr;
}
template <typename T>
std::string typeNameSimplifed(const T& v)
{
    std::string typeStr = typeName(v, false);
    typeStr = std::regex_replace(typeStr, std::regex("vector<complex<float>>"), "vectCompFloat");
    typeStr = std::regex_replace(typeStr, std::regex("vector<complex<double>>"), "vectCompDouble");
    typeStr = std::regex_replace(typeStr, std::regex("vector<float>"), "vectFloat");
    typeStr = std::regex_replace(typeStr, std::regex("vector<double>"), "vectDouble");
    typeStr = std::regex_replace(typeStr, std::regex("vector<string>"), "vectStr");
    typeStr = std::regex_replace(typeStr, std::regex("complex<float>"), "compFloat");
    typeStr = std::regex_replace(typeStr, std::regex("complex<double>"), "compDouble");

    return typeStr;
}

template <typename T>
std::vector<size_t> getShape(const T&)
{
    return std::vector<size_t>();
}
template <typename T>
std::vector<size_t> getShape(const std::vector<T>& v)
{
    size_t size{v.size()};
    std::vector<size_t> shp;
    shp.push_back(size);
    std::vector<size_t> nested;
    if (size == 0)
    {
        std::vector<T> vt(v);
        vt.push_back(T());
        nested = getShape(vt.front());
    }
    else
    {
        nested = getShape(v.front());
    }
    shp.insert(shp.end(), nested.begin(), nested.end());
    return shp;
}

// clang-format off
size_t countSubString(const std::string& str, const std::string& sub, size_t start = 0)
{
    size_t num = 0;
    const size_t len = sub.empty() ? 1 : sub.length();
    for (size_t i = start; (i = str.find(sub, i)) != std::string::npos; num++, i += len);
    return num;
}
#endif // PRINT_H


