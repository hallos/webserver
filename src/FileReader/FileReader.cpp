#include "FileReader.h"
#include <fstream>
#include <sstream>
#include "Logger.h"


/**
 * 
 */ 
FileReader::FileReader(const std::string& rootDirectory): directory(rootDirectory)
{
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
    bool fileBuffered = false;
    std::fstream file;
    file.exceptions( std::ifstream::failbit | std::ifstream::badbit );

    try
    {
        //Open file and read the file to string
        file.open(this->getDirectory() + filename, std::ios_base::in|std::ios_base::binary);
        std::stringstream contentStream;
        contentStream << file.rdbuf();

        fileBuffered = this->addFileToCache(
            std::make_shared<File>(filename, contentStream.str(), "text/html")
        );
    }
    catch(const std::ios_base::failure& e)
    {
        Logger::log("FileReader::bufferFile(): file " + directory.string() + filename + "not found.");
    }

    //Close file
    if (file.is_open())
        file.close();

    return fileBuffered;
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

/** \brief Returns path to current root directory as a string
 *
 * \return const string& - Root-directory path
 *
 */
const std::string FileReader::getDirectory()
{
    return directory.string();
};