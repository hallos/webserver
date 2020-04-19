#ifndef FILE_H
#define FILE_H

#include <string>

class File
{
    public:
        File(const std::string &filename, const std::string &content, const std::string &contentType)
            : filename(filename), content(content), contentType(contentType) {};
        ~File() = default;

        std::string getFilename() { return filename; }
        void setFilename(const std::string& name) { filename = name; }
        std::string getContent() { return content; }
        void setContent(const std::string& cont) { content = cont; }
        std::string getContentType() { return contentType; }
        void setContentType(const std::string& contType) { contentType = contType; }

    private:
        std::string filename;
        std::string content;
        std::string contentType;
};

#endif // FILE_H
