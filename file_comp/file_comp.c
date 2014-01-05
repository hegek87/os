#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>

#include "file_comp.h"

static struct stack *to_visit;
static struct dl_list *visited;

void usage(void){
	printf("./file_comp <directory-name>\n");
}

//0 is success, 1 is link, '.', or '..', -1 is error
int process(char *path){
	struct stat cur_obj;
	printf("Stack size: %d\n", stack_size(to_visit));
	//char *path = (char *)(pop(to_visit)->data);
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
				printf("A component of the path doesn't exist\n");
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
				int p_len = strlen(path) + strlen(dirp->d_name)+2;
				char new_path[p_len];
				strcpy(new_path, path);
				strcat(new_path, "/");
				strcat(new_path, dirp->d_name);
				printf("Pushing: %s\n", new_path);
				push_key(to_visit, new_path);
				printf("Size of stack: %d\n", stack_size(to_visit));
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
		return 1;
	}
	return 0;
}

int main(int argc, char **argv){


	to_visit = create_stack();
	visited = create_empty_list();
	char *path = NULL;
	if(argc > 2 || argc < 1){
		usage();
		exit(-1);
	}
	//start search at specified directory
	if(argc == 2){
		path = *(argv+1);
	}
	//start search at current working directory
	else{
		//path = calloc(2, sizeof(char));
		path = ".";
	}
	
	push_key(to_visit, path);
	//char *cur;
	while(!is_stack_empty(to_visit)){
		struct stat cur_obj;
		path = (char *)(pop(to_visit)->data);
		printf("%s\n", path);
		if(lstat(path, &cur_obj)){
			perror("Failed to stat\n");
			printf("%d\n", errno);
			exit(-1);
		}
		mode_t x = cur_obj.st_mode;
		if(S_ISDIR(x)){
			//found a directory
			printf("Is a Directory\n");
			DIR *cwd;
			struct dirent *dirp;
			if(!(cwd = opendir(path))){
				perror("FAILED TO OPEN\n");
				exit(-1);
			}
			while((dirp = readdir(cwd))){
				printf("Name: %s\n", dirp->d_name);
				if(!strcmp(dirp->d_name, ".")){
					continue;
				}
				if(!strcmp(dirp->d_name, "..")){
					continue;
				}
				if(strcmp(path, ".")){
					//build path
					int p_len = strlen(path)+strlen(dirp->d_name)+2;
					char *new_path[p_len+1];
					strcpy(new_path, path);
					strcat(new_path, "/");
					strcat(new_path, dirp->d_name);
					push_key(to_visit, new_path);
					printf("Pushed: %s\n", new_path);
				}
				else{
					push_key(to_visit, dirp->d_name);
				}
			}
		}
		else{
			printf("Not a Directory\n");
		}
	}
	
	/*
	while(!is_stack_empty(to_visit)){
		printf("Second loop\n");
		printf("strlen of cur: %d\n", strlen(cur));
		cur = (char *)(pop(to_visit)->data);
		if(process(cur) == -1){
			break;
		}
	}*/
	/*
	struct node *t = pop(to_visit);
	printf("%s\n", t->data);
	*/
	/*
	char *x = (char *)(pop(to_visit)->data);
	printf("%s\n", x);
	printf("strlen of x: %d\n", strlen(x));
	x = (char *)(pop(to_visit)->data);
	//printf("%s\n", x);
	printf("strlen of x: %d\n", strlen(x));
	
	struct stat ab;
	if(lstat(x, &ab)){
		perror("FAILED\n");
	}
	*/
	printf("Done processing. Stack size: %d\n", stack_size(to_visit));
		
	is_empty(visited);
	//printf("HI\n");
	
	/*
	char *path = "objects/";
	struct stat t;
	if(lstat(path, &t)){
		perror("ERROR");
		printf("%d\n", errno);
	}
	printf("SUCCESS\n");
	*/
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
