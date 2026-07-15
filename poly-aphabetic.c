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

		close(fd);

	}else{
		/* we will read from the stdin stream */
	}
}

void cipher_text(){
	short KEY_LEN = (short)strlen(key);

	key_object key_array[KEY_LEN];
	for(int i = 0; i < KEY_LEN; i++){
		key_array[i].value = ((int)key[i]) - 48;;
	}

	output_cipher = calloc(1, sizeof(input_plain) + 1);
	memset(output_cipher,0, sizeof(output_cipher));

	if(output_cipher == NULL)
		ERROR("CALLOC");

	short offset = 0;
	char cipher;
	for(int i = 0; i < (int)strlen(input_plain) - 1; i++){
		offset = key_array[i % KEY_LEN].value;
		printf("Offset is %d\n", offset);
		if(isalpha(input_plain[i])){
			if(isupper(input_plain[i])){
				cipher = (char)('A' + ((input_plain[i] + offset) % 27));
				output_cipher[i] = cipher;
				printf("[%c]", cipher);
			}else{

				cipher = (char)('a' + ((input_plain[i] + offset) % 27));
				output_cipher[i] = cipher;
				printf("[%c]", cipher);
			}
		}else{
			output_cipher[i] = input_plain[i];
			printf("[%c]", output_cipher[i]);
		}
	}
	handle_output();
	free(output_cipher);
	munmap(input_plain, sizeof(input_plain));
}

void handle_output(){
	printf("OUTPUT::\n %s\n", output_cipher);
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
