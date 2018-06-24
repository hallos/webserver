CC=g++
FLAGS=-std=c++11
INCLUDE=include/
LINK=-lcurl -pthread
SRCDIR=src/
OBJDIR=
CFILES=$(SRCDIR)file.cpp $(SRCDIR)fileReader.cpp $(SRCDIR)http.cpp $(SRCDIR)ui.cpp $(SRCDIR)server.cpp main.cpp
OBJ=$(OBJDIR)file.o $(OBJDIR)fileReader.o $(OBJDIR)http.o $(OBJDIR)ui.o $(OBJDIR)server.o 
OUTPUT=webserver
TESTFILES=test/main.cpp test/httpTest.cpp
TESTOUTPUT=test.out

$(OBJDIR)%.o: $(SRCDIR)%.cpp
	$(CC) -c $^ $(FLAGS) -I$(INCLUDE) -o $@

make: $(OBJ)
	$(CC) $(FLAGS) main.cpp $(OBJ) -I$(INCLUDE) $(LINK) -o $(OUTPUT)
	
win: $(CFILES)
	$(CC) $(FLAGS) $(CFILES) -I$(INCLUDE) $(LINK) -DWINDOWS -o webserver.exe

check:
	cppcheck . -itest/ --error-exitcode=1 

test: $(TESTFILES) $(OBJ)
	$(CC) $(TESTFILES) $(OBJ) -Itest/ -Iinclude/ -lcppunit -lpthread -o $(TESTOUTPUT) && ./$(TESTOUTPUT) 

.PHONY: test

clean:
	rm obj/*.o bin/* *.o test.out $(OUTPUT)