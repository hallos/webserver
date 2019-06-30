#include "FileReader.h"
#include <fstream>
#include "Logger.h"

/**
 * 
 */ 
FileReader::FileReader(const std::string& rootDirectory): directory(rootDirectory)
{
    if (directory.back() != '/' && directory.back() != '\\')
    {
        directory.push_back('/');
    }
}

/** \brief Buffers a file in server-directory to a new file object
 *  and adds it to the fileCache
 *
 * \param string filename - File to buffer
 * \return bool - True if file is successfully buffered, false o/w
 *
 */
bool FileReader::bufferFile(std::string filename)
{
    std::fstream file;
    file.exceptions( std::ifstream::failbit | std::ifstream::badbit );

    try
    {
        std::string content;
        //Open file and read the file to string
        file.open(this->getDirectory() + filename, std::ios_base::in|std::ios_base::binary);
        file.seekg(0, file.beg);
        char tmpChar = 0;
        while( file.peek() != EOF )
        {
            file.read(&tmpChar, sizeof(tmpChar));
            content.push_back(tmpChar);
        }
        file.close();

        std::shared_ptr<File> tmpPtr( new File(filename, content, "text/html") );

        return this->addFileToCache(tmpPtr);
    }
    catch(const std::ios_base::failure& e)
    {
        Logger::log("FileReader::bufferFile(): file " + directory + filename + "not found.");
        if(file.is_open()) file.close(); //Close file if there is a file opened
        return false;
    }
}

/** \brief Returns root directory
 *
 * \return string
 *
 */
std::string FileReader::getDirectory()
{
    dirMutex.lock();
    std::string tmp = directory;
    dirMutex.unlock();
    return tmp;
}

/** \brief Adds a file to the fileCache.
 *
 * \param newFile shared_ptr<File> - Pointer to File to be added
 * \return bool - True if file is successfully added to cache, false o/w
 *
 */
bool FileReader::addFileToCache(std::shared_ptr<File> newFile)
{
    bool success = false;
    if (newFile)
    {
        cacheMutex.lock();
        fileCache.insert( 
            std::pair<std::string, std::shared_ptr<File>>(
                newFile->getFilename(), 
                newFile)
        );
        cacheMutex.unlock();
        success = true;
    }
    return success;
}

/** \brief Returns a requested file in working directory or a sub directory
 *  specified by filename.
 *
 * \param filename const string& - Requested file
 * \return std::shared_ptr<File> - Pointer to a File object containing filedata if found. Empty pointer otherwise.
 *
 */
const std::shared_ptr<File> FileReader::getFile(const std::string &filename)
{
    std::shared_ptr<File> file;
    if (!filename.empty())
    {
        cacheMutex.lock();
        auto it = fileCache.find(filename);
        cacheMutex.unlock();

        if( it != fileCache.end() || bufferFile(filename) )
        {
            cacheMutex.lock();
            file = fileCache[filename];
            cacheMutex.unlock();
        }
    }
    return file;
}
