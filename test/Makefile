#This simple make file must be executed from within the directory that contains
#the source files, header files, and main files.
     
#linking of all the main .o files
run : cMain.o cppSource.o cSource.o
	g++ -o run cMain.o cppSource.o cSource.o
     
# c files need to be compiles with gcc
cMain.o : cMain.c
	gcc -c cMain.c
     
cSource.o : cSource.c
	gcc -c cSource.c
     
# cpp files need to be compiled with g++
cppSource.o : cppSource.cpp
	g++ -c cppSource.cpp

# this allows you to remove all the .o files from the directory in which you compiled
# simply issue the command <make clean> and it will perform this action.    
clean:
	rm -f run *.o
