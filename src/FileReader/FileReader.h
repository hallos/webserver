#ifndef FILEREADER_H
#define FILEREADER_H

#include <string>
#include <mutex>
#include <map>
#include <memory>
#include "File.h"
#include <experimental/filesystem>

namespace filesystem = std::experimental::filesystem;

class FileReader
{
    public:
        FileReader(const std::string& rootDirectory);
        ~FileReader() = default;

        const std::shared_ptr<File> getFile(const std::string &filename);
        const std::string getDirectory();

    private:
        filesystem::path directory;
        std::map<std::string, std::shared_ptr<File>> fileCache;
        std::mutex cacheMutex;

        bool bufferFile(std::string filename);
        bool addFileToCache(std::shared_ptr<File> newFile);

};

#endif // FILEREADER_H