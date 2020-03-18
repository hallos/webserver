#ifndef FILEREADER_H
#define FILEREADER_H

#include <string>
#include <mutex>
#include <map>
#include <memory>
#include "File.h"
#include <experimental/filesystem>

class FileReader
{
    public:
        FileReader(const std::string& rootDirectory);
        ~FileReader() {}

        const std::shared_ptr<File> getFile(const std::string &filename);
        const std::string getDirectory() { return directory.string(); };

    private:
        std::experimental::filesystem::path directory;
        std::map<std::string, std::shared_ptr<File>> fileCache;
        std::mutex cacheMutex;

        bool bufferFile(std::string filename);
        bool addFileToCache(std::shared_ptr<File> newFile);

};

#endif // FILEREADER_H
