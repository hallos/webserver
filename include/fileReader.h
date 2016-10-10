#ifndef FILEREADER_H
#define FILEREADER_H

#include <string>
#include <mutex>
#include <map>
#include <memory>
#include "file.h"

using namespace std;

class fileReader
{
    public:
        fileReader() {}
        ~fileReader() {}

        bool getFile(const string &filename, string &retFileContent, string &retContentType);
        bool setDirectory(const string &dir);
        string getDirectory();


    protected:

    private:
        string directory;
        map<string, unique_ptr<fileObject>> fileCache;

        mutex dirMutex;
        mutex cacheMutex;

        bool bufferFile(string filename);
        bool addFileToCache(unique_ptr<fileObject> newFile);


};

#endif // FILEREADER_H
