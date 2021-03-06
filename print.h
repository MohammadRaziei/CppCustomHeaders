#ifndef PRINT_H
#define PRINT_H
#ifdef __GNUG__
#include <cxxabi.h>
#endif
#include <iostream>
#include <iterator>
#include <vector>
#include <sstream>
#include <regex>
#include <type_traits>
#include <iomanip>
#include <cmath>

#define eprintf(format, ...) fprintf(stderr, format __VA_OPT__(, ) __VA_ARGS__)
#define show(x)              \
    std::cout << #x << ": "; \
    repr(x);

#ifndef Max_Print_Vector_Size
#define Max_Print_Vector_Size (10)
#endif // Max_Print_Vector_Size

template <typename T>
struct has_const_iterator
{
private:
    typedef char yes;
    typedef struct
    {
        char array[2];
    } no;

    template <typename C>
    static yes test(typename C::const_iterator*);
    template <typename C>
    static no test(...);

public:
    static const bool value = sizeof(test<T>(0)) == sizeof(yes);
    typedef T type;
};

template <typename T>
struct has_begin_end
{
    template <typename C>
    static char (&f(typename std::enable_if<
         std::is_same<decltype(static_cast<typename C::const_iterator (C::*)() const>(&C::begin)),
              typename C::const_iterator (C::*)() const>::value,
         void>::type*))[1];

    template <typename C>
    static char (&f(...))[2];

    template <typename C>
    static char (&g(typename std::enable_if<
         std::is_same<decltype(static_cast<typename C::const_iterator (C::*)() const>(&C::end)),
              typename C::const_iterator (C::*)() const>::value,
         void>::type*))[1];

    template <typename C>
    static char (&g(...))[2];

    static bool const beg_value = sizeof(f<T>(0)) == 1;
    static bool const end_value = sizeof(g<T>(0)) == 1;
};
template <typename T>
struct is_container : std::integral_constant<bool, !std::is_same<T, std::string>::value && has_const_iterator<T>::value && has_begin_end<T>::beg_value && has_begin_end<T>::end_value>
{
};

