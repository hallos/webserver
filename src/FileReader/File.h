#ifndef FILE_H
#define FILE_H

#include <string>

class File
{
    public:
        File(const std::string &filename, const std::string &content, const std::string &contentType);
        ~File();

        std::string getFilename() { return filename; }
        void setFilename(std::string name) { filename = name; }
        std::string getContent() { return content; }
        void setContent(std::string cont) { content = cont; }
        std::string getContentType() { return contentType; }
        void setContentType(std::string contType) { contentType = contType; }

    private:
        std::string filename;
        std::string content;
        std::string contentType;
};

#endif // FILE_H
