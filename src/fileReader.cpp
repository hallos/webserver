#include "fileReader.h"
#include <fstream>

fileReader::fileReader()
{
    //ctor
}

fileReader::~fileReader()
{
    //dtor
}

bool fileReader::bufferFile(string filename)
{
    fstream file;
    file.exceptions( ifstream::failbit | ifstream::badbit );

    try{
        //Open file and read the file to string
        file.open(this->getDirectory() + filename);
        string tmp, readFromFile;

        while(!file.eof()){
            getline(file, tmp);
            readFromFile += tmp + "\n";
        }
        this->addFileToCache(readFromFile);

        return true;
    }
    catch(...){
        cerr << "file not found..";
        return false;
    }
}

bool fileReader::setDirectory(const string &dir)
{
    dirMutex.lock();
    //Save old directory
    string oldDir = directory;
    //Set directory string
    directory = dir;
    dirMutex.unlock();
    //Try to buffer index-file to fileCache
    if(bufferFile("index.html")){
        return true;
    }
    //Set directory as old-directory if buffering of index file fails
    dirMutex.lock();
    directory = oldDir;
    dirMutex.unlock();
    return false;
}

string fileReader::getDirectory()
{
    dirMutex.lock();
    string tmp = directory;
    dirMutex.unlock();
    return tmp;
}
