#include "File.h"

File::File(const std::string &filename, const std::string &content, const std::string &contentType)
{
    this->filename = filename;
    this->content = content;
    this->contentType = contentType;
}

File::~File()
{
    //dtor
}
