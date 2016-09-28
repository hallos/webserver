#include "file.h"

file::file(const string &filename, const string &content, const string &contentType)
{
    this->filename = filename;
    this->content = content;
    this->contentType = contentType;
}

file::~file()
{
    //dtor
}
