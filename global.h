#ifndef GLOBAL_H
#define GLOBAL_H

#include <vector>
#include <fstream>
#include <Windows.h>
#include <cstdlib>
#include <string>
#include <algorithm>

namespace io
{
template <typename T>
void readFromFile(const std::string filename,
     std::vector<T>& vec,
     size_t length = (size_t)(-1),
     size_t* start = nullptr) noexcept
{
    // open the file:
    std::ifstream infile(filename, std::ios::binary);
    if (!infile.is_open())
    {
        fprintf(stderr, "Cannot open: \"%s\"\n", filename.c_str());
        infile.close();
        return;
    }
    // Stop eating new lines in binary mode!!!
    infile.unsetf(std::ios::skipws);

    const size_t startByte = start ? *start * sizeof(T) : 0l;
    // get its size:
    infile.seekg(0, std::ios::end);
    size_t fileSizeByte = infile.tellg();
    if (startByte > fileSizeByte)
    {
        fprintf(stderr, "startByte > fileSizeByte\n");
        infile.close();
        return;
    }
    infile.seekg(startByte, std::ios::beg);
    fileSizeByte -= startByte;

    size_t fileSize = fileSizeByte / sizeof(T);
    fileSize = std::min<size_t>(fileSize, length);
    if (start) *start += fileSize;

    vec.resize(fileSize);
    infile.read(reinterpret_cast<char*>(vec.data()), vec.size() * sizeof(T));
    infile.close();
}
template <typename T = float>
std::vector<T> readFromFile(const std::string filename,
     size_t length = (size_t)(-1),
     size_t* start = nullptr) noexcept
{
    std::vector<T> buf;
    readFromFile(filename, buf, length, start);
    return buf;
}

template <typename T>
void readStereoFromFile(const std::string& filename, std::vector<T>& vecReal, std::vector<T>& vecImag, const size_t length = (size_t)-1, size_t* start = nullptr)
{
    // open the file:
    std::ifstream infile(filename, std::ios::binary);
    if (!infile.is_open())
    {
        fprintf(stderr, "Cannot open: \"%s\"\n", filename.c_str());
        infile.close();
        return;
    }
    // Stop eating new lines in binary mode!!!
    infile.unsetf(std::ios::skipws);

    const size_t startByte = start ? *start * sizeof(T) : 0l;
    // get its size:
    infile.seekg(0, std::ios::end);
    size_t fileSizeByte = infile.tellg();
    if (startByte > fileSizeByte)
    {
        fprintf(stderr, "startByte > fileSizeByte\n");
        infile.close();
        return;
    }
    infile.seekg(startByte, std::ios::beg);
    fileSizeByte -= startByte;

    size_t fileSize = fileSizeByte / sizeof(T);
    fileSize = std::min<size_t>(fileSize, length * 2);
    if (start) *start += fileSize;
    fileSize /= 2;

    vecReal.resize(fileSize);
    vecImag.resize(fileSize);

    T* vecRealPtr{vecReal.data()};
    T* vecImagPtr{vecImag.data()};
    T readingVar;
    char* readingChar = reinterpret_cast<char*>(&readingVar);
    size_t i = 0;
    while ((!infile.eof()) && (++i < fileSize))
    {
        infile.read(readingChar, sizeof(T));
        *(vecRealPtr++) = readingVar;
        infile.read(readingChar, sizeof(T));
        *(vecImagPtr++) = readingVar;
    }
    vecReal.resize(i);
    vecImag.resize(i);
    infile.close();
}

std::string readTextFile(const std::string& filename) noexcept
{
    std::ifstream infile(filename);
    std::string src((std::istreambuf_iterator<char>(infile)),
         (std::istreambuf_iterator<char>()));
    return src;
}

//template<typename T>
//void writeToFile(const std::string& filename, const std::vector<T>& myVector)
//{
//	std::ofstream ofs(filename, std::ios::out | std::ofstream::binary);
//	std::ostream_iterator<char> osi{ ofs };
//	std::copy(myVector.begin(), myVector.end(), osi);
//}

template <typename T>
void writeToFile(const std::string& filename, const std::vector<T>& data)
{
    std::ofstream outFile(filename, std::ios::out | std::ofstream::binary);
    if (!outFile.is_open())
    {
        fprintf(stderr, "Cannot open: \"%s\"\n", filename.c_str());
        return;
    }
    outFile.write(reinterpret_cast<const char*>(data.data()), sizeof(T) * data.size());
    outFile.close();
}
} // namespace io

namespace os
{
std::vector<std::string> listDir(const std::string& path, const std::string& mask = "*.*")
{
    std::vector<std::string> filenames;
    std::string search_path = path + "/" + mask;
    WIN32_FIND_DATAA fd;
    HANDLE hFind = ::FindFirstFileA(search_path.c_str(), &fd);
    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            /// read all (real) files in current folder
            /// , delete '!' read other 2 default folder . and ..
            if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                filenames.push_back(fd.cFileName);
            }
        } while (::FindNextFileA(hFind, &fd));
        ::FindClose(hFind);
    }
    return filenames;
}
} // namespace os

#endif // GLOBAL_H
