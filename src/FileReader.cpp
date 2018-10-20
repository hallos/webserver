#include "FileReader.h"
#include <fstream>
#include "Logger.h"

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

        std::unique_ptr<File> tmpPtr( new File(filename, content, "text/html") );

        if( this->addFileToCache(std::move(tmpPtr)) )
            return true;

        return false;
    }
    catch(const std::ios_base::failure& e)
    {
        Logger::log("FileReader::bufferFile(): file " + directory + filename + "not found.");
        if(file.is_open()) file.close(); //Close file if there is a file opened
        return false;
    }
}

/** \brief Sets the working directory of the FileReader
 *  and buffers the index.html file to the fileCache
 *
 * \param dir
 * \return bool - True if directory is successfully set and a index.html file is buffered, false o/w
 *
 */
bool FileReader::setDirectory(std::string dir)
{
    if( dir.back() != '/' && dir.back() != '\\')
        dir.push_back('/');
    dirMutex.lock();
    //Save old directory
    std::string oldDir = directory;
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
std::string FileReader::getDirectory()
{
    dirMutex.lock();
    std::string tmp = directory;
    dirMutex.unlock();
    return tmp;
}

/** \brief Adds a file to the fileCache.
 *
 * \param newFile unique_ptr<File> - Pointer to File to be added
 * \return bool - True if file is successfully added to cache, false o/w
 *
 */
bool FileReader::addFileToCache(std::unique_ptr<File> newFile)
{
    if(newFile)
    {
        cacheMutex.lock();
        fileCache.insert( 
            std::pair<std::string, std::unique_ptr<File>>(
                newFile->getFilename(), 
                std::move(newFile))
        );
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
bool FileReader::getFile(const std::string &filename, std::string &retFileContent, std::string &retContentType)
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
