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
		printf("size of the data we did read is %d.\n", strlen(target_data));
		int margin = strlen(key) - strlen(target_data) % strlen(key);

		printf("MARGIN FOUND IS %d.\n", margin);
		final_size = strlen(target_data) + margin;
		printf("%d converts to %d\n", strlen(target_data), final_size);
		
		int i = 0;
		do{
			//memset(target_data + strlen(target_data), '`', margin);
			target_data[strlen(target_data) + i] = '~';
			i++;
		}while(i <= margin);
		target_data[final_size] = '\0';
		printf("we read >>>>>>>>>>>>>>>%s<<<<<<<<<<<<<", target_data);
	}else{
		/* Things here will move away from stack and go to heap
		 * for easier management and simplicity 
		 */
		target_fd = open(target_data, O_RDONLY);
		struct stat file_size ;
		fstat(target_fd, &file_size);
		int actual_size = file_size.st_size ;
		int margin = strlen(key) - actual_size % strlen(key);
		final_size = actual_size + margin; 
		// printf("%d converts to %d\n", actual_size, final_size);
		/* The above statement is a source of many questions
		 * that after a well analysis will prove code entirely
		 * correct or entirely wrong 
		 * in the area of reading data
		 */
		target_data = mmap(NULL,4096, PROT_READ, MAP_PRIVATE, target_fd, 0 );
		if(target_data != NULL)
			fprintf(stderr, "Success mapping our file.\n");
#if 0
		printf("MARGIN FOUND IS %d.\n", margin);
		printf("%d converts to %d\n", strlen(target_data), final_size);
		printf("mapped file size is %d bytes.\n", strlen(target_data));
#endif
		strcpy(target_data, "```````````````");
		 printf("we read >>>>>>>>>>>>>>>%s<<<<<<<<<<<<<", target_data);
	}

	char *cipher_text = (char *)malloc(final_size * sizeof(char));
	free(cipher_text);
	return 0;
}
