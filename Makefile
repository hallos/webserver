CC=g++
FLAGS=-std=c++11
INCLUDE=include/
LINK=-lcurl -pthread
CFILES=src/file.cpp src/fileReader.cpp src/http.cpp src/ui.cpp src/server.cpp main.cpp
OUTPUT=webserver

make: $(CFILES)
	$(CC) $(FLAGS) $(CFILES) -I$(INCLUDE) $(LINK) -o $(OUTPUT)
	
win: $(CFILES)
	$(CC) $(FLAGS) $(CFILES) -I$(INCLUDE) $(LINK) -DWINDOWS -o webserver.exe

test:
	exit 0