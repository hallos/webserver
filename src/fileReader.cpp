#include "fileReader.h"
#include <fstream>
#include <iostream>

/** \brief Buffers a file in server-directory to a new file object
 *  and adds it to the fileCache
 *
 * \param string filename - File to buffer
 * \return bool - True if file is successfully buffered, false o/w
 *
 */
bool fileReader::bufferFile(string filename)
{
    fstream file;
    file.exceptions( ifstream::failbit | ifstream::badbit );

    try{
        string content;
        //Open file and read the file to string
        if( filename.find(".jpg") != string::npos )
        {
            char tmpChar=0;
            string tmpStr;
            file.open(this->getDirectory() + filename, ios_base::in|ios_base::binary);
            file.seekg(0);
            while( file.read((char*) &tmpChar, sizeof(tmpChar)) )
            {
                tmpStr = tmpChar;
                content.append(tmpStr);
            }
            file.close();
        }
        else
        {
            file.open(this->getDirectory() + filename);
            string tmp, readFromFile;

            while(!file.eof())
            {
                getline(file, tmp);
                readFromFile += tmp + "\n";
            }
            file.close();
            content = readFromFile;
        }

        unique_ptr<fileObject> tmpPtr( new fileObject(filename, content, "text/html") );

        if( this->addFileToCache(move(tmpPtr) ))
            return true;

        return false;
    }
    catch(const ios_base::failure& e){
        cerr << "file not found.." << e.what() << endl;
        return false;
    }
}

/** \brief Sets the working directory of the fileReader
 *  and buffers the index.html file to the fileCache
 *
 * \param dir
 * \return bool - True if directory is successfully set and a index.html file is buffered, false o/w
 *
 */
bool fileReader::setDirectory(string dir)
{
    if( dir.back() != '/' && dir.back() != '\\')
        dir.push_back('/');
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

/** \brief Returns current working directory
 *
 * \return string
 *
 */
string fileReader::getDirectory()
{
    dirMutex.lock();
    string tmp = directory;
    dirMutex.unlock();
    return tmp;
}

/** \brief Adds a file to the fileCache.
 *
 * \param newFile unique_ptr<fileObject> - Pointer to fileObject to be added
 * \return bool - True if file is successfully added to cache, false o/w
 *
 */
bool fileReader::addFileToCache(unique_ptr<fileObject> newFile)
{
    if(newFile)
    {
        cacheMutex.lock();
        fileCache.insert( pair<string, unique_ptr<fileObject>>(newFile->getFilename(), move(newFile) ));
        cacheMutex.unlock();
        return true;
    }
    return false;
}

/** \brief Returns a requested file in working directory or a sub directory
 *  specified by filename.
 *
 * \param filename const string& - Requested file
 * \param retFileContent string& - Content of requested file if found
 * \param retContentType string& - Content-type of requested file if found
 * \return bool - True if requested file is found, false o/w
 *
 */
bool fileReader::getFile(const string &filename, string &retFileContent, string &retContentType)
{
    if(!filename.empty())
    {
        bool fileInCache = false;
        cacheMutex.lock();
        auto it = fileCache.find(filename);
        cacheMutex.unlock();

        if( it != fileCache.end() )
        {
            fileInCache = true;
        }
        else if(bufferFile(filename))
        {
            fileInCache = true;
        }
        if(fileInCache)
        {
            cacheMutex.lock();
            retFileContent = fileCache[filename]->getContent();
            retContentType = fileCache[filename]->getContentType();
            cacheMutex.unlock();
            return true;
        }
    }
    return false;
}
