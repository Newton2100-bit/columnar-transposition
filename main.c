#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>


int main(int argc, char *argv[]){
	char *key = NULL;
	char *target_data;;
	int target_fd = -1;
	int final_size;
	int argument;
	bool should_we_countinue = false;
	bool using_file = false;

	while((argument = getopt(argc, argv, ":k:f:")) != -1){
		switch(argument){
			case 'k':
				key = optarg;
				should_we_countinue = true;
				break;
			case 'f':
				target_data = optarg;
				using_file = true;
				break;
			default:
				fprintf(stderr, "Usage :\n"
						"%s -k key [-f filename]"
						"\n", __FILE__);
		}

	}

	if(!should_we_countinue) return 1;

	// printf("we abtained the key :: %s.\n", key);
	if(!using_file){
		/* Here things are going on an array of characters
		 * reading everything to a buffer 
		 */
		target_data = (char *)malloc(sizeof(char) * 4096 );
		int offset = 0;
		while(true){
			if(scanf("%[^'\n']", target_data + offset) == EOF)
				break;
			getchar();
			offset = strlen(target_data);
			target_data[offset] = '\n';
			offset++;
		 }
	//	printf("we read >>>>>>>>>>>>>>>%s<<<<<<<<<<<<<", target_data);
	printf("size of the data we did read is %d.\n", strlen(target_data));
	}else{
		/* Things here will move away from stack and go to heap
		 * for easier management and simplicity 
		 */
		target_fd = open(target_data, O_RDONLY);
		struct stat file_size ;
		fstat(target_fd, &file_size);
		int actual_size = file_size.st_size - 1;
		printf("The size is of %d bytes.\n", actual_size);

		final_size = (actual_size +  (strlen(key) - actual_size % strlen(key))) + 1;
		printf("%d converts to %d\n", actual_size, final_size);
		target_data = mmap(NULL,final_size, PROT_READ, MAP_PRIVATE, target_fd, 0 );
		if(target_data != NULL)
			fprintf(stderr, "Success mapping our file.\n");

	}

	/* Page size of our machine */
	printf("The page size of this machine is %d.\n", sysconf(_SC_PAGESIZE));
	char *cipher_text = (char *)malloc(final_size * sizeof(char));
	free(cipher_text);
	return 0;
}
