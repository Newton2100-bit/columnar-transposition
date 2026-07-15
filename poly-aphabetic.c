/* headers */
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>

/* functions */
void usage(void);
void obtain_data(int argc, char **argv);

/* Variables */
char *filename =  NULL;
char *input = NULL;
char *output = NULL;
char *key = NULL;

int main(int argc, char **argv){
	obtain_data(argc, argv);

	return 0;
}

void usage(void){
	fprintf(stderr, "USAGE:\n%s -k key [-f filename] [-o output]\n", __FILE__);
}

void obtain_data(int argc, char **argv){
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
