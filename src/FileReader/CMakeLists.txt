
include_directories(../)
add_library(FileReader STATIC
    FileReader.cpp
)

target_link_libraries(FileReader
    WebserverLib
    stdc++fs
)