/* headers */
#include <ctype.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

/* some macros */
#define ERROR(message) do{ \
	fprintf(stderr, "\x1b[38;5;9m");\
	perror(message);\
	fprintf(stderr, "\x1b[0m");\
}while(0)

/* functions */
void usage(void);
void process_arguments(int argc, char **argv);
void handle_output();
void cipher_text();
void obtain_plaintext();

/* Variables */
char *filename =  NULL;
char *input_plain = NULL;
bool should_we_unmap = false;
char *output_cipher = NULL;
char *key = NULL;

/* some objects schemas */
typedef struct{
	short value;
	short position;
}key_object;

int main(int argc, char **argv){
	process_arguments(argc, argv);
	obtain_plaintext();
	cipher_text();
	return 0;
}

void obtain_plaintext(){
	if(filename != NULL){
		/* we will read from a file */
		short fd = open(filename, O_RDONLY);
		if(fd < 0)
			ERROR("OPEN");

		struct stat st;
		fstat(fd, &st);

		input_plain = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
		if(input_plain == MAP_FAILED)
			ERROR("MMAP");
		fprintf(stderr, "The size of mmap is %ld.\n", strlen(input_plain));

		close(fd);
		should_we_unmap = true;

	}else{
		/* we will read from the stdin stream */
		input_plain = (char*)malloc(sizeof(char) * 12289);
		if(input_plain == NULL)
			ERROR("1. MALLOC");
		/* a very naive approach !!!!!! */
		fread(input_plain,sizeof(char), sizeof(input_plain), stdin);
	}
}

void cipher_text(){
	short KEY_LEN = (short)strlen(key);

	key_object key_array[KEY_LEN];
	for(int i = 0; i < KEY_LEN; i++){
		key_array[i].value = ((int)key[i]) - 48;;
	}

	output_cipher = malloc(strlen(input_plain) + 1);
	memset(output_cipher,0, strlen(input_plain) + 1);

	if(output_cipher == NULL)
		ERROR("CALLOC");
	fprintf(stderr, "You were given %ld bytes to work with.\n", strlen(input_plain));

	short offset = 0;
	char cipher;
	int temp;
	for(int i = 0; i < (int)strlen(input_plain); i++){
		offset = key_array[i % KEY_LEN].value;
		if(isalpha(input_plain[i])){
			if(isupper(input_plain[i])){
				temp = (((input_plain[i] - 65) + offset) % 26);
				cipher = (char)(65 + temp);
				output_cipher[i] = cipher;
			}else{
				temp = ((input_plain[i] - 97) + offset) % 26;
				cipher = (char)(97 + temp); 
				output_cipher[i] = cipher;
			}
		}else
			output_cipher[i] = input_plain[i];
	}
	handle_output();
	free(output_cipher);

	if(should_we_unmap)
		munmap(input_plain, sizeof(input_plain));
	else free(input_plain);
}

void handle_output(){
	fprintf(stderr, "Size of output is %ld.\n", strlen(output_cipher));
	printf("%s\n", output_cipher);
}

void usage(void){
	fprintf(stderr, "USAGE:\n\t%s -k key [-f filename] [-o output]\n", __FILE__);
}

void process_arguments(int argc, char **argv){
	int parameter;
	while((parameter = getopt(argc, argv, ":f:o:k:")) != -1){
		switch(parameter){
			case 'k':
				key = optarg;
				break;
			case 'f':
				filename = optarg;
				break;
			case 'o':
				output_cipher = optarg;
				break;
			default:
				usage();
		}
	}
}
