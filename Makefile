CC=g++
FLAGS=
INCLUDE=include/
LINK=-lcurl -pthread
CFILES=src/file.cpp src/http.cpp src/ui.cpp src/server.cpp
OUTPUT=webserver

make: $(CFILES)
	$(CC) $(CFILES) -I$(INCLUDE) $(LINK) -o $(OUTPUT)
	
win: $(CFILES)
	$(CC) $(CFILES) -I$(INCLUDE) $(LINK) -DWINDOWS -o webserver.exe