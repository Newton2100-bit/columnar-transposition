#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main(){
	int fd = open("main.c", O_RDONLY);
	struct stat this;
	fstat(fd, &this);
	char *file = mmap(NULL, this.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if(file == MAP_FAILED)
		perror("MMAP: ");

	printf("CONTENTS::: \n%s", file);
	munmap(file, this.st_size);
	return 0;
}
