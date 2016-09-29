#ifndef FILE_H
#define FILE_H

#include <string>

using namespace std;

class fileObject
{
    public:
        fileObject(const string &filename, const string &content, const string &contentType);
        ~fileObject();

        string getFilename() { return filename; }
        void setFilename(string name) { filename = name; }
        string getContent() { return content; }
        void setContent(string cont) { content = cont; }
        string getContentType() { return contentType; }
        void setContentType(string contType) { contentType = contType; }

    private:
        string filename;
        string content;
        string contentType;
};

#endif // FILE_H
