#ifndef FILEREADER_H
#define FILEREADER_H

#include <string>
#include <mutex>
#include <map>
#include <memory>
#include "File.h"

class FileReader
{
    public:
        FileReader(const std::string& rootDirectory);
        ~FileReader() {}

        bool getFile(const std::string &filename, std::string &retFileContent, std::string &retContentType);
        std::string getDirectory();

    private:
        std::string directory;
        std::map<std::string, std::unique_ptr<File>> fileCache;

        std::mutex dirMutex;
        std::mutex cacheMutex;

        bool bufferFile(std::string filename);
        bool addFileToCache(std::unique_ptr<File> newFile);

};

#endif // FILEREADER_H
