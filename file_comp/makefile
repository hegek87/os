all:
	gcc -c -g -Wall ~/code/csrc/Data-Structures/stack/stack.c
	gcc -c -g -Wall ~/code/csrc/Data-Structures/double_linked_list/double_linked_list.c
	gcc -c -g -Wall file_comp.c
	
	mv file_comp.o stack.o double_linked_list.o ./objects
	rm -f stack.o double_linked_list.o file_comp.o
	
	gcc -g -Wall -o ./objects/file_comp ./objects/stack.o ./objects/double_linked_list.o ./objects/file_comp.o
