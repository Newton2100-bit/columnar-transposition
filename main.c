#include <stdio.h>
#include <unistd.h>
#include <ctype.h>


int main(int argc, char *argv[]){
	char *key = NULL;
	char *filename;

	int argument;
	bool should_we_countinue = false;
	while((argument = getopt(argc, argv, ":k:f:")) != -1){
		switch(argument){
			case 'k':
				key = optarg;
				should_we_countinue = true;
				break;
			case 'f':
				filename = optarg;
				break;
			default:
				fprintf(stderr, "Usage :\n"
						"%s -k key [-f filename]"
						"\n",__FILE__);
		}

	}

	if(!should_we_countinue) return 1;
	return 0;
}
