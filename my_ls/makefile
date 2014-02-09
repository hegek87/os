CC=gcc
CFLAGS = -c -g -Wall -o
OBJECTS =	bin/dll.o	\
		bin/my_ls.o
		
DSPATH = ../../Data-Structures
DLL=double_linked_list/double_linked_list.c

all: ls_clone

ls_clone: $(OBJECTS)
	$(CC) -o bin/my_ls $(OBJECTS)
	
bin/dll.o: $(DSPATH)/$(DLL)
	$(CC) $(CFLAGS) bin/dll.o $(DSPATH)/$(DLL)
	
bin/my_ls.o: my_ls.c
	$(CC) $(CFLAGS) bin/my_ls.o my_ls.c
