#ifndef FILE_COMP_H
#define FILE_COMP_H
#include "../../Data-Structures/stack/stack.h"
#include "../../Data-Structures/double_linked_list/double_linked_list.h"

#define HASH_SIZE 32

struct file_data{
	char *digest;
	struct dl_list *files;
};

int visit(char *, char *);
void usage(void);
int process(char *);
int walk_dir(char *);
char *compute_hash(char **);
void print_results(void);

#endif
