/* headers */
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <ctype.h>

/* some macros */
#define ERROR(message) do{ \
	fprintf(stderr, "\x1b[38;5;9m");\
	perror(message);\
	fprintf(stderr, "\x1b[0m");\
}while(0)

/* functions */
void usage(void);
void process_arguments(int argc, char **argv);

/* Variables */
char *filename =  NULL;
char *input_plain = NULL;
char *output_cipher = NULL;
char *key = NULL;

/* some objects schemas */
typedef struct {
	short value;
	short position;
}key_t;

int main(int argc, char **argv){
	process_arguments(argc, argv);
	obtain_plaintext();

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
				output = optarg;
				break;
			default:
				usage();
		}
	}
}
