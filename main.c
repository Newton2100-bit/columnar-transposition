#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>

/* variables */
char *key = NULL;
char *filename;
struct stat file_size;
char *target_data;
int argument;
bool should_we_countinue = false;
bool using_file = false;
bool is_unmap = false;

/* functions */
void obtain_plain_text();
void finalize();
void transpose();

int main(int argc, char *argv[]){

	while((argument = getopt(argc, argv, ":k:f:")) != -1){
		switch(argument){
			case 'k':
				key = optarg;
				should_we_countinue = true;
				break;
			case 'f':
				filename = optarg;
				using_file = true;
				break;
			default:
				fprintf(stderr, "Usage :\n"
						"%s -k key [-f filename]"
						"\n", __FILE__);
		}

	}

	/* if we don't have the key then why continue */
	if(!should_we_countinue) return 1;

	obtain_plain_text();
	transponse();
	finalize();
	return 0;
}

void obtain_plain_text(){
	// printf("we abtained the key :: %s.\n", key);
	if(!using_file){
		/* Here things are going on an array of characters
		 * reading everything to a buffer 
		 */
		target_data = (char *)malloc(sizeof(char) * 4096 );
		fread(target_data , 1, 4096, stdin);
		/* if there is no data then we should exit */
		if (strlen(target_data) < 1) return 1;
	}else{
		/* Things here will move away from stack and go to heap
		 * for easier management and simplicity 
		 */
		int fd = open(filename, O_RDONLY);
		fstat(fd, &file_size);

		target_data = mmap(NULL, file_size.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
		if(target_data == MAP_FAILED){
			perror("MMAP :: ");
			return 1;
		}

		close(fd);
	}
}

void transpose(){
/* the whole logic */
	char *cipher_text = (char *)malloc(4096);

	free(cipher_text);

}
void finalize(){
	if(is_unmap)
		munmap(target_data, file_size.st_size);
	else
		free(target_data);
}
