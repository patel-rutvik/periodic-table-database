# Kaden Dreger and Rutvik Patel
# Periodic Table Database

#  This makefile allows the user to link, compile and clean the files
#  necessary to run server.cpp.
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
CC = g++ # compiler of choice
OBJS = server.o serialport.o # object files generated
CFLAGS = -c -Wall -std=c++11 # compile flags
LFLAGS = -static
EXC = server # executable name

# Builds the project, generating the server executable
$(EXC): $(OBJS)
	$(CC) $(OBJS) -o server $(LFLAGS)

serialport.o: serialport.h
	$(CC) serialport.cpp -o serialport.o $(CFLAGS)

server.o: types.h server.cpp trie.h element.h
	$(CC) server.cpp -o server.o $(CFLAGS)

# removes the .o and executable files from the directory
clean:
	@rm -f $(OBJS) $(EXC)

# printing out the makefile target objectives
help:
	@echo 'make server       : builds the project and generates the server executable'
	@echo 'make serialport.o : compiles serialport.cpp into its respective object (.o) file'
	@echo 'make server.o     : compiles server.cpp into its respective object (.o) file'
	@echo 'make clean        : removes the .o and executable files from the directory'
	@echo 'make help         : prints out information about the targets of the make file'
