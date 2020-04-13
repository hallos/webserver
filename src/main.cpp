#include "HTTP.h"
#include "Server.h"
#include <FileReader.h>
#include "WebConnectionHandler.h"
#include <vector>
#include <iostream>
#include <clara.hpp>


int main(int argc, char** argv)
{
    int port = 8080;
    int nrThreads = 4;
    std::string rootDirectory = "test/system/";
    bool showHelp = false;
    auto cli = clara::Help(showHelp)
             | clara::Opt(port, "port")
                ["-p"]["--port"]
                ("Port to be used by webserver to receive connections." )
             | clara::Opt(rootDirectory, "directorypath")
                ["-d"]["--root-directory"]
                ("Root directory of webserver. (Where to find index.html)" )
             | clara::Opt(nrThreads, "threadcount")
                ["-t"]["--number-threads"]
                ("Number if threads in internal threadpool serving incoming requests." );
    auto result = cli.parse(clara::Args(argc, argv));
    if (!result) 
    {
        std::cerr << "Error in command line: " << result.errorMessage() << std::endl;
        exit(1);
    }
    else if (showHelp)
    {
        cli.writeToStream(std::cout);
        return 0;
    }
    // Validate input data
    if (nrThreads < 1)
    {
        nrThreads = 1;
    }

    auto serverSocket = std::make_shared<TCPServerSocket>(port);
    auto fileReader = std::make_shared<FileReader>(rootDirectory);
    auto connectionHandler = std::make_shared<WebConnectionHandler>(fileReader);
    Server webserver(serverSocket, connectionHandler, nrThreads);
    webserver.startServer();

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    return 0;
}