namespace np
{
// clang-format off
size_t countSubString(const std::string& str, const std::string& sub, size_t start = 0)
{
    size_t num = 0;
    const size_t len = sub.empty() ? 1 : sub.length();
    for (size_t i = start; (i = str.find(sub, i)) != std::string::npos; num++, i += len);
    return num;
}
// clang-format on

///
/// typeName
///
template <typename T>
std::string typeName(const T& v, bool showStd = true)
{
    std::string typeStr = typeid(v).name();
#ifdef __GNUG__
    int status = -4; /// some arbitrary value to eliminate the compiler warning
    typeStr = std::string(abi::__cxa_demangle(typeStr.c_str(), nullptr, nullptr, &status));
    typeStr = std::regex_replace(typeStr, std::regex("__cxx\\d*::"), "");
    typeStr = std::regex_replace(typeStr, std::regex("__1::"), "");
    std::smatch match;
    if (std::regex_search(typeStr, match, std::regex("\\d+(ull)")))
        typeStr = typeStr.substr(0, match.position(0) + 2) + typeStr.substr(match.position(0) + match.str(0).size());
#endif
    typeStr = std::regex_replace(typeStr, std::regex("class"), "");
    typeStr = std::regex_replace(typeStr, std::regex("struct"), "");
    typeStr = std::regex_replace(typeStr, std::regex("const"), "");
    typeStr = std::regex_replace(typeStr, std::regex("__ptr64"), "");
    typeStr = std::regex_replace(typeStr, std::regex(" "), "");
    typeStr = std::regex_replace(typeStr, std::regex("std::basic_string<char,std::char_traits<char>,std::allocator<char>>"), "std::string");
    std::string classAllocator = ",std::allocator";
    const size_t found = typeStr.find(classAllocator);
    if (found != std::string::npos)
    {
        size_t numFound = countSubString(typeStr, classAllocator, found + 1) + 1;
        numFound = static_cast<size_t>(std::log2(static_cast<float>(numFound + 1)));
        typeStr = typeStr.substr(0, found) + std::string(numFound, '>');
    }
    if (!showStd) typeStr = std::regex_replace(typeStr, std::regex("std::"), "");
    return typeStr;
}
template <typename T>
std::string typeNameSimplifed(const T& v)
{
    std::string typeStr = typeName(v, false);
    typeStr = std::regex_replace(typeStr, std::regex("vector<complex<float>>"), "vectCompFloat");
    typeStr = std::regex_replace(typeStr, std::regex("vector<complex<double>>"), "vectCompDouble");
    typeStr = std::regex_replace(typeStr, std::regex("vector<complex<int>>"), "vectCompInt");
    typeStr = std::regex_replace(typeStr, std::regex("vector<float>"), "vectFloat");
    typeStr = std::regex_replace(typeStr, std::regex("vector<double>"), "vectDouble");
    typeStr = std::regex_replace(typeStr, std::regex("vector<int>"), "vectInt");
    typeStr = std::regex_replace(typeStr, std::regex("vector<string>"), "vectStr");
    typeStr = std::regex_replace(typeStr, std::regex("complex<float>"), "compFloat");
    typeStr = std::regex_replace(typeStr, std::regex("complex<double>"), "compDouble");
    typeStr = std::regex_replace(typeStr, std::regex("complex<int>"), "compInt");
    return typeStr;
}

///
/// getShape:
///
typedef std::vector<size_t> TensorShape;
template <typename T, typename = typename std::enable_if<!is_container<T>::value>::type>
void getShape(const T&, TensorShape&)
{
}
template <typename Container,
     typename = typename std::enable_if<is_container<Container>::value>::type,
     typename T = typename std::iterator_traits<typename Container::iterator>::value_type>
void getShape(const Container& v, TensorShape& sizeShape)
{
    size_t size{v.size()};
    sizeShape.push_back(size);
    getShape((size == 0) ? T() : *v.begin(), sizeShape);
}
template <typename T>
TensorShape getShape(const T& v)
{
    TensorShape shp;
    getShape(v, shp);
    return shp;
}

///
/// toStr
///
template <typename Container,
          typename = typename std::enable_if<is_container<Container>::value>::type,
     typename const_iterator = typename Container::const_iterator>
std::string toStr(const Container& v,
     const size_t maxPrintSize = size_t(-1),
     const std::string& sep = ", ",
     const std::string& opening = "[",
     const std::string& closing = "]",
     bool removeEndSep = true)
{
    std::ostringstream out;
    if (v.size() == 0)
    {
        out << opening << " " << closing;
    }
    else
    {
        size_t size{std::min<size_t>(maxPrintSize, v.size())};
        bool isContinue{(v.size() > maxPrintSize)};
        out << opening;
        const_iterator it = v.begin();
        for (size_t i{0}; (++i) < size; ++it)
            out << *it << sep;
        out << *it << (isContinue ? sep + "..." : (removeEndSep ? "" : sep)) << closing;
    }
    return out.str();
}
template <typename T, typename = typename std::enable_if<!is_container<T>::value>::type>
std::string toStr(const T& v)
{
    std::ostringstream out;
    out << std::boolalpha << v;
    return out.str();
}
template <typename CT,
     std::size_t N,
     //     typename T = typename std::remove_const<CT>::type,
     typename = typename std::enable_if<!std::is_same<CT, char>::value>::type>
std::string toStr(const CT (&arr)[N], const size_t& maxPrintSize = Max_Print_Vector_Size)
{
    return toStr(std::vector<CT>(arr, arr + N), maxPrintSize);
}

} // namespace np

template <class STREAM, typename Container, typename = typename std::enable_if<is_container<Container>::value>::type>
STREAM& operator<<(STREAM& out, const Container& v)
{
    out << np::toStr(v, Max_Print_Vector_Size);
    return out;
}
template <class STREAM,
     typename CT,
     std::size_t N,
     typename = typename std::enable_if<!std::is_same<CT, char>::value>::type>
STREAM& operator<<(STREAM& out, const CT (&v)[N])
{
    out << np::toStr(v, Max_Print_Vector_Size);
    return out;
}

void print()
{
    std::cout << std::endl;
}
template <typename T>
void printn(const T& a)
{
    std::cout << np::toStr(a);
}
template <typename T, class... ARG>
void printn(const T& a, const ARG&... arg)
{
    printn(a);
    std::cout << ", ";
    printn(arg...);
}
template <class... ARG>
void print(const ARG&... arg)
{
    printn(arg...);
    std::cout << std::endl;
}

///
/// REPR
///

template <typename T>
inline void _reprn(const T& value, std::false_type)
{
    std::ostringstream out;
    out << np::toStr(value);
    std::cout << np::typeNameSimplifed(value) << "(" << out.str() << ")";
}
template <typename Container>
inline void _reprn(const Container& v, std::true_type)
{
    std::ostringstream out;
    out << np::toStr(v, Max_Print_Vector_Size);
    std::cout << np::typeNameSimplifed(v) << "(shape:" << np::toStr(np::getShape(v), 5, ",", "(", ")", false)
              << ", " << out.str() << ")";
}
template <typename T>
void reprn(const T& v)
{
    _reprn(v, is_container<T>());
}
template <typename T, class... ARG>
void reprn(const T& a, const ARG&... arg)
{
    reprn(a);
    std::cout << ", ";
    reprn(arg...);
}
template <class... ARG>
void repr(const ARG&... arg)
{
    reprn(arg...);
    std::cout << std::endl;
}

#endif // PRINT_H
