#ifndef FILEREADER_H
#define FILEREADER_H

#include <string>
#include <mutex>
#include <map>
#include <vector>

using namespace std;

class fileReader
{
    public:
        fileReader();
        ~fileReader();

        bool getFile(const string &filename, string &retFileContent, string &retContentType);
        bool setDirectory(const string &dir);
        string getDirectory();

    protected:

    private:
        string directory;
        map<string,vector<string>> fileCache;

        mutex dirMutex;
        mutex cacheMutex;

        bool bufferFile(string filename);
        bool addFileToCache(const string &filename, const string &content);
};

#endif // FILEREADER_H
