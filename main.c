#include <stdio.h>
#include <unistd.h>
#include <ctype.h>


int main(int argc, char *argv[]){
	char *key = NULL;
	char *filename;

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
				filename = optarg;
				using_file = true;
				break;
			default:
				fprintf(stderr, "Usage :\n"
						"%s -k key [-f filename]"
						"\n",__FILE__);
		}

	}

	if(!should_we_countinue) return 1;

//	printf("we abtained the key :: %s.\n", key);
	if(!using_file){
		/* Here things are going on an array of characters
		 * reading everything to a buffer 
		 */


	}else{
		/* Things here will move away from stack and go to heap
		 * for easier management and simplicity 
		 */
	}
	return 0;
}
