#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>

#include "file_comp.h"

//static struct stack *to_visit;
static struct dl_list *visited;

void usage(void){
	printf("./file_comp <directory-name>\n");
}

//0 is success, 1 is link, '.', or '..', -1 is error
int process(struct stack *to_visit){
	struct stat cur_obj;
	printf("Stack size: %d\n", stack_size(to_visit));
	char *path = (char *)(pop(to_visit)->data);
	printf("Statting: %s\n", path);
	if(lstat(path, &cur_obj)){
		//printf("\n\n%s\n", path);
		//perror("Failed to stat object\n");
		printf("%d %d\n", ENOENT, errno);
		switch(errno){
			case EACCES:
				printf("Search permission denied\n");
				break;
			case EBADF:
				printf("fd is bad\n");
				break;
			case EFAULT:
				printf("Bad address\n");
				break;
			case ELOOP:
				printf("Too many links\n");
				break;
			case ENAMETOOLONG:
				printf("Path too long\n");
				break;
			case ENOENT:
				printf("A component of the path doesn't exits\n");
				break;
			case ENOMEM:
				printf("Out of kernel mem\n");
				break;
			case ENOTDIR:
				printf("Component of path is not a directory\n");
				break;
			case EOVERFLOW:
				printf("Too large\n");
			default:
				printf("%d\n", errno);
				break;
		}
		return -1;
	}
	mode_t x = cur_obj.st_mode & S_IFMT;
	if(x == S_IFLNK){
		printf("Is Link\n");
		return 1;
	}
	else if(x == S_IFDIR){
		DIR *cd;
		struct dirent *dirp;
		if(!(cd = opendir(path))){
			printf("Error opening %s\n", path);
			return -1;
		}
		printf("Found Directory\n");
		printf("Opening: %s\n", path);
		while((dirp = readdir(cd))){
			//skip .
			if(!strcmp(dirp->d_name, ".")){
				printf("Found '.'\n");
				return 1;
			}
			//skip ..
			if(!strcmp(dirp->d_name, "..")){
				printf("Found '..'");
				return 1;
			}
			if(strcmp(path, ".")){
				int p_len = strlen(path) + strlen(dirp->d_name)+1;
				char new_path[p_len+1];
				strcpy(new_path, path);
				strcat(new_path, "/");
				strcat(new_path, dirp->d_name);
				printf("Pushing: %s\n", new_path);
				push_key(to_visit, new_path);
			}
			else{
				printf("Pushing: %s\n", dirp->d_name);
				push_key(to_visit, dirp->d_name);
			}
			printf("Stack size: %d\n", stack_size(to_visit));
		}
		closedir(cd);		
	}
	else{
		printf("Found File\n");
		printf("%s\n", path);
	}
	return 0;
}

int main(int argc, char **argv){

/*
	struct stack *to_visit = create_stack();
	visited = create_empty_list();
	//struct stat cur_obj;
	char *path = NULL;
	if(argc > 2 || argc < 1){
		usage();
		return -1;
	}
	//start search at specified directory
	if(argc == 2){
		path = *(argv+1);
		//printf("%s\n", path);
		push_key(to_visit, (void *)path);
		//char *x = (char *)(pop(to_visit)->data);
		//printf("%s\n", x);
	}
	//start search at current working directory
	else{
		push_key(to_visit, ".");
	}
	
	while(!is_stack_empty(to_visit)){
		//char *cur = (char *)(pop(to_visit)->data);
		//printf("Popped: %s\n", cur);
		if(process(to_visit)==-1){
			exit(-1);
		}
	}
	
	printf("Done processing. Stack size: %d\n", stack_size(to_visit));
		
	is_empty(visited);
	//printf("HI\n");
	*/
	char *path = "objects/";
	struct stat t;
	if(lstat(path, &t)){
		perror("ERROR");
		printf("%d\n", errno);
	}
	printf("SUCCESS\n");
	/*
	if(argc != 3){
		perror("Include 3 args\n");
		exit(-1);
	}
	int p_len = strlen(argv[1]) + strlen(argv[2]) + 1 + 1;
	char new_path[p_len+1];
	struct stack *st = create_stack();
	strcpy(new_path, argv[1]);
	strcat(new_path, "/");
	strcat(new_path, argv[2]);
	printf("BEFORE PUSHING: %s\n", new_path);
	push_key(st, new_path);
	char *z = (char *)(pop(st)->data);
	printf("AFTER PUSHING:  %s\n", z);
	*/
	return 0;
}
