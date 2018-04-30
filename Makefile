CC=g++
FLAGS=-std=C++11
INCLUDE=include/
LINK=-lcurl -pthread
CFILES=src/file.cpp src/fileReader.cpp src/http.cpp src/ui.cpp src/server.cpp main.cpp
OUTPUT=webserver

make: $(CFILES)
	$(CC) $(CFILES) -I$(INCLUDE) $(LINK) -o $(OUTPUT)
	
win: $(CFILES)
	$(CC) $(CFILES) -I$(INCLUDE) $(LINK) -DWINDOWS -o webserver.exe