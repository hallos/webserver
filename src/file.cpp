#include "file.h"

fileObject::fileObject(const string &filename, const string &content, const string &contentType)
{
    this->filename = filename;
    this->content = content;
    this->contentType = contentType;
}

fileObject::~fileObject()
{
    //dtor
}
