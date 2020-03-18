
include_directories(../)
add_library(FileReader STATIC
    File.cpp
    FileReader.cpp
)

target_link_libraries(FileReader
    WebserverLib
    stdc++fs
)