#ifndef FILE_COMP_H
#define FILE_COMP_H
#include "../../Data-Structures/stack/stack.h"
#include "../../Data-Structures/double_linked_list/double_linked_list.h"

struct file_data{
	char digest[16];
	FILE *file;
};

void usage(void);
int process(char *);

#endif
