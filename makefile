# Use the first FLAGS definition for debugging, the 2nd for optimizing your code
FLAGS = -Wall -Wextra -g

CA = 5
LASTNAME =
BU_USERID =
GITHUB_USERID =
EXECUTABLE = splite
inputdir = songsdir
workdir = outputdir

## ADD TARGETS TO SEPARATELY COMPILE YOUR CODE FOR CA3 AND CREATE AN EXECUTABLE.

all: main

main:	Main.o Song.o User.o Library.o
	g++ -ggdb main.o Song.o User.o Library.o -o splite

Main.o: main.cpp
	g++ -ggdb -c main.cpp

Song.o: Song.cpp
	g++ -ggdb -c Song.cpp

User.o: User.cpp
	g++ -ggdb -c User.cpp

Library.o: Library.cpp
	g++ -ggdb -c Library.cpp

tar: clean
	cd .. && tar -cvzf CA$(CA)_$(LASTNAME)_$(BU_USERID).tar.gz cs240-spring21-ca$(CA)-$(GITHUB_USERID) && cd -

scrub: clean
	rm ~* *.swp *\#

memcheck: $(EXECUTABLE)
	valgrind --leak-check=full ./$(EXECUTABLE)

# Specify the object files and executables that are generated
# and need to be removed to re-compile the whole thing
clean:
	rm -f *.o $(EXECUTABLE)