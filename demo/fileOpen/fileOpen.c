#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include "global.h"

FileStatus fileOpen(char *file_name) {
	FileStatus temp;
	char *path = (char*)malloc(256*sizeof(char));
	
	if (chdir(".") != 0) {
		perror("directory");
		exit(1);
	}

	// full_path
	if (getcwd(path, 256) == NULL) {
		perror("getcwd");
		exit(1);
	}
	printf("%s\n", path);
	strcat(path, "/");
	strcat(path, file_name);
	printf("%s\n", path);
	
	// file existence check
	if (access(path, F_OK) == 0) {
		// initializing
        	temp.file_name = strdup(file_name);
        	temp.full_path = strdup(path);
        	temp.row = 0; temp.col = 0;	
	}
	else {
		fprintf(stderr, "file does not exist\n");
		exit(1);
	}

	return temp;
}

int main(int argc, char* argv[]) {
	FileStatus file1;

	if (argc != 2) {
		fprintf(stderr, "Usage: fileOpen.out <filename>\n");
		exit(1);
	}
	
	file1 = fileOpen(argv[1]);

	printf("file name : %s\n", file1.file_name);
	printf("file full path : %s\n", file1.full_path);
	
	return 0;
}
	
