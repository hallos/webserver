#include "catch.hpp"
#include "FileReader.h"
#include <fstream>

TEST_CASE("getDirectory() returns root directory that has been set")
{
	std::string rootDir = "/home/baby/jesus/";
    FileReader fileReader(rootDir);

    std::string returnedDir = fileReader.getDirectory(); 
    REQUIRE(returnedDir == rootDir);
}

TEST_CASE("getFile() returns a File-object corresponding to requested file")
{
	std::string rootDir = "test/tmp/";
    FileReader fileReader(rootDir);

    // Create a test file
    std::string filename = "test.html";
    std::string filecontent = "I am a test file";
    std::ofstream file(rootDir + filename);
    file << filecontent;
    file.close();

    auto returnedFile = fileReader.getFile(filename);
    REQUIRE(returnedFile);
    REQUIRE(returnedFile->getContent() == filecontent);
}