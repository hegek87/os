all:
	gcc -c -g -Wall ../../Data-Structures/double_linked_list/double_linked_list.c
	gcc -c -g -Wall my_ls.c
	
	gcc -o ./objects/my_ls -g -Wall double_linked_list.o my_ls.o
	
	mv my_ls.o ./objects
	mv double_linked_list.o ./objects
